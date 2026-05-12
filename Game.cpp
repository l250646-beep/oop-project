#include "Game.h"
#include <string>
#include <cmath>

Game::Game()
    : window(sf::VideoMode(1400, 900), "OOP Tower Defense"),
    pathCount(0),
    enemyCount(0),
    towerCount(0),
    bulletCount(0),
    rockCount(0),
    gold(250),
    lives(15),
    wave(0),
    selectedTower(1),
    enemiesToSpawn(0),
    enemiesSpawned(0),
    enemiesKilled(0),
    enemiesEscaped(0),
    waveActive(false),
    gameOver(false),
    playerWon(false),
    startScreen(true),
    spawnTimer(0.0f),
    spawnInterval(1.0f)
{
    window.setFramerateLimit(60);

    resources.loadAll();

    font.loadFromFile("assest/arial.ttf");
    titleFont.loadFromFile("assest/FORTE.TTF");
    instructionFont.loadFromFile("assest/COLONNA.TTF");

    backgroundMusic.openFromFile("assest/sound/game_music.mp3");
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(35.0f);
    backgroundMusic.play();

    initializeArrays();
    initializePath();
    initializeRocks();
}

Game::~Game() {
    clearEnemies();
    clearTowers();
}

void Game::initializeArrays() {
    for (int i = 0; i < MAX_ENEMIES; i++) enemies[i] = nullptr;
    for (int i = 0; i < MAX_TOWERS; i++) towers[i] = nullptr;
}

void Game::clearEnemies() {
    for (int i = 0; i < enemyCount; i++) {
        delete enemies[i];
        enemies[i] = nullptr;
    }

    enemyCount = 0;
}

void Game::clearTowers() {
    for (int i = 0; i < towerCount; i++) {
        delete towers[i];
        towers[i] = nullptr;
    }

    towerCount = 0;
}

void Game::initializePath() {
    pathCount = 0;

    /*
        Path matched with your latest map.

        Enemy direction:
        Left entry
        Right
        Down
        Right
        Up
        Right exit

        These points are the center line of the dirt path.
    */

    path[pathCount++] = Vector2(-60.0f, 300.0f);
    path[pathCount++] = Vector2(330.0f, 300.0f);

    path[pathCount++] = Vector2(330.0f, 585.0f);
    path[pathCount++] = Vector2(530.0f, 585.0f);

    path[pathCount++] = Vector2(530.0f, 490.0f);
    path[pathCount++] = Vector2(820.0f, 490.0f);

    path[pathCount++] = Vector2(820.0f, 300.0f);
    path[pathCount++] = Vector2(1460.0f, 300.0f);
}

void Game::initializeRocks() {
    rockCount = 0;
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
                startScreen = false;
            }

            if (event.key.code == sf::Keyboard::Num1) selectedTower = 1;
            if (event.key.code == sf::Keyboard::Num2) selectedTower = 2;
            if (event.key.code == sf::Keyboard::Num3) selectedTower = 3;
            if (event.key.code == sf::Keyboard::Num4) selectedTower = 4;
            if (event.key.code == sf::Keyboard::Num5) selectedTower = 5;

            if (event.key.code == sf::Keyboard::R) {
                restart();
            }
        }

        if (!startScreen &&
            !gameOver &&
            !playerWon &&
            event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {

            placeTower(event.mouseButton.x, event.mouseButton.y);
        }
    }
}

