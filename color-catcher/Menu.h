#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
using namespace std;
class Menu {
    sf::Font font;
    std::vector<sf::Text> items;
    int selectedIndex;
public:
    Menu(const std::vector<std::string>& options, int winWidth, int winHeight);
    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    int getSelectedIndex() const;
};