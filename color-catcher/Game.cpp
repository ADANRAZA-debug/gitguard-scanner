#include "Game.h"
#include "Menu.h"
#include "utils.h"
#include <ctime>
#include <sstream>

Game::Game()
    : window(sf::VideoMode(600, 650), "Color Catcher Game"),
      player(600),
      score(0), lives(3), highScore(0), level(1), maxLives(3),
      ballSpeed(180.f), spawnTimer(0), spawnDelay(1.0f),
      levelTimer(0), levelInterval(18.0f),
      freezeActive(false), gameOver(false), bonusEffectActive(false),
      freezeTimer(0), bonusEffectTimer(0)
{
    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    targetColor = static_cast<BallColor>(rand() % 4);
}

void Game::startGame() {
    score = 0;
    lives = maxLives;
    level = 1;
    ballSpeed = 180.f;
    spawnDelay = 1.0f;
    levelTimer = 0;
    gameOver = false;
    balls.clear();
    freezeActive = false;
    bonusEffectActive = false;
    targetColor = static_cast<BallColor>(rand() % 4);
}

void Game::spawnBall() {
    balls.emplace_back(600, randomBallColor(), 0, freezeActive ? ballSpeed * 0.5f : ballSpeed);
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        else if (gameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
            startGame();
        }
    }
    if (!gameOver)
        player.handleInput();
}

void Game::handleCatch(Ball& ball) {
    BallColor col = ball.getColor();
    if (col == bonusBall) {
        lives = maxLives;
        triggerBonusEffect();
    } else if (col == freezeBall) {
        freezeActive = true;
        freezeTimer = 3.0f;
    } else if (col == targetColor) {
        score++;
        targetColor = static_cast<BallColor>(rand() % 4);
    } else {
        // If not bonus/freeze, and not the target color, lose a life!
        lives--;
    }
}

void Game::triggerBonusEffect() {
    bonusEffectActive = true;
    bonusEffectTimer = 1.0f;
}

void Game::update(float dt) {
    if (gameOver) return;

    if (freezeActive) {
        freezeTimer -= dt;
        if (freezeTimer <= 0) {
            freezeActive = false;
        }
    }

    // Level up by time
    levelTimer += dt;
    if (levelTimer >= levelInterval) {
        level++;
        // Make game FASTER each level!
        ballSpeed += 40.f; // increase by 40 per level
        if (spawnDelay > 0.30f) spawnDelay -= 0.07f;
        levelTimer = 0;
        for (auto& ball : balls) {
            if (!freezeActive) ball.setSpeed(ballSpeed);
        }
    }

    if (bonusEffectActive) {
        bonusEffectTimer -= dt;
        if (bonusEffectTimer <= 0) bonusEffectActive = false;
    }

    spawnTimer += dt;
    if (spawnTimer >= spawnDelay) {
        spawnBall();
        spawnTimer = 0;
    }

    for (auto& ball : balls) ball.update(dt);

    for (size_t i = 0; i < balls.size();) {
        if (balls[i].getBounds().intersects(player.getBounds())) {
            handleCatch(balls[i]);
            balls.erase(balls.begin() + i);
        } else if (balls[i].isOffScreen()) {
            // Only lose a life if the missed ball is the target color!
            if (balls[i].getColor() == targetColor) {
                lives--;
            }
            balls.erase(balls.begin() + i);
        } else {
            ++i;
        }
    }

    if (score > highScore) highScore = score;
    scoreboard.setScore(score);
    scoreboard.setLives(lives);
    scoreboard.setHighScore(highScore);
    scoreboard.setLevel(level);
    scoreboard.setFreeze(freezeActive);

    if (lives <= 0) {
        gameOver = true;
    }
}

void Game::drawBackground() {
    sf::VertexArray bg(sf::Quads, 4);
    bg[0].position = sf::Vector2f(0, 0);
    bg[1].position = sf::Vector2f(window.getSize().x, 0);
    bg[2].position = sf::Vector2f(window.getSize().x, window.getSize().y);
    bg[3].position = sf::Vector2f(0, window.getSize().y);
    bg[0].color = sf::Color(190, 180, 255);
    bg[1].color = sf::Color(155, 210, 255);
    bg[2].color = sf::Color(170, 255, 200);
    bg[3].color = sf::Color(255, 220, 180);

    window.draw(bg);

    if (bonusEffectActive) {
        sf::RectangleShape flash(sf::Vector2f(window.getSize().x, window.getSize().y));
        flash.setFillColor(sf::Color(255, 255, 0, 100));
        window.draw(flash);
    }
}

void Game::render() {
    window.clear();
    drawBackground();

    player.draw(window);
    for (auto& ball : balls) ball.draw(window);
    scoreboard.draw(window);

    // Show target color
    sf::Font font;
    font.loadFromFile("resources/font.ttf");
    sf::Text t("Catch: " + colorName(targetColor), font, 32);
    t.setFillColor(getSFMLColor(targetColor));
    t.setStyle(sf::Text::Bold | sf::Text::Italic);
    t.setPosition(170, 80);
    window.draw(t);

    if (gameOver) {
        sf::Text over("GAME OVER", font, 58);
        over.setFillColor(sf::Color::Red);
        over.setStyle(sf::Text::Bold | sf::Text::Underlined);
        over.setPosition(100, 260);
        window.draw(over);

        sf::Text restart("Press SPACE to Restart", font, 30);
        restart.setFillColor(sf::Color::Black);
        restart.setPosition(120, 340);
        window.draw(restart);
    }

    window.display();
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}