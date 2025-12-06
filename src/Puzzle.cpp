/*
 * Museum Escape - Puzzle Class Implementation
 * CS/CE 224/272 - Fall 2025
 */

#include "Puzzle.h"
#include <algorithm>
#include <iostream>
#include <cctype>

// Puzzle Base Class
Puzzle::Puzzle(const std::string& desc, const std::string& hintText, int bonus, int penalty)
    : isSolved(false), 
      description(desc), 
      hint(hintText), 
      timeBonus(bonus), 
      timePenalty(penalty),
      justSolved(false),
      storedPosition({0.f, 0.f}) // Initialize default position
{
    // worldSprite is left empty (std::nullopt) here, preventing the error
}

bool Puzzle::isSolvedStatus() const { return isSolved; }
std::string Puzzle::getDescription() const { return description; }
std::string Puzzle::getHint() const { return hint; }
int Puzzle::getTimeBonus() const { return timeBonus; }
int Puzzle::getTimePenalty() const { return timePenalty; }
void Puzzle::setSolved(bool status) { isSolved = status; }

bool Puzzle::wasJustSolved() {
    if (justSolved) {
        justSolved = false; // Reset after reading
        return true;
    }
    return false;
}

void Puzzle::setPosition(float x, float y) {
    storedPosition = {x, y};
    if (worldSprite.has_value()) {
        worldSprite->setPosition(storedPosition);
    }
}

// === FIXED: Create the sprite now that we have a texture ===
void Puzzle::setWorldTexture(const sf::Texture& texture) {
    // Construct the sprite IN PLACE using the texture
    worldSprite.emplace(texture);
    
    // Apply the stored settings
    worldSprite->setPosition(storedPosition);
    float targetSize = 40.0f; 
    
    sf::Vector2u texSize = texture.getSize();
    // Calculate scale factor (Target / Max Dimension)
    float scaleFactor = targetSize / std::max(texSize.x, texSize.y);
    
    worldSprite->setScale({scaleFactor, scaleFactor}); 
    worldSprite->setColor(sf::Color(255, 200, 200));
}

// === FIXED: Check if sprite exists before drawing ===
void Puzzle::drawWorldSprite(sf::RenderWindow& window) {
    if (!isSolved && worldSprite.has_value()) {
        window.draw(*worldSprite); // Dereference (*) to get the actual sprite
    }
}

// === FIXED: Check if sprite exists before collision ===
bool Puzzle::checkCollision(const sf::FloatRect& bounds) const {
    if (isSolved || !worldSprite.has_value()) return false;
    return worldSprite->getGlobalBounds().findIntersection(bounds).has_value();
}
// ============================================================================
// RiddlePuzzle - Fully Interactive
// ============================================================================

RiddlePuzzle::RiddlePuzzle(const std::string& riddleText, const std::string& answer)
    : Puzzle(riddleText, "Think carefully...", 30, 10),
      riddle(riddleText),
      correctAnswer(answer),
      riddleText(defaultFont),
      inputText(defaultFont),
      showFeedback(false) {
    
    // Convert answer to lowercase for case-insensitive comparison
    std::transform(correctAnswer.begin(), correctAnswer.end(), correctAnswer.begin(), ::tolower);
}

bool RiddlePuzzle::solve(const std::string& answer) {
    // Convert input to lowercase
    std::string lowerAnswer = answer;
    std::transform(lowerAnswer.begin(), lowerAnswer.end(), lowerAnswer.begin(), ::tolower);
    
    // Trim whitespace
    lowerAnswer.erase(0, lowerAnswer.find_first_not_of(" \t\n\r"));
    lowerAnswer.erase(lowerAnswer.find_last_not_of(" \t\n\r") + 1);
    
    if (lowerAnswer == correctAnswer) {
        isSolved = true;
        feedbackMessage = "Correct! +" + std::to_string(timeBonus) + " seconds!";
        showFeedback = true;
        return true;
    } else {
        feedbackMessage = "Wrong! -" + std::to_string(timePenalty) + " seconds. Try again.";
        showFeedback = true;
        return false;
    }
}

void RiddlePuzzle::display(sf::RenderWindow& window) {
    // Dark overlay
    sf::RectangleShape overlay({800.0f, 600.0f});
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);
    
    // Puzzle box
    sf::RectangleShape puzzleBox({600.0f, 400.0f});
    puzzleBox.setPosition({100.0f, 100.0f});
    puzzleBox.setFillColor(sf::Color(40, 40, 60));
    puzzleBox.setOutlineThickness(3.0f);
    puzzleBox.setOutlineColor(sf::Color::White);
    window.draw(puzzleBox);
    
    // Title
    sf::Text title(font);
    title.setString("RIDDLE PUZZLE");
    title.setCharacterSize(28);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition({250.0f, 120.0f});
    window.draw(title);
    
    // Riddle text
    riddleText.setFont(font);
    riddleText.setString(riddle);
    riddleText.setCharacterSize(20);
    riddleText.setFillColor(sf::Color::White);
    riddleText.setPosition({130.0f, 180.0f});
    window.draw(riddleText);
    
    // Input prompt
    sf::Text promptText(font);
    promptText.setString("Your Answer:");
    promptText.setCharacterSize(18);
    promptText.setFillColor(sf::Color::Cyan);
    promptText.setPosition({130.0f, 320.0f});
    window.draw(promptText);
    
    // Input box
    sf::RectangleShape inputBox({540.0f, 40.0f});
    inputBox.setPosition({130.0f, 350.0f});
    inputBox.setFillColor(sf::Color(20, 20, 30));
    inputBox.setOutlineThickness(2.0f);
    inputBox.setOutlineColor(sf::Color::White);
    window.draw(inputBox);
    
    // User input text
    inputText.setFont(font);
    inputText.setString(userAnswer + "_");  // Cursor
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition({140.0f, 357.0f});
    window.draw(inputText);
    
    // Feedback message
    if (showFeedback) {
        sf::Text feedback(font);
        feedback.setString(feedbackMessage);
        feedback.setCharacterSize(18);
        
        if (isSolved) {
            feedback.setFillColor(sf::Color::Green);
        } else {
            feedback.setFillColor(sf::Color::Red);
        }
        
        feedback.setPosition({130.0f, 410.0f});
        window.draw(feedback);
    }
    
    // Instructions
    sf::Text instructions(font);
    instructions.setString("Press ENTER to submit | ESC to exit");
    instructions.setCharacterSize(16);
    instructions.setFillColor(sf::Color(150, 150, 150));
    instructions.setPosition({200.0f, 460.0f});
    window.draw(instructions);
}

