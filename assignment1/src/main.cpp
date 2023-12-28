#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <fstream>

class MyShape
{
    float mSx, mSy;
    int radius;
    int width, height;
public:
    std::shared_ptr<sf::Shape> mShape;
    std::string mName;
    MyShape(std::string name, std::shared_ptr<sf::Shape> shape, float sx, float sy)
        : mName(name), mShape(shape), mSx(sx), mSy(sy)
    {
        radius = 0;
        width = 0;
        height = 0;
    }

    void setRadius(int radius)
    {
        this->radius = radius;
    }

    void setWidthAndHeight(int width, int height)
    {
        this->width = width;
        this->height = height;
    }

    void update(int mWidth, int mHeight)
    {
        float shapeWidth = mShape->getLocalBounds().width;
        float shapeHeight = mShape->getLocalBounds().height;
        // check if shape is off screen
        if (mShape->getPosition().x < 0 || (mShape->getPosition().x + shapeWidth) > mWidth)
        {
            mSx = -mSx;
        }
        if (mShape->getPosition().y < 0 || (mShape->getPosition().y + shapeHeight) > mHeight)
        {
            mSy = -mSy;
        }

        // update position
        mShape->setPosition(mShape->getPosition().x + mSx, mShape->getPosition().y + mSy);
    }

    sf::Vector2f getShapeCenter()
    {
        auto position = mShape->getPosition();
        if (this->radius > 0)
        {
            return sf::Vector2f(position.x + this->radius, position.y + this->radius);
        }
        else
        {
            return sf::Vector2f(position.x + (this->width / 2.0f), position.y + (this->height / 2.0f));
        }
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(*mShape);
    }
};

// TODO: read window size from config file
// TODO: read shapes from config file
// TODO: bounce shapes off of walls
// TODO: draw names of shape in the middle of the shape
int main(int argc, char * argv[])
{
    int mWidth = 640;
    int mHeight = 480;
    sf::Font font;
    int mFontSize, mFontRed, mFontGreen, mFontBlue;

    std::vector<std::shared_ptr<MyShape>> myShapes;

    std::ifstream fin("assets/config.txt");
    std::string instruction;
    std::cout << "Reading config file...\n";
    while (fin >> instruction)
    {
        std::cout << "Instruction: " << instruction << "\n";
        if (instruction == "Window")
        {
            fin >> mWidth >> mHeight;
            std::cout << "Window size: " << mWidth << "x" << mHeight << "\n";
        }
        else if (instruction == "Font")
        {
            std::string mFont;
            fin >> mFont >> mFontSize >> mFontRed >> mFontGreen >> mFontBlue;
            if (!font.loadFromFile(mFont))
            {
                std::cout << "Error loading font\n";
                exit(-1);
            }
        }
        else if (instruction == "Circle")
        {
            std::string name;
            float sx, sy;
            int x, y, radius, red, green, blue;
            fin >> name >> x >> y >> sx >> sy >> red >> green >> blue >> radius;
            auto newCircle = std::make_shared<sf::CircleShape>(radius);
            newCircle->setPosition(x, y);
            newCircle->setFillColor(sf::Color(red, green, blue));
            auto newShape = std::make_shared<MyShape>(name, newCircle, sx, sy);
            newShape->setRadius(radius);
            myShapes.push_back(newShape);
        }
        else if (instruction == "Rectangle")
        {
            std::string name;
            float sx, sy;
            int x, y, red, green, blue, width, height;
            fin >> name >> x >> y >> sx >> sy >> red >> green >> blue >> width >> height;
            auto newRectangle = std::make_shared<sf::RectangleShape>(sf::Vector2f(width, height));
            newRectangle->setPosition(x, y);
            newRectangle->setFillColor(sf::Color(red, green, blue));
            auto newShape = std::make_shared<MyShape>(name, newRectangle, sx, sy);
            newShape->setWidthAndHeight(width, height);
            myShapes.push_back(newShape);
        }
    }

    std::cout << "Done reading config file\n";

    sf::RenderWindow window(sf::VideoMode(mWidth, mHeight), "SFML works!");
    window.setFramerateLimit(1000);

    // sf::Text text("Sample text", font, 24);
    // text.setPosition(0, mHeight - (float)text.getCharacterSize());

    while (window.isOpen()) // main loop
    {
        sf::Event event;
        while (window.pollEvent(event)) // event loop
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        
        for (auto& s : myShapes)
        {            
            window.draw(*s->mShape);
            s->update(mWidth, mHeight);

            std::string name = s->mName;
            auto shapeCenter = s->getShapeCenter();

            sf::Text text(name, font, 24);

            text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
            text.setPosition(shapeCenter);
            
            window.draw(text);
            
        }

        window.display();        
    }

    return 0;
}