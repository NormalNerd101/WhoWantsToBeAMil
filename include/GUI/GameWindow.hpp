#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

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
        shape.setPosition(position);
        shape.setSize(size);
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
    
public:
    Application() : backgroundColor(Color(50, 50, 50)) {
        // Create the main window
        window.create(VideoMode(800, 600), "SFML Three Panel GUI");
        window.setFramerateLimit(60);
        
        // Create three panels
        // Left panel
        panels.emplace_back(
            Vector2f(10, 10),                       // Position
            Vector2f(200, 580),                     // Size
            Color(100, 100, 150, 255),              // Color
            "Left Panel"                                // Name
        );
        
        // Center panel
        panels.emplace_back(
            Vector2f(220, 10),                      // Position
            Vector2f(350, 580),                     // Size
            Color(100, 150, 100, 255),              // Color
            "Center Panel"                              // Name
        );
        
        // Right panel
        panels.emplace_back(
            Vector2f(580, 10),                      // Position
            Vector2f(210, 580),                     // Size
            Color(150, 100, 100, 255),              // Color
            "Right Panel"                               // Name
        );
    }
    
    void run() {
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
            else if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    handleMouseClick(mousePos);
                }
            }
        }
    }
    
    void handleMouseClick(const Vector2i& mousePos) {
        for (size_t i = 0; i < panels.size(); ++i) {
            if (panels[i].contains(mousePos)) {
                cout << "Clicked on " << panels[i].getName() << endl;
                // Add specific panel interaction logic here
            }
        }
    }
    
    void update() {
        // Update logic goes here
    }
    
    void render() {
        window.clear(backgroundColor);
        
        // Draw all panels
        for (auto& panel : panels) {
            panel.draw(window);
        }
        
        window.display();
    }
};

int main() {
    try {
        Application app;
        app.run();
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}