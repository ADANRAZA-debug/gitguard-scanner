#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Scoreboard {
    int score, lives, highScore, level;
    bool freezeActive;
    sf::Font font;
public:
    Scoreboard();
    void setScore(int s);
    void setLives(int l);
    void setHighScore(int h);
    void setLevel(int lvl);
    void setFreeze(bool active);
    void draw(sf::RenderWindow& window);
};