#include "Bullet.h"
#include <cmath>

Bullet::Bullet()
    : position(0.0f, 0.0f),
    target(0.0f, 0.0f),
    speed(500.0f),
    alive(false)
{
    shape.setRadius(7.0f);
    shape.setOrigin(7.0f, 7.0f);
    shape.setFillColor(sf::Color::Yellow);
}

Bullet::Bullet(Vector2 startPosition, Vector2 targetPosition, sf::Color color)
    : position(startPosition),
    target(targetPosition),
    speed(500.0f),
    alive(true)
{
    shape.setRadius(7.0f);
    shape.setOrigin(7.0f, 7.0f);
    shape.setFillColor(color);
    shape.setPosition(position.x, position.y);
}

void Bullet::update(float deltaTime) {
    if (!alive) return;

    float dx = target.x - position.x;
    float dy = target.y - position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < 8.0f) {
        alive = false;
        return;
    }

    if (distance != 0.0f) {
        position.x += (dx / distance) * speed * deltaTime;
        position.y += (dy / distance) * speed * deltaTime;
    }

    shape.setPosition(position.x, position.y);
}

void Bullet::render(sf::RenderWindow& window) {
    if (alive) {
        window.draw(shape);
    }
}

bool Bullet::isAlive() const {
    return alive;
}