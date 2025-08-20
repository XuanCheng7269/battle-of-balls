#include "../include/player.hpp"

Player::Player(int score, int winWidth, int winHeight) : Ball(score, winWidth, winHeight)
{
    // Player自己的初始化
    speed = 0.f;
    resetVelocity();
}

void Player::rebirth(int score, int winWidth, int winHeight)
{
    // Player自己的重生逻辑
    speed = 0.f;
    Ball::rebirth(score, winWidth, winHeight); // 显式调用父类的rebirth
}

void Player::computeVelocity(const sf::Vector2f targetPos,const bool vectorChanged)
{
    if(!vectorChanged){return;}
    computeSpeed();
    float length = distance(ball.getPosition(), targetPos);
    if(length > 0.f) {
        velocity = (targetPos - ball.getPosition()) / length * speed;
    } else {
        velocity = sf::Vector2f(0.f, 0.f);
    }
}

void Player::resetVelocity()
{
    velocity = randomVector(speed);
}

void Player::draw(sf::RenderWindow &window)
{
    // 绘制Player
    window.draw(ball);
}