void RiddlePuzzle::handleInput(sf::Event& event) {
    if (isSolved) return;  // Don't accept input if already solved
    
    if (const auto* textEntered = event.getIf<sf::Event::TextEntered>()) {
        char entered = static_cast<char>(textEntered->unicode);
        
        // Handle backspace
        if (entered == 8 && !userAnswer.empty()) {  // 8 is backspace
            userAnswer.pop_back();
            showFeedback = false;
        }
        // Handle regular characters (letters, numbers, spaces)
        else if (entered >= 32 && entered < 127 && userAnswer.length() < 30) {
            userAnswer += entered;
            showFeedback = false;
        }
    }
    
    // Handle Enter key to submit
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Enter && !userAnswer.empty()) {
            solve(userAnswer);
        }
    }
}

void RiddlePuzzle::update(float deltaTime) {
    // Animation or timer logic could go here
}

void RiddlePuzzle::setFont(const sf::Font& f) {
    font = f;
    riddleText.setFont(font);
    inputText.setFont(font);
}

// ============================================================================
// PatternPuzzle - Click switches in correct order
// ============================================================================

PatternPuzzle::PatternPuzzle(const std::vector<int>& pattern)
    : Puzzle("Match the pattern", "Watch carefully...", 40, 15),
      correctPattern(pattern),
      instructionText(defaultFont),
      maxSwitches(pattern.size()) {
    
    // Create 4 colored switches
    std::vector<sf::Color> colors = {
        sf::Color::Blue,   // 1
        sf::Color::Red,    // 2
        sf::Color::Green,  // 3
        sf::Color::Yellow  // 4
    };
    
    float startX = 200.0f;
    float spacing = 120.0f;
    float y = 350.0f;
    
    for (int i = 0; i < 4; i++) {
        sf::RectangleShape button({80.0f, 80.0f});
        button.setPosition({startX + i * spacing, y});
        button.setFillColor(colors[i]);
        button.setOutlineThickness(3.0f);
        button.setOutlineColor(sf::Color::White);
        switches.push_back(button);
    }
}

bool PatternPuzzle::solve(const std::string& answer) {
    return checkPattern();
}

void PatternPuzzle::display(sf::RenderWindow& window) {
    // Dark overlay
    sf::RectangleShape overlay({800.0f, 600.0f});
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);
    
    // Puzzle box
    sf::RectangleShape puzzleBox({600.0f, 450.0f});
    puzzleBox.setPosition({100.0f, 75.0f});
    puzzleBox.setFillColor(sf::Color(40, 40, 60));
    puzzleBox.setOutlineThickness(3.0f);
    puzzleBox.setOutlineColor(sf::Color::White);
    window.draw(puzzleBox);
    
    // Title
    sf::Text title(font);
    title.setString("PATTERN PUZZLE");
    title.setCharacterSize(28);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition({250.0f, 95.0f});
    window.draw(title);
    
    // Instructions
    sf::Text instructions(font);
    instructions.setString("Click the switches in this order:\nBlue -> Green -> Red -> Yellow");
    instructions.setCharacterSize(20);
    instructions.setFillColor(sf::Color::White);
    instructions.setPosition({150.0f, 150.0f});
    window.draw(instructions);
    
    // Your sequence
    sf::Text sequenceText(font);
    std::string seq = "Your sequence: ";
    for (size_t i = 0; i < playerPattern.size(); i++) {
        std::vector<std::string> names = {"Blue", "Red", "Green", "Yellow"};
        if (i > 0) seq += " -> ";
        seq += names[playerPattern[i] - 1];
    }
    sequenceText.setString(seq);
    sequenceText.setCharacterSize(18);
    sequenceText.setFillColor(sf::Color::Cyan);
    sequenceText.setPosition({150.0f, 240.0f});
    window.draw(sequenceText);
    
    // Draw switches
    for (auto& sw : switches) {
        window.draw(sw);
    }
    
    // Instructions
    sf::Text controls(font);
    controls.setString("Click buttons in order | Press R to reset | ESC to exit");
    controls.setCharacterSize(16);
    controls.setFillColor(sf::Color(150, 150, 150));
    controls.setPosition({180.0f, 480.0f});
    window.draw(controls);
}

void PatternPuzzle::handleInput(sf::Event& event) {
    if (isSolved) return;
    
    // Handle mouse clicks
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos(mousePressed->position.x, mousePressed->position.y);
            
            // Check which switch was clicked
            for (size_t i = 0; i < switches.size(); i++) {
                if (switches[i].getGlobalBounds().contains(mousePos)) {
                    // Add to player pattern (1-indexed)
                    playerPattern.push_back(i + 1);
                    
                    // Check if pattern is complete
                    if (playerPattern.size() >= correctPattern.size()) {
                        if (checkPattern()) {
                            isSolved = true;
                        } else {
                            // Wrong! Auto-reset after a moment
                            playerPattern.clear();
                        }
                    }
                    break;
                }
            }
        }
    }
    
    // Handle reset key
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::R) {
            resetPattern();
        }
    }
}

