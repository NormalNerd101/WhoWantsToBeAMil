#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <UIElements.hpp>

// import LifeLineSupports
#include <AudiencePoll.hpp>
#include <PhoneFriend.hpp>

// import supporting classes
#include <Questions.hpp>
#include <json.hpp>

// importing windows
#include <EventManager.hpp>
#include <WinResponseWindow.hpp>
#include <FailResponseWindow.hpp>

using namespace std;
using namespace sf;
using json = nlohmann::json;
void loadDataFromJson(vector<Question>& questions);

class Panel {
private:
    Vector2f position;
    Vector2f size;
    Color backgroundColor;
    string name;
    RectangleShape shape;

public:
    Panel(const Vector2f& pos, const Vector2f& sz, const Color& color, const string& panelName)
        : position(pos), size(sz), backgroundColor(color), name(panelName) {
        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(backgroundColor);
    }

    void draw(RenderWindow& window) {
        window.draw(shape);
        
        // Optional: Draw panel name
        // Would need to add font loading and text rendering
    }

    bool contains(const Vector2i& point) const {
        FloatRect bounds = shape.getGlobalBounds();
        return bounds.contains(static_cast<float>(point.x), static_cast<float>(point.y));
    }

    // Getters and setters
    const Vector2f& getPosition() const { return position; }
    const Vector2f& getSize() const { return size; }
    const string& getName() const { return name; }
};
    


class Application {
private:
    RenderWindow window;
    vector<Panel> panels;
    Color backgroundColor;
    
    // UI components
    Font font;
    TextBox* questionBox;
    vector<Button*> answerButtons;
    CountdownClock* timer;
    PrizeTierBoard* prizeBoard;

    // LifeLineSupport Event handlers
    Button* audiencePollBtn;
    Button* phoneFriendBtn;
    Button* fiftyFiftyBtn;

    // Questions and Answers from database
    vector<Question> questions;
    size_t currentIndex = 0;


