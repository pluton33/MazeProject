//
// Created by Sebastian on 15/12/2025.
//

#include "Button.h"

Button::Button(float width, float height,
               const std::string& textStr, const sf::Font& font,
               sf::Color baseColor, sf::Color hoverColor)
    : idleColor(baseColor), hoverColor(hoverColor), text(font, textStr)
{
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(idleColor);
    shape.setOutlineThickness(1.f);
    shape.setOutlineColor(sf::Color::Black);

    text.setFont(font);
    text.setString(textStr);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);

    // Inicjalizujemy pustą akcją, żeby program nie wybuchł, jak klikniemy bez przypisania
    onClickCallback = [](){};
}

void Button::setPosition(const sf::Vector2f& pos) {
    shape.setPosition(pos);

    // Centrowanie tekstu względem NOWEJ pozycji kształtu
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
                   textRect.position.y + textRect.size.y / 2.0f});

    text.setPosition(
        {shape.getPosition().x + (shape.getSize().x / 2.f),
        shape.getPosition().y + (shape.getSize().y / 2.f)}
    );
}

void Button::setOnClick(std::function<void()> callback) {
    onClickCallback = callback;
}

void Button::update(const sf::Vector2f& mousePos) {
    if (shape.getGlobalBounds().contains(mousePos)) {
        shape.setFillColor(hoverColor);
    } else {
        shape.setFillColor(idleColor);
    }
}

bool Button::checkClick(const sf::Vector2f& mousePos) const {
    if (shape.getGlobalBounds().contains(mousePos)) {
        // JEŚLI kliknięto, wywołaj zapisaną lambdę!
        onClickCallback();
        return true;
    }
    return false;
}

void Button::render(sf::RenderWindow& target) const {
    target.draw(shape);
    target.draw(text);
}