void Game::update(float deltaTime) {
    if (startScreen || gameOver || playerWon) return;

    if (!waveActive && enemyCount == 0) {
        startNextWave();
    }

    if (waveActive) {
        spawnTimer += deltaTime;

        if (spawnTimer >= spawnInterval && enemiesSpawned < enemiesToSpawn) {
            spawnEnemy();
            enemiesSpawned++;
            spawnTimer = 0.0f;
        }

        if (enemiesSpawned >= enemiesToSpawn) {
            waveActive = false;
        }
    }

    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i] != nullptr) {
            enemies[i]->update(deltaTime);
        }
    }

    for (int i = 0; i < towerCount; i++) {
        if (towers[i] != nullptr) {
            towers[i]->update(deltaTime);
            towers[i]->attack(enemies, enemyCount);
        }
    }

    for (int i = 0; i < bulletCount; i++) {
        bullets[i].update(deltaTime);
    }

    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i] != nullptr && !enemies[i]->isAlive()) {
            if (enemies[i]->getHp() <= 0.0f) {
                gold += enemies[i]->getReward();
                enemiesKilled++;
            }
            else {
                lives--;
                enemiesEscaped++;
            }
        }
    }

    removeDeadEnemies();
    removeDeadBullets();

    if (lives <= 0) {
        gameOver = true;
    }

    if (wave > 5 && enemyCount == 0 && !waveActive) {
        playerWon = true;
    }
}

void Game::startNextWave() {
    wave++;

    if (wave > 5) return;

    enemiesToSpawn = 5 + wave * 3;
    enemiesSpawned = 0;

    spawnInterval = 1.1f - wave * 0.1f;

    if (spawnInterval < 0.35f) {
        spawnInterval = 0.35f;
    }

    spawnTimer = spawnInterval;
    waveActive = true;
}

void Game::spawnEnemy() {
    if (enemyCount >= MAX_ENEMIES) return;

    Enemy* enemy = nullptr;

    if (wave == 1) {
        enemy = new BasicEnemy(path[0], path, pathCount);
    }
    else if (wave == 2) {
        if (enemiesSpawned % 2 == 0)
            enemy = new FastEnemy(path[0], path, pathCount);
        else
            enemy = new BasicEnemy(path[0], path, pathCount);
    }
    else if (wave == 3) {
        if (enemiesSpawned % 3 == 0)
            enemy = new TankEnemy(path[0], path, pathCount);
        else
            enemy = new FastEnemy(path[0], path, pathCount);
    }
    else if (wave == 4) {
        if (enemiesSpawned % 3 == 0)
            enemy = new FlyingEnemy(path[0], path, pathCount);
        else
            enemy = new TankEnemy(path[0], path, pathCount);
    }
    else {
        if (enemiesSpawned % 5 == 0)
            enemy = new MegaEnemy(path[0], path, pathCount);
        else
            enemy = new FlyingEnemy(path[0], path, pathCount);
    }

    if (enemy == nullptr) return;

    if (enemy->getName() == "BasicEnemy") {
        enemy->setTexture(&resources.getTexture("enemy_basic"), 72.0f);
    }
    else if (enemy->getName() == "FastEnemy") {
        enemy->setTexture(&resources.getTexture("enemy_fast"), 64.0f);
    }
    else if (enemy->getName() == "TankEnemy") {
        enemy->setTexture(&resources.getTexture("enemy_tank"), 82.0f);
    }
    else if (enemy->getName() == "FlyingEnemy") {
        enemy->setTexture(&resources.getTexture("enemy_flying"), 70.0f);
    }
    else if (enemy->getName() == "MegaEnemy") {
        enemy->setTexture(&resources.getTexture("enemy_mega"), 95.0f);
    }

    enemies[enemyCount] = enemy;
    enemyCount++;
}

void Game::placeTower(int mouseX, int mouseY) {
    if (mouseY >= 810) return;
    if (towerCount >= MAX_TOWERS) return;
    if (gold < selectedTowerCost()) return;

    const int cellSize = 80;

    int col = mouseX / cellSize;
    int row = mouseY / cellSize;

    float towerX = col * cellSize + 40.0f;
    float towerY = row * cellSize + 40.0f;

    if (towerX < 20.0f || towerX > 1380.0f) return;
    if (towerY < 20.0f || towerY > 790.0f) return;

    if (isPathTile(col, row)) return;
    if (isTowerOnTile(col, row)) return;

    Vector2 towerPosition(towerX, towerY);

    Tower* tower = nullptr;

    if (selectedTower == 1) tower = new CannonTower(towerPosition);
    else if (selectedTower == 2) tower = new SniperTower(towerPosition);
    else if (selectedTower == 3) tower = new MachineGunTower(towerPosition);
    else if (selectedTower == 4) tower = new SlowTower(towerPosition);
    else tower = new FireTower(towerPosition);

    if (tower == nullptr) return;

    if (tower->getName() == "CannonTower") {
        tower->setTexture(resources.getTexture("tower_cannon"));
    }
    else if (tower->getName() == "SniperTower") {
        tower->setTexture(resources.getTexture("tower_sniper"));
    }
    else if (tower->getName() == "MachineGunTower") {
        tower->setTexture(resources.getTexture("tower_machine"));
    }
    else if (tower->getName() == "SlowTower") {
        tower->setTexture(resources.getTexture("tower_slow"));
    }
    else if (tower->getName() == "FireTower") {
        tower->setTexture(resources.getTexture("tower_fire"));
    }

    tower->setBulletList(bullets, &bulletCount, MAX_BULLETS);

    gold -= tower->getCost();

    towers[towerCount] = tower;
    towerCount++;
}

