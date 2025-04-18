# pragma once
#include <bits/stdc++.h>
#include "Questions.hpp"
#include "LifeLineSystem.hpp"
#include "PrizeTier.hpp"
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