void PatternPuzzle::update(float deltaTime) {
    // Could add animations here
}

void PatternPuzzle::setFont(const sf::Font& f) {
    font = f;
    instructionText.setFont(font);
}

bool PatternPuzzle::checkPattern() {
    if (playerPattern.size() != correctPattern.size()) {
        return false;
    }
    
    for (size_t i = 0; i < playerPattern.size(); i++) {
        if (playerPattern[i] != correctPattern[i]) {
            return false;
        }
    }
    justSolved = true;
    return true;
}

void PatternPuzzle::resetPattern() {
    playerPattern.clear();
}


// ============================================================================
// LockPuzzle - Fully Interactive Numeric Keypad
// ============================================================================

LockPuzzle::LockPuzzle(const std::string& code)
    : Puzzle("Enter the code", "Look for clues...", 35, 10),
      correctCode(code),
      codeDisplay(defaultFont),
      instructionText(defaultFont),
      maxDigits(code.length()) {}

bool LockPuzzle::solve(const std::string& answer) {
    if (enteredCode == correctCode) {
        isSolved = true;
        justSolved = true;
        return true;
    }
    return false;
}

void LockPuzzle::display(sf::RenderWindow& window) {
    // Dark overlay
    sf::RectangleShape overlay({800.0f, 600.0f});
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);
    
    // Puzzle box - SMALLER to fit everything
    sf::RectangleShape puzzleBox({550.0f, 550.0f});
    puzzleBox.setPosition({125.0f, 25.0f});
    puzzleBox.setFillColor(sf::Color(40, 40, 60));
    puzzleBox.setOutlineThickness(3.0f);
    puzzleBox.setOutlineColor(sf::Color::White);
    window.draw(puzzleBox);
    
    // Title
    sf::Text title(font);
    title.setString("LOCK PUZZLE");
    title.setCharacterSize(26);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition({310.0f, 40.0f});
    window.draw(title);
    
    // Instructions
    sf::Text instructions(font);
    instructions.setString("Enter the 4-digit code:");
    instructions.setCharacterSize(18);
    instructions.setFillColor(sf::Color::White);
    instructions.setPosition({280.0f, 80.0f});
    window.draw(instructions);
    
    // Code display box
    sf::RectangleShape displayBox({300.0f, 50.0f});
    displayBox.setPosition({250.0f, 115.0f});
    displayBox.setFillColor(sf::Color(20, 20, 30));
    displayBox.setOutlineThickness(3.0f);
    displayBox.setOutlineColor(sf::Color::Cyan);
    window.draw(displayBox);
    
    // Display entered code
    std::string displayCode = "";
    for (size_t i = 0; i < enteredCode.length(); i++) {
        displayCode += enteredCode[i];
        displayCode += " ";
    }
    
    // Add underscores for remaining digits
    for (size_t i = enteredCode.length(); i < (size_t)maxDigits; i++) {
        displayCode += "_ ";
    }
    
    codeDisplay.setFont(font);
    codeDisplay.setString(displayCode);
    codeDisplay.setCharacterSize(32);
    codeDisplay.setFillColor(sf::Color::White);
    codeDisplay.setPosition({290.0f, 125.0f});
    window.draw(codeDisplay);
    
    // Draw numeric keypad - ADJUSTED POSITIONS AND SMALLER BUTTONS
    float keypadStartX = 235.0f;
    float keypadStartY = 200.0f;
    float buttonSize = 65.0f;  // Smaller buttons
    float spacing = 85.0f;     // Tighter spacing
    
    // Buttons 1-9
    for (int i = 1; i <= 9; i++) {
        int row = (i - 1) / 3;
        int col = (i - 1) % 3;
        
        float x = keypadStartX + col * spacing;
        float y = keypadStartY + row * spacing;
        
        // Button background
        sf::RectangleShape button({buttonSize, buttonSize});
        button.setPosition({x, y});
        button.setFillColor(sf::Color(60, 60, 80));
        button.setOutlineThickness(2.0f);
        button.setOutlineColor(sf::Color::White);
        window.draw(button);
        
        // Button number
        sf::Text buttonText(font);
        buttonText.setString(std::to_string(i));
        buttonText.setCharacterSize(28);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition({x + 23.0f, y + 15.0f});
        window.draw(buttonText);
    }
    
    // Bottom row: Clear, 0, Enter
    // Clear button
    sf::RectangleShape clearButton({buttonSize, buttonSize});
    clearButton.setPosition({keypadStartX, keypadStartY + 3 * spacing});
    clearButton.setFillColor(sf::Color(100, 50, 50));
    clearButton.setOutlineThickness(2.0f);
    clearButton.setOutlineColor(sf::Color::White);
    window.draw(clearButton);
    
    sf::Text clearText(font);
    clearText.setString("C");
    clearText.setCharacterSize(26);
    clearText.setFillColor(sf::Color::White);
    clearText.setPosition({keypadStartX + 23.0f, keypadStartY + 3 * spacing + 16.0f});
    window.draw(clearText);
    
    // 0 button
    sf::RectangleShape zeroButton({buttonSize, buttonSize});
    zeroButton.setPosition({keypadStartX + spacing, keypadStartY + 3 * spacing});
    zeroButton.setFillColor(sf::Color(60, 60, 80));
    zeroButton.setOutlineThickness(2.0f);
    zeroButton.setOutlineColor(sf::Color::White);
    window.draw(zeroButton);
    
    sf::Text zeroText(font);
    zeroText.setString("0");
    zeroText.setCharacterSize(28);
    zeroText.setFillColor(sf::Color::White);
    zeroText.setPosition({keypadStartX + spacing + 23.0f, keypadStartY + 3 * spacing + 15.0f});
    window.draw(zeroText);
    
    // OK button
    sf::RectangleShape enterButton({buttonSize, buttonSize});
    enterButton.setPosition({keypadStartX + 2 * spacing, keypadStartY + 3 * spacing});
    enterButton.setFillColor(sf::Color(50, 100, 50));
    enterButton.setOutlineThickness(2.0f);
    enterButton.setOutlineColor(sf::Color::White);
    window.draw(enterButton);
    
    sf::Text enterText(font);
    enterText.setString("OK");
    enterText.setCharacterSize(22);
    enterText.setFillColor(sf::Color::White);
    enterText.setPosition({keypadStartX + 2 * spacing + 15.0f, keypadStartY + 3 * spacing + 18.0f});
    window.draw(enterText);
    
    // Feedback message
    if (isSolved) {
        sf::Text feedback(font);
        feedback.setString("Correct! +" + std::to_string(timeBonus) + " seconds!");
        feedback.setCharacterSize(20);
        feedback.setFillColor(sf::Color::Green);
        feedback.setPosition({235.0f, 510.0f});
        window.draw(feedback);
    }
    
    // Instructions at bottom
    sf::Text controls(font);
    controls.setString("Click keypad or use keyboard | ESC to exit");
    controls.setCharacterSize(15);
    controls.setFillColor(sf::Color(150, 150, 150));
    controls.setPosition({220.0f, 545.0f});
    window.draw(controls);
}
void LockPuzzle::handleInput(sf::Event& event) {
    if (isSolved) return;  // Don't accept input if already solved
    
    // Handle mouse clicks on keypad - UPDATED POSITIONS
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            float mouseX = static_cast<float>(mousePressed->position.x);
            float mouseY = static_cast<float>(mousePressed->position.y);
            
            // UPDATED to match display() positions
            float keypadStartX = 235.0f;
            float keypadStartY = 200.0f;
            float buttonSize = 65.0f;
            float spacing = 85.0f;
            
            // Check buttons 1-9
            for (int i = 1; i <= 9; i++) {
                int row = (i - 1) / 3;
                int col = (i - 1) % 3;
                
                float x = keypadStartX + col * spacing;
                float y = keypadStartY + row * spacing;
                
                if (mouseX >= x && mouseX <= x + buttonSize &&
                    mouseY >= y && mouseY <= y + buttonSize) {
                    addDigit('0' + i);
                    return;
                }
            }
            
            // Check Clear button (bottom left)
            float clearX = keypadStartX;
            float clearY = keypadStartY + 3 * spacing;
            if (mouseX >= clearX && mouseX <= clearX + buttonSize &&
                mouseY >= clearY && mouseY <= clearY + buttonSize) {
                clearCode();
                return;
            }
            
            // Check 0 button (bottom middle)
            float zeroX = keypadStartX + spacing;
            float zeroY = keypadStartY + 3 * spacing;
            if (mouseX >= zeroX && mouseX <= zeroX + buttonSize &&
                mouseY >= zeroY && mouseY <= zeroY + buttonSize) {
                addDigit('0');
                return;
            }
            
            // Check OK button (bottom right)
            float enterX = keypadStartX + 2 * spacing;
            float enterY = keypadStartY + 3 * spacing;
            if (mouseX >= enterX && mouseX <= enterX + buttonSize &&
                mouseY >= enterY && mouseY <= enterY + buttonSize) {
                solve(enteredCode);
                return;
            }
        }
    }
    
    // Handle keyboard input
    if (const auto* textEntered = event.getIf<sf::Event::TextEntered>()) {
        char entered = static_cast<char>(textEntered->unicode);
        
        // Handle backspace
        if (entered == 8) {  // Backspace
            removeDigit();
        }
        // Handle digits 0-9
        else if (entered >= '0' && entered <= '9') {
            addDigit(entered);
        }
    }
    
    // Handle Enter key
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Enter && enteredCode.length() == (size_t)maxDigits) {
            solve(enteredCode);
        }
    }
}

