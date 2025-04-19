#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <UIElements.hpp>

// import LifeLineSupports
#include <AudiencePoll.hpp>
#include <PhoneFriend.hpp>

// import other classes
#include <Questions.hpp>
#include <json.hpp>

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
    

public:
    Application() : backgroundColor(Color(50, 50, 50)) {
        // Create the main window
        window.create(VideoMode(1030, 600), "Who wants to be a FUCKING MILLIONAIRE, ey?", Style::Titlebar|Style::Close);
        window.setFramerateLimit(60);
        
        // Load font
        if (!font.loadFromFile("assets/fonts/LiberationSans-Regular.ttf")) {
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
        
        // Right panel for prize board
        panels.emplace_back(
            Vector2f(830, 10),                      // Position
            Vector2f(190, 580),                     // Size
            Color(218, 112, 112, 255),              // Color
            "Prize Board Panel"                     // Name
        );
        
        // Create UI elements
        // Question box in center top panel
        questionBox = new TextBox(
            Vector2f(240, 30),                      // Position
            Vector2f(560, 240),                     // Size
            &font,                                  // Font
            questions[currentIndex].getQuestionText(), // Default text
            "Question Text"                         // Name
        );
        
        // Answer buttons in center bottom panel
        string answerOptions[4] = {};
        for (int i = 0; i < 4; i++) {
            answerOptions[i] = questions[currentIndex].getOptions()[i];
        }
        
        for (int i = 0; i < 4; i++) {
            Button* btn = new Button(
                Vector2f(240, 320 + i * 65),        // Position (stacked vertically)
                Vector2f(560, 55),                  // Size
                &font,                              // Font
                answerOptions[i],                   // Text
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
        
        // Prize board in right panel
        prizeBoard = new PrizeTierBoard(
            Vector2f(830, 10),                      // Position
            Vector2f(190, 580),                     // Size
            &font,                                  // Font
            "Prize Tier Board"                      // Name
        );
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
                if (answerButtons[i]->gettext() == questions[currentIndex].getCorrectOption()) {
                    currentIndex++;
                    if (currentIndex < questions.size()) {
                        MovetoNextQuestion();
                    } else {
                        // Handle end of game
                        cout << "Game completed!" << endl;
                    }
                } else {
                    // Handle incorrect answer
                    cout << "Incorrect answer!" << endl;
                }
                break; // Exit the loop after handling the clicked button
            }
        }
    }
    
    void update() {
        // Update timer
        timer->update();
    }
    
    void render() {
        window.clear(backgroundColor);
        
        // Draw all panels
        for (auto& panel : panels) {
            panel.draw(window);
        }
        
        // Draw UI elements
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
        
        window.display();
    }

    // Add any additional methods for handling game logic, events, etc.

    void handleAudiencePoll() {
        // Handle audience poll logic
        BarChartPoll poll;
        poll.run();
    }

    void handlePhoneFriend() {
        PhoneFriendApp phoneFriend;
        phoneFriend.run();
    }

    void handleFiftyFifty() {
        // we'll remove two wrong answers from the answer buttons.
        string correctAnswer = questions[currentIndex].getCorrectOption();
        vector<Button*> wrongAnswers;
        for (auto button : answerButtons) {
            if (button->gettext() != correctAnswer) {
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
    }

    void MovetoNextQuestion() {
        questionBox->setText(questions[currentIndex].getQuestionText());
        vector<string> options = questions[currentIndex].getOptions();
        for (int i = 0; i < 4; i++) {
            answerButtons[i]->setText(questions[currentIndex].getOptions()[i]);
        }

        // redraw the UI elements
        questionBox->draw(window);
        for (auto button : answerButtons) {
            button->setVisibility(true); // Make sure all buttons are visible
            button->draw(window);
        }
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
            int toPick = min(5, (int)data.size()); // Pick 5 questions or as many as available

            set<int> usedIndexes;
            uniform_int_distribution<> dis(0, data.size() - 1);

            int attempts = 0;
            const int maxAttempts = 20 * toPick;

            while (usedIndexes.size() < toPick && attempts < maxAttempts) {
                int randomIndex = dis(g);

                if (usedIndexes.find(randomIndex) == usedIndexes.end()) {
                    auto& item = data[randomIndex];

                    if (!item.contains("correct_answer") ||
                        !item.contains("incorrect_answers") ||
                        !item["incorrect_answers"].is_array() ||
                        item["incorrect_answers"].size() < 3) {
                        cout << "Error: Invalid question format at index " << randomIndex << " in file " << filename << endl;
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
        }
        catch (const json::exception& e) {
            cout << "JSON error in file " << filename << ": " << e.what() << endl;
        }
    }
}