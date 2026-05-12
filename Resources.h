#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Resources {
private:
    sf::Texture backgroundTexture;

    sf::Texture enemyBasic;
    sf::Texture enemyFast;
    sf::Texture enemyTank;
    sf::Texture enemyFlying;
    sf::Texture enemyMega;

    sf::Texture towerCannon;
    sf::Texture towerSniper;
    sf::Texture towerMachine;
    sf::Texture towerSlow;
    sf::Texture towerFire;

public:
    void loadAll();

    sf::Texture& getTexture(const std::string& name);
    sf::Texture& getBackground();
};