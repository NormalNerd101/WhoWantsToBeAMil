# pragma once
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

using namespace std;
using namespace sf;

// Base UI element class
class UIElement {
protected:
    Vector2f position;
    Vector2f size;
    bool isVisible;
    string name;
    
public:
    UIElement(const Vector2f& pos, const Vector2f& sz, const string& elementName)
        : position(pos), size(sz), isVisible(true), name(elementName) {}
        
    virtual void draw(RenderWindow& window) = 0;
    virtual bool contains(const Vector2i& point) const = 0;
    virtual void handleEvent(const Event& event) {}
    
    // Getters and setters
    const Vector2f& getPosition() const { return position; }
    const Vector2f& getSize() const { return size; }
    const string& getName() const { return name; }
    bool getVisibility() const { return isVisible; }
    void setVisibility(bool visible) { isVisible = visible; }
};

// Text box class
class TextBox : public UIElement {
private:
    Text text;
    RectangleShape background;
    Font* font;
    string content;
    Color textColor;
    Color backgroundColor;
    
public:
    TextBox(const Vector2f& pos, const Vector2f& sz, Font* fnt, 
            const string& defaultText, const string& elementName)
        : UIElement(pos, sz, elementName), 
        font(fnt),
        content(defaultText), 
        textColor(Color::Black),
        backgroundColor(Color::White)
    {
        // Set up background
        background.setSize(size);
        background.setPosition(position);
        background.setFillColor(backgroundColor);
        
        // Set up text
        text.setFont(*font);
        text.setString(content);
        text.setCharacterSize(24);
        text.setFillColor(textColor);
        
        // Center text in the box
        centerText();
        fitAndWrapText();
    }
    virtual ~TextBox() = default;
    
    void draw(RenderWindow& window) override {
        if (isVisible) {
            window.draw(background);
            window.draw(text);
        }
    }
    
    bool contains(const Vector2i& point) const override {
        FloatRect bounds = background.getGlobalBounds();
        return bounds.contains(static_cast<float>(point.x), static_cast<float>(point.y));
    }
    
    void setText(const string& newText) {
        content = newText;
        fitAndWrapText();
    }

    void removeBackground() {
        background.setFillColor(Color::Transparent);
    }
    
private:
    void centerText() {
        FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            position.x + (size.x - textBounds.width) / 2.0f,
            position.y + (size.y - textBounds.height) / 2.0f - textBounds.top
        );
    }

    void fitAndWrapText() {
        const float padding = 10.0f;
        float maxWidth = size.x - 2 * padding;
        float maxHeight = size.y - 2 * padding;
    
        text.setCharacterSize(24); // Reset to default size first
        unsigned int characterSize = text.getCharacterSize();
    
        bool fits = false;
    
        while (characterSize > 5 && !fits) {
            // Create a temporary text object for measuring
            Text tempText(text);
            tempText.setCharacterSize(characterSize);
    
            std::string finalText;
            std::string currentLine;
            std::istringstream words(content);
            std::string word;
    
            while (words >> word) {
                std::string testLine = currentLine + (currentLine.empty() ? "" : " ") + word;
                tempText.setString(testLine);
                if (tempText.getLocalBounds().width > maxWidth) {
                    if (!currentLine.empty()) {
                        finalText += currentLine + '\n';
                    }
                    currentLine = word;
                } else {
                    currentLine = testLine;
                }
            }
            finalText += currentLine;
    
            // Set real text only after finishing wrapping
            text.setCharacterSize(characterSize);
            text.setString(finalText);
    
            FloatRect bounds = text.getLocalBounds();
            if (bounds.height <= maxHeight) {
                fits = true;
            } else {
                characterSize--; // Shrink and try again
            }
        }
    
        centerText();
    }    
};

// Simple text display
class SimpleText : public UIElement {
private:
    Text text;
    Font* font;
    Color textColor;

public:
    SimpleText(const Vector2f& pos, const Vector2f& sz, Font* fnt,
        const string& displayText, const string& elementName,
        unsigned int characterSize = 16)  // <-- new parameter with default value
        : UIElement(pos, sz, elementName),
        font(fnt),
        textColor(Color::Black)
    {
        text.setFont(*font);
        text.setString(displayText);
        text.setCharacterSize(characterSize); // <-- use it here
        text.setFillColor(textColor);
        text.setPosition(pos);
    }

