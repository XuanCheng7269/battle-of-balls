SRC_DIR = src
INC_DIR = include
GAME_SRC = $(SRC_DIR)/game.cpp
BALL_SRC = $(SRC_DIR)/ball.cpp
PLAY_SRC = $(SRC_DIR)/player.cpp
BOT_SRC = $(SRC_DIR)/bot.cpp
UTILS_SRC = $(SRC_DIR)/utils.cpp
FOOD_SRC = $(SRC_DIR)/food.cpp
GRID_SRC = $(SRC_DIR)/grid.cpp

game: $(GAME_SRC) $(BOT_SRC) $(UTILS_SRC) $(FOOD_SRC) $(GRID_SRC) $(PLAY_SRC) $(BALL_SRC)
	g++ -g $(GAME_SRC) $(BOT_SRC) $(UTILS_SRC) $(FOOD_SRC) $(GRID_SRC) $(PLAY_SRC) $(BALL_SRC) -o $@ -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f game