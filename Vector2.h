#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>

struct Vector2 {
    float x;
    float y;

    Vector2(float px = 0.f, float py = 0.f) : x(px), y(py) {}

    Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
    Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
    Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }

    float length() const { return std::sqrt(x * x + y * y); }
    sf::Vector2f toSFML() const { return sf::Vector2f(x, y); }
};
