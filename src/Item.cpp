/*
 * Museum Escape - Item Class Implementation (ENHANCED)
 * CS/CE 224/272 - Fall 2025
 */

#include "Item.h"
#include <algorithm> // For std::max

// Item Constructor
Item::Item(const std::string& itemName, const std::string& desc, float x, float y, const sf::Texture& texture)
    : name(itemName), 
      description(desc), 
      position(x, y), 
      sprite(texture), // <--- FIXED: Initialize sprite here!
      isCollected(false) 
{
    // sprite.setTexture(texture); // Removed, handled in initializer list
    sprite.setPosition(position);
    
    // Scale sprite to be approx 30x30 pixels
    sf::Vector2u size = texture.getSize();
    float targetSize = 80.0f;
    if (size.x > 0 && size.y > 0) {
        float scale = targetSize / std::max(size.x, size.y);
        sprite.setScale({scale, scale});
    }
}

std::string Item::getName() const { return name; }
std::string Item::getDescription() const { return description; }
sf::Vector2f Item::getPosition() const { return position; }
bool Item::isItemCollected() const { return isCollected; }
sf::FloatRect Item::getBounds() const { return sprite.getGlobalBounds(); }

void Item::collect() { isCollected = true; }
void Item::draw(sf::RenderWindow& window) { 
    if (!isCollected) {
        window.draw(sprite); 
    }
}
bool Item::checkCollision(const sf::FloatRect& bounds) {
    return sprite.getGlobalBounds().findIntersection(bounds).has_value();
}

// Key Constructor - Auto-tints based on name!
Key::Key(const std::string& keyName, const std::string& doorIdentifier, float x, float y, const sf::Texture& texture)
    : Item(keyName, "A key to unlock doors", x, y, texture), doorID(doorIdentifier) {
    
    // Auto-tint keys so you can use one white key texture for all of them
    if (keyName.find("Red") != std::string::npos) sprite.setColor(sf::Color::Red);
    else if (keyName.find("Blue") != std::string::npos) sprite.setColor(sf::Color::Cyan);
    else if (keyName.find("Yellow") != std::string::npos) sprite.setColor(sf::Color::Yellow);
    else if (keyName.find("Green") != std::string::npos) sprite.setColor(sf::Color::Green);
    else if (keyName.find("Master") != std::string::npos) sprite.setColor(sf::Color(255, 215, 0)); // Gold
}

void Key::use() {}
std::string Key::getDoorID() const { return doorID; }

// Passcode Constructor
Passcode::Passcode(const std::string& passcodeName, const std::string& codeValue, float x, float y, const sf::Texture& texture)
    : Item(passcodeName, "A numeric passcode", x, y, texture), code(codeValue) {
    // Passcodes use the paper texture
}

void Passcode::use() {}
std::string Passcode::getCode() const { return code; }

// BasicItem Constructor
BasicItem::BasicItem(const std::string& itemName, const std::string& desc, float x, float y, const sf::Texture& texture)
    : Item(itemName, desc, x, y, texture) {
    // Basic items like Map or Red Keycard (if used as BasicItem)
    if (itemName.find("Red Keycard") != std::string::npos) sprite.setColor(sf::Color::Red);
}

void BasicItem::use() {}

// Tool Constructor
Tool::Tool(const std::string& toolName, const std::string& type, const std::string& desc, float x, float y, const sf::Texture& texture)
    : Item(toolName, desc, x, y, texture), toolType(type), isActive(false) {
    // No special tinting needed for tools usually
}

void Tool::use() { isActive = !isActive; }
std::string Tool::getToolType() const { return toolType; }
void Tool::activate() { isActive = true; }
void Tool::deactivate() { isActive = false; }
bool Tool::isToolActive() const { return isActive; }

// Inventory Implementation (Unchanged)
Inventory::Inventory(int capacity)
    : maxCapacity(capacity), isVisible(false), background({400.0f, 500.0f}) {
    background.setFillColor(sf::Color(0, 0, 0, 200));
    background.setOutlineThickness(3.0f);
    background.setOutlineColor(sf::Color::White);
    background.setPosition({200.0f, 50.0f});
}

bool Inventory::addItem(std::shared_ptr<Item> item) {
    if (items.size() < static_cast<size_t>(maxCapacity)) {
        items.push_back(item);
        return true;
    }
    return false;
}

bool Inventory::removeItem(const std::string& itemName) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if ((*it)->getName() == itemName) {
            items.erase(it);
            return true;
        }
    }
    return false;
}

bool Inventory::hasItem(const std::string& itemName) const {
    for (const auto& item : items) {
        if (item->getName() == itemName) return true;
    }
    return false;
}

std::shared_ptr<Item> Inventory::getItem(const std::string& itemName) {
    for (auto& item : items) {
        if (item->getName() == itemName) return item;
    }
    return nullptr;
}

bool Inventory::hasTool(const std::string& toolType) const {
    for (const auto& item : items) {
        Tool* tool = dynamic_cast<Tool*>(item.get());
        if (tool && tool->getToolType() == toolType) {
            return true;
        }
    }
    return false;
}

int Inventory::getItemCount() const { return static_cast<int>(items.size()); }
int Inventory::getMaxCapacity() const { return maxCapacity; }
bool Inventory::isFull() const { return items.size() >= static_cast<size_t>(maxCapacity); }
std::vector<std::shared_ptr<Item>>& Inventory::getItems() { return items; }

void Inventory::toggleVisibility() { isVisible = !isVisible; }
void Inventory::setVisible(bool visible) { isVisible = visible; }
bool Inventory::getVisible() const { return isVisible; }
void Inventory::setFont(const sf::Font& f) { font = f; }

void Inventory::draw(sf::RenderWindow& window) {
    if (!isVisible) return;
    window.draw(background);
    
    sf::Text title(font);
    title.setString("INVENTORY");
    title.setCharacterSize(24);
    title.setFillColor(sf::Color::White);
    title.setPosition({350.0f, 70.0f});
    window.draw(title);
    
    float yPos = 110.0f;
    int index = 1;
    
    for (const auto& item : items) {
        sf::Text itemText(font);
        itemText.setString(std::to_string(index) + ". " + item->getName());
        itemText.setCharacterSize(18);
        itemText.setFillColor(sf::Color::White);
        itemText.setPosition({220.0f, yPos});
        window.draw(itemText);
        yPos += 30.0f;
        index++;
    }
    
    if (items.empty()) {
        sf::Text emptyText(font);
        emptyText.setString("No items");
        emptyText.setCharacterSize(18);
        emptyText.setFillColor(sf::Color(150, 150, 150));
        emptyText.setPosition({220.0f, 110.0f});
        window.draw(emptyText);
    }
}

void Inventory::clear() { items.clear(); }
