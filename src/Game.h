// Player
// Guard
// Timer

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include <map>
#include "Player.h"
#include "Room.h"
#include "Timer.h"
#include "Item.h"

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    PUZZLE_ACTIVE,
    GAME_OVER,
    VICTORY
};

class Game {
private:
    // Window
    sf::RenderWindow window;
    sf::Clock clock;
    float deltaTime;
    
    // Game state
    GameState currentState;
    
    // Core components
    std::unique_ptr<Player> player;
    std::unique_ptr<Timer> gameTimer;
    std::unique_ptr<Inventory> inventory;
};
#endif // GAME_H