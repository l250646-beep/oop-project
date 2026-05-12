#pragma once
#include <SFML/Graphics.hpp>
#include "Vector2.h"

class Bullet {
private:
    Vector2 position;
    Vector2 target;
    float speed;
    bool alive;
    sf::CircleShape shape;

public:
    Bullet();

    Bullet(Vector2 startPosition, Vector2 targetPosition, sf::Color color);

    void update(float deltaTime);
    void render(sf::RenderWindow& window);

    bool isAlive() const;
};