void Game::removeDeadEnemies() {
    int writeIndex = 0;

    for (int readIndex = 0; readIndex < enemyCount; readIndex++) {
        if (enemies[readIndex] != nullptr && enemies[readIndex]->isAlive()) {
            enemies[writeIndex] = enemies[readIndex];

            if (writeIndex != readIndex) {
                enemies[readIndex] = nullptr;
            }

            writeIndex++;
        }
        else {
            delete enemies[readIndex];
            enemies[readIndex] = nullptr;
        }
    }

    enemyCount = writeIndex;
}

void Game::removeDeadBullets() {
    int writeIndex = 0;

    for (int readIndex = 0; readIndex < bulletCount; readIndex++) {
        if (bullets[readIndex].isAlive()) {
            bullets[writeIndex] = bullets[readIndex];
            writeIndex++;
        }
    }

    bulletCount = writeIndex;
}

bool Game::isPathTile(int col, int row) const {
    const int cellSize = 80;

    float tileCenterX = col * cellSize + 40.0f;
    float tileCenterY = row * cellSize + 40.0f;

    /*
        Exact road blocking for your current zig-zag map.
        This blocks only the dirt path rectangles, not the nearby grass.
    */

    // Road thickness. Reduce to 85 if still blocking grass.
    const float roadHalfWidth = 50.0f;

    // 1. Left horizontal road
    if (tileCenterX >= -60.0f && tileCenterX <= 330.0f &&
        tileCenterY >= 300.0f - roadHalfWidth && tileCenterY <= 300.0f + roadHalfWidth) {
        return true;
    }

    // 2. First vertical road going down
    if (tileCenterX >= 330.0f - roadHalfWidth && tileCenterX <= 330.0f + roadHalfWidth &&
        tileCenterY >= 300.0f && tileCenterY <= 585.0f) {
        return true;
    }

    // 3. Bottom horizontal road
    if (tileCenterX >= 330.0f && tileCenterX <= 530.0f &&
        tileCenterY >= 585.0f - roadHalfWidth && tileCenterY <= 585.0f + roadHalfWidth) {
        return true;
    }

    // 4. Small vertical road going up
    if (tileCenterX >= 530.0f - roadHalfWidth && tileCenterX <= 530.0f + roadHalfWidth &&
        tileCenterY >= 490.0f && tileCenterY <= 585.0f) {
        return true;
    }

    // 5. Middle horizontal road
    if (tileCenterX >= 530.0f && tileCenterX <= 820.0f &&
        tileCenterY >= 490.0f - roadHalfWidth && tileCenterY <= 490.0f + roadHalfWidth) {
        return true;
    }

    // 6. Second vertical road going up
    if (tileCenterX >= 820.0f - roadHalfWidth && tileCenterX <= 820.0f + roadHalfWidth &&
        tileCenterY >= 300.0f && tileCenterY <= 490.0f) {
        return true;
    }

    // 7. Right horizontal road
    if (tileCenterX >= 820.0f && tileCenterX <= 1460.0f &&
        tileCenterY >= 300.0f - roadHalfWidth && tileCenterY <= 300.0f + roadHalfWidth) {
        return true;
    }

    return false;
}

