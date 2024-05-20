#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
  PlaceObstacles();
}

void Game::Run(Controller const &controller, Renderer &renderer, Scoreboard &scoreboard,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  bool paused = false;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, paused, snake);
    if (!paused) {
      Update();
      renderer.Render(snake, food, obstacles);
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
  scoreboard.AddScore(score);
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake or obstacle before placing food.
    if (!snake.SnakeCell(x, y) && !ObstacleCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::PlaceObstacles() {
  int num_obstacles = 5; // You can change the number of obstacles here
  for (int i = 0; i < num_obstacles; ++i) {
    SDL_Point obstacle;
    while (true) {
      obstacle.x = random_w(engine);
      obstacle.y = random_h(engine);
      if (!snake.SnakeCell(obstacle.x, obstacle.y) && !ObstacleCell(obstacle.x, obstacle.y)) {
        obstacles.push_back(obstacle);
        break;
      }
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }

  // Check if snake collides with any obstacle
  for (auto const &obstacle : obstacles) {
    if (new_x == obstacle.x && new_y == obstacle.y) {
      snake.alive = false;
    }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

bool Game::ObstacleCell(int x, int y) {
  for (auto const &obstacle : obstacles) {
    if (x == obstacle.x && y == obstacle.y) {
      return true;
    }
  }
  return false;
}