void LockPuzzle::update(float deltaTime) {
    // Animation or timer logic could go here
}

void LockPuzzle::setFont(const sf::Font& f) {
    font = f;
    codeDisplay.setFont(font);
    instructionText.setFont(font);
}

void LockPuzzle::addDigit(char digit) {
    if (enteredCode.length() < (size_t)maxDigits && digit >= '0' && digit <= '9') {
        enteredCode += digit;
    }
}

void LockPuzzle::removeDigit() {
    if (!enteredCode.empty()) {
        enteredCode.pop_back();
    }
}

void LockPuzzle::clearCode() {
    enteredCode.clear();
}
/*
 * Museum Escape - Puzzle Class Implementation (PHASE 2)
 * CS/CE 224/272 - Fall 2025
 * 
 * Phase 2 Additions:
 * - MathPuzzle class
 * - WirePuzzle class
 */

// NOTE: This file should be APPENDED to your existing Puzzle.cpp
// Copy everything from "// PHASE 2 NEW PUZZLES" onwards to the end of your Puzzle.cpp

// ============================================================================
// PHASE 2: NEW PUZZLE IMPLEMENTATIONS
// ============================================================================

// ============================================================================
// Math Puzzle Implementation
// ============================================================================

MathPuzzle::MathPuzzle(const std::string& eq, const std::string& answer)
    : Puzzle("Solve the equation", "Basic math...", 30, 10),
      equation(eq),
      correctAnswer(answer),
      equationText(defaultFont),
      answerDisplay(defaultFont),
      maxDigits(3) {
    
    equationText.setString(equation);
    equationText.setCharacterSize(32);
    equationText.setFillColor(sf::Color::White);
    
    answerDisplay.setString("");
    answerDisplay.setCharacterSize(28);
    answerDisplay.setFillColor(sf::Color::Cyan);
}

