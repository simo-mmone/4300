#include "Game.h"

#include <iostream>
#include <fstream>

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

    // std::string cmdName;
    // while (fin >> cmdName)
    // {
    //     if (cmdName == "Window")
    //     {
    //         fin >> videoMode.width >> videoMode.height >> frameRate >> fullScreen;
    //     }
    //     else if (cmdName == "Font")
    //     {
    //     }
    //     else if (cmdName == "Player")
    //     {
    //         // fin >> m_playerConfig.SR >> m_playerConfig.CR;
    //     }
    //     else if (cmdName == "Enemy")
    //     {
    //     }
    //     else if (cmdName == "Bullet")
    //     {
    //     }
    // }
    
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

    float ex = rand() % m_window.getSize().x;
    float ey = rand() % m_window.getSize().y;

    entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(0.0f, 0.0f), 0.0f);
    entity->cShape = std::make_shared<CShape>(16.0f, 3, sf::Color(0, 0, 255), sf::Color(255, 255, 255), 4.0f);

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
    float bulletSpeed = 5;
    direction *= bulletSpeed;

    bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, direction, 0.0f);
    bullet->cShape = std::make_shared<CShape>(2.0f, 20, sf::Color(255, 255, 255), sf::Color(255, 255, 255), 1.0f);
    bullet->cLifespan = std::make_shared<CLifespan>(20);
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
                Vec2 mousePos = Vec2(event.mouseButton.x, event.mouseButton.y);
                m_player->cInput->shoot = true;
                spawnBullet(m_player, mousePos);
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                Vec2 mousePos = Vec2(event.mouseButton.x, event.mouseButton.y);
                m_player->cInput->special = true;
            }
        }
    }
}

void Game::sCollision()
{
    for (auto b : m_entities.getEntities("bullet"))
    {
        for (auto e : m_entities.getEntities("enemy"))
        {
            // TODO collisions
        }
    }
}

void Game::sEnemySpawner()
{
    if (m_currentFrame - m_lastEnemySpawnTime > 180)
    {
        spawnEnemy();
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
    }
}