#include "Scoreboard.h"
#include <fstream>
#include <sstream>

Scoreboard::Scoreboard() : score(0), lives(3), highScore(0), level(1), freezeActive(false) {
    font.loadFromFile("resources/font.ttf");
}

void Scoreboard::setScore(int s) { score = s; }
void Scoreboard::setLives(int l) { lives = l; }
void Scoreboard::setHighScore(int h) { highScore = h; }
void Scoreboard::setLevel(int lvl) { level = lvl; }
void Scoreboard::setFreeze(bool active) { freezeActive = active; }

void Scoreboard::draw(sf::RenderWindow& window) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);

    std::ostringstream ss;
    ss << "Score: " << score << "  Lives: " << lives << "  Level: " << level << "  High Score: " << highScore;
    text.setString(ss.str());
    text.setPosition(10, 10);
    window.draw(text);

    if (freezeActive) {
        sf::Text fr("Freeze!", font, 28);
        fr.setFillColor(sf::Color::Cyan);
        fr.setStyle(sf::Text::Bold | sf::Text::Underlined);
        fr.setPosition(350, 40);
        window.draw(fr);
    }
}