bool MathPuzzle::solve(const std::string& answer) {
    std::cout << "\n>>> MathPuzzle::solve() called <<<" << std::endl;
    std::cout << "Player answer: '" << playerAnswer << "'" << std::endl;
    std::cout << "Correct answer: '" << correctAnswer << "'" << std::endl;    
    if (playerAnswer == correctAnswer) {
        isSolved = true;
        justSolved = true; // <--- ADD THIS
        std::cout << "Setting isSolved = true AND justSolved = true" << std::endl;
        return true;
    }
    return false;
}

void MathPuzzle::display(sf::RenderWindow& window) {
    // Dark overlay
    sf::RectangleShape overlay({800.0f, 600.0f});
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);
    
    // Background - SMALLER
    sf::RectangleShape background({500.0f, 480.0f});
    background.setPosition({150.0f, 60.0f});
    background.setFillColor(sf::Color(30, 30, 40, 240));
    background.setOutlineThickness(3.0f);
    background.setOutlineColor(sf::Color::Cyan);
    window.draw(background);
    
    // Title
    sf::Text title(font);
    title.setString("MATH PUZZLE - SAFE LOCK");
    title.setCharacterSize(24);
    title.setFillColor(sf::Color::Cyan);
    title.setPosition({230.0f, 75.0f});
    window.draw(title);
    
    // Instructions
    sf::Text instructions(font);
    instructions.setString("Solve the equation to unlock the safe:");
    instructions.setCharacterSize(16);
    instructions.setFillColor(sf::Color::White);
    instructions.setPosition({200.0f, 110.0f});
    window.draw(instructions);
    
    // Draw equation text - BIGGER
    equationText.setFont(font);
    equationText.setCharacterSize(36);
    equationText.setPosition({310.0f, 145.0f});
    window.draw(equationText);
    
    // Answer display box
    sf::RectangleShape answerBox({180.0f, 45.0f});
    answerBox.setPosition({310.0f, 200.0f});
    answerBox.setFillColor(sf::Color::Transparent);
    answerBox.setOutlineThickness(2.0f);
    answerBox.setOutlineColor(sf::Color::Cyan);
    window.draw(answerBox);
    
    // Draw player's answer
    answerDisplay.setFont(font);
    answerDisplay.setString(playerAnswer.empty() ? "_ _ _" : playerAnswer);
    answerDisplay.setCharacterSize(26);
    answerDisplay.setPosition({345.0f, 208.0f});
    window.draw(answerDisplay);
    
    // Create number pad buttons - SMALLER & MORE COMPACT
    float buttonSize = 50.0f;
    float spacing = 8.0f;
    float startX = 310.0f;
    float startY = 260.0f;
    
    // Buttons 1-9 in 3x3 grid
    for (int i = 0; i < 9; i++) {
        int row = i / 3;
        int col = i % 3;
        
        sf::RectangleShape button({buttonSize, buttonSize});
        button.setPosition({
            startX + col * (buttonSize + spacing),
            startY + row * (buttonSize + spacing)
        });
        button.setFillColor(sf::Color(60, 60, 70));
        button.setOutlineThickness(2.0f);
        button.setOutlineColor(sf::Color::White);
        window.draw(button);
        
        sf::Text num(font);
        num.setString(std::to_string(i + 1));
        num.setCharacterSize(22);
        num.setFillColor(sf::Color::White);
        
        // Center text in button
        sf::FloatRect textBounds = num.getLocalBounds();
        num.setPosition({
            startX + col * (buttonSize + spacing) + (buttonSize - textBounds.size.x) / 2.0f - textBounds.position.x,
            startY + row * (buttonSize + spacing) + (buttonSize - textBounds.size.y) / 2.0f - textBounds.position.y - 5.0f
        });
        window.draw(num);
    }
    
    // Button 0 (left bottom)
    sf::RectangleShape zeroButton({buttonSize, buttonSize});
    zeroButton.setPosition({
        startX,
        startY + 3 * (buttonSize + spacing)
    });
    zeroButton.setFillColor(sf::Color(60, 60, 70));
    zeroButton.setOutlineThickness(2.0f);
    zeroButton.setOutlineColor(sf::Color::White);
    window.draw(zeroButton);
    
    sf::Text zero(font);
    zero.setString("0");
    zero.setCharacterSize(22);
    zero.setFillColor(sf::Color::White);
    sf::FloatRect zeroBounds = zero.getLocalBounds();
    zero.setPosition({
        startX + (buttonSize - zeroBounds.size.x) / 2.0f - zeroBounds.position.x,
        startY + 3 * (buttonSize + spacing) + (buttonSize - zeroBounds.size.y) / 2.0f - zeroBounds.position.y - 5.0f
    });
    window.draw(zero);
    
    // Clear button (middle bottom)
    sf::RectangleShape clearButton({buttonSize, buttonSize});
    clearButton.setPosition({
        startX + 1 * (buttonSize + spacing),
        startY + 3 * (buttonSize + spacing)
    });
    clearButton.setFillColor(sf::Color(80, 40, 40));
    clearButton.setOutlineThickness(2.0f);
    clearButton.setOutlineColor(sf::Color::Red);
    window.draw(clearButton);
    
    sf::Text clearText(font);
    clearText.setString("CLR");
    clearText.setCharacterSize(16);
    clearText.setFillColor(sf::Color::White);
    sf::FloatRect clearBounds = clearText.getLocalBounds();
    clearText.setPosition({
        startX + 1 * (buttonSize + spacing) + (buttonSize - clearBounds.size.x) / 2.0f,
        startY + 3 * (buttonSize + spacing) + 15.0f
    });
    window.draw(clearText);
    
    // Submit button (right bottom)
    sf::RectangleShape submitButton({buttonSize, buttonSize});
    submitButton.setPosition({
        startX + 2 * (buttonSize + spacing),
        startY + 3 * (buttonSize + spacing)
    });
    submitButton.setFillColor(sf::Color(40, 80, 40));
    submitButton.setOutlineThickness(2.0f);
    submitButton.setOutlineColor(sf::Color::Green);
    window.draw(submitButton);
    
    sf::Text submitText(font);
    submitText.setString("OK");
    submitText.setCharacterSize(18);
    submitText.setFillColor(sf::Color::White);
    sf::FloatRect submitBounds = submitText.getLocalBounds();
    submitText.setPosition({
        startX + 2 * (buttonSize + spacing) + (buttonSize - submitBounds.size.x) / 2.0f,
        startY + 3 * (buttonSize + spacing) + 13.0f
    });
    window.draw(submitText);
    
    // Feedback message
    if (isSolved) {
        sf::Text feedback(font);
        feedback.setString("Correct! +" + std::to_string(timeBonus) + "s");
        feedback.setCharacterSize(16);
        feedback.setFillColor(sf::Color::Green);
        feedback.setPosition({310.0f, 495.0f});
        window.draw(feedback);
    } else if (playerAnswer.length() == maxDigits && !solve("") && !isSolved) {
        sf::Text feedback(font);
        feedback.setString("Wrong! -" + std::to_string(timePenalty) + "s");
        feedback.setCharacterSize(16);
        feedback.setFillColor(sf::Color::Red);
        feedback.setPosition({320.0f, 495.0f});
        window.draw(feedback);
    }
    
    // Controls
    sf::Text controls(font);
    controls.setString("ESC to exit");
    controls.setCharacterSize(12);
    controls.setFillColor(sf::Color(150, 150, 150));
    controls.setPosition({350.0f, 520.0f});
    window.draw(controls);
}

