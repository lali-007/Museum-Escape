/*
 * Museum Escape - Room Class Implementation
 * CS/CE 224/272 - Fall 2025
 */

#include "Room.h"
#include "Puzzle.h"
#include "Item.h"
#include "Guard.h"
#include <cstdint> // <--- ADDED: Required for std::uint8_t

Room::Room(int id, const std::string& name, float x, float y, float width, float height)
    : roomID(id),
      roomName(name),
      position(x, y),
      size(width, height),
      hasSolvedTexture(false),
      isTransitioning(false),
      transitionAlpha(0.0f),
      isExitRoom(false),
      isVisited(false)
{
    // Normal Background
    background.setSize({width, height});
    background.setPosition(position);
    background.setFillColor(sf::Color(40, 40, 50)); 
    background.setOutlineThickness(2.0f);
    background.setOutlineColor(sf::Color::White);

    // Solved Background - Start INVISIBLE
    solvedBackground.setSize({width, height});
    solvedBackground.setPosition(position);
    solvedBackground.setFillColor(sf::Color(255, 255, 255, 0)); // Alpha = 0
}

void Room::setBackgroundTexture(const sf::Texture& texture) {
    background.setTexture(&texture);
    background.setFillColor(sf::Color::White);
}

// Ensure it starts transparent
void Room::setSolvedBackgroundTexture(const sf::Texture& texture) {
    solvedBackground.setTexture(&texture);
    hasSolvedTexture = true;
    
    // Force it to be invisible initially!
    solvedBackground.setFillColor(sf::Color(255, 255, 255, 0));
    transitionAlpha = 0.0f;
}

// Start smooth fade-in
void Room::revealSolvedBackground() {
    if (hasSolvedTexture && transitionAlpha < 255.0f) {
        isTransitioning = true;
    }
}

// Instant show (for re-entering solved rooms)
void Room::forceSolvedBackground() {
    if (hasSolvedTexture) {
        transitionAlpha = 255.0f;
        solvedBackground.setFillColor(sf::Color(255, 255, 255, 255));
        isTransitioning = false;
    }
}

void Room::update(float deltaTime) {
    // Handle fade-in transition
    if (isTransitioning) {
        transitionAlpha += deltaTime * 200.0f; // Speed of fade
        
        if (transitionAlpha >= 255.0f) {
            transitionAlpha = 255.0f;
            isTransitioning = false;
        }
        
        // Update alpha
        // FIXED: Replaced sf::Uint8 with std::uint8_t
        solvedBackground.setFillColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(transitionAlpha)));
    }

    // Update puzzles
    for (auto& puzzle : puzzles) {
        puzzle->update(deltaTime);
    }
}

void Room::draw(sf::RenderWindow& window) {
    // 1. Always draw normal background at bottom
    window.draw(background);
    
    // 2. Draw solved background on top (only if partially/fully visible)
    if (hasSolvedTexture && transitionAlpha > 0.0f) {
        window.draw(solvedBackground);
    }
    for (auto& puzzle : puzzles) {
        puzzle->drawWorldSprite(window);
    }
    // Draw entities
    for (auto& guard : guards) guard->draw(window, true);
    for (auto& door : doors) door->draw(window);
    for (auto& item : items) {
        if (!item->isItemCollected()) item->draw(window);
    }
}

// ... (Rest of Room methods) ...

void Room::addPuzzle(std::shared_ptr<Puzzle> puzzle) { puzzles.push_back(puzzle); }
std::vector<std::shared_ptr<Puzzle>>& Room::getPuzzles() { return puzzles; }
bool Room::allPuzzlesSolved() const {
    for (const auto& puzzle : puzzles) {
        if (!puzzle->isSolvedStatus()) return false;
    }
    return true;
}
void Room::addItem(std::shared_ptr<Item> item) { items.push_back(item); }
void Room::removeItem(std::shared_ptr<Item> item) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (*it == item) { items.erase(it); return; }
    }
}
std::vector<std::shared_ptr<Item>>& Room::getItems() { return items; }
void Room::addGuard(std::shared_ptr<Guard> guard) { guards.push_back(guard); }
std::vector<std::shared_ptr<Guard>>& Room::getGuards() { return guards; }
void Room::addDoor(std::shared_ptr<Door> door) { doors.push_back(door); }
std::vector<std::shared_ptr<Door>>& Room::getDoors() { return doors; }
int Room::getRoomID() const { return roomID; }
std::string Room::getRoomName() const { return roomName; }
sf::Vector2f Room::getPosition() const { return position; }
sf::Vector2f Room::getSize() const { return size; }
sf::FloatRect Room::getBounds() const { return background.getGlobalBounds(); }
void Room::setExitRoom(bool isExit) { isExitRoom = isExit; }
bool Room::isExit() const { return isExitRoom; }
void Room::setVisited(bool visited) { isVisited = visited; }
bool Room::hasBeenVisited() const { return isVisited; }
bool Room::containsPoint(const sf::Vector2f& point) const { return background.getGlobalBounds().contains(point); }

// === DOOR IMPLEMENTATION ===
Door::Door(float x, float y, int targetRoom, bool locked, const std::string& keyName)
    : position(x, y), targetRoomID(targetRoom), isLocked(locked), requiredKey(keyName) {
    sprite.setSize({50.0f, 100.0f});
    sprite.setPosition(position);
    sprite.setFillColor(sf::Color::Transparent); // Invisible door
}
void Door::unlock() { isLocked = false; }
bool Door::canOpen(const std::string& keyName) {
    if (!isLocked) return true;
    if (keyName == requiredKey || requiredKey.empty()) { unlock(); return true; }
    return false;
}
bool Door::checkCollision(const sf::FloatRect& bounds) { return sprite.getGlobalBounds().findIntersection(bounds).has_value(); }
int Door::getTargetRoomID() const { return targetRoomID; }
bool Door::getLockedStatus() const { return isLocked; }
sf::FloatRect Door::getBounds() const { return sprite.getGlobalBounds(); }
void Door::draw(sf::RenderWindow& window) { window.draw(sprite); }
std::shared_ptr<Puzzle> Room::getIntersectingPuzzle(const sf::FloatRect& playerBounds) {
    for (auto& puzzle : puzzles) {
        if (puzzle->checkCollision(playerBounds)) {
            return puzzle;
        }
    }
    return nullptr;
}