#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <queue>

// 全局随机数引擎（建议只声明一次）
std::mt19937& globalRNG();

// 随机整数 [min, max]
int randomInt(int min, int max);

// 随机浮点数 [min, max)
float randomFloat11(float min, float max);

sf::Vector2f randomVector(float length);
sf::Color randomColor();
float randomFloat(float min, float max);
bool randomBool();
float distance(const sf::Vector2f& a, const sf::Vector2f& b);
float distance(const float a, const float b, const sf::Vector2f& c);
float distance(const float ax, const float ay, const float bx, const float by);
std::string computeTime(int seconds);
sf::Vector2f vectorNormalize(const sf::Vector2f& v);

#endif // UTILS_HPP