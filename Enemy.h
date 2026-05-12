#pragma once
#include "Entity.h"
#include <string>

class Enemy : public Entity {
protected:
    float hp;
    float maxHp;
    float speed;
    float originalSpeed;
    int reward;
    int currentWaypoint;
    Vector2* path;
    int pathCount;
    sf::Sprite sprite;
    sf::CircleShape fallbackShape;
    std::string name;
    float slowTimer;

public:
    Enemy(Vector2 startPosition, Vector2* pathRef, int totalPathPoints, float hpValue, float speedValue, int goldReward, const std::string& enemyName);
    virtual ~Enemy() {}

    virtual void update(float deltaTime) override;
    virtual void move(float deltaTime);
    virtual void takeDamage(float damage);
    virtual void render(sf::RenderWindow& window) override;

    void setTexture(sf::Texture* texture, float drawSize = 42.f);
    void applySlow(float factor, float duration);
    int getReward() const { return reward; }
    float getHp() const { return hp; }
    float getMaxHp() const { return maxHp; }
    std::string getName() const { return name; }
};

class BasicEnemy : public Enemy {
public:
    BasicEnemy(Vector2 startPosition, Vector2* pathRef, int totalPathPoints);
    void move(float deltaTime) override;
};

class FastEnemy : public Enemy {
public:
    FastEnemy(Vector2 startPosition, Vector2* pathRef, int totalPathPoints);
    void move(float deltaTime) override;
};

class TankEnemy : public Enemy {
public:
    TankEnemy(Vector2 startPosition, Vector2* pathRef, int totalPathPoints);
    void takeDamage(float damage) override;
};

class FlyingEnemy : public Enemy {
private:
    Vector2 exitPoint;
public:
    FlyingEnemy(Vector2 startPosition, Vector2* pathRef, int totalPathPoints);
    void move(float deltaTime) override;
};

class MegaEnemy : public Enemy {
private:
    float regenerationTimer;

public:
    MegaEnemy(Vector2 startPosition, Vector2* pathRef, int totalPathPoints);
    void update(float deltaTime) override;
    void takeDamage(float damage) override;
};