void MathPuzzle::handleInput(sf::Event& event) {
    if (isSolved) return;
    
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            float mouseX = static_cast<float>(mousePressed->position.x);
            float mouseY = static_cast<float>(mousePressed->position.y);
            
            float buttonSize = 50.0f;
            float spacing = 8.0f;
            float startX = 310.0f;
            float startY = 260.0f;
            
            // Check number buttons 1-9
            for (int i = 0; i < 9; i++) {
                int row = i / 3;
                int col = i % 3;
                
                float btnX = startX + col * (buttonSize + spacing);
                float btnY = startY + row * (buttonSize + spacing);
                
                if (mouseX >= btnX && mouseX <= btnX + buttonSize &&
                    mouseY >= btnY && mouseY <= btnY + buttonSize) {
                    addDigit('1' + i);
                    return;
                }
            }
            
            // Check 0 button
            float zero_x = startX;
            float zero_y = startY + 3 * (buttonSize + spacing);
            if (mouseX >= zero_x && mouseX <= zero_x + buttonSize &&
                mouseY >= zero_y && mouseY <= zero_y + buttonSize) {
                addDigit('0');
                return;
            }
            
            // Check CLEAR button
            float clear_x = startX + 1 * (buttonSize + spacing);
            float clear_y = startY + 3 * (buttonSize + spacing);
            if (mouseX >= clear_x && mouseX <= clear_x + buttonSize &&
                mouseY >= clear_y && mouseY <= clear_y + buttonSize) {
                clearAnswer();
                return;
            }
            
            // Check SUBMIT button
            float submit_x = startX + 2 * (buttonSize + spacing);
            float submit_y = startY + 3 * (buttonSize + spacing);
            if (mouseX >= submit_x && mouseX <= submit_x + buttonSize &&
                mouseY >= submit_y && mouseY <= submit_y + buttonSize) {
                if (playerAnswer.length() == maxDigits) {
                    solve(playerAnswer);
                }
                return;
            }
        }
    }
    
    // Keyboard input (same as before)
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code >= sf::Keyboard::Key::Num0 && keyPressed->code <= sf::Keyboard::Key::Num9) {
            int keyValue = static_cast<int>(keyPressed->code) - static_cast<int>(sf::Keyboard::Key::Num0);
            addDigit('0' + keyValue);
        } 
        else if (keyPressed->code >= sf::Keyboard::Key::Numpad0 && keyPressed->code <= sf::Keyboard::Key::Numpad9) {
            int keyValue = static_cast<int>(keyPressed->code) - static_cast<int>(sf::Keyboard::Key::Numpad0);
            addDigit('0' + keyValue);
        } 
        else if (keyPressed->code == sf::Keyboard::Key::Backspace) {
            removeDigit();
        } 
        else if (keyPressed->code == sf::Keyboard::Key::Enter && playerAnswer.length() == maxDigits) {
            solve(playerAnswer);
        }
    }
}

void MathPuzzle::update(float deltaTime) {
    // No animation needed
}

void MathPuzzle::setFont(const sf::Font& f) {
    font = f;
    equationText.setFont(font);
    answerDisplay.setFont(font);
}

void MathPuzzle::addDigit(char digit) {
    if (playerAnswer.length() < maxDigits && digit >= '0' && digit <= '9') {
        playerAnswer += digit;
    }
}

