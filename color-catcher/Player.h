#pragma once
#include "Entity.h"

class Player : public Entity {
    sf::RectangleShape basket;
    float speed;
    int windowWidth;
public:
    Player(int winWidth);
    void handleInput(); // <--- ADD THIS
    void update(float dt);
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const;
};