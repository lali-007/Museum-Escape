/*
 * Museum Escape - Item Class STUB Implementation
 * CS/CE 224/272 - Fall 2025
 * TEMPORARY - For testing only
 */

#include "Item.h"

// Item Constructor
Item::Item(const std::string& itemName, const std::string& desc, float x, float y)
    : name(itemName), description(desc), position(x, y), isCollected(false) {
    sprite.setSize({20.0f, 20.0f});
    sprite.setPosition(position);
    sprite.setFillColor(sf::Color::Yellow);
}

std::string Item::getName() const { return name; }
std::string Item::getDescription() const { return description; }
sf::Vector2f Item::getPosition() const { return position; }
bool Item::isItemCollected() const { return isCollected; }
sf::FloatRect Item::getBounds() const { return sprite.getGlobalBounds(); }

void Item::collect() { isCollected = true; }
void Item::draw(sf::RenderWindow& window) { window.draw(sprite); }
bool Item::checkCollision(const sf::FloatRect& bounds) {
    return sprite.getGlobalBounds().findIntersection(bounds).has_value();
}

// Key Constructor
Key::Key(const std::string& keyName, const std::string& doorIdentifier, float x, float y)
    : Item(keyName, "A key", x, y), doorID(doorIdentifier) {}

void Key::use() {}
std::string Key::getDoorID() const { return doorID; }

// Passcode Constructor
Passcode::Passcode(const std::string& passcodeName, const std::string& codeValue, float x, float y)
    : Item(passcodeName, "A passcode", x, y), code(codeValue) {}

void Passcode::use() {}
std::string Passcode::getCode() const { return code; }

// Inventory Constructor
Inventory::Inventory(int capacity)
    : maxCapacity(capacity), isVisible(false), background({300.0f, 400.0f}) {
    background.setFillColor(sf::Color(0, 0, 0, 200));
    background.setPosition({250.0f, 100.0f});
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

int Inventory::getItemCount() const { return items.size(); }
int Inventory::getMaxCapacity() const { return maxCapacity; }
bool Inventory::isFull() const { return items.size() >= static_cast<size_t>(maxCapacity); }
std::vector<std::shared_ptr<Item>>& Inventory::getItems() { return items; }

void Inventory::toggleVisibility() { isVisible = !isVisible; }
void Inventory::setVisible(bool visible) { isVisible = visible; }
bool Inventory::getVisible() const { return isVisible; }
void Inventory::setFont(const sf::Font& f) { font = f; }

void Inventory::draw(sf::RenderWindow& window) {
    if (isVisible) {
        window.draw(background);
    }
}

void Inventory::clear() { items.clear(); }