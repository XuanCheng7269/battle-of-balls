#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <numeric>
#include <algorithm>
#include "../include/ball.hpp"
#include "../include/bot.hpp"
#include "../include/player.hpp"
#include "../include/grid.hpp"

std::string fontpath = "assets/fangzheng.ttf";
bool isRefresh = false;
int endGame(int width, int height, int score, sf::RenderWindow &window, sf::Font &font)
{
    sf::Text endText;
    endText.setFont(font);
    endText.setString("Game Over! Your score: " + std::to_string(score));
    endText.setCharacterSize(100);
    endText.setFillColor(sf::Color::Black);
    endText.setPosition(static_cast<float>(width) / 2 - endText.getGlobalBounds().width / 2,
                        static_cast<float>(height) / 2 - endText.getGlobalBounds().height / 2);
    sf::Text returnText;
    returnText.setFont(font);
    returnText.setString("Return to Menu");
    returnText.setCharacterSize(50);
    returnText.setFillColor(sf::Color::Black);
    returnText.setPosition(static_cast<float>(width) / 2 - returnText.getGlobalBounds().width / 2,
                           static_cast<float>(height) / 2 + 250);

    window.clear(sf::Color::White);
    window.draw(endText);
    window.draw(returnText);
    window.display();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0; // Exit the program
            }
            else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (returnText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    isRefresh = true; // Set refresh flag to true
                    return 1; // Return to main menu
                }
            }
        }
    }
    return 0;
}

int game(int width, int height, int botCount, int initScore, sf::RenderWindow &window, sf::Font &font)
{
    sf::Clock clock;
    sf::Text timeText;
    float timeFloat = 0.0;
    int time = 0;
    timeText.setFont(font);
    timeText.setCharacterSize(24);
    timeText.setFillColor(sf::Color::White);

    Grid grid(width, height, 50); // 创建网格，假设每个格子大小为20×20
    grid.update(); // 初始化网格

    std::vector<Spore*> spores;

    Ball* ball[botCount + 1];
    ball[0] = new Player(initScore, width, height);
    for (int i = 1; i <= botCount; ++i) {
        ball[i] = new Bot(initScore, width, height);
    }
    sf::Text player;
    player.setFont(font);
    player.setString("Player");
    player.setFillColor(sf::Color(255, 215, 0)); // 鎏金色
    sf::Clock sporeClock;
    
    bool isFollowingMouse = false;
    bool isSpawningSpores = false;
    while(window.isOpen())
    {
        if(time > 299){
            int score = ball[0]->getScore();
            for(int i = 0; i <= botCount; ++i) {
                delete ball[i];
            }
            return score;
        }
        window.clear(sf::Color(30,30,30)); // 清空窗口，设置黑色背景
        // 事件处理
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                for(int i = 0; i <= botCount; ++i) {
                    delete ball[i];
                }
                return 1; // avoid processing other events
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    for(int i = 0; i <= botCount; ++i) {
                        delete ball[i];
                    }
                    isRefresh = true;
                    return 0;
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                isFollowingMouse = true;
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                isFollowingMouse = false;
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                // 右键点击生成孢子
                isSpawningSpores = true;
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
                // 右键释放停止生成孢子
                isSpawningSpores = false;
            }
        }
        if(isSpawningSpores) {
            if (sporeClock.getElapsedTime().asSeconds() > 0.125f && ball[0]->getScore() >= 32) {
                sf::Vector2f velocity;
                sf::Vector2f position = ball[0]->getPosition();
                if(isFollowingMouse) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    velocity = sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) - position;
                }
                else {
                    velocity = ball[0]->getVelocity();
                }
                velocity = vectorNormalize(velocity);
                float radius = ball[0]->getRadius();
                position += velocity * radius;
                Spore* spore = new Spore(width, height, position, velocity);
                spores.push_back(spore);
                sporeClock.restart();
                ball[0]->subScore(14); // 减少分数
            }
        }
        if(isFollowingMouse) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f targetPos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
            ball[0]->computeVelocity(targetPos, true); // 玩家点击位置
        }
        grid.update(); // 更新网格
        for(auto it = spores.begin(); it != spores.end();) {
            Spore* spore = *it;
            if(spore->clock.getElapsedTime().asSeconds() > 0.2f) {
                grid.addSpore(spore); // 将孢子添加到网格
                it = spores.erase(it);
            } else {
                spore->update(width, height);
                ++it;
            }
        }
        grid.drawFood(window); // 绘制食物
        for(auto it = spores.begin(); it != spores.end(); ++it) {
            Spore* spore = *it;
            spore -> draw(window); // 绘制孢子
        }
        grid.drawSpores(window); // 绘制孢子
        grid.occupyReset(); // 重置占用状态
        for(int i = 0; i <= botCount; ++i) {
            for(int j = i + 1; j <= botCount; ++j) {
                if (!ball[i]->getIsDead() && !ball[j]->getIsDead()) {
                    ball[i]->eatBall(*ball[j], width, height);
                }
            }
        }
        for(int i = 0; i <= botCount; ++i) {
            if(ball[i]->getScore() > 17) {
                for(auto it = spores.begin(); it != spores.end();) {
                    Spore* spore = *it;
                    if(distance(ball[i]->getPosition(), spore->getPosition()) < ball[i]->getRadius()) {
                        ball[i]->addScores(14); 
                        delete spore; // 删除孢子对象
                        it = spores.erase(it);
                    } else {
                        ++it;
                    }
                }
            }
            ball[i]->update(width, height, grid);
        }
        int idx[botCount + 1];
        std::iota(idx, idx + botCount + 1, 0); // 初始化索引数组
        std::sort(idx, idx + botCount + 1, [&](int i, int j) {
            return ball[i]->getScore() < ball[j]->getScore(); // 按分数升序排序
        });
        for(int i = 0; i < botCount + 1; ++i) {
            ball[idx[i]]->draw(window); // 按分数顺序绘制Bot
            if(idx[i] == 0){
                // 让文本大小和位置与球适应，并基本完全显示在球内
                float radius = ball[0]->getRadius();
                sf::Vector2f pos = ball[0]->getPosition();
                // 字体大小略小于直径，避免溢出
                float charSize = radius * 0.9f; // 直径为2*radius，1.1倍半径较合适
                player.setCharacterSize(static_cast<unsigned int>(charSize));
                sf::FloatRect textRect = player.getLocalBounds();
                player.setOrigin(textRect.width / 2, textRect.height / 2);
                player.setPosition(pos.x, pos.y - textRect.height / 4); // 轻微上移，视觉更居中
                window.draw(player);
            }
        }
        timeFloat += clock.restart().asSeconds();
        time = static_cast<int>(timeFloat);
        timeText.setString(computeTime(time));
        // 上方居中
        sf::FloatRect timeRect = timeText.getLocalBounds();
        timeText.setOrigin(timeRect.width / 2, 0);
        timeText.setPosition(width / 2, 10); // 顶部居中
        window.draw(timeText);
        window.display();
    }
    return 0;
}

