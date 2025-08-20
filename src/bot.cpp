#include "../include/bot.hpp"

Bot::Bot(int score, int winWidth, int winHeight) : Ball(score, winWidth, winHeight){
    // Bot自己的初始化
    computeSpeed();
    resetVelocity();
}

void Bot::computeVelocity(const sf::Vector2f targetPos, const bool vectorChanged) {
    // Bot自己的速度计算逻辑
    if (clock.getElapsedTime().asSeconds() > 1.0f && randomInt(0, 299) == 0) {
        computeSpeed();
        velocity = randomVector(speed);
        clock.restart();
    }
    else {
        float old_speed = speed;
        computeSpeed();
        if (speed != old_speed) {
            velocity *= speed / old_speed;
        }
    }
}

void Bot::resetVelocity() {
    // Bot自己的速度重置逻辑
    velocity = randomVector(speed);
}

void Bot::draw(sf::RenderWindow &window) {
    // 绘制Bot
    window.draw(ball);
}