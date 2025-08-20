#ifndef FOOL_HPP
#define FOOL_HPP

#include <SFML/Graphics.hpp>
#include "../include/utils.hpp"

class Grid;

class Food
{
public:
    Food(int cellX, int cellY, int cellSize, bool isXEdge, bool isYEdge);
    void draw(sf::RenderWindow &window);
    sf::Vector2f getPosition() const;

private:
    sf::ConvexShape food;

};

class Spore
{
    public:
        Spore(int width, int height, sf::Vector2f position, sf::Vector2f velocity);
        sf::Clock clock;
        void update(int width, int height);
        void draw(sf::RenderWindow &window);
        sf::Vector2f getPosition() const;

    private:
        sf::CircleShape spore;
        sf::Vector2f velocity;
        sf::Vector2f position;
};

#endif // FOOD_HPP