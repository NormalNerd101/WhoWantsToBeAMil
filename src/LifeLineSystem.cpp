#include "LifeLineSystem.hpp"



void LifeLineSystem::useFiftyFifty(Question& q) {
    if (fiftyFiftyUsed) {
        cout << "50:50 lifeline already used!" << endl;
        return;
    }
    fiftyFiftyUsed = true;
    // display two options, one is correct.
    string wrongOption = q.getOneWrongOption();
    int correctOption = q.getCorrectOption();
    string correctAnswer = q.getOptions()[correctOption];
    cout << "50:50 Lifeline used! The remaining options are:" << endl;
    cout << "1. " << correctAnswer << endl;
    cout << "2. " << wrongOption << endl;
}


void LifeLineSystem::usePhoneFriend(const Question& q) {
    if (phoneFriendUsed) {
        cout << "Phone a Friend lifeline already used!" << endl;
        return;
    }
    phoneFriendUsed = true;
    cout << "Yo, I think I read this somewhere. I think the answer is: " << q.getCorrectOption() << endl;
    cout << "Good luck!" << endl;
}

void displayAudiencePoll(const vector<int>& percentages) {
    cout << "\nAudience Poll Results:\n";
    for (int i = 0; i < 4; i++) {
        cout << char('A' + i) << ": " << percentages[i] << "% ["<< string(percentages[i] / 2, '#') << "]\n"; // Visual bar
    }
}


void LifeLineSystem::useAudiencePoll(const Question& q) {
    if (audiencePollUsed) {
        cout << "Audience Poll lifeline already used!" << endl;
        return;
    }
    audiencePollUsed = true;

    int correct = q.getCorrectOptionIndex();
    vector<int> percentages(4, 0);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> correctDist(40, 70); // Base for correct answer
    uniform_int_distribution<> wrongDist(5, 25);    // Base for wrong answers

    // Assign correct answer a higher base %
    percentages[correct] = correctDist(gen);

    // Distribute remaining % among wrong options
    int remaining = 100 - percentages[correct];
    for (int i = 0; i < 4; i++) {
        if (i != correct) {
            int part = (remaining > 0) ? wrongDist(gen) : 0;
            percentages[i] = part;
            remaining -= part;
        }
    }

    // Normalize to ensure total is 100 (fix rounding errors)
    if (remaining != 0) {
        percentages[correct] += remaining; // Give leftover to correct answer
    }

    uniform_int_distribution<> noise(-5, 5);
    for (int& p : percentages) {
        p += noise(gen);
        p = max(0, min(100, p)); // Clamp to 0-100
    }

    displayAudiencePoll(percentages);
}


string LifeLineSystem::getAvailableLifeLines() {
    cout << "Available Lifelines: " << endl;
    string output_text = "";
    if (!fiftyFiftyUsed) {
        output_text += "1. 50:50  ";
    }
    if (!phoneFriendUsed) {
        output_text += "2. Phone a Friend  ";
    }
    if (!audiencePollUsed) {
        output_text += "3. Audience Poll ";
    }
    return output_text;
}