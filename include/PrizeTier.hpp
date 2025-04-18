# pragma once
#include <bits/stdc++.h>
using namespace std;

class PrizeTier {
private:
  int currentLevel; // 1 to 15 (15 = million)
  unordered_map<int, string> prizeMap; // Level-to-prize mapping

public:
  PrizeTier() : currentLevel(1), 
                prizeMap({
                  {1, "$ 1,000"},
                  {2, "$ 2,000"},
                  {3, "$ 5,000"},
                  {4, "$ 10,000"},
                  {5, "$ 20,000"},
                  {6, "$ 40,000"},
                  {7, "$ 75,000"},
                  {8, "$ 1,50,000"},
                  {9, "$ 3,00,000"},
                  {10, "$ 6,00,000"},
                  {11, "$ 12,50,000"},
                  {12, "$ 25,00,000"},
                  {13, "$ 50,00,000"},
                  {14, "$ 80,000,000"},
                  {15, "$ 100,000,000"}
                }) {}
  void advanceLevel();
  void resetToSafeLevel(); // On wrong answer
  string getCurrentPrize() const;
  int getCurrentLevel() const;
  bool isSafeHaven(int level) const; // Check if level is a safe point
  void updatePrizeTier(int prizeAmount);
};