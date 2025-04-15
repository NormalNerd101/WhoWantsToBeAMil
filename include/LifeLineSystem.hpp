# pragma once
#include <bits/stdc++.h>
#include "Questions.hpp"
using namespace std;


class LifeLineSystem {
private:
    bool fiftyFiftyUsed;
    bool phoneFriendUsed;
    bool audiencePollUsed;

public:
    LifeLineSystem() {}
    ~LifeLineSystem() {}
    void useFiftyFifty(Question& currentQuestion);                // Hides 2 wrong answers
    void usePhoneFriend(const Question& q);                       // Simulates a "hint"
    void useAudiencePoll(const Question& q);                      // Simulates audience's suggestion in a poll (%)
    void resetLifeLines();                                        // Resets all lifelines
    string getAvailableLifeLines();                                 // Displays available lifelines
    bool isAudiencePollUsed() const { return audiencePollUsed; }
    bool isFifyFiftyUsed() const { return fiftyFiftyUsed; }
    bool isPhoneFriendUsed() const { return phoneFriendUsed; }
};