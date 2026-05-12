# oop-project-amna
Tower Defense Game
OOP Semester Project

Project Overview

This is my Object Oriented Programming semester project. I made a 2D Tower Defense game in C++ using SFML.

In this game, the player has to place towers on the map to stop enemies from reaching the end of the path. The enemies move on a fixed path and the towers attack them when they come in range. The player gets gold after killing enemies and can use that gold to place more towers.

The game includes different types of enemies, different towers, multiple waves, lives, gold system, game over condition, and winning condition.

GUI Library Used

I used SFML for this project. SFML is used for the game window, graphics, sprites, mouse clicks, keyboard input, fonts, and background music.

How to Compile and Run

Requirements

Visual Studio
C++ compiler
SFML library installed and configured
All project files
Assest folder with images, fonts, and sound files

Steps to Run

1. Open the project in Visual Studio.
2. Make sure SFML is properly linked in the project settings.
3. Select Debug mode and x64 platform.
4. Build the project.
5. Run the project.
6. Press Enter to start the game.
7. Use number keys 1 to 5 to select towers.
8. Click on the grass area to place towers.
9. Press R to restart the game after winning or losing.

Controls

Enter: Start the game
1: Select Cannon Tower
2: Select Sniper Tower
3: Select Machine Gun Tower
4: Select Slow Tower
5: Select Fire Tower
Left Mouse Click: Place tower
R: Restart game

Game Features

2D graphical game window
Tower defense map with visible enemy path
Enemies follow a fixed path
Player can place towers using mouse click
Towers cannot be placed on the path
Different enemy types
Different tower types
5 progressive waves
Gold system
Lives system
Enemy health bars
Game over screen
Victory screen
Background music

OOP Concepts Used

Inheritance

Inheritance is used in the project to create a proper class structure.

Entity is the base class.
Enemy and Tower are derived from Entity.
Different enemy classes are derived from Enemy.
Different tower classes are derived from Tower.

Polymorphism

Polymorphism is used by storing enemies and towers using base class pointers.

For example:

Enemy pointer array is used to store different enemy objects.
Tower pointer array is used to store different tower objects.

This allows the game to handle different types of enemies and towers through common base class pointers.

Encapsulation

Class data is kept private or protected where needed. Getter and setter functions are used to access or update values safely.

Constructors and Destructors

Constructors are used to initialize enemies, towers, bullets, and game objects. Destructors and cleanup functions are used to delete dynamically created objects and avoid memory leaks.

Operator Overloading

Operator overloading is used in the Vector2 class to make vector calculations easier.

Enemy Types

BasicEnemy
FastEnemy
TankEnemy
FlyingEnemy
MegaEnemy

Tower Types

CannonTower
SniperTower
MachineGunTower
SlowTower
FireTower

Known Issues and Limitations

The game currently has only one map.
Tower placement is grid based.
Tower upgrade feature is not added.
The game is designed for a 1400 x 900 window size.
The path restriction depends on the current background map.

Submission

This project is submitted as the Object Oriented Programming semester project.
