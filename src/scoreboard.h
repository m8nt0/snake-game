#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <vector>

class Scoreboard {
 public:
  void AddScore(int score);
  void DisplayTopScores() const;

 private:
  std::vector<int> scores;
};

#endif
