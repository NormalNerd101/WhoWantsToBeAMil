#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <UIElement.hpp>

// import LifeLineSupports
#include <AudiencePoll.hpp>
#include <PhoneFriend.hpp>

// import Questions
#include <Questions.hpp>

using namespace std;
using namespace sf;


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
    
public:
    Application() : backgroundColor(Color(50, 50, 50)) {
        // Create the main window
        window.create(VideoMode(1030, 600), "Who wants to be a FUCKING MILLIONAIRE, ey?", Style::Titlebar|Style::Close);
        window.setFramerateLimit(60);
        
        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            // Try common font locations as fallback
            if (!font.loadFromFile("assets/fonts/LiberationSans-Regular.ttf")) {
                throw runtime_error("Could not load font");
            }
        }
        
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
            "Who was the first president of the United States?", // Default text
            "Question Text"                         // Name
        );
        
        // Answer buttons in center bottom panel
        string answerOptions[4] = {
            "A: George Washington",
            "B: Thomas Jefferson",
            "C: Abraham Lincoln",
            "D: John Adams"
        };
        
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
            "Game Timer"                            // Name
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
        phoneFriendBtn = new Button(
            Vector2f(10, 340),                     // Position
            Vector2f(200, 100),                    // Size
            &font,                                  // Font
            "Phone a Friend",                       // Text
            "Phone a Friend Button"                 // Name
        );
        fiftyFiftyBtn = new Button(
            Vector2f(10, 450),                     // Position
            Vector2f(200, 100),                    // Size
            &font,                                  // Font
            "50/50",                                // Text
            "50/50 Button"                          // Name
        );

        
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
                        handleAuditioncePoll();
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
        // Check if any answer button was clicked
        for (size_t i = 0; i < answerButtons.size(); ++i) {
            if (answerButtons[i]->contains(mousePos)) {
                cout << "Selected answer: " << answerButtons[i]->getName() << endl;
                // Handle answer selection logic here
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

    void handleAuditioncePoll() {
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
        string correctAnswer = "A: George Washington"; // Placeholder for the correct answer
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
};