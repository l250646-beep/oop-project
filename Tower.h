#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Vector2.h"
#include "Enemy.h"
#include "Bullet.h"

class Tower {
protected:
    Vector2 position;
    float range;
    float damage;
    float fireRate;
    float fireTimer;
    int cost;

    sf::Sprite sprite;
    sf::CircleShape fallbackShape;

    Bullet* bullets;
    int* bulletCount;
    int maxBullets;

public:
    Tower(Vector2 pos, float towerRange, float towerDamage, float towerFireRate, int towerCost);
    virtual ~Tower();

    virtual void update(float deltaTime);
    virtual void attack(Enemy* enemies[], int enemyCount) = 0;
    virtual void render(sf::RenderWindow& window);
    virtual std::string getName() const = 0;

    Vector2 getPosition() const;
    float getRange() const;
    float getDamage() const;
    int getCost() const;

    void setTexture(sf::Texture& texture);
    void setBulletList(Bullet bulletArray[], int* countPointer, int maxCount);

protected:
    Enemy* findNearestEnemy(Enemy* enemies[], int enemyCount);
    Enemy* findStrongestEnemy(Enemy* enemies[], int enemyCount);
    bool isEnemyInRange(Enemy* enemy);
    void shootBullet(Vector2 targetPosition, sf::Color color);
};

class CannonTower : public Tower {
public:
    CannonTower(Vector2 pos);
    void attack(Enemy* enemies[], int enemyCount) override;
    std::string getName() const override;
};

class SniperTower : public Tower {
public:
    SniperTower(Vector2 pos);
    void attack(Enemy* enemies[], int enemyCount) override;
    std::string getName() const override;
};

class MachineGunTower : public Tower {
public:
    MachineGunTower(Vector2 pos);
    void attack(Enemy* enemies[], int enemyCount) override;
    std::string getName() const override;
};

class SlowTower : public Tower {
public:
    SlowTower(Vector2 pos);
    void attack(Enemy* enemies[], int enemyCount) override;
    std::string getName() const override;
};

class FireTower : public Tower {
public:
    FireTower(Vector2 pos);
    void attack(Enemy* enemies[], int enemyCount) override;
    std::string getName() const override;
};