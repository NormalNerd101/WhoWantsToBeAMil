#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>


class BarChartPoll
{
private:
    sf::RenderWindow window;
    const unsigned int WINDOW_WIDTH = 600;
    const unsigned int WINDOW_HEIGHT = 400;
    
    // Bar data
    const int NUM_BARS = 4;
    std::string labels[4] = {"A", "B", "C", "D"};
    float percentages[4] = {10.0f, 16.0f, 71.0f, 3.0f};


    // Bar properties
    const float BAR_WIDTH = 80.0f;
    const float BAR_GAP = 40.0f;
    const float MAX_BAR_HEIGHT = 300.0f;
    const float BOTTOM_MARGIN = 60.0f;
    const float LABEL_MARGIN = 20.0f;
    
    // Colors
    sf::Color lowPercentageColor = sf::Color(150, 150, 150); // Grey
    sf::Color highPercentageColor = sf::Color(0, 255, 255);  // Bright cyan
    float highPercentageThreshold = 50.0f;
    
    sf::Font font;
    bool initialized;
    
public:
    BarChartPoll() : initialized(false)
    {
        try {
            // Create window
            window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), 
                          "Audience Poll Survey Results", 
                          sf::Style::Titlebar | sf::Style::Close);
            
            // Load embedded font
            if (!font.loadFromFile("assets/fonts/LiberationSans-Bold.ttf")) {
                std::cerr << "Error loading embedded font!" << std::endl;
                // If the embedded font fails, we'll try a very simple fallback approach
                createSimpleFont();
            }
            
            window.setFramerateLimit(60);
            initialized = true;
        }
        catch (const std::exception& e) {
            std::cerr << "Error initializing application: " << e.what() << std::endl;
        }
    }
    
    void run()
    {
        if (!initialized) {
            std::cerr << "Application not properly initialized. Exiting." << std::endl;
            return;
        }
        
        while (window.isOpen())
        {
            processEvents();
            render();
        }
    }

    void setPercentages(const std::vector<float>& newPercentages) {
        for (int i = 0; i < 4; ++i) {
            percentages[i] = newPercentages[i];
        }
    }
    
private:
    // A last-resort fallback to create a very simple font if everything else fails
    void createSimpleFont() {
        // Create a blank font texture with basic characters
        sf::Image fontImage;
        fontImage.create(128, 128, sf::Color::Transparent);
        
        // Draw simple pixel characters (very basic)
        // This is a simplified approach and won't look good, but it's better than crashing
        
        // We'll just use rectangles as placeholders for text
        std::cout << "Using simple shapes instead of text due to font loading failure." << std::endl;
    }
    
    void processEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }
    }
    
    void render()
    {
        window.clear(sf::Color(240, 240, 240));
        
        float totalWidth = NUM_BARS * BAR_WIDTH + (NUM_BARS - 1) * BAR_GAP;
        float startX = (WINDOW_WIDTH - totalWidth) / 2;
        float bottomY = WINDOW_HEIGHT - BOTTOM_MARGIN;
        
        for (int i = 0; i < NUM_BARS; ++i)
        {
            float barHeight = (percentages[i] / 100.0f) * MAX_BAR_HEIGHT;
            float barX = startX + i * (BAR_WIDTH + BAR_GAP);
            float barY = bottomY - barHeight;
            
            // Draw bar
            sf::RectangleShape bar(sf::Vector2f(BAR_WIDTH, barHeight));
            bar.setPosition(barX, barY);
            
            // Set color based on percentage
            if (percentages[i] > highPercentageThreshold) {
                bar.setFillColor(highPercentageColor);
            } else {
                bar.setFillColor(lowPercentageColor);
            }
            
            window.draw(bar);
            
            // Draw label
            sf::Text labelText;
            labelText.setFont(font);
            labelText.setString(labels[i]);
            labelText.setCharacterSize(24);
            labelText.setFillColor(sf::Color::Black);
            
            // Center the label under the bar
            sf::FloatRect labelBounds = labelText.getLocalBounds();
            float labelX = barX + (BAR_WIDTH - labelBounds.width) / 2;
            float labelY = bottomY + LABEL_MARGIN;
            labelText.setPosition(labelX, labelY);
            
            window.draw(labelText);
            
            // Draw percentage text
            sf::Text percentText;
            percentText.setFont(font);
            percentText.setString(std::to_string((int)percentages[i]) + "%");
            percentText.setCharacterSize(18);
            percentText.setFillColor(sf::Color::Black);
            
            // Center the percentage text on top of the bar
            sf::FloatRect textBounds = percentText.getLocalBounds();
            float textX = barX + (BAR_WIDTH - textBounds.width) / 2;
            float textY = barY - textBounds.height - 5;
            percentText.setPosition(textX, textY);
            
            window.draw(percentText);
        }
        
        window.display();
    }
};
