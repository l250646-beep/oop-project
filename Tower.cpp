#include "Tower.h"
#include <cmath>

Tower::Tower(Vector2 pos, float towerRange, float towerDamage, float towerFireRate, int towerCost)
    : position(pos),
    range(towerRange),
    damage(towerDamage),
    fireRate(towerFireRate),
    fireTimer(0.0f),
    cost(towerCost),
    bullets(nullptr),
    bulletCount(nullptr),
    maxBullets(0)
{
    fallbackShape.setRadius(28.0f);
    fallbackShape.setOrigin(28.0f, 28.0f);
    fallbackShape.setPosition(position.x, position.y);
    fallbackShape.setFillColor(sf::Color::Blue);
    fallbackShape.setOutlineColor(sf::Color::White);
    fallbackShape.setOutlineThickness(2.0f);
}

Tower::~Tower() {
}

void Tower::update(float deltaTime) {
    fireTimer += deltaTime;
}

void Tower::render(sf::RenderWindow& window) {
    if (sprite.getTexture() != nullptr) {
        window.draw(sprite);
    }
    else {
        window.draw(fallbackShape);
    }
}

Vector2 Tower::getPosition() const {
    return position;
}

float Tower::getRange() const {
    return range;
}

float Tower::getDamage() const {
    return damage;
}

int Tower::getCost() const {
    return cost;
}

void Tower::setTexture(sf::Texture& texture) {

    sprite.setTexture(texture);

    sf::Vector2u size = texture.getSize();

    if (size.x > 0 && size.y > 0) {

        sprite.setTextureRect(
            sf::IntRect(
                0,
                0,
                size.x,
                size.y
            )
        );

        sprite.setOrigin(
            size.x / 2.0f,
            size.y / 2.0f
        );

        sprite.setScale(
            170.0f / static_cast<float>(size.x),
            170.0f / static_cast<float>(size.y)
        );

        sprite.setPosition(
            position.x,
            position.y - 18.0f
        );
    }
}

void Tower::setBulletList(Bullet bulletArray[], int* countPointer, int maxCount) {
    bullets = bulletArray;
    bulletCount = countPointer;
    maxBullets = maxCount;
}

bool Tower::isEnemyInRange(Enemy* enemy) {
    if (enemy == nullptr) return false;

    Vector2 enemyPosition = enemy->getPosition();

    float dx = enemyPosition.x - position.x;
    float dy = enemyPosition.y - position.y;

    float distance = std::sqrt(dx * dx + dy * dy);

    return distance <= range;
}

Enemy* Tower::findNearestEnemy(Enemy* enemies[], int enemyCount) {

    Enemy* nearest = nullptr;

    float nearestDistance = 999999.0f;

    for (int i = 0; i < enemyCount; i++) {

        if (enemies[i] != nullptr &&
            enemies[i]->isAlive() &&
            isEnemyInRange(enemies[i])) {

            Vector2 enemyPosition =
                enemies[i]->getPosition();

            float dx =
                enemyPosition.x - position.x;

            float dy =
                enemyPosition.y - position.y;

            float distance =
                std::sqrt(dx * dx + dy * dy);

            if (distance < nearestDistance) {

                nearestDistance = distance;

                nearest = enemies[i];
            }
        }
    }

    return nearest;
}

Enemy* Tower::findStrongestEnemy(Enemy* enemies[], int enemyCount) {

    Enemy* strongest = nullptr;

    float highestHp = -1.0f;

    for (int i = 0; i < enemyCount; i++) {

        if (enemies[i] != nullptr &&
            enemies[i]->isAlive() &&
            isEnemyInRange(enemies[i])) {

            if (enemies[i]->getHp() > highestHp) {

                highestHp =
                    enemies[i]->getHp();

                strongest = enemies[i];
            }
        }
    }

    return strongest;
}

void Tower::shootBullet(Vector2 targetPosition, sf::Color color) {

    if (bullets == nullptr ||
        bulletCount == nullptr)
        return;

    if (*bulletCount >= maxBullets)
        return;

    bullets[*bulletCount] =
        Bullet(position, targetPosition, color);

    (*bulletCount)++;
}

CannonTower::CannonTower(Vector2 pos)
    : Tower(pos, 260.0f, 35.0f, 1.2f, 80)
{
    fallbackShape.setFillColor(
        sf::Color(120, 120, 120)
    );
}