    // Tracking texts in top-right panel
    Font fontTracking;
    SimpleText* questionTracking;
    SimpleText* prizeTracking;

public:
    Application() : backgroundColor(Color(50, 50, 50)) {

        // register for callback events
        EventManager::getInstance().registerCallback("restartGame", [this]() {
            this->restart();
        });
        
        EventManager::getInstance().registerCallback("quitGame", [this]() {
            this->restart();
        });

        // Create the main window
        window.create(VideoMode(1200, 600), "Who wants to be a FUCKING MILLIONAIRE, ey?", Style::Titlebar|Style::Close);
        window.setFramerateLimit(60);
        
        // Load font
        if (!font.loadFromFile("assets/fonts/LiberationSans-Regular.ttf")) {
            throw runtime_error("Could not load font");
        }

        // Load font for tracking texts
        if (!fontTracking.loadFromFile("assets/fonts/LiberationSans-Bold.ttf")) {
            throw runtime_error("Could not load font");
        }

        // Load questions from JSON files
        loadDataFromJson(questions);
        
        // Create panels
        // Left panel will be divided into two sections
        // Top-left panel for timer
        panels.emplace_back(
            Vector2f(10, 10),                      // Position
            Vector2f(200, 200),                     // Size
            Color(100, 150, 100, 255),              // Color
            "Timer Panel"                 // Name
        );
        // Bottom-left panel for LifeLineSupport
        panels.emplace_back(
            Vector2f(10, 230),                       // Position
            Vector2f(200, 370),                     // Size
            Color(100, 100, 150, 255),              // Color
            "LifeLineSupport Panel"                           // Name
        );
        
        // Center top panel for question
        panels.emplace_back(
            Vector2f(220, 10),                      // Position
            Vector2f(600, 280),                     // Size
            Color(140, 206, 242, 255),              // Color
            "Question Panel"                        // Name
        );

        // Center bottom panel for answers
        panels.emplace_back(
            Vector2f(220, 300),                     // Position
            Vector2f(600, 290),                     // Size
            Color(160, 226, 255, 255),              // Color
            "Answer Options Panel"                  // Name
        );
        
        // Panel for prize board
        panels.emplace_back(
            Vector2f(830, 10),                      // Position
            Vector2f(160, 580),                     // Size
            Color(218, 112, 112, 255),              // Color
            "Prize Board Panel"                     // Name
        );

        // Panel for tracking questions and prizes.
        panels.emplace_back(
            Vector2f(995, 10),                     // Position
            Vector2f(190, 580),                      // Size
            Color(255, 238, 88, 255),              // Color
            "Question Tracking Panel"               // Name
        );

        
        // Create UI elements

        // Prize board in right panel
        prizeBoard = new PrizeTierBoard(
            Vector2f(830, 10),                      // Position
            Vector2f(190, 580),                     // Size
            &font,                                  // Font
            "Prize Tier Board"                      // Name
        );

        // Tracking texts in top-right panel
        questionTracking = new SimpleText(
            Vector2f(1000, 10),                     // Position
            Vector2f(175, 20),                      // Size
            &fontTracking,                          // Font
            "Question: " + to_string(prizeBoard->getCurrentTier()),                    // Default text
            "Question Tracking Text"                // Name
        );

        prizeTracking = new SimpleText(
            Vector2f(1000, 30),                     // Position
            Vector2f(175, 100),                      // Size
            &fontTracking,                           // Font
            "Prize: " + prizeBoard->getCurrentTierAmount(prizeBoard->getCurrentTier()),                       // Default text
            "Prize Tracking Text"                   // Name
        );

        // Question box in center top panel
        questionBox = new TextBox(
            Vector2f(240, 30),                      // Position
            Vector2f(560, 240),                     // Size
            &font,                                  // Font
            questions[currentIndex].getQuestionText(), // Default text
            "Question Text"                         // Name
        );
        
        // Initiate answer buttons in center bottom panel
        string answerOptions[4] = {};
        for (int i = 0; i < 4; i++) {
            answerOptions[i] = questions[currentIndex].getOptions()[i];
        }
        
        for (int i = 0; i < 4; i++) {
            Button* btn = new Button(
                Vector2f(240, 320 + i * 65),        // Position (stacked vertically)
                Vector2f(560, 55),                  // Size
                &font,                              // Font
                string(1,'A' + i) + ". " + answerOptions[i],                   // Text
                "Answer " + to_string(i + 1)        // Name
            );
            answerButtons.push_back(btn);
        }
        
        // Timer in top-left panel (matched with division size)
        timer = new CountdownClock(
            Vector2f(10, 10),                       // Position
            Vector2f(200, 200),                     // Size
            &font,                                  // Font
            31,                                     // Starting time (30 seconds) (screen-countdown starts from 30)
            "Game Timer"                           // Name
        );

        // LifeLineSupport in bottom-left panel
        // Set up three seperate buttons for each lifeline.
        audiencePollBtn = new Button(
            Vector2f(10, 230),                     // Position
            Vector2f(200, 100),                    // Size
            &font,                                  // Font
            "Audience Poll",                        // Text
            "Audience Poll Button"                  // Name
        );
        audiencePollBtn->setIdleColor(Color(255, 152, 0, 100));
        audiencePollBtn->setHoverColor(Color(255, 152, 0, 77));
        audiencePollBtn->setActiveColor(Color(140, 190, 140));
        
        phoneFriendBtn = new Button(
            Vector2f(10, 340),                     // Position
            Vector2f(200, 100),                    // Size
            &font,                                  // Font
            "Phone a Friend",                       // Text
            "Phone a Friend Button"                 // Name
        );
        phoneFriendBtn->setIdleColor(Color(255, 152, 0, 100));
        phoneFriendBtn->setHoverColor(Color(255, 152, 0, 77));
        phoneFriendBtn->setActiveColor(Color(140, 190, 140));
        
        fiftyFiftyBtn = new Button(
            Vector2f(10, 450),                     // Position
            Vector2f(200, 100),                    // Size
            &font,                                  // Font
            "50/50",                                // Text
            "50/50 Button"                          // Name
        );
        fiftyFiftyBtn->setIdleColor(Color(255, 152, 0, 100));
        fiftyFiftyBtn->setHoverColor(Color(255, 152, 0, 77));
        fiftyFiftyBtn->setActiveColor(Color(140, 190, 140));
    }
    
