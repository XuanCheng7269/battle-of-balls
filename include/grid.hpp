#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "utils.hpp"
#include "food.hpp"

class Bot;
class Player;
class Ball;

class Grid
{
public:
    Grid(int width, int height, int cellSize);
    void drawFood(sf::RenderWindow &window);
    void drawSpores(sf::RenderWindow &window);
    void cellsInit();
    void occupyReset();
    void update();
    void eatFood(Ball &ball);
    void eatFoodInCell(int row, int col, float x, float y, float radius, Ball& ball);
    void addSpore(Spore* spore);

private:
    int width, height;
    int rows, cols;
    int cellSize;
    std::vector<std::vector<std::vector<Food*>>> gridCells;
    std::vector<std::vector<std::vector<Spore*>>> sporeCells;
    std::vector<std::vector<bool>> occupied;

};

#endif // GRID_HPP