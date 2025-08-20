#include "../include/ball.hpp"
#include "../include/grid.hpp"

Ball::Ball(int score, int winWidth, int winHeight)
{
    this->score = score;
    computeRadius();
    ball.setFillColor(randomColor());
    sf::Vector2f position = sf::Vector2f(randomFloat(0.f, 1.f), randomFloat(0.f, 1.f));
    position.x = radius + position.x * (static_cast<float>(winWidth) - 2 * radius); // 初始位置在窗口内
    position.y = radius + position.y * (static_cast<float>(winHeight) - 2 * radius);
    ball.setPosition(position);
}

void Ball::move(int winWidth, int winHeight)
{
    computeRadius();
    ball.move(velocity);
    sf::Vector2f position = ball.getPosition();
    bool isColliding = false;
    float width = static_cast<float>(winWidth);
    float height = static_cast<float>(winHeight);
    // 碰撞检测
    if(position.x < radius) // 碰到左边界
    {
        isColliding = true;
        position.x = radius + (radius - position.x);
        velocity.x = -velocity.x; // 反向
    }
    if(position.x > width - radius) // 碰到右边界
    {
        isColliding = true;
        position.x = (width - radius) - (position.x - (width - radius));
        velocity.x = -velocity.x; // 反向
    }
    if(position.y < radius) // 碰到上边界
    {
        isColliding = true;
        position.y = radius + (radius - position.y);
        velocity.y = -velocity.y; // 反向
    }
    if(position.y > height - radius) // 碰到下边界
    {
        isColliding = true;
        position.y = (height - radius) - (position.y - (height - radius));
        velocity.y = -velocity.y; // 反向
    }
    if(isColliding)
    {
        ball.setPosition(position);
    }
}

void Ball::update(int winWidth, int winHeight, Grid &grid)
{
    if(isDead)
    {
        rebirth(10, winWidth, winHeight);
        return;
    }
    if(score > 50){
        if(randomInt(0, 999) < score / 20){
            score -= 1;
        }
        if(randomInt(0, 999) < score / 100) {
            score -= score / 500;
        }
    }
    int old_score = score;
    computeVelocity();
    move(winWidth, winHeight);
    grid.eatFood(*this);
    if(score != old_score)
    {
        computeRadius();
        ball.setPosition(ball.getPosition());
    }
}

void Ball::eatBall(Ball &other, int winWidth, int winHeight)
{
    if (isDead || other.isDead) return;
    // 计算碰撞范围
    float dist = distance(ball.getPosition(), other.getPosition());
    if (getScore() > other.getScore() && dist < (radius - other.getRadius() * 0.6f))
    {
        // 吃掉对方
        score += other.getScore();
        other.setIsDead(true);
        computeRadius();
        positionCorrect(winWidth, winHeight);
    }
    else if (getScore() < other.getScore() && dist < (other.getRadius() - radius * 0.6f))
    {
        // 被对方吃掉
        other.addScores(score);
        isDead = true;
        other.computeRadius();
        other.positionCorrect(winWidth, winHeight);
    }
}

void Ball::setIsDead(bool dead)
{
    isDead = dead;
}

void Ball::rebirth(int score, int winWidth, int winHeight)
{
    this->score = score; // 重置分数
    computeRadius();
    sf::Vector2f position = randomVector(1.0f);
    position.x = position.x * (static_cast<float>(winWidth) - radius);
    position.y = position.y * (static_cast<float>(winHeight) - radius);
    ball.setPosition(position);
    resetVelocity();
    isDead = false;
}

sf::Vector2f Ball::getPosition() const
{
    return ball.getPosition();
}

int Ball::getScore() const
{
    return score;
}

float Ball::getRadius() const
{
    return radius;
}

void Ball::addScores(int points)
{
    score += points; // 增加分数
    if(score > 10000) {
        score = 10000;
    }
}

void Ball::computeRadius()
{
    radius = 10 + 3.9 * std::sqrt(static_cast<float>(score) - 10); // 根据分数调整半径（立方根）
    ball.setRadius(radius);
    ball.setOrigin(radius, radius);
}

bool Ball::getIsDead() const
{
    return isDead;
}

void Ball::computeSpeed()
{
    // 计算速度
    speed = 300.0f / (100.0f + 0.1f * (score - 10.0f)); // 基础速度除以分数的影响
    if(speed < 0.5f) {
        speed = 0.5f; // 最小速度限制
    }
}

void Ball::positionCorrect(int winWidth, int winHeight)
{
    sf::Vector2f position = ball.getPosition();
    position.x = std::clamp(position.x, radius, static_cast<float>(winWidth) - radius);
    position.y = std::clamp(position.y, radius, static_cast<float>(winHeight) - radius);
    ball.setPosition(position);
}

sf::Vector2f Ball::getVelocity() const
{
    return velocity;
}

void Ball::subScore(int points)
{
    score -= points;
    if(score < 0) {
        score = 0;
    }
}