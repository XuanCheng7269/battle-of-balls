#include "../include/utils.hpp"

// 全局随机数引擎
std::mt19937& globalRNG() {
    static std::mt19937 rng(std::random_device{}());
    return rng;
}

int randomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(globalRNG());
}

float randomFloat11(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(globalRNG());
}

sf::Vector2f randomVector(float length) {
    // 使用C++11随机数生成角度
    float angle = randomFloat11(0.0f, 2 * 3.1415926f);
    return sf::Vector2f(length * cos(angle), length * sin(angle));
}

sf::Color randomColor() {
    return sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255));
}

float randomFloat(float min, float max) {
    return randomFloat11(min, max);
}

bool randomBool() {
    return randomInt(0, 1) == 0;
}

float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    sf::Vector2f d = a - b;
    return std::sqrt(d.x * d.x + d.y * d.y);
}

float distance(const float a, const float b, const sf::Vector2f& c) {
    return std::sqrt((a - c.x) * (a - c.x) + (b - c.y) * (b - c.y));
}

float distance(const float ax, const float ay, const float bx, const float by) {
    return std::sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
}

std::string computeTime(int seconds) {
    int mins = seconds / 60;
    int secs = seconds % 60;
    return (mins < 10 ? "0" : "") + std::to_string(mins) + ":" + (secs < 10 ? "0" : "") + std::to_string(secs);
}

sf::Vector2f vectorNormalize(const sf::Vector2f& v) {
    float length = std::sqrt(v.x * v.x + v.y * v.y);
    if (length == 0) return sf::Vector2f(0, 0); // 避免除以零
    return sf::Vector2f(v.x / length, v.y / length);
}