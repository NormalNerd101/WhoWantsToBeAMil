# pragma once
#include <bits/stdc++.h>
#include "Questions.h"
#include "LifeLineSystem.h"
#include "PrizeTier.h"
using namespace std;


class GameEngine {
private:
  vector<Question> questionBank;
  LifeLineSystem lifelines;
  PrizeTier prizes;
  
public:
  void startGame();
  void displayMainMenu();
  void handleAnswer(int choice, Question& q);
  void processGameLogic(); // Main loop
  int loadQuestionsFromFile(const string& filename); // Initialize questions
};