#include "Game.h"

#include <iostream>
#include <fstream>
#include <array>
#include <math.h>

Game::Game(const std::string & config)
{
    init(config);
}

void Game::init(const std::string & path)
{
    // TODO: read file
    // use the premade PlayerConfig, EnemyConfig, BulletConfig variables
    std::ifstream fin(path);
    auto videoMode = sf::VideoMode(1280, 720);
    int frameRate = 60, fullScreen = 0;

    std::string cmdName;
    while (fin >> cmdName)
    {
        std::cout << cmdName << std::endl;
        if (cmdName == "Window")
        {
            fin >> videoMode.width >> videoMode.height >> frameRate >> fullScreen;
        }
        else if (cmdName == "Font")
        {
            std::string fontName;
            int fontSize, fontR, fontG, fontB;
            fin >> fontName >> fontSize >> fontR >> fontG >> fontB;
            if (!m_font.loadFromFile(fontName))
            {
                throw("font not found!");
            }
            m_text.setFont(m_font);
            m_text.setCharacterSize(fontSize);
            m_text.setFillColor(sf::Color(fontR, fontG, fontB));
            m_text.setPosition(sf::Vector2f(0,0));
            m_text.setString("0");
        }
        else if (cmdName == "Player")
        {
            fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S;
            fin >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB;
            fin >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB;
            fin >> m_playerConfig.OT >> m_playerConfig.V;
        }
        else if (cmdName == "Enemy")
        {
            fin >> m_enemyConfig.SR >> m_enemyConfig.CR;
            fin >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX;
            fin >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB;
            fin >> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX;
            fin >> m_enemyConfig.L >> m_enemyConfig.SI;
        }
        else if (cmdName == "Bullet")
        {
            fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S;
            fin >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB;
            fin >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB;
            fin >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
        }
    }
    
    auto windowStyle = sf::Style::Default;
    if (fullScreen > 0)
    {
        windowStyle = sf::Style::Fullscreen;
    }
    m_window.create(videoMode, "Assignment2", windowStyle);
    m_window.setFramerateLimit(frameRate);

    spawnPlayer();
}

void Game::run()
{
    while (m_running)
    {

        m_entities.update();

        if (!m_paused)
        {
            sEnemySpawner();
            sMovement();
            sCollision();
            sUserInput();
            sLifespan();
            sShoot();
        }


        sRender();

        if (!m_paused)
        {
            m_currentFrame++;
        }
    }
}

void Game::setPaused(bool paused)
{
    m_paused = paused;
}

/// @brief respawn player in the middle of the screen
void Game::spawnPlayer()
{
    // TODO: finish adding all proprieties of the player with the correct values from the config

    auto entity = m_entities.addEntity("player");

    float mx = m_window.getSize().x / 2.0f;
    float my = m_window.getSize().y / 2.0f;

    entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(0.0f, 0.0f), 0.0f);
    entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);
    // entity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);
    entity->cInput = std::make_shared<CInput>();

    m_player = entity;
}

/// @brief spawn an enemy at a random position
void Game::spawnEnemy()
{
    // TODO: properly spawn enemy
    auto entity = m_entities.addEntity("enemy");

    float shapeRadius = m_enemyConfig.SR;

    int shapeSpeedRange = m_enemyConfig.SMAX - m_enemyConfig.SMIN;
    int shapeSpeed = m_enemyConfig.SMIN;
    if (shapeSpeedRange > 0)
    {
        shapeSpeed += (rand() % shapeSpeedRange);
    }
    
    auto randDirection = Vec2(rand(), rand());
    randDirection.normalize();

    int xRange = m_window.getSize().x - (shapeRadius * 2);
    int yRange = m_window.getSize().y - (shapeRadius * 2);

    float ex = (rand() % xRange) + shapeRadius;
    float ey = (rand() % yRange) + shapeRadius;

    int shapePointsRange = m_enemyConfig.VMAX - m_enemyConfig.VMIN;
    int shapePoints = m_enemyConfig.VMIN;
    if (shapePointsRange > 0)
    {
        shapePoints += (rand() % shapePointsRange);
    }

    auto outlineColor = sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB);

    entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), randDirection * shapeSpeed, 0.0f);
    entity->cShape = std::make_shared<CShape>(shapeRadius, shapePoints, sf::Color(0, 0, 0, 0), outlineColor, m_enemyConfig.OT);
    entity->cScore = std::make_shared<CScore>(shapePoints * 100);

    m_lastEnemySpawnTime = m_currentFrame;
}