void CannonTower::attack(Enemy* enemies[], int enemyCount) {

    if (fireTimer < fireRate)
        return;

    Enemy* target =
        findStrongestEnemy(enemies, enemyCount);

    if (target != nullptr) {

        target->takeDamage(damage);

        shootBullet(
            target->getPosition(),
            sf::Color(255, 180, 40)
        );

        fireTimer = 0.0f;
    }
}

std::string CannonTower::getName() const {
    return "CannonTower";
}

SniperTower::SniperTower(Vector2 pos)
    : Tower(pos, 420.0f, 50.0f, 1.8f, 120)
{
    fallbackShape.setFillColor(
        sf::Color(80, 160, 220)
    );
}

void SniperTower::attack(Enemy* enemies[], int enemyCount) {

    if (fireTimer < fireRate)
        return;

    Enemy* target =
        findNearestEnemy(enemies, enemyCount);

    if (target != nullptr) {

        target->takeDamage(damage);

        shootBullet(
            target->getPosition(),
            sf::Color(120, 210, 255)
        );

        fireTimer = 0.0f;
    }
}

std::string SniperTower::getName() const {
    return "SniperTower";
}

MachineGunTower::MachineGunTower(Vector2 pos)
    : Tower(pos, 240.0f, 12.0f, 0.25f, 65)
{
    fallbackShape.setFillColor(
        sf::Color(210, 210, 80)
    );
}

void MachineGunTower::attack(Enemy* enemies[], int enemyCount) {

    if (fireTimer < fireRate)
        return;

    Enemy* target =
        findNearestEnemy(enemies, enemyCount);

    if (target != nullptr) {

        target->takeDamage(damage);

        shootBullet(
            target->getPosition(),
            sf::Color(255, 255, 80)
        );

        fireTimer = 0.0f;
    }
}

std::string MachineGunTower::getName() const {
    return "MachineGunTower";
}

SlowTower::SlowTower(Vector2 pos)
    : Tower(pos, 230.0f, 4.0f, 0.8f, 75)
{
    fallbackShape.setFillColor(
        sf::Color(100, 180, 255)
    );
}

void SlowTower::attack(Enemy* enemies[], int enemyCount) {

    if (fireTimer < fireRate)
        return;

    bool attacked = false;

    for (int i = 0; i < enemyCount; i++) {

        if (enemies[i] != nullptr &&
            enemies[i]->isAlive() &&
            isEnemyInRange(enemies[i])) {

            enemies[i]->takeDamage(damage);

            enemies[i]->applySlow(
                0.55f,
                1.0f
            );

            shootBullet(
                enemies[i]->getPosition(),
                sf::Color(90, 220, 255)
            );

            attacked = true;
        }
    }

    if (attacked) {
        fireTimer = 0.0f;
    }
}

std::string SlowTower::getName() const {
    return "SlowTower";
}

FireTower::FireTower(Vector2 pos)
    : Tower(pos, 250.0f, 18.0f, 0.65f, 95)
{
    fallbackShape.setFillColor(
        sf::Color(230, 80, 35)
    );
}

void FireTower::attack(Enemy* enemies[], int enemyCount) {

    if (fireTimer < fireRate)
        return;

    Enemy* target =
        findNearestEnemy(enemies, enemyCount);

    if (target != nullptr) {

        Vector2 targetPosition =
            target->getPosition();

        for (int i = 0; i < enemyCount; i++) {

            if (enemies[i] != nullptr &&
                enemies[i]->isAlive()) {

                Vector2 enemyPosition =
                    enemies[i]->getPosition();

                float dx =
                    enemyPosition.x - targetPosition.x;

                float dy =
                    enemyPosition.y - targetPosition.y;

                float distance =
                    std::sqrt(dx * dx + dy * dy);

                if (distance <= 70.0f) {

                    enemies[i]->takeDamage(damage);
                }
            }
        }

        shootBullet(
            Vector2(targetPosition.x - 18.f, targetPosition.y - 18.f),
            sf::Color(255, 80, 20)
        );

        shootBullet(
            Vector2(targetPosition.x + 18.f, targetPosition.y - 18.f),
            sf::Color(255, 120, 20)
        );

        shootBullet(
            Vector2(targetPosition.x - 18.f, targetPosition.y + 18.f),
            sf::Color(255, 160, 20)
        );

        shootBullet(
            Vector2(targetPosition.x + 18.f, targetPosition.y + 18.f),
            sf::Color(255, 200, 20)
        );

        fireTimer = 0.0f;
    }
}

std::string FireTower::getName() const {
    return "FireTower";
}