int main()
{
    int width = 1600;
    int height = 1200;
    sf::RenderWindow window(sf::VideoMode(width, height), "Battle of Balls");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    window.clear(sf::Color::White); // 设置白色背景
    sf::Font font;
    if (!font.loadFromFile(fontpath))
    {
        printf("Failed to load font\n");
    }
    sf::Text title;
    title.setFont(font);
    title.setString("Battle of Balls");
    title.setCharacterSize(100);
    title.setFillColor(sf::Color::Black);
    title.setPosition( (width - title.getLocalBounds().width) / 2, height / 4 );
    window.draw(title);
    sf::Text start;
    start.setFont(font);
    start.setString("Start");
    start.setCharacterSize(75);
    start.setFillColor(sf::Color::Black);
    start.setPosition( (width - start.getLocalBounds().width) / 2, height / 2 + 100);
    window.draw(start);
    sf::Text exit;
    exit.setFont(font);
    exit.setString("Exit");
    exit.setCharacterSize(75);
    exit.setFillColor(sf::Color::Black);
    exit.setPosition( (width - exit.getLocalBounds().width) / 2, height / 2 + 200 );
    window.draw(exit);
    window.display();
    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return 0; // Exit the program
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (start.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    int result = game(width, height, 24, 10, window, font);
                    if (result == 1) { // Start the game with 24 bots and initial score of 10
                        window.close();
                        return 0;
                    }
                    else if(result) {
                        if(endGame(width, height, result, window, font) == 0) {
                            window.close();
                            return 0;
                        }
                    }
                }
                if (exit.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    window.close();
                    return 0;
                }
            }
            if(isRefresh) {
                window.clear(sf::Color::White);
                window.draw(title);
                window.draw(start);
                window.draw(exit);
                window.display();
                isRefresh = false; // Reset refresh flag
            }
        }
    }
    return 0;
}