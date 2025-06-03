#include "Ball.h"
#include "utils.h"
#include <cstdlib>

Ball::Ball(int winWidth, BallColor col, float y, float spd)
    : speed(spd), color(col)
{
    shape.setRadius(20);
    shape.setFillColor(getSFMLColor(col));
    if (col == freezeBall) {
        shape.setPointCount(6); // diamond for Freeze
        shape.setOutlineColor(sf::Color::Cyan);
    } else if (col == bonusBall) {
        shape.setOutlineColor(sf::Color(255, 215, 0));
    } else {
        shape.setOutlineColor(sf::Color::Black);
    }
    shape.setOutlineThickness(2);

    float x = 40 + rand() % (winWidth - 80);
    shape.setPosition(x, y);
}

void Ball::update(float dt) {
    shape.move(0, speed * dt);
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Ball::getBounds() const {
    return shape.getGlobalBounds();
}

BallColor Ball::getColor() const {
    return color;
}

bool Ball::isOffScreen() const {
    return shape.getPosition().y > 600;
}

void Ball::setSpeed(float newSpeed) { speed = newSpeed; }