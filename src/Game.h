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

enum class GameState { MENU, PLAYING, PAUSED, PUZZLE_ACTIVE, GAME_OVER, VICTORY };

class Game {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    float deltaTime;
    GameState currentState;
    
    std::unique_ptr<Player> player;
    std::unique_ptr<Timer> gameTimer;
    std::unique_ptr<Inventory> inventory;
    
    std::map<int, std::shared_ptr<Room>> rooms;
    std::map<int, sf::Texture> roomTextures;
    std::map<int, sf::Texture> solvedRoomTextures; // <--- NEW MAP

    int currentRoomID;
    std::shared_ptr<Puzzle> activePuzzle;
    
    sf::Texture playerTexture;
    sf::Texture guardTexture;
    sf::Texture keyTexture;
    sf::Texture flashlightTexture;
    sf::Texture mapTexture;
    sf::Texture cutterTexture;
    sf::Texture paperTexture;
    sf::Texture patternTexture;
    sf::Texture riddleTexture;
    sf::Texture lockTexture;
    sf::Texture mathTexture;
    sf::Texture wireTexture;
    sf::Font mainFont;
    sf::Font defaultFont;
    sf::Font notificationFont;
    sf::Music backgroundMusic;
    
    sf::Text stateText;
    sf::RectangleShape overlay;
    
    sf::Text notificationText;
    std::string currentNotification;
    float notificationTimer;
    sf::Color notificationColor;

    void initialize();
    void loadAssets();
    void createRooms();
    void setupPuzzles();
    void processEvents();
    void update();
    void render();
    
    void handleMenuInput(const sf::Event& event);
    void handlePlayingInput(const sf::Event& event);
    void handlePuzzleInput(const sf::Event& event);
    void handlePauseInput(const sf::Event& event);
    void handleGameOverInput(const sf::Event& event);
    
    void updateMenu();
    void updatePlaying();
    void updatePuzzle();
    void updateGameOver();
    void renderMenu();
    void renderPlaying();
    void renderPuzzle();
    void renderGameOver();
    void renderVictory();
    
    void changeRoom(int newRoomID);
    void activatePuzzle(std::shared_ptr<Puzzle> puzzle);
    void checkCollisions();
    void checkGuardDetection();
    void checkDoorInteraction();
    void checkItemPickup();
    
    void checkWinCondition();
    void checkLoseCondition();
    void setGameOver(bool victory);
    void resetGame();
    void pauseGame();
    void resumeGame();
    void showNotification(const std::string& message, const sf::Color& color, float duration = 3.0f);
    void showStoryText(int roomID);
    
public:
    Game();
    ~Game();
    void run();


};
#endif