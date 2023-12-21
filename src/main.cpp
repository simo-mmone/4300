#include <SFML/Graphics.hpp>
#include <iostream>

int main(int argc, char * argv[])
{
    const int mWidth = 640;
    const int mHeight = 480;
    sf::RenderWindow window(sf::VideoMode(mWidth, mHeight), "SFML works!");
    window.setFramerateLimit(60);

    int r = 0;
    int g = 255;
    int b = 255;

    sf::CircleShape shape(50);
    shape.setFillColor(sf::Color(r, g, b));
    shape.setPosition(300.0f, 300.0f);
    float circleMoveSpeed = 0.5f;

    sf::Font font;
    if (!font.loadFromFile("fonts/tech.ttf"))
    {
        std::cout << "Error loading font\n";
        exit(-1);
    }

    sf::Text text("Sample text", font, 24);
    text.setPosition(0, mHeight - (float)text.getCharacterSize());

    while (window.isOpen()) // main loop
    {
        sf::Event event;
        while (window.pollEvent(event)) // event loop
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                std::cout << "Key pressed with code = " << event.key.code << std::endl;

                if (event.key.code == sf::Keyboard::X)
                {
                    circleMoveSpeed *= -1.0f;
                }
            }
        }
        
    }
    
    return 0;
}