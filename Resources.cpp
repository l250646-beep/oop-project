#include "Resources.h"

void Resources::loadAll() {
    backgroundTexture.loadFromFile("assest/background.png");

    enemyBasic.loadFromFile("assest/enemy1.png");
    enemyFast.loadFromFile("assest/enemy2.png");
    enemyTank.loadFromFile("assest/enemy3.png");
    enemyFlying.loadFromFile("assest/enemy4.png");
    enemyMega.loadFromFile("assest/enemy5.png");

    towerCannon.loadFromFile("assest/tower1.png");
    towerSniper.loadFromFile("assest/tower2.png");
    towerMachine.loadFromFile("assest/tower3.png");
    towerSlow.loadFromFile("assest/tower4.png");
    towerFire.loadFromFile("assest/tower5.png");
}

sf::Texture& Resources::getBackground() {
    return backgroundTexture;
}

sf::Texture& Resources::getTexture(const std::string& name) {
    if (name == "enemy_basic") return enemyBasic;
    if (name == "enemy_fast") return enemyFast;
    if (name == "enemy_tank") return enemyTank;
    if (name == "enemy_flying") return enemyFlying;
    if (name == "enemy_mega") return enemyMega;

    if (name == "tower_cannon") return towerCannon;
    if (name == "tower_sniper") return towerSniper;
    if (name == "tower_machine") return towerMachine;
    if (name == "tower_slow") return towerSlow;
    if (name == "tower_fire") return towerFire;

    return enemyBasic;
}