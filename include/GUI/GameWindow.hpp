#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <UIElement.hpp>

using namespace std;
using namespace sf;


class Panel {
private:
    RectangleShape shape;
    Color backgroundColor;
    Vector2f position;
    Vector2f size;
    string name;

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
    
public:
    Application() : backgroundColor(Color(50, 50, 50)) {
        // Create the main window
        window.create(VideoMode(1030, 600), "Who wants to be a FUCKING MILLIONAIRE, ey?", Style::Titlebar|Style::Close);
        window.setFramerateLimit(60);
        
        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            // Try common font locations as fallback
            if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf") && 
                !font.loadFromFile("/usr/share/fonts/LiberationSans-Regular.ttf") &&
                !font.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) {
                throw runtime_error("Could not load font");
            }
        }
        
        // Create panels
        // Left panel for timer
        panels.emplace_back(
            Vector2f(10, 10),                       // Position
            Vector2f(200, 580),                     // Size
            Color(100, 100, 150, 255),              // Color
            "Timer Panel"                           // Name
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
        
        // Timer in left panel
        timer = new CountdownClock(
            Vector2f(10, 10),                       // Position
            Vector2f(200, 200),                     // Size
            &font,                                  // Font
            30,                                     // Starting time (30 seconds)
            "Game Timer"                            // Name
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
        timer->draw(window);
        prizeBoard->draw(window);
        
        window.display();
    }
};