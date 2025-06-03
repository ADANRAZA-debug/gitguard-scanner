#pragma once
#include "Player.h"
#include "Ball.h"
#include "Scoreboard.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Game {
    sf::RenderWindow window;
    Player player;
    Scoreboard scoreboard;
    std::vector<Ball> balls;
    BallColor targetColor;
    int score, lives, highScore, level, maxLives;
    float ballSpeed, spawnTimer, spawnDelay;
    float levelTimer, levelInterval;
    bool freezeActive, gameOver, bonusEffectActive;
    float freezeTimer, bonusEffectTimer;

    void spawnBall();
    void processEvents();
    void update(float dt);
    void render();
    void handleCatch(Ball& ball);
    void startGame();
    void drawBackground();
    void triggerBonusEffect();

public:
    Game();
    void run();
};