#include "Menu.h"

Menu::Menu(const std::vector<std::string>& options, int winWidth, int winHeight) : selectedIndex(0) {
    font.loadFromFile("resources/font.ttf");
    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text txt;
        txt.setFont(font);
        txt.setString(options[i]);
        txt.setCharacterSize(36);
        txt.setFillColor(i == 0 ? sf::Color::Red : sf::Color::Black);
        txt.setPosition(winWidth / 2 - 100, winHeight / 2 + i * 50);
        items.push_back(txt);
    }
}

void Menu::draw(sf::RenderWindow& window) {
    for (const auto& txt : items) window.draw(txt);
}

void Menu::moveUp() {
    if (selectedIndex > 0) {
        items[selectedIndex].setFillColor(sf::Color::Black);
        selectedIndex--;
        items[selectedIndex].setFillColor(sf::Color::Red);
    }
}

void Menu::moveDown() {
    if (selectedIndex < (int)items.size() - 1) {
        items[selectedIndex].setFillColor(sf::Color::Black);
        selectedIndex++;
        items[selectedIndex].setFillColor(sf::Color::Red);
    }
}

int Menu::getSelectedIndex() const { return selectedIndex; }