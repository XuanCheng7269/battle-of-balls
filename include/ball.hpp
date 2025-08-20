#ifndef BALL_HPP
#define BALL_HPP

#include <SFML/Graphics.hpp>
#include "../include/utils.hpp"

class Grid;

class Ball{
protected:
    sf::CircleShape ball;
    sf::Vector2f velocity;
    float speed;
    int score;
    float radius;
    bool isDead = false; // 是否死亡
public:
    Ball(int score, int winWidth, int winHeight);
    void move(int winWidth, int winHeight);
    void update(int winWidth, int winHeight, Grid &grid);
    void eatBall(Ball &other, int winWidth, int winHeight);
    void setIsDead(bool dead);
    sf::Vector2f getPosition() const;
    int getScore() const;
    float getRadius() const;
    void addScores(int points);
    void computeRadius();
    bool getIsDead() const;
    void computeSpeed();
    void positionCorrect(int winWidth, int winHeight);
    sf::Vector2f getVelocity() const;
    void subScore(int points);
    virtual void rebirth(int score, int winWidth, int winHeight);
    virtual void computeVelocity(const sf::Vector2f targetPos = sf::Vector2f(0.f, 0.f), const bool vectorChanged = false) = 0;
    virtual void resetVelocity() = 0;
    virtual void draw(sf::RenderWindow &window) = 0;
};



#endif // BALL_HPP