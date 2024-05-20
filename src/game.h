#ifndef GAME_H
#define GAME_H

#include <random>
#include <vector>
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "scoreboard.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer, Scoreboard &scoreboard,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  SDL_Point food;
  std::vector<SDL_Point> obstacles;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void PlaceObstacles();
  void Update();
  bool ObstacleCell(int x, int y);
};

#endif
