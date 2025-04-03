#include <bits/stdc++.h>
using namespace std;



class Question {
private:
  string questionText;
  string options[4];
  int correctOption; // e.g., 0-3 (A-D)
  int difficultyLevel; // 1-15 (for 15 tiers)

public:
  Question() {}
  ~Question() {}
  void displayQuestion() const;
  bool isCorrect(int playerChoice) const;
  string getOneWrongOption() const;           // For 50:50 lifeline
  string getCorrectOption() const;
  int getCorrectOptionIndex() const;
};