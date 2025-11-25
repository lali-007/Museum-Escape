
/*
 * Museum Escape - Item Class Implementation
 * CS/CE 224/272 - Fall 2025
 * SFML 3.0 COMPATIBLE
 */

#include "Item.h"

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
void Item::draw(sf::RenderWindow& window) { if (!isCollected) window.draw(sprite); }
bool Item::checkCollision(const sf::FloatRect& bounds) {
    return sprite.getGlobalBounds().findIntersection(bounds).has_value();
}

Key::Key(const std::string& keyName, const std::string& doorIdentifier, float x, float y)
    : Item(keyName, "A key to unlock doors", x, y), doorID(doorIdentifier) {
    sprite.setFillColor(sf::Color::Cyan);
}
void Key::use() {}
std::string Key::getDoorID() const { return doorID; }

Passcode::Passcode(const std::string& passcodeName, const std::string& codeValue, float x, float y)
    : Item(passcodeName, "A numeric passcode", x, y), code(codeValue) {
    sprite.setFillColor(sf::Color::Magenta);
}
void Passcode::use() {}
std::string Passcode::getCode() const { return code; }

BasicItem::BasicItem(const std::string& itemName, const std::string& desc, float x, float y)
    : Item(itemName, desc, x, y) {
    sprite.setFillColor(sf::Color::White);
}
void BasicItem::use() {}

Inventory::Inventory(int capacity)
    : maxCapacity(capacity), isVisible(false), background({400.0f, 500.0f}) {
    background.setFillColor(sf::Color(0, 0, 0, 200));
    background.setOutlineThickness(3.0f);
    background.setOutlineColor(sf::Color::White);
    background.setPosition({200.0f, 50.0f}); // Fixed
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
    for (const auto& item : items) if (item->getName() == itemName) return true;
    return false;
}

std::shared_ptr<Item> Inventory::getItem(const std::string& itemName) {
    for (auto& item : items) if (item->getName() == itemName) return item;
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
    if (!isVisible) return;
    
    sf::RectangleShape dimmer({800.0f, 600.0f});
    dimmer.setFillColor(sf::Color(0, 0, 0, 100));
    window.draw(dimmer);

    sf::RectangleShape card({400.0f, 500.0f});
    card.setPosition({200.0f, 50.0f}); // FIXED
    card.setFillColor(sf::Color(40, 44, 52));
    card.setOutlineThickness(2.0f);
    card.setOutlineColor(sf::Color::Cyan);
    window.draw(card);

    sf::RectangleShape header({400.0f, 60.0f});
    header.setPosition({200.0f, 50.0f}); // FIXED
    header.setFillColor(sf::Color(25, 28, 33));
    window.draw(header);

    sf::Text title(font);
    title.setString("BACKPACK");
    title.setCharacterSize(28);
    title.setStyle(sf::Text::Bold);
    title.setFillColor(sf::Color::White);
    title.setPosition({220.0f, 62.0f}); // FIXED
    window.draw(title);
    
    sf::Text capacity(font);
    capacity.setString(std::to_string(items.size()) + "/" + std::to_string(maxCapacity));
    capacity.setCharacterSize(20);
    capacity.setFillColor(sf::Color::Cyan);
    capacity.setPosition({540.0f, 68.0f}); // FIXED
    window.draw(capacity);

    float yOffset = 130.0f;
    if (items.empty()) {
        sf::Text emptyText(font);
        emptyText.setString("Your backpack is empty.");
        emptyText.setCharacterSize(18);
        emptyText.setFillColor(sf::Color(150, 150, 150));
        sf::FloatRect bounds = emptyText.getLocalBounds();
        emptyText.setPosition({400.0f - bounds.size.y/2.0f, 250.0f}); // FIXED
        window.draw(emptyText);
    } else {
        for (size_t i = 0; i < items.size(); ++i) {
            if (i % 2 == 0) {
                sf::RectangleShape strip({380.0f, 50.0f});
                strip.setPosition({210.0f, yOffset - 10.0f}); // FIXED
                strip.setFillColor(sf::Color(255, 255, 255, 10));
                window.draw(strip);
            }
            sf::Text itemText(font);
            itemText.setString(items[i]->getName());
            itemText.setCharacterSize(22);
            itemText.setFillColor(sf::Color::Yellow);
            itemText.setPosition({230.0f, yOffset}); // FIXED
            window.draw(itemText);
            
            sf::Text descText(font);
            descText.setString(items[i]->getDescription());
            descText.setCharacterSize(14);
            descText.setStyle(sf::Text::Italic);
            descText.setFillColor(sf::Color(200, 200, 200));
            descText.setPosition({230.0f, yOffset + 26.0f}); // FIXED
            window.draw(descText);
            
            yOffset += 60.0f;
        }
    }
}

void Inventory::clear() { items.clear(); }
