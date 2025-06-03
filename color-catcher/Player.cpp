#include "Player.h"
#include <SFML/Window/Keyboard.hpp>

Player::Player(int winWidth) : speed(350.f), windowWidth(winWidth) {
    basket.setSize(sf::Vector2f(100, 30));
    basket.setFillColor(sf::Color::White);
    basket.setOutlineColor(sf::Color::Black);
    basket.setOutlineThickness(2);
    basket.setPosition((winWidth - 100) / 2, 600);
}

void Player::handleInput() {
    float move = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        move -= speed / 60.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        move += speed / 60.f;
    float newX = basket.getPosition().x + move;
    if (newX < 0) newX = 0;
    if (newX > windowWidth - basket.getSize().x) newX = windowWidth - basket.getSize().x;
    basket.setPosition(newX, basket.getPosition().y);
}

void Player::update(float dt) {
    // If you want per-frame logic, put here
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(basket);
}

sf::FloatRect Player::getBounds() const {
    return basket.getGlobalBounds();
}