bool Game::isTowerOnTile(int col, int row) const {
    const int cellSize = 80;

    for (int i = 0; i < towerCount; i++) {
        if (towers[i] != nullptr) {
            Vector2 towerPos = towers[i]->getPosition();

            int towerCol = static_cast<int>(towerPos.x) / cellSize;
            int towerRow = static_cast<int>(towerPos.y) / cellSize;

            if (towerCol == col && towerRow == row) {
                return true;
            }
        }
    }

    return false;
}

int Game::selectedTowerCost() const {
    if (selectedTower == 1) return 80;
    if (selectedTower == 2) return 120;
    if (selectedTower == 3) return 65;
    if (selectedTower == 4) return 75;

    return 95;
}

void Game::render() {
    window.clear(sf::Color(20, 35, 30));

    if (startScreen) {
        renderStartScreen();
    }
    else {
        renderMap();

        for (int i = 0; i < towerCount; i++) {
            if (towers[i] != nullptr) {
                towers[i]->render(window);
            }
        }

        for (int i = 0; i < enemyCount; i++) {
            if (enemies[i] != nullptr) {
                enemies[i]->render(window);
            }
        }

        for (int i = 0; i < bulletCount; i++) {
            bullets[i].render(window);
        }

        renderUI();
    }

    window.display();
}

void Game::renderStartScreen() {
    sf::Sprite background;
    background.setTexture(resources.getBackground());

    sf::Vector2u bgSize = resources.getBackground().getSize();

    if (bgSize.x > 0 && bgSize.y > 0) {
        background.setScale(
            1400.0f / static_cast<float>(bgSize.x),
            810.0f / static_cast<float>(bgSize.y)
        );
    }

    background.setPosition(0.0f, 0.0f);
    window.draw(background);

    sf::RectangleShape overlay(sf::Vector2f(1400.0f, 900.0f));
    overlay.setFillColor(sf::Color(0, 0, 0, 105));
    window.draw(overlay);

    sf::RectangleShape card(sf::Vector2f(900.0f, 330.0f));
    card.setPosition(250.0f, 150.0f);
    card.setFillColor(sf::Color(20, 20, 20, 225));
    card.setOutlineColor(sf::Color(255, 190, 80));
    card.setOutlineThickness(5.0f);
    window.draw(card);

    sf::Text title("TOWER DEFENSE", titleFont, 76);
    title.setFillColor(sf::Color(255, 225, 145));
    title.setPosition(410.0f, 185.0f);
    window.draw(title);

    sf::Text controls(
        "Press ENTER to Start\n"
        "Use 1-5 to select towers\n"
        "Click to place your tower\n"
        "Protect the base for 5 waves",
        instructionFont,
        40
    );

    controls.setFillColor(sf::Color(240, 240, 240));
    controls.setPosition(435.0f, 295.0f);
    window.draw(controls);
}

void Game::renderMap() {
    sf::Sprite background;
    background.setTexture(resources.getBackground());

    sf::Vector2u bgSize = resources.getBackground().getSize();

    if (bgSize.x > 0 && bgSize.y > 0) {
        background.setScale(
            1400.0f / static_cast<float>(bgSize.x),
            810.0f / static_cast<float>(bgSize.y)
        );
    }

    background.setPosition(0.0f, 0.0f);
    window.draw(background);

    sf::RectangleShape startPole(sf::Vector2f(6.0f, 70.0f));
    startPole.setPosition(35.0f, 260.0f);
    startPole.setFillColor(sf::Color(80, 50, 25));
    window.draw(startPole);

    sf::ConvexShape startFlag;
    startFlag.setPointCount(3);
    startFlag.setPoint(0, sf::Vector2f(41.0f, 260.0f));
    startFlag.setPoint(1, sf::Vector2f(110.0f, 280.0f));
    startFlag.setPoint(2, sf::Vector2f(41.0f, 300.0f));
    startFlag.setFillColor(sf::Color(30, 190, 60));
    startFlag.setOutlineColor(sf::Color::White);
    startFlag.setOutlineThickness(2.0f);
    window.draw(startFlag);

    sf::RectangleShape endPole(sf::Vector2f(6.0f, 70.0f));
    endPole.setPosition(1345.0f, 260.0f);
    endPole.setFillColor(sf::Color(80, 50, 25));
    window.draw(endPole);

    sf::ConvexShape endFlag;
    endFlag.setPointCount(3);
    endFlag.setPoint(0, sf::Vector2f(1351.0f, 260.0f));
    endFlag.setPoint(1, sf::Vector2f(1280.0f, 280.0f));
    endFlag.setPoint(2, sf::Vector2f(1351.0f, 300.0f));
    endFlag.setFillColor(sf::Color(210, 35, 35));
    endFlag.setOutlineColor(sf::Color::White);
    endFlag.setOutlineThickness(2.0f);
    window.draw(endFlag);
}

