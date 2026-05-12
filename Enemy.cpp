#include "Enemy.h"
#include <algorithm>

Enemy::Enemy(Vector2 startPosition, Vector2* pathRef, int totalPathPoints, float hpValue, float speedValue, int goldReward, const std::string& enemyName)
    : Entity(startPosition), hp(hpValue), maxHp(hpValue), speed(speedValue), originalSpeed(speedValue), reward(goldReward), currentWaypoint(1), path(pathRef), pathCount(totalPathPoints), name(enemyName), slowTimer(0.f) {
    fallbackShape.setRadius(18.f);
    fallbackShape.setOrigin(18.f, 18.f);
    fallbackShape.setPosition(position.toSFML());
    fallbackShape.setFillColor(sf::Color::Red);
}

void Enemy::setTexture(sf::Texture* texture, float drawSize) {
    if (texture) {
        sprite.setTexture(*texture);
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        sprite.setScale(drawSize / bounds.width, drawSize / bounds.height);
    }
}

void Enemy::update(float deltaTime) {
    if (slowTimer > 0.f) {
        slowTimer -= deltaTime;
        if (slowTimer <= 0.f) speed = originalSpeed;
    }
    move(deltaTime);
}

void Enemy::move(float deltaTime) {
    if (!alive || path == nullptr || currentWaypoint >= pathCount) return;

    Vector2 target = path[currentWaypoint];
    Vector2 direction = target - position;
    float distance = direction.length();

    if (distance < 4.f) {
        currentWaypoint++;
        if (currentWaypoint >= pathCount) alive = false;
        return;
    }

    Vector2 normalized(direction.x / distance, direction.y / distance);
    position = position + normalized * (speed * deltaTime);
}

void Enemy::takeDamage(float damage) {
    hp -= damage;
    if (hp <= 0.f) {
        hp = 0.f;
        alive = false;
    }
}

void Enemy::applySlow(float factor, float duration) {
    speed = originalSpeed * factor;
    slowTimer = std::max(slowTimer, duration);
}

void Enemy::render(sf::RenderWindow& window) {
    if (sprite.getTexture()) {
        sprite.setPosition(position.toSFML());
        window.draw(sprite);
    } else {
        fallbackShape.setPosition(position.toSFML());
        window.draw(fallbackShape);
    }

    sf::RectangleShape barBack(sf::Vector2f(42.f, 6.f));
    barBack.setPosition(position.x - 21.f, position.y - 32.f);
    barBack.setFillColor(sf::Color(35, 35, 35));
    window.draw(barBack);

    float hpRatio = maxHp > 0.f ? hp / maxHp : 0.f;
    sf::RectangleShape barFront(sf::Vector2f(42.f * hpRatio, 6.f));
    barFront.setPosition(position.x - 21.f, position.y - 32.f);
    barFront.setFillColor(sf::Color(50, 230, 70));
    window.draw(barFront);
}

BasicEnemy::BasicEnemy(Vector2 startPosition, Vector2* pathRef, int totalPathPoints)
    : Enemy(startPosition, pathRef, totalPathPoints, 80.f, 75.f, 15, "BasicEnemy") {
    fallbackShape.setFillColor(sf::Color(230, 70, 70));
}

void BasicEnemy::move(float deltaTime) { Enemy::move(deltaTime); }

FastEnemy::FastEnemy(Vector2 startPosition, Vector2* pathRef, int totalPathPoints)
    : Enemy(startPosition, pathRef, totalPathPoints, 45.f, 135.f, 20, "FastEnemy") {
    fallbackShape.setFillColor(sf::Color::Yellow);
}

void FastEnemy::move(float deltaTime) { Enemy::move(deltaTime); }

TankEnemy::TankEnemy(Vector2 startPosition, Vector2* pathRef, int totalPathPoints)
    : Enemy(startPosition, pathRef, totalPathPoints, 220.f, 45.f, 35, "TankEnemy") {
    fallbackShape.setFillColor(sf::Color(120, 120, 120));
    fallbackShape.setRadius(22.f);
    fallbackShape.setOrigin(22.f, 22.f);
}

void TankEnemy::takeDamage(float damage) { Enemy::takeDamage(damage * 0.85f); }

FlyingEnemy::FlyingEnemy(Vector2 startPosition, Vector2* pathRef, int totalPathPoints)
    : Enemy(startPosition, pathRef, totalPathPoints, 70.f, 105.f, 28, "FlyingEnemy") {
    exitPoint = (pathRef != nullptr && totalPathPoints > 0) ? pathRef[totalPathPoints - 1] : Vector2(850.f, 280.f);
    fallbackShape.setFillColor(sf::Color(120, 210, 255));
}

void FlyingEnemy::move(float deltaTime) {
    Vector2 direction = exitPoint - position;
    float distance = direction.length();
    if (distance < 6.f) {
        alive = false;
        return;
    }
    Vector2 normalized(direction.x / distance, direction.y / distance);
    position = position + normalized * (speed * deltaTime);
}

MegaEnemy::MegaEnemy(Vector2 startPosition, Vector2* pathRef, int totalPathPoints)
    : Enemy(
        startPosition,
        pathRef,
        totalPathPoints,
        380.f,
        68.f,
        65,
        "MegaEnemy"
    ),
    regenerationTimer(0.f)
{
    fallbackShape.setFillColor(
        sf::Color(130, 40, 180)
    );

    fallbackShape.setRadius(25.f);

    fallbackShape.setOrigin(
        25.f,
        25.f
    );
}

void MegaEnemy::update(float deltaTime) {

    regenerationTimer += deltaTime;

    if (regenerationTimer >= 0.5f &&
        hp > 0.f &&
        hp < maxHp) {

        hp = std::min(
            maxHp,
            hp + 12.f
        );

        regenerationTimer = 0.f;
    }

    Enemy::update(deltaTime);
}

void MegaEnemy::takeDamage(float damage) {

    Enemy::takeDamage(
        damage * 0.72f
    );
}

