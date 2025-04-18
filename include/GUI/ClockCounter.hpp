#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cmath>

int main()
{
    // Create a window (popup sized)
    sf::RenderWindow window(sf::VideoMode(300, 200), "Countdown Timer", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        // Fallback to default font if available
        if (!font.loadFromFile("/usr/share/fonts/LiberationSans-Bold.ttf")) {
            return EXIT_FAILURE;
        }
    }

    // Timer text
    sf::Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(50);
    timerText.setFillColor(sf::Color::White);
    timerText.setStyle(sf::Text::Bold);
    
    // Center the text
    timerText.setPosition(
        (window.getSize().x - timerText.getLocalBounds().width) / 2,
        (window.getSize().y - timerText.getLocalBounds().height) / 2 - 25
    );

    // Clock for timing
    sf::Clock clock;
    
    // Start with 30 seconds
    float timeRemaining = 30.0f;
    bool timerFinished = false;

    // Main loop
    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            // Restart timer if space is pressed
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                timeRemaining = 30.0f;
                timerFinished = false;
                clock.restart();
            }
        }

        // Update timer if not finished
        if (!timerFinished)
        {
            float elapsed = clock.getElapsedTime().asSeconds();
            clock.restart();
            
            timeRemaining -= elapsed;
            
            // Check if timer finished
            if (timeRemaining <= 0)
            {
                timeRemaining = 0;
                timerFinished = true;
            }
        }

        // Update timer text
        int seconds = static_cast<int>(std::ceil(timeRemaining));
        std::string timerString = std::to_string(seconds);
        timerText.setString(timerString);
        
        // Re-center the text (as string length might change)
        timerText.setPosition(
            (window.getSize().x - timerText.getLocalBounds().width) / 2,
            (window.getSize().y - timerText.getLocalBounds().height) / 2 - 10
        );
        
        // Change color based on time remaining
        if (timeRemaining <= 5.0f)
            timerText.setFillColor(sf::Color::Red);
        else if (timeRemaining <= 10.0f)
            timerText.setFillColor(sf::Color::Yellow);
        else
            timerText.setFillColor(sf::Color::White);

        // Clear and draw
        window.clear(sf::Color(50, 50, 50));
        window.draw(timerText);
        window.display();
    }

    return 0;
}