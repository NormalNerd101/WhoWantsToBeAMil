# pragma once
#include <SFML/Graphics.hpp>
#include <UIElements.hpp>

const std::string CHAD_IMAGE_PATH = "assets/damn.jpg";

class WinResponseWindow {
private:
    Font font;
    Color backgroundColor;
    sf::RenderWindow window;
    SimpleText* insultText;
    Button* RoundTwoButton;
    sf::Texture texture;
    sf::Sprite image;
    std::function<void()> onRoundTwoButtonClicked;

public:
    WinResponseWindow() : window(sf::VideoMode(700, 500), "Oh no .... Anyway", sf::Style::Close) {
        window.setFramerateLimit(60);

        // Load font
        if (!font.loadFromFile("assets/fonts/PoetsenOne-Regular.ttf")) {
            throw std::runtime_error("Could not load font");
        }
        // Center the window on the screen(1920x1080)
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        window.setPosition(sf::Vector2i(
            (desktop.width - 700) / 2,
            (desktop.height - 500) / 2
        ));

        // Create the text
        insultText = new SimpleText(
            sf::Vector2f(10, 30), // Position
            sf::Vector2f(0, 0), // Size
            &font,                  // Font
            "Insult Text",         // Default text
            "Insult Text Element",  // Name
            50                  // Character size
        );
        insultText->setText("\t\t\t Wait ...That's it?....");


        // set up image
        if (!texture.loadFromFile(CHAD_IMAGE_PATH)) {
            throw std::runtime_error("Could not load image");
        }
        image.setTexture(texture); // you forgot this!
        image.setScale(1.0f, 1.0f);
        
        // Now *after* scaling, set the origin
        sf::FloatRect bounds = image.getLocalBounds();
        image.setOrigin(bounds.width / 2, bounds.height / 2);
        image.setPosition(window.getSize().x / 2, window.getSize().y / 2);
        


        // Create the button
        RoundTwoButton = new Button(
            sf::Vector2f(250, 380), // Position
            sf::Vector2f(200, 80), // Size
            &font,                  // Font
            "Revenge?",            // Text
            "Revenge Button"       // Name
        );
        RoundTwoButton->setText("Revenge?");
    }

    ~WinResponseWindow() {
        delete RoundTwoButton;
    }

    void open() {
        while (window.isOpen()) {
            processEvents();
            render();
        }
    }

    void setOnRoundTwoButtonClicked(std::function<void()> callback) {
        onRoundTwoButtonClicked = callback;
    }

private:
    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // Handle mouse events
            RoundTwoButton->handleEvent(event);

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                if (RoundTwoButton->contains(mousePos)) {
                    // close the window
                    window.close();
                    // Call the callback function if set
                    if (onRoundTwoButtonClicked) {
                        onRoundTwoButtonClicked();
                    }
                }
            }
        }
    }

    void render() {
        window.clear(sf::Color(255, 171, 145, 255)); // Bright orange.
        
        // Draw the text and button
        insultText->draw(window);
        window.draw(image);
        RoundTwoButton->draw(window);
        
        window.display();
    }
};


