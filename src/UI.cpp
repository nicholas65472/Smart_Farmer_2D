#include "UI.h"

sf::Text makeText(
    const std::string& value,
    const sf::Font& font,
    unsigned int size,
    sf::Vector2f position,
    sf::Color color,
    bool bold
) {
    sf::Text text(value, font, size);
    text.setPosition(position);
    text.setFillColor(color);
    if (bold) {
        text.setStyle(sf::Text::Bold);
    }
    return text;
}

sf::RectangleShape makeButton(sf::Vector2f size, sf::Vector2f position, sf::Color color) {
    sf::RectangleShape button(size);
    button.setPosition(position);
    button.setFillColor(color);
    return button;
}