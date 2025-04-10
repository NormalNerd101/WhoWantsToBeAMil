#include <GameEngine.h>
#include <LifeLineSystem.h>
#include <format>

map<string,string> ansiColors = {
    {"reset",        "\033[0m"},
    {"bold",         "\033[1m"},
    {"red",          "\033[31m"},
    {"green",        "\033[32m"},
    {"yellow",       "\033[33m"},
    {"blue",         "\033[34m"},
    {"magenta",      "\033[35m"},
    {"cyan",         "\033[36m"},
    {"white",        "\033[37m"},
    {"bright_red",   "\033[91m"},
    {"bright_green", "\033[92m"},
    {"bright_yellow","\033[93m"},
    {"bright_blue",  "\033[94m"},
    {"bright_magenta","\033[95m"},
    {"bright_cyan",  "\033[96m"},
    {"bright_white", "\033[97m"}
};


void GameEngine::startGame() {
    cout << ansiColors["bright_cyan"]
         << R"(
        ██╗    ██╗██╗  ██╗ ██████╗     ██╗    ██╗ █████╗ ███╗   ██╗████████╗███████╗               
        ██║    ██║██║  ██║██╔═══██╗    ██║    ██║██╔══██╗████╗  ██║╚══██╔══╝██╔════╝               
        ██║ █╗ ██║███████║██║   ██║    ██║ █╗ ██║███████║██╔██╗ ██║   ██║   ███████╗               
        ██║███╗██║██╔══██║██║   ██║    ██║███╗██║██╔══██║██║╚██╗██║   ██║   ╚════██║               
        ╚███╔███╔╝██║  ██║╚██████╔╝    ╚███╔███╔╝██║  ██║██║ ╚████║   ██║   ███████║               
         ╚══╝╚══╝ ╚═╝  ╚═╝ ╚═════╝      ╚══╝╚══╝ ╚═╝  ╚═╝╚═╝  ╚═══╝   ╚═╝   ╚══════╝               
        ████████╗ ██████╗     ██████╗ ███████╗     █████╗                                          
        ╚══██╔══╝██╔═══██╗    ██╔══██╗██╔════╝    ██╔══██╗                                         
           ██║   ██║   ██║    ██████╔╝█████╗      ███████║                                         
           ██║   ██║   ██║    ██╔══██╗██╔══╝      ██╔══██║                                         
           ██║   ╚██████╔╝    ██████╔╝███████╗    ██║  ██║                                         
           ╚═╝    ╚═════╝     ╚═════╝ ╚══════╝    ╚═╝  ╚═╝                                         
        ███╗   ███╗██╗██╗     ██╗     ██╗ ██████╗ ███╗   ██╗ █████╗ ██╗██████╗ ███████╗    ██████╗ 
        ████╗ ████║██║██║     ██║     ██║██╔═══██╗████╗  ██║██╔══██╗██║██╔══██╗██╔════╝    ╚════██╗
        ██╔████╔██║██║██║     ██║     ██║██║   ██║██╔██╗ ██║███████║██║██████╔╝█████╗        ▄███╔╝
        ██║╚██╔╝██║██║██║     ██║     ██║██║   ██║██║╚██╗██║██╔══██║██║██╔══██╗██╔══╝        ▀▀══╝ 
        ██║ ╚═╝ ██║██║███████╗███████╗██║╚██████╔╝██║ ╚████║██║  ██║██║██║  ██║███████╗      ██╗   
        ╚═╝     ╚═╝╚═╝╚══════╝╚══════╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚══════╝      ╚═╝   
    )" << ansiColors["reset"] << endl;
    displayMainMenu();
}


void GameEngine::displayMainMenu() {
    cout << "1. Start Game" << endl;
    cout << "2. Exit" << endl;
    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            processGameLogic();
            break;
        case 2:
            cout << "Exiting the game." << endl;
            exit(0);
        default:
            cout << "Invalid choice. Please try again." << endl;
            displayMainMenu();
    }
}


void GameEngine::processGameLogic() {
    // Load questions from file
    if (loadQuestionsFromFile("questions.txt") == -1) {
        cout << "Failed to load questions." << endl;
        return;
    }

    // Main game loop
    for (auto& q : questionBank) {
        string question = q.getQuestionText();
        vector<string> answers = q.getOptions();
        LifeLineSystem lifeLines;
        string availables = lifeLines.getAvailableLifeLines();
        std::cout << format(R"(
        Question: {}
                A. {}                       B. {}
                C. {}                       D. {}

        Available Lifeliness : {}

        NOTE: 0 to quit.

        )", question, answers[0], answers[1], answers[2], answers[3], availables) << std::endl;

        char choice;
        cout << "Your choice: ";
        cin >> choice;
        if (choice == '0') {
            cout << "Exiting the game." << endl;
            exit(0);
        } else if (choice == q.getCorrectOptionIndex()) {
            cout << "Correct answer!" << endl;
            prizes.updatePrizeTier(q.getDifficultyLevel());
        } else if (choice == '1') {
            lifeLines.useFiftyFifty(q);
            cout << "50:50 Lifeline used!" << endl;
        } else if (choice == '2') {
            lifeLines.usePhoneFriend(q);
            cout << "Phone a Friend Lifeline used!" << endl;
        } else if (choice == '3') {
            lifeLines.useAudiencePoll(q);
            cout << "Audience Poll Lifeline used!" << endl;
        } else {
            cout << "Invalid choice. Please try again." << endl;
            continue;
        }
    }
}


void handleAnswer(int choice, Question& q, PrizeTier& prizes) {
    if (choice == q.getCorrectOptionIndex()) {
        cout << "Correct answer!" << endl;
        prizes.updatePrizeTier(q.getDifficultyLevel());
    } else {
        cout << "Wrong answer! Game Over." << endl;
        exit(0);
    }
}