void MathPuzzle::removeDigit() {
    if (!playerAnswer.empty()) {
        playerAnswer.pop_back();
    }
}

void MathPuzzle::clearAnswer() {
    playerAnswer.clear();
}

// Continue to Part 2 for WirePuzzle...
// ============================================================================
// WirePuzzle - Cut wires in correct order (COMPLETE IMPLEMENTATION)
// ============================================================================

WirePuzzle::WirePuzzle(const std::vector<std::string>& sequence)
    : Puzzle("Cut wires in correct order", "Think about wire hierarchy...", 50, 20),
      correctSequence(sequence),
      instructionText(defaultFont),
      hasBoltCutters(false),
      showWrongFeedback(false) {
    
    // Initialize wire colors
    wireColors = {"Red", "Yellow", "Blue", "Green", "Purple"};
    
    // Initialize all wires as uncut
    wireCut = std::vector<bool>(wireColors.size(), false);
    
    // Create wire shapes (visual representation)
    float wireStartY = 180.0f;
    float wireSpacing = 60.0f;
    
    for (size_t i = 0; i < wireColors.size(); i++) {
        sf::RectangleShape wire({400.0f, 25.0f});
        wire.setPosition({200.0f, wireStartY + i * wireSpacing});
        wire.setFillColor(getWireColor(wireColors[i]));
        wires.push_back(wire);
    }
}

bool WirePuzzle::solve(const std::string& answer) {
    // Check if we've cut all required wires
    if (cutSequence.size() == correctSequence.size()) {
        if (cutSequence == correctSequence) {
            isSolved = true;
            justSolved = true;
            showWrongFeedback = false;
            return true;
        } else {
            // Wrong sequence - show feedback but allow retry
            showWrongFeedback = true;
            return false;
        }
    }
    return false;
}

void WirePuzzle::display(sf::RenderWindow& window) {
    // Dark overlay
    sf::RectangleShape overlay({800.0f, 600.0f});
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);
    
    // Puzzle box
    sf::RectangleShape puzzleBox({650.0f, 550.0f});
    puzzleBox.setPosition({75.0f, 25.0f});
    puzzleBox.setFillColor(sf::Color(40, 40, 60));
    puzzleBox.setOutlineThickness(3.0f);
    puzzleBox.setOutlineColor(sf::Color(255, 100, 100)); // Red outline for danger
    window.draw(puzzleBox);
    
    // Title
    sf::Text title(font);
    title.setString("WIRE CUTTING PUZZLE");
    title.setCharacterSize(28);
    title.setFillColor(sf::Color(255, 100, 100));
    title.setPosition({250.0f, 45.0f});
    window.draw(title);
    
    // Instructions
    sf::Text instructions(font);
    if (!hasBoltCutters) {
        instructions.setString("ERROR: Bolt Cutters required!");
        instructions.setFillColor(sf::Color::Red);
    } else {
        instructions.setString("Cut wires in correct order to disable alarm");
        instructions.setFillColor(sf::Color::White);
    }
    instructions.setCharacterSize(18);
    instructions.setPosition({180.0f, 90.0f});
    window.draw(instructions);
    
    // Hint
    sf::Text hintText(font);
    hintText.setString("Hint: Primary colors first, then secondary");
    hintText.setCharacterSize(16);
    hintText.setFillColor(sf::Color(200, 200, 100));
    hintText.setPosition({200.0f, 120.0f});
    window.draw(hintText);
    
    // Draw wires
    float wireStartY = 180.0f;
    float wireSpacing = 60.0f;
    
    for (size_t i = 0; i < wireColors.size(); i++) {
        float y = wireStartY + i * wireSpacing;
        
        // Wire background (terminal to terminal)
        sf::RectangleShape wireBg({400.0f, 25.0f});
        wireBg.setPosition({200.0f, y});
        wireBg.setFillColor(getWireColor(wireColors[i]));
        
        // If wire is cut, show it as disconnected
        if (wireCut[i]) {
            // Left half
            sf::RectangleShape wireLeft({180.0f, 25.0f});
            wireLeft.setPosition({200.0f, y});
            wireLeft.setFillColor(sf::Color(80, 80, 80)); // Gray when cut
            window.draw(wireLeft);
            
            // Right half
            sf::RectangleShape wireRight({180.0f, 25.0f});
            wireRight.setPosition({420.0f, y});
            wireRight.setFillColor(sf::Color(80, 80, 80));
            window.draw(wireRight);
            
            // Cut mark (scissors symbol)
            sf::Text cutMark(font);
            cutMark.setString("✂");
            cutMark.setCharacterSize(30);
            cutMark.setFillColor(sf::Color::White);
            cutMark.setPosition({385.0f, y - 8.0f});
            window.draw(cutMark);
        } else {
            // Wire is intact
            window.draw(wireBg);
        }
        
        // Wire label
        sf::Text label(font);
        label.setString(wireColors[i]);
        label.setCharacterSize(18);
        label.setFillColor(sf::Color::White);
        label.setPosition({100.0f, y + 2.0f});
        window.draw(label);
        
        // Click button (if not cut and have bolt cutters and not solved)
        if (!wireCut[i] && hasBoltCutters && !isSolved) {
            sf::RectangleShape cutButton({60.0f, 35.0f});
            cutButton.setPosition({620.0f, y - 5.0f});
            cutButton.setFillColor(sf::Color(100, 50, 50));
            cutButton.setOutlineThickness(2.0f);
            cutButton.setOutlineColor(sf::Color::White);
            window.draw(cutButton);
            
            sf::Text cutText(font);
            cutText.setString("CUT");
            cutText.setCharacterSize(14);
            cutText.setFillColor(sf::Color::White);
            cutText.setPosition({630.0f, y + 5.0f});
            window.draw(cutText);
        }
    }
    
    // Cut sequence display
    sf::Text sequenceLabel(font);
    sequenceLabel.setString("Cut Sequence:");
    sequenceLabel.setCharacterSize(18);
    sequenceLabel.setFillColor(sf::Color::White);
    sequenceLabel.setPosition({100.0f, 480.0f});
    window.draw(sequenceLabel);
    
    std::string sequenceStr = "";
    for (size_t i = 0; i < cutSequence.size(); i++) {
        sequenceStr += cutSequence[i];
        if (i < cutSequence.size() - 1) sequenceStr += " → ";
    }
    
    sf::Text sequenceDisplay(font);
    sequenceDisplay.setString(sequenceStr.empty() ? "None" : sequenceStr);
    sequenceDisplay.setCharacterSize(16);
    sequenceDisplay.setFillColor(sf::Color::Cyan);
    sequenceDisplay.setPosition({100.0f, 510.0f});
    window.draw(sequenceDisplay);
    
    // RESET BUTTON - Always visible when not solved
    if (!isSolved && hasBoltCutters) {
        sf::RectangleShape resetButton({100.0f, 40.0f});
        resetButton.setPosition({450.0f, 475.0f});
        resetButton.setFillColor(sf::Color(80, 60, 40));
        resetButton.setOutlineThickness(2.0f);
        resetButton.setOutlineColor(sf::Color::Yellow);
        window.draw(resetButton);
        
        sf::Text resetText(font);
        resetText.setString("RESET");
        resetText.setCharacterSize(18);
        resetText.setFillColor(sf::Color::White);
        sf::FloatRect resetBounds = resetText.getLocalBounds();
        resetText.setPosition({
            475.0f - resetBounds.position.x,
            485.0f - resetBounds.position.y
        });
        window.draw(resetText);
    }
    
    // Feedback messages
    if (isSolved) {
        sf::Text feedback(font);
        feedback.setString("Success! Alarm disabled! +" + std::to_string(timeBonus) + " seconds!");
        feedback.setCharacterSize(18);
        feedback.setFillColor(sf::Color::Green);
        feedback.setPosition({180.0f, 540.0f});
        window.draw(feedback);
    } else if (showWrongFeedback) {
        // Wrong sequence - allow retry
        sf::Text feedback(font);
        feedback.setString("WRONG SEQUENCE! -" + std::to_string(timePenalty) + "s. Press RESET (R) to try again.");
        feedback.setCharacterSize(16);
        feedback.setFillColor(sf::Color::Red);
        feedback.setPosition({130.0f, 540.0f});
        window.draw(feedback);
    }
    
    // Controls
    sf::Text controls(font);
    controls.setString("Click CUT buttons | R to RESET | ESC to exit");
    controls.setCharacterSize(14);
    controls.setFillColor(sf::Color(150, 150, 150));
    controls.setPosition({220.0f, 555.0f});
    window.draw(controls);
}

