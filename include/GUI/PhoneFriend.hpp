#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

// Configuration constants
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 700;
const std::string WINDOW_TITLE = "Phone A Friend";

// Asset paths
// TODO: Replace these with your actual asset paths
const std::string FRIEND_IMAGE_PATH = "include/GUI/assets/friend.png";  // Place your friend image here
const char CORRECT_ANSWER = 'C';  // You can modify this to be A, B, C, or D

class PhoneFriendApp {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Texture friendTexture;
    sf::Sprite friendSprite;
    sf::Text messageText;
    sf::RectangleShape backgroundShape;
    sf::RectangleShape dividerLine;
    
    bool loadResources() {
        // Load font
        if (!font.loadFromFile("/usr/share/fonts/LiberationSans-Bold.ttf")) {  // Place your preferred font here
            std::cerr << "Failed to load font!" << std::endl;
            return false;
        }
        
        // Load friend image
        if (!friendTexture.loadFromFile(FRIEND_IMAGE_PATH)) {
            std::cerr << "Failed to load friend image!" << std::endl;
            return false;
        }
        
        return true;
    }
    
    void initializeGraphics() {
        // Setup friend sprite
        friendSprite.setTexture(friendTexture);
        float FriendHieght = friendSprite.getGlobalBounds().height;

        // Center the friend image in the upper part of the window
        float spriteX = (WINDOW_WIDTH - friendSprite.getGlobalBounds().width) / 2.0f;
        friendSprite.setPosition(spriteX, 20.0f);
        
        // Create the divider line
        dividerLine.setSize(sf::Vector2f(WINDOW_WIDTH - 40, 2));
        dividerLine.setFillColor(sf::Color(100, 100, 100));

        float DividerLineY = FriendHieght + 40;
        dividerLine.setPosition(20, DividerLineY);
        
        // Setup background
        backgroundShape.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        backgroundShape.setFillColor(sf::Color(132, 104, 104, 100));  // Dark blue with slight transparency
        
        // Create message text
        messageText.setFont(font);
        messageText.setCharacterSize(16);
        messageText.setFillColor(sf::Color::White);
        
        // Construct the message
        std::string message = "Hi! Congratulations for going this far!\n\nHmm I've read this somewhere. I think the answer is ";
        message += CORRECT_ANSWER;
        messageText.setString(message);
        
        // Center the text horizontally and position it below the line
        float textX = (WINDOW_WIDTH - messageText.getGlobalBounds().width) / 2.0f;
        messageText.setPosition(textX, DividerLineY + 10.0f);
    }
    
public:
    PhoneFriendApp() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, 
                              sf::Style::Close) {
        // Set window properties
        window.setFramerateLimit(60);
        
        // Load assets and initialize graphics
        if (!loadResources()) {
            std::cerr << "Application failed to initialize resources properly." << std::endl;
            window.close();
            return;
        }
        
        initializeGraphics();
    }
    
    void run() {
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }
    
    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }
    }
    
    void update() {
        // Update logic here if needed
    }
    
    void render() {
        window.clear();
        
        // Draw background
        window.draw(backgroundShape);
        
        // Draw friend image
        window.draw(friendSprite);
        
        // Draw divider line
        window.draw(dividerLine);
        
        // Draw message
        window.draw(messageText);
        
        window.display();
    }
};

int main() {
    PhoneFriendApp app;
    app.run();
    
    return 0;
}