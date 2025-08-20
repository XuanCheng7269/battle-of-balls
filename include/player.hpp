#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "../include/utils.hpp"
#include "../include/ball.hpp"

class Player : public Ball {
public:
    Player(int score, int winWidth, int winHeight);

    void rebirth(int score, int winWidth, int winHeight) override;
    void computeVelocity(const sf::Vector2f targetPos, const bool vectorChanged = false) override;
    void resetVelocity();
    void draw(sf::RenderWindow &window) override;
};

#endif // PLAYER_HPP