void WirePuzzle::handleInput(sf::Event& event) {
    if (isSolved || !hasBoltCutters) return;
    
    // Handle keyboard R key for reset
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::R) {
            resetPuzzle();
            return;
        }
    }
    
    // Handle mouse clicks
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            float mouseX = static_cast<float>(mousePressed->position.x);
            float mouseY = static_cast<float>(mousePressed->position.y);
            
            // Check RESET button click
            if (mouseX >= 450.0f && mouseX <= 550.0f &&
                mouseY >= 475.0f && mouseY <= 515.0f) {
                resetPuzzle();
                return;
            }
            
            float wireStartY = 180.0f;
            float wireSpacing = 60.0f;
            
            // Check which wire's CUT button was clicked
            for (size_t i = 0; i < wireColors.size(); i++) {
                if (wireCut[i]) continue; // Skip already cut wires
                
                float y = wireStartY + i * wireSpacing;
                float btnX = 620.0f;
                float btnY = y - 5.0f;
                
                if (mouseX >= btnX && mouseX <= btnX + 60.0f &&
                    mouseY >= btnY && mouseY <= btnY + 35.0f) {
                    cutWire(i);
                    return;
                }
            }
        }
    }
}

void WirePuzzle::update(float deltaTime) {
    // No animation needed for this puzzle
}

void WirePuzzle::setFont(const sf::Font& f) {
    font = f;
}

void WirePuzzle::setBoltCutters(bool has) {
    hasBoltCutters = has;
}

void WirePuzzle::resetPuzzle() {
    // Reset all wires to uncut state
    for (size_t i = 0; i < wireCut.size(); i++) {
        wireCut[i] = false;
    }
    
    // Clear the cut sequence
    cutSequence.clear();
    
    // Hide wrong feedback
    showWrongFeedback = false;
    
    // Don't reset isSolved - only successful completion sets it to true
}

void WirePuzzle::cutWire(int wireIndex) {
    if (wireIndex >= 0 && wireIndex < (int)wireColors.size() && !wireCut[wireIndex]) {
        wireCut[wireIndex] = true;
        cutSequence.push_back(wireColors[wireIndex]);
        
        // Check if puzzle is solved after cutting
        solve("");
    }
}

sf::Color WirePuzzle::getWireColor(const std::string& colorName) {
    if (colorName == "Red") return sf::Color(255, 50, 50);
    if (colorName == "Yellow") return sf::Color(255, 255, 50);
    if (colorName == "Blue") return sf::Color(50, 50, 255);
    if (colorName == "Green") return sf::Color(50, 255, 50);
    if (colorName == "Purple") return sf::Color(200, 50, 200);
    return sf::Color::White;
}