    bool contains(const Vector2i& point) const override {
        FloatRect bounds = text.getGlobalBounds();
        return bounds.contains(static_cast<float>(point.x), static_cast<float>(point.y));
    }

    void setText(const string& newText) {
        text.setString(newText);
    }

    void draw(RenderWindow& window) override {
        if (isVisible) {
            window.draw(text);
        }
    }
};
    

// Button class
class Button : public UIElement {
private:
    RectangleShape shape;
    Text text;
    Font* font;
    Color idleColor;
    Color hoverColor;
    Color activeColor;
    Color currentColor;
    
public:
    Button(const Vector2f& pos, const Vector2f& sz, Font* fnt,
            const string& buttonText, const string& elementName)
        : UIElement(pos, sz, elementName),
            font(fnt),
            idleColor(Color(128, 222, 234)),
            hoverColor(Color(153, 222, 117)),
            activeColor(Color(160, 160, 160))
    {
        currentColor = idleColor;
        
        // Set up shape
        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(currentColor);
        
        // Set up text
        text.setFont(*font);
        text.setString(buttonText);
        text.setCharacterSize(24);
        text.setFillColor(Color::White);
        
        // Center text
        centerText();
    }
    virtual ~Button() = default;
    
    void draw(RenderWindow& window) override {
        if (isVisible) {
            window.draw(shape);
            window.draw(text);
        }
    }
    
    bool contains(const Vector2i& point) const override {
        FloatRect bounds = shape.getGlobalBounds();
        return bounds.contains(static_cast<float>(point.x), static_cast<float>(point.y));
    }
    
    void handleEvent(const Event& event) override {
        if (event.type == Event::MouseMoved) {
            Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
            if (contains(mousePos)) {
                shape.setFillColor(hoverColor);
            } else {
                shape.setFillColor(idleColor);
            }
        }
        
        if (event.type == Event::MouseButtonPressed) {
            if (event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
                if (contains(mousePos)) {
                    shape.setFillColor(activeColor);
                }
            }
        }
        
        if (event.type == Event::MouseButtonReleased) {
            if (event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
                if (contains(mousePos)) {
                    // Button action goes here
                    shape.setFillColor(hoverColor);
                }
            }
        }
    }
    
    string gettext() {
        return text.getString();
    }
    void setText(const string& newText) {
        text.setString(newText);
        centerText();
    }

    // set customized colors
    void setIdleColor(const Color& color) { idleColor = color; }
    void setHoverColor(const Color& color) { hoverColor = color; }
    void setActiveColor(const Color& color) { activeColor = color; }
    
private:
    void centerText() {
        FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            position.x + (size.x - textBounds.width) / 2.0f,
            position.y + (size.y - textBounds.height) / 2.0f - textBounds.top
        );
    }
};

class CountdownClock : public UIElement {
private:
    Text timeText;
    Font* font;
    Clock clock;
    float remainingTime;
    bool isRunning;
    
public:
    CountdownClock(const Vector2f& pos, const Vector2f& sz, Font* fnt, 
                    float startTime, const string& elementName)
        : UIElement(pos, sz, elementName),
            font(fnt),
            remainingTime(startTime),
            isRunning(false)
    {
        // Set up time text
        timeText.setFont(*font);
        timeText.setCharacterSize(48);
        timeText.setFillColor(Color::White);
        
        updateTimeDisplay();
        centerText();
    }
    virtual ~CountdownClock() = default;
    
    void draw(RenderWindow& window) override {
        if (isVisible) {
            window.draw(timeText);
        }
    }
    
    bool contains(const Vector2i& point) const override {
        FloatRect bounds = timeText.getGlobalBounds();
        return bounds.contains(static_cast<float>(point.x), static_cast<float>(point.y));
    }
    
    void update() {
        if (isRunning && remainingTime > 0) {
            float elapsed = clock.restart().asSeconds();
            remainingTime -= elapsed;
            
            if (remainingTime <= 0) {
                remainingTime = 0;
                isRunning = false;
                // Time's up event can be triggered here
            }
            
            updateTimeDisplay();
        }
    }
    
    void start() {
        isRunning = true;
        clock.restart();
    }
    
    void pause() {
        isRunning = false;
    }
    
