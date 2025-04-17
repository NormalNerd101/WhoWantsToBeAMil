#include "Questions.h"
using namespace std;
// Constructor mặc định
Question::Question() {}


// Kiểm tra xem lựa chọn của người chơi có đúng không
bool Question::isCorrect(int playerChoice) const {
    return playerChoice == correctOption;
}

// Trả về nội dung của đáp án đúng
string Question::getCorrectOption() const {
    return options[correctOption];
}

// Trả về chỉ số đáp án đúng (0-3)
int Question::getCorrectOptionIndex() const {
    return correctOption;
}

// Trả về một đáp án sai (bất kỳ)
string Question::getOneWrongOption() const {
    for (int i = 0; i < 4; ++i) {
        if (i != correctOption)
            return options[i]; // Gặp đáp án sai đầu tiên là trả về
    }
    return ""; // Trường hợp này không xảy ra, vì luôn có đáp án sai
}

// Trả về độ khó của câu hỏi
int Question::getDifficultyLevel() const {
    return difficultyLevel;
}