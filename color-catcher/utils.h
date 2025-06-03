#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Ball.h"

BallColor randomBallColor();
sf::Color getSFMLColor(BallColor color);
std::string colorName(BallColor color);