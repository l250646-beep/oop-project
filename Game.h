#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameConfig.h"
#include "Enemy.h"
#include "Tower.h"
#include "Resources.h"
#include "Bullet.h"

class Game {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Font titleFont;
    sf::Font instructionFont;
    sf::Music backgroundMusic;
    Resources resources;

    Vector2 path[MAX_PATH_POINTS];
    int pathCount;

    Enemy* enemies[MAX_ENEMIES];
    int enemyCount;

    Tower* towers[MAX_TOWERS];
    int towerCount;

    Bullet bullets[MAX_BULLETS];
    int bulletCount;

    sf::CircleShape rocks[MAX_ROCKS];
    int rockCount;

    int gold;
    int lives;
    int wave;
    int selectedTower;
    int enemiesToSpawn;
    int enemiesSpawned;
    int enemiesKilled;
    int enemiesEscaped;
    bool waveActive;
    bool gameOver;
    bool playerWon;
    bool startScreen;

    float spawnTimer;
    float spawnInterval;

    void initializeArrays();
    void initializePath();
    void initializeRocks();

    void processEvents();
    void update(float deltaTime);
    void render();

    void renderStartScreen();
    void renderMap();
    void renderUI();

    void startNextWave();
    void spawnEnemy();
    void placeTower(int mouseX, int mouseY);

    bool isPathTile(int col, int row) const;
    bool isTowerOnTile(int col, int row) const;

    int selectedTowerCost() const;

    void removeDeadEnemies();
    void removeDeadBullets();
    void clearEnemies();
    void clearTowers();
    void restart();

public:
    Game();
    ~Game();

    void run();
};