/// @brief spawn a bullet at player position aimed towards the mouse position
/// @param entity player
/// @param mousePos mouse position
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 & mousePos)
{
    auto bullet = m_entities.addEntity("bullet");

    Vec2 direction = mousePos - entity->cTransform->pos;
    direction.normalize();
    direction *= m_bulletConfig.S;

    bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, direction, 0.0f);
    auto fillColor = sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB);
    auto outlineColor = sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB);
    bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, fillColor, outlineColor, m_bulletConfig.OT);
    bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
}

/// @brief spawn the small enemies when a big one explodes
/// @param e big entity exploding
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{

}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
    // TODO
}

void Game::sRender()
{
    m_window.clear();

    for (auto e : m_entities.getEntities())
    {
        e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

        e->cTransform->angle += 1.0f;
        e->cShape->circle.setRotation(e->cTransform->angle);

        m_window.draw(e->cShape->circle);
    }

    m_window.draw(m_text);

    m_window.display();
}

void Game::sLifespan()
{
    for (auto b : m_entities.getEntities("bullet"))
    {
        if (b->cLifespan->remaining <= 0)
        {
            b->destroy();
        }
        else
        {
            b->cLifespan->remaining--;
            auto prevFillColor = b->cShape->circle.getFillColor();
            auto prevOutlineColor = b->cShape->circle.getOutlineColor();
            prevFillColor.a = 255 * b->cLifespan->remaining / b->cLifespan->total;
            prevOutlineColor.a = 255 * b->cLifespan->remaining / b->cLifespan->total;
            b->cShape->circle.setFillColor(prevFillColor);
            b->cShape->circle.setOutlineColor(prevOutlineColor);
        }
    }
}

void Game::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
        {
            m_running = false;
            m_window.close();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                m_player->cInput->up = true;
                break;
            case sf::Keyboard::A:
                m_player->cInput->left = true;
                break;
            case sf::Keyboard::S:
                m_player->cInput->down = true;
                break;
            case sf::Keyboard::D:
                m_player->cInput->right = true;
                break;
            
            default:
                break;
            }
        }

        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                m_player->cInput->up = false;
                break;
            case sf::Keyboard::A:
                m_player->cInput->left = false;
                break;
            case sf::Keyboard::S:
                m_player->cInput->down = false;
                break;
            case sf::Keyboard::D:
                m_player->cInput->right = false;
                break;
            
            default:
                break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                m_lastShootPos = Vec2(event.mouseButton.x, event.mouseButton.y);
                m_player->cInput->shoot = true;
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                m_lastShootPos = Vec2(event.mouseButton.x, event.mouseButton.y);
                m_player->cInput->special = true;
            }
        }
    }
}

bool IsColliding(const std::shared_ptr<Entity> & e1, const std::shared_ptr<Entity> & e2)
{
    auto r1 = e1->cShape->circle.getRadius();
    auto r2 = e2->cShape->circle.getRadius();

    float xDistance = e1->cTransform->pos.x - e2->cTransform->pos.x;
    float yDistance = e1->cTransform->pos.y - e2->cTransform->pos.y;

    float dist2 = xDistance * xDistance + yDistance * yDistance;
    float radius2 = (r1 + r2) * (r1 + r2);

    return dist2 <= radius2;
}

