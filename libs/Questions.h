# pragma once
#include <bits/stdc++.h>
using namespace std;



class Question {
private:
  string questionText;
  vector<string> options;
  int correctOption; // e.g., 0-3 (A-D)
  int difficultyLevel; // 1-15 (for 15 tiers)

public:
  Question() {}
  Question(string q, string a, string b, string c, string d, int correct, int level);
  ~Question() {}
  
  // getter
  string getQuestionText() const { return questionText; }
  vector<string> getOptions() const { return options; }
  int getCorrectOption() const { return correctOption; }
  int getDifficultyLevel() const { return difficultyLevel; }
  
  // setter
  void setQuestionText(const string& text) { questionText = text; }
  void setOption(int index, const string& option) { options[index] = option; }
  void setCorrectOption(int index) { correctOption = index; }
  void setDifficultyLevel(int level) { difficultyLevel = level; }
  void setOptions(const string& opt1, const string& opt2, const string& opt3, const string& opt4) {
    options[0] = opt1;
    options[1] = opt2;
    options[2] = opt3;
    options[3] = opt4;
  }
  void displayQuestion() const;
  bool isCorrect(int playerChoice) const;
  string getOneWrongOption() const;           // For 50:50 lifeline
  string getCorrectOption() const;
  int getCorrectOptionIndex() const;
  int getDifficultyLevel() const;
};