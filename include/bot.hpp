#ifndef BOT_HPP
#define BOT_HPP

#include <SFML/Graphics.hpp>
#include "../include/utils.hpp"
#include "../include/ball.hpp"

class Bot : public Ball{
public:
    Bot(int score, int winWidth, int winHeight);

    void computeVelocity(const sf::Vector2f targetPos = sf::Vector2f(0.f, 0.f), const bool vectorChanged = false) override;
    void resetVelocity() override;
    void draw(sf::RenderWindow &window) override;

private:
    sf::Clock clock;
};


#endif // BOT_HPP