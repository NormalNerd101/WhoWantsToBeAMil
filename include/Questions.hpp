# pragma once
#include <bits/stdc++.h>
#include <fstream>
#include <json.hpp>
using namespace std;



class Question {
private:
  string questionText;
  vector<string> options;
  int correctOptionIndex; // e.g., 0-3 for 4 options
  int difficultyLevel; // 1-15 (for 15 tiers)

public:
  Question() {}
  ~Question() {}

  // getter
  string getQuestionText() const { return questionText; }
  vector<string> getOptions() const { return options; }
  int getCorrectOptionIndex() const;
  string getCorrectOption() const { return options[correctOptionIndex]; }
  int getDifficultyLevel() const { return difficultyLevel; }

  // setter
  void setQuestionText(const string& text) { questionText = text; }
  void setOption(int index, const string& option) { options[index] = option; }
  void setCorrectOptionIndex(int index) { correctOptionIndex = index; }
  void setDifficultyLevel(int level) { difficultyLevel = level; }
  void setOptions(const string& opt1, const string& opt2, const string& opt3, const string& opt4) {
    options[0] = opt1;
    options[1] = opt2;
    options[2] = opt3;
    options[3] = opt4;
  }
  
  // methods
  void loadData();
};