std::array<Vec2, 2> collisionBounce(const Vec2 & posA, const Vec2 & posB, const Vec2 & velA, const Vec2 & velB)
{
    // Calculate the difference in positions
    Vec2 delta = posA - posB;

    // Calculate the collision angle
    float collisionAngle = atan2(delta.y, delta.x);

    // Calculate the velocities in terms of magnitude and direction
    float magnitude1 = sqrt(velA.x * velA.x + velA.y * velA.y);
    float magnitude2 = sqrt(velB.x * velB.x + velB.y * velB.y);
    float direction1 = atan2(velA.y, velA.x);
    float direction2 = atan2(velB.y, velB.x);

    // Calculate the new velocity using the elastic collision equations
    float new_x_velocity1 = magnitude1 * cos(direction1 - collisionAngle);
    float new_y_velocity1 = magnitude2 * sin(direction2 - collisionAngle);
    float new_x_velocity2 = magnitude2 * cos(direction2 - collisionAngle);
    float new_y_velocity2 = magnitude1 * sin(direction1 - collisionAngle);

    // Convert the velocity and direction back into a vector
    Vec2 resultA;
    resultA.x = cos(collisionAngle) * new_x_velocity1 + cos(collisionAngle + M_PI / 2) * new_y_velocity1;
    resultA.y = sin(collisionAngle) * new_x_velocity1 + sin(collisionAngle + M_PI / 2) * new_y_velocity1;

    Vec2 resultB;
    resultB.x = cos(collisionAngle) * new_x_velocity2 + cos(collisionAngle + M_PI / 2) * new_y_velocity2;
    resultB.y = sin(collisionAngle) * new_x_velocity2 + sin(collisionAngle + M_PI / 2) * new_y_velocity2;

    return {resultA, resultB};
}

void Game::sCollision()
{
    for (auto b : m_entities.getEntities("bullet"))
    {
        for (auto e : m_entities.getEntities("enemy"))
        {
            if (!IsColliding(b, e))
            {
                continue;
            }

            // update score
            m_score += e->cScore->score;
            m_text.setString(std::to_string(m_score));

            b->destroy();
            e->destroy();
        }
    }

    auto entities = m_entities.getEntities("enemy");
    size_t i = 0;

    for (auto e : entities)
    {
        // enemy collisions
        for (size_t j = (i + 1); j < entities.size(); j++)
        {
            auto x = entities[j];
            if (IsColliding(e, x))
            {
                std::array<Vec2, 2> collisionResult = collisionBounce(
                    e->cTransform->pos,
                    x->cTransform->pos,
                    e->cTransform->velocity,
                    x->cTransform->velocity
                );
                
                e->cTransform->velocity = collisionResult[1];
                x->cTransform->velocity = collisionResult[0];
            }
        }
        i++;

        if (!IsColliding(e, m_player))
        {
            continue;
        }

        m_player->destroy();
        e->destroy();

        spawnPlayer();
    }
}

void Game::sEnemySpawner()
{
    if (m_currentFrame - m_lastEnemySpawnTime > 120)
    {
        spawnEnemy();
    }
}

void Game::sShoot()
{
    if (m_player->cInput->shoot)
    {
        spawnBullet(m_player, m_lastShootPos);
        m_player->cInput->shoot = false;
    }
}

void Game::sMovement()
{
    m_player->cTransform->velocity = Vec2(0,0);

    if (m_player->cInput->up)
    {
        m_player->cTransform->velocity.y = -5;
    }
    if (m_player->cInput->down)
    {
        m_player->cTransform->velocity.y = 5;
    }
    if (m_player->cInput->left)
    {
        m_player->cTransform->velocity.x = -5;
    }
    if (m_player->cInput->right)
    {
        m_player->cTransform->velocity.x = 5;
    }

    for (auto e : m_entities.getEntities())
    {
        e->cTransform->pos += e->cTransform->velocity;

        float radius = e->cShape->circle.getRadius();

        if (e->cTransform->pos.x < radius)
        {
            e->cTransform->velocity.x *= -1;
            e->cTransform->pos.x = radius;
        }
        if (e->cTransform->pos.x > (m_window.getSize().x - radius))
        {
            e->cTransform->velocity.x *= -1;
            e->cTransform->pos.x = m_window.getSize().x - radius;
        }
        if (e->cTransform->pos.y < radius)
        {
            e->cTransform->velocity.y *= -1;
            e->cTransform->pos.y = radius;
        }
        if (e->cTransform->pos.y > (m_window.getSize().y - radius))
        {
            e->cTransform->velocity.y *= -1;
            e->cTransform->pos.y = m_window.getSize().y - radius;
        }
    }
}