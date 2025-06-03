#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>

enum BallColor { Red, Green, Blue, Yellow, bonusBall, freezeBall };

class Ball : public Entity {
    sf::CircleShape shape;
    float speed;
    BallColor color;
public:
    Ball(int winWidth, BallColor col, float y, float speed = 180.f);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const;
    BallColor getColor() const;
    bool isOffScreen() const;
    void setSpeed(float newSpeed);
};