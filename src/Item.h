#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

// Base Item class
class Item {
protected:
    std::string name;
    std::string description;
    sf::Vector2f position;
    sf::Sprite sprite; // <--- CHANGED to Sprite
    bool isCollected;
    
public:
    // Constructor now takes a texture
    Item(const std::string& itemName, const std::string& desc, float x, float y, const sf::Texture& texture);
    virtual ~Item() = default;
    
    // Getters
    std::string getName() const;
    std::string getDescription() const;
    sf::Vector2f getPosition() const;
    bool isItemCollected() const;
    sf::FloatRect getBounds() const;
    
    // Actions
    void collect();
    virtual void use() = 0; 
    
    // Rendering
    void draw(sf::RenderWindow& window);
    
    // Collision
    bool checkCollision(const sf::FloatRect& bounds);
};

// Key item
class Key : public Item {
private:
    std::string doorID;
    
public:
    Key(const std::string& keyName, const std::string& doorIdentifier, float x, float y, const sf::Texture& texture);
    
    void use() override;
    std::string getDoorID() const;
};

// Passcode item
class Passcode : public Item {
private:
    std::string code;
    
public:
    Passcode(const std::string& passcodeName, const std::string& codeValue, float x, float y, const sf::Texture& texture);
    
    void use() override;
    std::string getCode() const;
};

// BasicItem
class BasicItem : public Item {
public:
    BasicItem(const std::string& itemName, const std::string& desc, float x, float y, const sf::Texture& texture);
    
    void use() override;
};

// Tool item
class Tool : public Item {
private:
    std::string toolType;
    bool isActive;
    
public:
    Tool(const std::string& toolName, const std::string& type, const std::string& desc, float x, float y, const sf::Texture& texture);
    
    void use() override;
    std::string getToolType() const;
    void activate();
    void deactivate();
    bool isToolActive() const;
};

// Inventory class (Unchanged)
class Inventory {
private:
    std::vector<std::shared_ptr<Item>> items;
    int maxCapacity;
    sf::Font font;
    sf::RectangleShape background;
    bool isVisible;
    
public:
    Inventory(int capacity = 15);
    bool addItem(std::shared_ptr<Item> item);
    bool removeItem(const std::string& itemName);
    bool hasItem(const std::string& itemName) const;
    std::shared_ptr<Item> getItem(const std::string& itemName);
    bool hasTool(const std::string& toolType) const;
    int getItemCount() const;
    int getMaxCapacity() const;
    bool isFull() const;
    std::vector<std::shared_ptr<Item>>& getItems();
    void toggleVisibility();
    void setVisible(bool visible);
    bool getVisible() const;
    void setFont(const sf::Font& f);
    void draw(sf::RenderWindow& window);
    void clear();
};

#endif // ITEM_H