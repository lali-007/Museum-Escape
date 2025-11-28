
#ifndef GUARD_H
#define GUARD_H

#include <SFML/Graphics.hpp>
#include <vector>

class Player; // Forward declaration

class Guard {
private:
    sf::Vector2f position;
    sf::Sprite sprite; // CHANGED: Now a Sprite
    float speed;
    
    // Patrol logic
    std::vector<sf::Vector2f> patrolPoints;
    int currentPatrolIndex;
    bool movingForward;
    
    // Detection logic
    float detectionRadius;
    bool hasDetectedPlayer;
    float detectionCooldown; // Time before guard can detect again
    float cooldownTime;
    
    // Visuals
    sf::CircleShape detectionCircle;
    sf::FloatRect roomBounds;
    
public:
    // Constructor - CHANGED: Takes Texture
    Guard(float x, float y, float detectionRange, const sf::Texture& texture);
    
    // Patrol management
    void addPatrolPoint(float x, float y);
    void setPatrolPoints(const std::vector<sf::Vector2f>& points);
    void setRoomBounds(const sf::FloatRect& bounds);
    
    // AI Logic
    void patrol(float deltaTime);
    bool detectPlayer(const Player& player);
    void update(float deltaTime, const Player& player);
    
    // Rendering
    void draw(sf::RenderWindow& window, bool showDetectionRadius = true);
    
    // Utilities
    bool checkCollision(const sf::FloatRect& bounds);
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void setPosition(float x, float y);
    
private:
    void moveTowards(const sf::Vector2f& target, float deltaTime);
    float distanceTo(const sf::Vector2f& point) const;
};

#endif // GUARD_H