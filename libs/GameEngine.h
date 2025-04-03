#include <bits/stdc++.h>
#include "Questions.h"
#include "LifeLineSystem.h"
#include "PrizeTier.h"
using namespace std;


class GameEngine {
private:
  vector<Question> questionBank;
  LifelineSystem lifelines;
  PrizeTier prizes;
  int loadQuestionsFromFile(const string& filename); // Initialize questions

public:
  void startGame();
  void displayMainMenu();
  void handleAnswer(int choice, Question& q);
  void processGameLogic(); // Main loop
};