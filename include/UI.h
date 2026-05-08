#pragma once

#include <SFML/Graphics.hpp>

sf::Text makeText(
    const std::string& value,
    const sf::Font& font,
    unsigned int size,
    sf::Vector2f position,
    sf::Color color = sf::Color::White,
    bool bold = false
);

sf::RectangleShape makeButton(sf::Vector2f size, sf::Vector2f position, sf::Color color);