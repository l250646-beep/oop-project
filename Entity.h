#pragma once
#include <SFML/Graphics.hpp>
#include "Vector2.h"

class Entity {
protected:
    Vector2 position;
    bool alive;

public:
    Entity(Vector2 startPosition) : position(startPosition), alive(true) {}
    virtual ~Entity() {}

    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    bool isAlive() const { return alive; }
    Vector2 getPosition() const { return position; }
    void setAlive(bool value) { alive = value; }
};
