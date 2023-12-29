#pragma once

#include "Entity.h"
#include "EntityManager.h"

#include <SFML/Graphics.hpp>

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

class Game
{
    /// @brief the window we will draw to
    sf::RenderWindow m_window;
    /// @brief vector of entities to maintain
    EntityManager m_entities;
    /// @brief the font we will use to draw
    sf::Font m_font;
    /// @brief the score text to be drawn to the screen
    sf::Text m_text;
    PlayerConfig m_playerConfig;
    EnemyConfig m_enemyConfig;
    BulletConfig m_bulletConfig;
    int m_score = 0;
    int m_currentFrame = 0;
    int m_lastEnemySpawnTime = 0;
    /// @brief whether we update game logic
    bool m_paused = false;
    /// @brief whether the game is running
    bool m_running = true;
    Vec2 m_lastShootPos = Vec2(0,0);

    std::shared_ptr<Entity> m_player;

    /// @brief initialize the GameState with a config file path
    /// @param config 
    void init(const std::string & config);
    /// @brief pause the game
    void setPaused(bool paused);

    /// @brief System: Entity position / movement update
    void sMovement();
    /// @brief System: User input
    void sUserInput();
    /// @brief System: Lifespan
    void sLifespan();
    /// @brief System: Render / drawing
    void sRender();
    /// @brief System: Spawn enemies
    void sEnemySpawner();
    /// @brief System: Collisions
    void sCollision();
    /// @brief System: Shoot
    void sShoot();

    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemies(std::shared_ptr<Entity> entity);
    void spawnBullet(std::shared_ptr<Entity> entity, const Vec2 & mousePos);
    void spawnSpecialWeapon(std::shared_ptr<Entity> entity);
    bool IsColliding(const Vec2 & pos1, const Vec2 & pos2, const std::string & tag1, const std:: string & tag2);
    int tagToRadius(const std::string & tag);

public:

    /// @brief constructor
    /// @param config game config file path
    Game(const std::string & config);

    void run();
};