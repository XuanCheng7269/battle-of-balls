SRC_DIR = src
INC_DIR = include
GAME_SRC = $(SRC_DIR)/game.cpp
BALL_SRC = $(SRC_DIR)/ball.cpp
PLAY_SRC = $(SRC_DIR)/player.cpp
BOT_SRC = $(SRC_DIR)/bot.cpp
UTILS_SRC = $(SRC_DIR)/utils.cpp
FOOD_SRC = $(SRC_DIR)/food.cpp
GRID_SRC = $(SRC_DIR)/grid.cpp

# g++ -g $(GAME_SRC) $(BOT_SRC) $(UTILS_SRC) $(FOOD_SRC) $(GRID_SRC) $(PLAY_SRC) $(BALL_SRC) -I../SFML-2.6.2/include -L../SFML-2.6.2/lib -o game.exe -lsfml-graphics -lsfml-window -lsfml-system

game: $(GAME_SRC) $(BOT_SRC) $(UTILS_SRC) $(FOOD_SRC) $(GRID_SRC) $(PLAY_SRC) $(BALL_SRC)
	g++ -g $(GAME_SRC) $(BOT_SRC) $(UTILS_SRC) $(FOOD_SRC) $(GRID_SRC) $(PLAY_SRC) $(BALL_SRC) -I../SFML-2.6.2/include -L../SFML-2.6.2/lib -o game.exe -DSFML_STATIC -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lgdi32 -lwinmm -lsfml-audio-s -lsfml-network-s -lws2_32 -lwinmm -static-libgcc -static-libstdc++

clean:
	@cmd /C del /Q /F game.exe