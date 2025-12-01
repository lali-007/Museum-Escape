#ifndef ROOM_H
#define ROOM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>

class Puzzle;
class Item;
class Guard;
class Door;

class Room {
private:
    int roomID;
    std::string roomName;
    sf::Vector2f position;
    sf::Vector2f size;
    
    // === BACKGROUNDS ===
    sf::RectangleShape background;       // Normal (Closed)
    sf::RectangleShape solvedBackground; // Solved (Open)
    
    // Transition Logic
    bool hasSolvedTexture;
    bool isTransitioning;
    float transitionAlpha; // 0.0f (Invisible) to 255.0f (Fully Visible)
    
    std::vector<std::shared_ptr<Puzzle>> puzzles;
    std::vector<std::shared_ptr<Item>> items;
    std::vector<std::shared_ptr<Guard>> guards;
    std::vector<std::shared_ptr<Door>> doors;
    
    bool isExitRoom;
    bool isVisited;
    
public:
    Room(int id, const std::string& name, float x, float y, float width, float height);
    
    void setBackgroundTexture(const sf::Texture& texture);
    
    // === NEW METHODS ===
    void setSolvedBackgroundTexture(const sf::Texture& texture); // Load the open image
    void revealSolvedBackground(); // Start the fade-in effect
    void forceSolvedBackground();  // Show immediately (for when re-entering room)

    void addPuzzle(std::shared_ptr<Puzzle> puzzle);
    std::vector<std::shared_ptr<Puzzle>>& getPuzzles();
    bool allPuzzlesSolved() const;
    
    void addItem(std::shared_ptr<Item> item);
    void removeItem(std::shared_ptr<Item> item);
    std::vector<std::shared_ptr<Item>>& getItems();
    
    void addGuard(std::shared_ptr<Guard> guard);
    std::vector<std::shared_ptr<Guard>>& getGuards();
    
    void addDoor(std::shared_ptr<Door> door);
    std::vector<std::shared_ptr<Door>>& getDoors();
    
    int getRoomID() const;
    std::string getRoomName() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    sf::FloatRect getBounds() const;
    
    void setExitRoom(bool isExit);
    bool isExit() const;
    
    void setVisited(bool visited);
    bool hasBeenVisited() const;
    
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    
    bool containsPoint(const sf::Vector2f& point) const;
     std::shared_ptr<Puzzle> getIntersectingPuzzle(const sf::FloatRect& playerBounds);
};

// Door class remains mostly logic-based now
class Door {
private:
    sf::Vector2f position;
    sf::RectangleShape sprite;
    int targetRoomID;
    bool isLocked;
    std::string requiredKey;
    
public:
    Door(float x, float y, int targetRoom, bool locked = false, const std::string& keyName = "");
    
    void unlock();
    bool canOpen(const std::string& keyName);
    bool checkCollision(const sf::FloatRect& bounds);
    
    int getTargetRoomID() const;
    bool getLockedStatus() const;
    sf::FloatRect getBounds() const;
    
    void draw(sf::RenderWindow& window);
   
};

#endif // ROOM_H