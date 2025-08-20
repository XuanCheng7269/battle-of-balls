#include "../include/food.hpp"

float sporeRadius = 8.f;

Food::Food(int cellX, int cellY, int cellSize, bool isXEdge, bool isYEdge)
{
    int pointCount = randomBool() ? 3 : 4;
    food.setPointCount(pointCount);
    float targetRadius = 5.f;
    sf::Vector2f center(0.f, 0.f);
    for (int i = 0; i < pointCount; ++i) {
        float angle = 2 * M_PI * i / pointCount + randomFloat(-0.2f, 0.2f);
        float r = targetRadius * randomFloat(0.8f, 1.2f);
        float x = r * cos(angle);
        float y = r * sin(angle);
        center += sf::Vector2f(x, y);
        food.setPoint(i, sf::Vector2f(x, y));
    }
    center.x /= pointCount;
    center.y /= pointCount;
    food.setOrigin(center); // 以几何中心为原点
    // 随机生成不会出界的位置（假设最大半径为5）
    float x = randomFloat(0.f, static_cast<float>(cellSize));
    float y = randomFloat(0.f, static_cast<float>(cellSize));
    if (isXEdge) {
        float x = cellX ? randomFloat(0.f, static_cast<float>(cellSize) - 5.f) : randomFloat(5.f, static_cast<float>(cellSize));
    }
    if(isYEdge) {
        float y = cellY ? randomFloat(0.f, static_cast<float>(cellSize) - 5.f) : randomFloat(5.f, static_cast<float>(cellSize));
    }
    x += cellX * cellSize;
    y += cellY * cellSize;
    food.setPosition(x, y);
    food.setFillColor(randomColor());
}

void Food::draw(sf::RenderWindow &window)
{
    window.draw(food);
}

sf::Vector2f Food::getPosition() const
{
    return food.getPosition();
}

Spore::Spore(int width, int height, sf::Vector2f position, sf::Vector2f velocity)
{
    spore.setRadius(sporeRadius);
    spore.setOrigin(sporeRadius, sporeRadius);
    this -> position.x = std::clamp(position.x, sporeRadius, static_cast<float>(width) - sporeRadius);
    this -> position.y = std::clamp(position.y, sporeRadius, static_cast<float>(height) - sporeRadius);
    spore.setFillColor(sf::Color(144, 238, 144)); // 浅绿色
    this -> velocity = velocity * 5.0f;
}

void Spore::update(int winWidth, int winHeight)
{
    position += velocity;
    float width = static_cast<float>(winWidth);
    float height = static_cast<float>(winHeight);
    // 碰撞检测
    if(position.x < sporeRadius) // 碰到左边界
    {
        position.x = sporeRadius + (sporeRadius - position.x);
        velocity.x = -velocity.x; // 反向
    }
    if(position.x > width - sporeRadius) // 碰到右边界
    {
        position.x = (width - sporeRadius) - (position.x - (width - sporeRadius));
        velocity.x = -velocity.x; // 反向
    }
    if(position.y < sporeRadius) // 碰到上边界
    {
        position.y = sporeRadius + (sporeRadius - position.y);
        velocity.y = -velocity.y; // 反向
    }
    if(position.y > height - sporeRadius) // 碰到下边界
    {
        position.y = (height - sporeRadius) - (position.y - (height - sporeRadius));
        velocity.y = -velocity.y; // 反向
    }
        spore.setPosition(position);
}

void Spore::draw(sf::RenderWindow &window)
{
    window.draw(spore);
}

sf::Vector2f Spore::getPosition() const
{
    return position;
}