    ~Application() {
        // Clean up dynamically allocated UI elements
        delete questionBox;
        for (auto button : answerButtons) {
            delete button;
        }
        delete timer;
        delete prizeBoard;
    }
    
    void run() {
        // Start the timer
        timer->start();
        
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

    void restart() {
        // Reset the game state
        currentIndex = 0;
        timer->reset(31);
        timer->start();
        prizeBoard->setCurrentTier(1);

        loadDataFromJson(questions);

        // Reset tracking texts
        questionTracking->setText("Question: " + to_string(prizeBoard->getCurrentTier()));
        prizeTracking->setText("Prize: " + prizeBoard->getCurrentTierAmount(prizeBoard->getCurrentTier()));

        // Reset UI elements
        questionBox->setText(questions[currentIndex].getQuestionText());
        for (int i = 0; i < 4; i++) {
            answerButtons[i]->setText(string(1, 'A' + i) + ". " + questions[currentIndex].getOptions()[i]);
            answerButtons[i]->setVisibility(true);
        }
        
        // Show all LifeLineSupport buttons
        audiencePollBtn->setVisibility(true);
        phoneFriendBtn->setVisibility(true);
        fiftyFiftyBtn->setVisibility(true);
    }
    
private:
    void processEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            // handle LifeLineSupport button events
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    if (audiencePollBtn->contains(mousePos)) {
                        handleAudiencePoll();
                    } else if (phoneFriendBtn->contains(mousePos)) {
                        handlePhoneFriend();
                    } else if (fiftyFiftyBtn->contains(mousePos)) {
                        handleFiftyFifty();
                    }
                }
            }
            
            // Handle button events
            for (auto button : answerButtons) {
                button->handleEvent(event);
            }
            
            // Example: Handle mouse clicks on panels
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    handleMouseClick(mousePos);
                }
            }
        }
    }
    
    void handleMouseClick(const Vector2i& mousePos) {
        for (size_t i = 0; i < answerButtons.size(); ++i) {
            if (answerButtons[i]->contains(mousePos)) {
                cout << "Selected answer: " << answerButtons[i]->getName() << endl;
                
                // Check if the selected answer is correct
                if (isContained(answerButtons[i]->gettext(), questions[currentIndex].getCorrectOption())) {
                    currentIndex++;
                    if (currentIndex < questions.size()) {
                        MovetoNextQuestion();
                    } else {
                        // Game over, player has answered all questions
                        cout << "Congratulations! You've answered all questions!" << endl;
                        restartGame();
                    }
                } else {
                    // Handle incorrect answer
                    cout << "Incorrect answer!" << endl;
                    // Show response window
                    quitGame();
                }
                break; // Exit the loop after handling the clicked button
            }
        }
    }

    void quitGame() {
        FailResponseWindow* responseWindow = new FailResponseWindow();
        
        responseWindow->setOnRevengeButtonClicked([this]() {
            // What you want to happen when the button is clicked
            restart();  // For example, restart the game
        });
        
        responseWindow->open();  // Show the window
        delete responseWindow;   // Clean up when done
    }

    void restartGame() {
        WinResponseWindow* responseWindow = new WinResponseWindow();
        responseWindow->setOnRoundTwoButtonClicked([this]() {
            restart();
        });

        responseWindow->open();
        delete responseWindow;
    }
    
    void update() {
        // Update timer
        timer->update();

        if (timer->getTimeRemaining() == 0.0) {
            quitGame();
        }
    }
    
    void render() {
        window.clear(backgroundColor);
        
        // Draw all panels
        for (auto& panel : panels) {
            panel.draw(window);
        }
        
        // Draw UI elements
        // Draw question box and answer buttons
        questionBox->draw(window);
        for (auto button : answerButtons) {
            button->draw(window);
        }

        // Draw lifeline buttons
        audiencePollBtn->draw(window);
        phoneFriendBtn->draw(window);
        fiftyFiftyBtn->draw(window);

        timer->draw(window);
        prizeBoard->draw(window);

        // Draw tracking texts
        questionTracking->draw(window);
        prizeTracking->draw(window);
        
        window.display();
    }

    // Add any additional methods for handling game logic, events, etc.

    bool isContained(const string& mainString, const string& subString) {
        return mainString.find(subString) != string::npos;
    }

    void handleAudiencePoll() {
        // Use proper random number generation
        random_device rd;
        mt19937 gen(rd());
        
        BarChartPoll poll;
        vector<float> percentages(4, 0.0f);
        int correctIndex = questions[currentIndex].getCorrectOptionIndex();
        
        // Create a more natural distribution
        // Dirichlet-like distribution for audience polls
        vector<float> weights(4, 1.0f);
        
        // Give the correct answer a variable advantage
        // between 1.5x and 3x more likely to be chosen
        uniform_real_distribution<float> advantageDist(1.5f, 3.0f);
        weights[correctIndex] *= advantageDist(gen);
        
        // Generate raw values based on gamma distribution for each option
        vector<float> rawValues(4);
        float sum = 0.0f;
        
        for (int i = 0; i < 4; ++i) {
            gamma_distribution<float> gammaDist(weights[i], 1.0f);
            rawValues[i] = gammaDist(gen);
            sum += rawValues[i];
        }
        
        // Normalize to percentages that sum to 100%
        for (int i = 0; i < 4; ++i) {
            percentages[i] = (rawValues[i] / sum) * 100.0f;
        }
        
        // Optional: Add small random noise to make it look more "human"
        normal_distribution<float> noiseDist(0.0f, 0.5f);
        
        // Add noise but preserve sum of 100%
        float noiseSum = 0.0f;
        for (int i = 0; i < 4; ++i) {
            float noise = noiseDist(gen);
            percentages[i] += noise;
            noiseSum += noise;
        }
        
        // Adjust to ensure sum is still 100%
        for (int i = 0; i < 4; ++i) {
            percentages[i] -= noiseSum / 4.0f;
            
            // Ensure no negative percentages
            percentages[i] = max(0.0f, percentages[i]);
        }
        
        // Final normalization to exactly 100%
        sum = accumulate(percentages.begin(), percentages.end(), 0.0f);
        for (int i = 0; i < 4; ++i) {
            percentages[i] = (percentages[i] / sum) * 100.0f;
        }
        
        // Round to one decimal place for display
        for (int i = 0; i < 4; ++i) {
            percentages[i] = round(percentages[i] * 10.0f) / 10.0f;
        }
        
        // Adjust final rounding errors if needed
        float finalSum = accumulate(percentages.begin(), percentages.end(), 0.0f);
        if (abs(finalSum - 100.0f) > 0.1f) {
            // Add the difference to the largest value to maintain 100% sum
            int largestIdx = max_element(percentages.begin(), percentages.end()) - percentages.begin();
            percentages[largestIdx] += (100.0f - finalSum);
        }
        
        poll.setPercentages(percentages);
        poll.run();

        // Hide the Audience Poll button
        audiencePollBtn->setVisibility(false);
        audiencePollBtn->draw(window);
    }
    

    void handlePhoneFriend() {
        PhoneFriendApp phoneFriend;
        phoneFriend.setCorrectAnswer(questions[currentIndex].getCorrectOption());
        phoneFriend.run();

        // Hide the Phone a Friend button
        phoneFriendBtn->setVisibility(false);
        phoneFriendBtn->draw(window);
    }

    void handleFiftyFifty() {
        // we'll remove two wrong answers from the answer buttons.
        vector<Button*> wrongAnswers;
        for (auto button : answerButtons) {
            if (!isContained(button->gettext(), questions[currentIndex].getCorrectOption())) {
                wrongAnswers.push_back(button);
            }
        }
        // Remove two wrong answers
        for (int i = 0; i < 2; ++i) {
            if (wrongAnswers[i]->getVisibility()) {
                wrongAnswers[i]->setVisibility(false);
            }
        }
        // Update the UI to reflect the changes
        for (auto button : answerButtons) {
            if (button->getVisibility()) {
                button->draw(window);
            }
        }
        // Hide the Fifty-Fifty button
        fiftyFiftyBtn->setVisibility(false);
        fiftyFiftyBtn->draw(window);
    }

    void MovetoNextQuestion() {
        questionBox->setText(questions[currentIndex].getQuestionText());
        vector<string> options = questions[currentIndex].getOptions();
        for (int i = 0; i < 4; i++) {
            answerButtons[i]->setText(string(1, 'A' + i) + ". " + questions[currentIndex].getOptions()[i]);
        }

        // redraw the UI elements
        questionBox->draw(window);
        for (auto button : answerButtons) {
            button->setVisibility(true); // Make sure all buttons are visible
            button->draw(window);
        }
        timer->reset(31);
        timer->start();

        // Update tracking texts
        prizeBoard->setCurrentTier(prizeBoard->getCurrentTier() + 1);
        questionTracking->setText("Question: " + to_string(prizeBoard->getCurrentTier()));
        prizeTracking->setText("Prize: " + prizeBoard->getCurrentTierAmount(prizeBoard->getCurrentTier()));
    }
};


