#include "utils.h"
#include <cstdlib>

// Make bonus and freeze balls appear less rare (1 in 8 chance for bonus, 1 in 12 for freeze)
BallColor randomBallColor() {
    int n = rand() % 12;
    if (n == 0) return bonusBall;      // 1 in 12 chance for Bonus
    if (n == 1) return freezeBall;     // 1 in 12 chance for Freeze
    return static_cast<BallColor>(rand() % 4); // Red, Green, Blue, Yellow
}

sf::Color getSFMLColor(BallColor c) {
    switch (c) {
        case Red:    return sf::Color::Red;
        case Green:  return sf::Color::Green;
        case Blue:   return sf::Color::Blue;
        case Yellow: return sf::Color::Yellow;
        case bonusBall:  return sf::Color(255, 215, 0); // Gold
        case freezeBall: return sf::Color::Cyan;
        default:     return sf::Color::White;
    }
}

std::string colorName(BallColor color) {
    switch (color) {
        case Red: return "Red";
        case Green: return "Green";
        case Blue: return "Blue";
        case Yellow: return "Yellow";
        case bonusBall: return "Bonus!";
        case freezeBall: return "Freeze!";
        default: return "Unknown";
    }
}