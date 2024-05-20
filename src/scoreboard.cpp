#include "scoreboard.h"
#include <algorithm>
#include <iostream>

void Scoreboard::AddScore(int score) {
  scores.push_back(score);
  std::sort(scores.begin(), scores.end(), std::greater<int>());
  if (scores.size() > 5) {
    scores.pop_back();
  }
}

void Scoreboard::DisplayTopScores() const {
  std::cout << "Top Scores:\n";
  for (size_t i = 0; i < scores.size(); ++i) {
    std::cout << (i + 1) << ": " << scores[i] << "\n";
  }
}