void loadDataFromJson(vector<Question>& questions) {
    vector<string> database = {
        "data/easy.json",
        "data/medium.json",
        "data/hard.json"
    };
    
    random_device rd;
    mt19937 g(rd()); // Random engine for shuffling and random selection
    
    // Clear any existing questions
    questions.clear();
    
    for (const auto& filename : database) {
        ifstream file(filename);
        if (!file) {
            cout << "Error opening file: " << filename << endl;
            continue;
        }
        
        try {
            json Doc = json::parse(file);
            if (!Doc.contains("results") || !Doc["results"].is_array()) {
                cout << "Error: Invalid JSON structure in file " << filename << endl;
                continue;
            }
            
            json data = Doc["results"];
            const int questionsToPickPerFile = 5; // Always pick exactly 5 questions from each file
            
            if (data.size() < questionsToPickPerFile) {
                cout << "Warning: Not enough questions in " << filename << 
                     ". Need " << questionsToPickPerFile << " but only found " << data.size() << endl;
                continue;
            }
            
            set<int> usedIndexes;
            uniform_int_distribution<> dis(0, data.size() - 1);
            int attempts = 0;
            const int maxAttempts = 100; // Higher max attempts to ensure we get enough questions
            
            while (usedIndexes.size() < questionsToPickPerFile && attempts < maxAttempts) {
                int randomIndex = dis(g);
                if (usedIndexes.find(randomIndex) == usedIndexes.end()) {
                    auto& item = data[randomIndex];
                    if (!item.contains("correct_answer") ||
                        !item.contains("incorrect_answers") ||
                        !item["incorrect_answers"].is_array() ||
                        item["incorrect_answers"].size() < 3) {
                        attempts++;
                        continue;
                    }
                    
                    usedIndexes.insert(randomIndex);
                    Question q;
                    vector<string> tempOptions(4);
                    string correctAnswer = item["correct_answer"];
                    tempOptions[0] = correctAnswer;
                    tempOptions[1] = item["incorrect_answers"][0];
                    tempOptions[2] = item["incorrect_answers"][1];
                    tempOptions[3] = item["incorrect_answers"][2];
                    shuffle(tempOptions.begin(), tempOptions.end(), g);
                    q.setOptions(tempOptions[0], tempOptions[1], tempOptions[2], tempOptions[3]);
                    q.setQuestionText(item["question"]);
                    
                    // Find and set the correct answer index
                    for (int i = 0; i < 4; i++) {
                        if (tempOptions[i] == correctAnswer) {
                            q.setCorrectOptionIndex(i);
                            break;
                        }
                    }
                    
                    questions.push_back(q); // Directly append to the main questions list
                }
                attempts++;
            }
            
            if (usedIndexes.size() < questionsToPickPerFile) {
                cout << "Warning: Could only pick " << usedIndexes.size() << " questions from " << 
                     filename << " instead of the required " << questionsToPickPerFile << endl;
            }
        }
        catch (const json::exception& e) {
            cout << "JSON error in file " << filename << ": " << e.what() << endl;
        }
    }
    
    // Final check
    if (questions.size() < 15) {
        cout << "Warning: Only loaded " << questions.size() << " questions instead of 15" << endl;
    }
}