    void reset(float newTime) {
        remainingTime = newTime;
        isRunning = false;
        updateTimeDisplay();
    }
    
private:
    void updateTimeDisplay() {
        int seconds = static_cast<int>(remainingTime);
        string timeString = to_string(seconds);
        timeText.setString(timeString);
        
        // Change color based on time left
        if (remainingTime <= 5) {
            timeText.setFillColor(Color::Red);
        } else if (remainingTime <= 10) {
            timeText.setFillColor(Color::Yellow);
        } else {
            timeText.setFillColor(Color::White);
        }
        
        centerText();
    }
    
    void centerText() {
        FloatRect textBounds = timeText.getLocalBounds();
        timeText.setPosition(
            position.x + (size.x - textBounds.width) / 2.0f,
            position.y + (size.y - textBounds.height) / 2.0f - textBounds.top
        );
    }
};


class PrizeTierBoard : public UIElement {
private:
    struct PrizeTier {
        int level;
        string amount;
        bool isMilestone;
        bool isCurrentQuestion;
    };
    
    vector<PrizeTier> tiers;
    vector<Text> tierTexts;
    Font* font;
    int currentTier;
    
public:
    PrizeTierBoard(const Vector2f& pos, const Vector2f& sz, Font* fnt, const string& elementName)
        : UIElement(pos, sz, elementName),
            font(fnt),
            currentTier(1)
    {
        // Initialize prize tiers (customize as needed)
        tiers = {
            {15, "$1,000,000", true, false},
            {14, "$500,000", false, false},
            {13, "$250,000", false, false},
            {12, "$125,000", false, false},
            {11, "$64,000", false, false},
            {10, "$32,000", true, false},
            {9, "$16,000", false, false},
            {8, "$8,000", false, false},
            {7, "$4,000", false, false},
            {6, "$2,000", false, false},
            {5, "$1,000", true, false},
            {4, "$500", false, false},
            {3, "$300", false, false},
            {2, "$200", false, false},
            {1, "$100", false, false}
        };
        
        // Set the first question as current
        tiers[14].isCurrentQuestion = true;
        
        // Create text for each tier
        float tierHeight = size.y / tiers.size();
        
        for (size_t i = 0; i < tiers.size(); i++) {
            Text tierText;
            tierText.setFont(*font);
            tierText.setCharacterSize(18);
            
            // Format: "Q15: $1,000,000"
            string displayText = "Q" + to_string(tiers[i].level) + ": " + tiers[i].amount;
            tierText.setString(displayText);
            
            // Milestones in gold, current question in white, others in gray
            if (tiers[i].isCurrentQuestion) {
                tierText.setFillColor(Color::White);
            } else if (tiers[i].isMilestone) {
                tierText.setFillColor(Color(255, 215, 0)); // Gold
            } else {
                tierText.setFillColor(Color(200, 200, 200));
            }
            
            // Position the text (centered horizontally, distributed vertically)
            tierText.setPosition(
                position.x + 10,
                position.y + i * tierHeight + (tierHeight - tierText.getLocalBounds().height) / 2
            );
            
            tierTexts.push_back(tierText);
        }
    }
    virtual ~PrizeTierBoard() = default;
    
    void draw(RenderWindow& window) override {
        if (isVisible) {
            for (const auto& text : tierTexts) {
                window.draw(text);
            }
        }
    }
    
    bool contains(const Vector2i& point) const override {
        FloatRect bounds(position.x, position.y, size.x, size.y);
        return bounds.contains(static_cast<float>(point.x), static_cast<float>(point.y));
    }
    
    void setCurrentTier(int tier) {
        // Reset current tier
        for (auto& t : tiers) {
            t.isCurrentQuestion = false;
        }
    
        // Set new current tier
        if (tier >= 1 && tier <= 15) {
            tiers[15 - tier].isCurrentQuestion = true;
            this->currentTier = tier;  
    
            // Update text colors
            for (size_t i = 0; i < tiers.size(); i++) {
                if (tiers[i].isCurrentQuestion) {
                    tierTexts[i].setFillColor(Color::White);
                } else if (tiers[i].isMilestone) {
                    tierTexts[i].setFillColor(Color(255, 215, 0)); // Gold
                } else {
                    tierTexts[i].setFillColor(Color(200, 200, 200));
                }
            }
        }
    }
    
    // Get the amount for the current tier
    string getCurrentTierAmount(int tier) {
        if (tier >= 1 && tier <= 15) {
            return this->tiers[15 - tier].amount;
        } else {
            return "Invalid tier";
        }
    }

    int getCurrentTier() {
        return this->currentTier;
    }
};