#include "../include/grid.hpp"
#include "../include/bot.hpp"
#include "../include/player.hpp"

Grid::Grid(int width, int height, int cellSize)
    : width(width), height(height), cellSize(cellSize)
{
    // 初始化网格
    cols = width / cellSize;
    rows = height / cellSize;
    gridCells.resize(rows); // 先分配行
    sporeCells.resize(rows); // 先分配行
    occupied.resize(rows);
    for (int i = 0; i < rows; ++i) {
        gridCells[i].resize(cols); // 每行分配列
        sporeCells[i].resize(cols); // 每行分配列
        occupied[i].resize(cols, false); // 默认都未占据
    }
    cellsInit(); // 初始化格子
}

void Grid::drawFood(sf::RenderWindow &window)
{
    // 绘制网格
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            for (Food* food : gridCells[i][j]) {
                if (food) {
                    food->draw(window);
                }
            }
        }
    }
}

void Grid::drawSpores(sf::RenderWindow &window)
{
    // 绘制网格
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            for (Spore* spore : sporeCells[i][j]) {
                if (spore) {
                    spore->draw(window);
                }
            }
        }
    }
}

void Grid::cellsInit()
{
    // 初始化每个格子的食物和占用状态
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!occupied[i][j] &&  randomInt(0, 2) == 0) {
                // 如果该格子未被占据，可以生成食物
                Food* food = new Food(j, i, cellSize, j == 0 || j == cols - 1, i == 0 || i == rows - 1 );
                gridCells[i][j].push_back(food);
                if(gridCells[i][j].size()) {
                    occupied[i][j] = true; // 标记为已占据
                }
            }
        }
    }
    
}

void Grid::occupyReset()
{
    // 重置网格状态
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (gridCells[i][j].size() < 2) {
                occupied[i][j] = false;
            }
        }
    }
}

void Grid::update()
{
    // 更新网格逻辑
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!occupied[i][j] && randomInt(0, 500) == 0) {
            // if (!occupied[i][j] && rand() % 600 == 0) {
                // 如果该格子未被占据，可以生成食物
                Food* food = new Food(j, i, cellSize, j == 0 || j == cols - 1, i == 0 || i == rows - 1 );
                gridCells[i][j].push_back(food);
                if(gridCells[i][j].size() >= 2) {
                    occupied[i][j] = true; // 标记为已占据
                }
            }
        }
    }
}

void Grid::eatFood(Ball &ball)
{
    // 处理Ball吃食物的逻辑
    sf::Vector2f ballPos = ball.getPosition();
    float radius = ball.getRadius();
    int cx = ballPos.x / cellSize;
    int cy = ballPos.y / cellSize;
    int cx_min = (ballPos.x - radius) / cellSize;
    int cx_max = (ballPos.x + radius) / cellSize;
    int cy_min = (ballPos.y - radius) / cellSize;
    int cy_max = (ballPos.y + radius) / cellSize;
    if(cx_min < 0) cx_min = 0;
    if(cx_max >= cols) cx_max = cols - 1;
    if(cy_min < 0) cy_min = 0;
    if(cy_max >= rows) cy_max = rows - 1;
    // 遍历Ball覆盖的格子
    for(int i = cx_min; i <= cx_max; ++i){
        occupied[cy][i] = true;
        eatFoodInCell(cy, i, ballPos.x, ballPos.y, radius, ball);
    }
    for(int j = cy_min; j <= cy_max; ++j){
        occupied[j][cx] = true;
        eatFoodInCell(j, cx, ballPos.x, ballPos.y, radius, ball);
    }
    // 左下检测
    for (int i = cx_min; i <= cx - 1; ++i) {
        for(int j = cy + 1; j <= cy_max; ++j){
            if(distance((i + 1) * cellSize - 1, j * cellSize, ballPos) < radius) {
                occupied[j][i] = true;
                if(distance(i * cellSize, (j + 1) * cellSize -1, ballPos) >= radius){
                    eatFoodInCell(j, i, ballPos.x, ballPos.y, radius, ball);
                }
            }
        }
    }
    // 右下检测
    for (int i = cx + 1; i <= cx_max; ++i) {
        for(int j = cy + 1; j <= cy_max; ++j){
            if(distance(i * cellSize, j * cellSize, ballPos) < radius) {
                occupied[j][i] = true;
                if(distance((i + 1) * cellSize - 1, (j + 1) * cellSize -1, ballPos) >= radius){
                    eatFoodInCell(j, i, ballPos.x, ballPos.y, radius, ball);
                }
            }
        }
    }
    // 右上检测
    for (int i = cx + 1; i <= cx_max; ++i) {
        for(int j = cy_min; j <= cy - 1; ++j){
            if(distance(i * cellSize, (j + 1) * cellSize -1, ballPos) < radius) {
                occupied[j][i] = true;
                if(distance((i + 1) * cellSize - 1, j * cellSize, ballPos) >= radius){
                    eatFoodInCell(j, i, ballPos.x, ballPos.y, radius, ball);
                }
            }
        }
    }
    // 左上检测
    for (int i = cx_min; i <= cx - 1; ++i) {
        for(int j = cy_min; j <= cy - 1; ++j){
            if(distance(i * cellSize, (j + 1) * cellSize -1, ballPos) < radius) {
                occupied[j][i] = true;
                if(distance((i + 1) * cellSize - 1, j * cellSize, ballPos) >= radius){
                    eatFoodInCell(j, i, ballPos.x, ballPos.y, radius, ball);
                }
            }
        }
    }
}

void Grid::eatFoodInCell(int row, int col, float x, float y, float radius, Ball& ball)
{
    for(auto it = gridCells[row][col].begin(); it != gridCells[row][col].end(); ) {
        Food* food = *it;
        if(food && distance(x, y, food->getPosition()) < radius) {
            ball.addScores(1); // 假设每吃到一个食物得1分
            delete food; // 删除食物对象
            food = nullptr; // 避免悬空指针
            it = gridCells[row][col].erase(it); // 移除已吃掉的食物
        } else {
            ++it;
        }
    }
    if(ball.getScore() < 18) {
        return; // 如果分数小于18，不吃孢子
    }
    for(auto it = sporeCells[row][col].begin(); it != sporeCells[row][col].end(); ) {
        Spore* spore = *it;
        if(spore && distance(x, y, spore->getPosition()) < radius) {
            ball.addScores(14); // 假设每吃到一个孢子得14分
            delete spore; // 删除孢子对象
            spore = nullptr; // 避免悬空指针
            it = sporeCells[row][col].erase(it); // 移除已吃掉的孢子
        } else {
            ++it;
        }
    }
}

void Grid::addSpore(Spore* spore)
{
    if (spore) {
        int col = spore->getPosition().x / cellSize;
        int row = spore->getPosition().y / cellSize;
        if (col >= 0 && col < cols && row >= 0 && row < rows) {
            sporeCells[row][col].push_back(spore);
        }
    }
}