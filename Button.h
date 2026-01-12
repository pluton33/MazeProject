//
// Created by Sebastian on 15/12/2025.
//

#ifndef MAZEPROJECT_BUTTON_H
#define MAZEPROJECT_BUTTON_H


#include <SFML/Graphics.hpp>
#include <string>
#include <functional> // <--- TO JEST POTRZEBNE DO LAMBD

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;

    sf::Color idleColor;
    sf::Color hoverColor;

    std::function<void()> onClickCallback;

public:
    Button(float width, float height,
           const std::string& textStr, const sf::Font& font,
           sf::Color baseColor = sf::Color(70, 70, 70),
           sf::Color hoverColor = sf::Color(100, 100, 100));

    void setPosition(const sf::Vector2f& pos);

    void setOnClick(std::function<void()> callback);

    void update(const sf::Vector2f& mousePos);

    bool checkClick(const sf::Vector2f& mousePos) const;

    void render(sf::RenderWindow& target) const; // const, żeby działało w draw()
};

#endif //MAZEPROJECT_BUTTON_H