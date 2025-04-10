
//PrizeTier.cpp
#include "PrizeTier.h"

// Tăng cấp độ khi người chơi trả lời đúng
void PrizeTier::advanceLevel() {
    if (currentLevel < 15) // Giới hạn tối đa là câu 15
        currentLevel++;
}

// Đặt lại cấp độ về mốc an toàn nếu trả lời sai
void PrizeTier::resetToSafeLevel() {
    if (currentLevel >= 10)
        currentLevel = 10; // Mốc an toàn 2
    else if (currentLevel >= 5)
        currentLevel = 5;  // Mốc an toàn 1
    else
        currentLevel = 0;  // Dưới câu 5 thì không được gì
}

// Trả về phần thưởng ứng với cấp độ hiện tại
string PrizeTier::getCurrentPrize() const {
    if (currentLevel == 0) return "$ 0";
    if (prizeMap.count(currentLevel))  // Kiểm tra cấp độ có trong map không
        return prizeMap.at(currentLevel);
    return "$ 0";
}

// Trả về cấp độ hiện tại (từ 1 đến 15)
int PrizeTier::getCurrentLevel() const {
    return currentLevel;
}

// Kiểm tra xem cấp độ hiện tại có phải là mốc an toàn hay không
bool PrizeTier::isSafeHaven(int level) const {
    return level == 5 || level == 10 || level == 15;
}
