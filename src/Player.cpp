/*
 * Museum Escape - Player Class Implementation
 * CS/CE 224/272 - Fall 2025
 */

#include "Player.h"
#include "Item.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

// Constructor
Player::Player(float x, float y) 
    : position({x, y}),
      // texture is default initialized here
      sprite(texture), // SFML 3.0: Sprite must be constructed with a Texture
      speed(200.0f),
      health(100),
      isWarned(false)
{
    // Load player texture
    if (texture.loadFromFile("assets/player.png")) {
        // SFML 3.0: Texture is already linked via reference, but loading changes size.
        // We might need to reset the texture rect if it doesn't update automatically,
        // but typically it's fine. explicit setTexture ensures it.
        sprite.setTexture(texture, true);
        
        sf::Vector2u textureSize = texture.getSize();
        // SFML 3.0: setScale takes a Vector2f, not two floats
        sprite.setScale({40.0f / textureSize.x, 40.0f / textureSize.y});
    } else {
        std::cerr << "Error: Could not load assets/player.png" << std::endl;
        sprite.setColor(sf::Color::Green);
        // Set a default size for the fallback colored square (requires texture rect trickery or just use scale)
        sprite.setTextureRect(sf::IntRect({0, 0}, {40, 40})); 
    }

    // SFML 3.0: setPosition takes Vector2f
    sprite.setPosition(position);
}

// Move player by delta amounts
void Player::move(float dx, float dy) {
    position.x += dx;
    position.y += dy;
    sprite.setPosition(position);
}

// Handle keyboard input for movement
void Player::handleInput(float deltaTime) {
    float moveX = 0.0f;
    float moveY = 0.0f;
    
    // Check WASD keys
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        moveY -= speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        moveY += speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        moveX -= speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        moveX += speed * deltaTime;
    }
    
    // Apply movement
    if (moveX != 0.0f || moveY != 0.0f) {
        move(moveX, moveY);
    }
}

// Set player position
void Player::setPosition(float x, float y) {
    position = {x, y};
    sprite.setPosition(position);
}

// Get player position
sf::Vector2f Player::getPosition() const {
    return position;
}

// Check collision with bounds
bool Player::checkCollision(const sf::FloatRect& bounds) {
    return sprite.getGlobalBounds().findIntersection(bounds).has_value();
}

// Get player bounding box
sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

// Add item to inventory
void Player::addItem(Item* item) {
    inventory.push_back(item);
}

// Check if player has specific item
bool Player::hasItem(const std::string& itemName) {
    for (const auto& item : inventory) {
        if (item->getName() == itemName) {
            return true;
        }
    }
    return false;
}

// Remove item from inventory
void Player::removeItem(const std::string& itemName) {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if ((*it)->getName() == itemName) {
            inventory.erase(it);
            return;
        }
    }
}

// Get inventory reference
std::vector<Item*>& Player::getInventory() {
    return inventory;
}

// Warn player (caught by guard once)
void Player::warn() {
    isWarned = true;
}

// Check if player is warned
bool Player::isPlayerWarned() const {
    return isWarned;
}

// Reset warning
void Player::resetWarning() {
    isWarned = false;
}

// Draw player
void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

// Update player
void Player::update(float deltaTime) {
    sprite.setPosition(position);
}