void Game::renderUI() {
    sf::RectangleShape panel(sf::Vector2f(1400.0f, 90.0f));
    panel.setPosition(0.0f, 810.0f);
    panel.setFillColor(sf::Color(15, 15, 15, 240));
    window.draw(panel);

    int shownWave = wave;
    if (shownWave > 5) shownWave = 5;

    std::string uiText =
        "Gold: " + std::to_string(gold) +
        "     Lives: " + std::to_string(lives) +
        "     Wave: " + std::to_string(shownWave) + "/5" +
        "     Selected Tower: " + std::to_string(selectedTower) +
        "     Cost: " + std::to_string(selectedTowerCost());

    sf::Text text(uiText, font, 24);
    text.setFillColor(sf::Color::White);
    text.setPosition(25.0f, 825.0f);
    window.draw(text);

    sf::Text controls(
        "1 Cannon $80     2 Sniper $120     3 Machine $65     4 Slow $75     5 Fire $95",
        font,
        22
    );
    controls.setFillColor(sf::Color(255, 220, 120));
    controls.setPosition(25.0f, 860.0f);
    window.draw(controls);

    if (gameOver || playerWon) {
        sf::RectangleShape overlay(sf::Vector2f(1400.0f, 900.0f));
        overlay.setFillColor(sf::Color(0, 0, 0, 190));
        window.draw(overlay);

        sf::RectangleShape card(sf::Vector2f(760.0f, 460.0f));
        card.setPosition(320.0f, 170.0f);
        card.setFillColor(sf::Color(22, 22, 22, 235));
        card.setOutlineColor(sf::Color(255, 190, 80));
        card.setOutlineThickness(5.0f);
        window.draw(card);

        sf::Text result(playerWon ? "VICTORY!" : "GAME OVER", titleFont, 72);
        result.setFillColor(playerWon ? sf::Color(120, 255, 120) : sf::Color(255, 90, 90));
        result.setPosition(515.0f, 210.0f);
        window.draw(result);

        std::string stats =
            "Final Report\n\n"
            "Gold Remaining: " + std::to_string(gold) + "\n"
            "Lives Remaining: " + std::to_string(lives) + "\n"
            "Waves Completed: " + std::to_string(wave > 5 ? 5 : wave) + "/5\n"
            "Towers Placed: " + std::to_string(towerCount) + "\n"
            "Enemies Killed: " + std::to_string(enemiesKilled) + "\n"
            "Enemies Escaped: " + std::to_string(enemiesEscaped) + "\n\n"
            "Press R to Restart";

        sf::Text statsText(stats, instructionFont, 34);
        statsText.setFillColor(sf::Color::White);
        statsText.setPosition(470.0f, 310.0f);
        window.draw(statsText);
    }
}

void Game::restart() {
    clearEnemies();
    clearTowers();

    bulletCount = 0;
    gold = 250;
    lives = 15;
    wave = 0;
    selectedTower = 1;
    enemiesToSpawn = 0;
    enemiesSpawned = 0;
    enemiesKilled = 0;
    enemiesEscaped = 0;
    waveActive = false;
    gameOver = false;
    playerWon = false;
    startScreen = true;
    spawnTimer = 0.0f;
}