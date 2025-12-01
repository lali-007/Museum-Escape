/*
 * Museum Escape - Game Class Implementation
 * CS/CE 224/272 - Fall 2025
 */

#include "Game.h"
#include "Room.h"
#include "Puzzle.h"
#include "Guard.h"
#include "Item.h"
#include <iostream>

// Constructor
Game::Game() 
    : window(sf::VideoMode({800u, 600u}), "Museum Escape - Enhanced"),
      currentState(GameState::MENU),
      deltaTime(0.0f),
      currentRoomID(1),
      activePuzzle(nullptr),
      stateText(defaultFont),
      notificationText(notificationFont),
      notificationTimer(0.0f),
      notificationColor(sf::Color::White)
{
    window.setFramerateLimit(60);
    initialize();
}

// Destructor
Game::~Game() {
    // Cleanup handled by smart pointers
}

// Initialize game components
void Game::initialize() {
    // Load assets FIRST
    loadAssets();
    
    // Create player (Pass the texture)
    player = std::make_unique<Player>(148.0f, 289.0f, playerTexture);
    
    // Create timer (10 minutes = 600 seconds)
    gameTimer = std::make_unique<Timer>(600.0f);
    gameTimer->setDisplayPosition(650.0f, 20.0f);
    gameTimer->setFont(mainFont);
    
    // Create inventory (increased capacity)
    inventory = std::make_unique<Inventory>(15);
    inventory->setFont(mainFont);
    
    // Create rooms and puzzles
    createRooms();
    
    setupPuzzles();
    
    // Setup UI text
    stateText.setFont(mainFont);
    stateText.setCharacterSize(30);
    stateText.setFillColor(sf::Color::White);
    stateText.setPosition({250.0f, 250.0f});
    
    // Setup notification text
    notificationText.setFont(mainFont);
    notificationText.setCharacterSize(24);
    notificationText.setPosition({50.0f, 50.0f});
    notificationText.setOutlineThickness(2.0f);
    notificationText.setOutlineColor(sf::Color::Black);
    
    // Setup overlay
    overlay.setSize({800.0f, 600.0f});
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    
    std::cout << "\n=== MUSEUM ESCAPE - ENHANCED VERSION ===" << std::endl;
    std::cout << "Story: You're an investigative journalist breaking into" << std::endl;
    std::cout << "the museum to find evidence of illegal experiments!" << std::endl;
    std::cout << "Find the Evidence File and escape before police arrive!\n" << std::endl;
    std::cout << "Game initialized successfully!" << std::endl;
}

// Load fonts, sounds, music AND TEXTURES
void Game::loadAssets() {
    bool fontLoaded = false;
    
    // Try multiple paths for the font
    if (mainFont.openFromFile("assets/arial.ttf")) {
        fontLoaded = true;
    } else if (mainFont.openFromFile("arial.ttf")) {
        fontLoaded = true;
    } else if (mainFont.openFromFile("D:/Assignments/Sem3/OOP/Prozect/main/assets/arial.ttf")) {
        fontLoaded = true;
    }
    
    if (!fontLoaded) {
        std::cerr << "Warning: Could not load font from any path!" << std::endl;
    } else {
        std::cout << "Font loaded successfully!" << std::endl;
    }
    
    // === Load Character Sprites ===
    if (playerTexture.loadFromFile("assets/player.png")) {
        std::cout << "Loaded player texture." << std::endl;
    } else {
        std::cerr << "Failed to load assets/player.png" << std::endl;
    }

    if (guardTexture.loadFromFile("assets/guard.png")) {
        std::cout << "Loaded guard texture." << std::endl;
    } else {
        std::cerr << "Failed to load assets/guard.png" << std::endl;
    }
    if (keyTexture.loadFromFile("assets/key.png")) {
        std::cout << "Loaded Key texture." << std::endl;
    } else {
        std::cerr << "Failed to load assets/key.png" << std::endl;
    }
    if (flashlightTexture.loadFromFile("assets/flashlight.png")) {
        std::cout << "Loaded Flashlight texture." << std::endl;
    } else {
        std::cerr << "Failed to load assets/flashlight.png" << std::endl;
    }
    if (mapTexture.loadFromFile("assets/map.png")) {
        std::cout << "Loaded map texture." << std::endl;
    } else {
        std::cerr << "Failed to load assets/map.png" << std::endl;
    }
    if (cutterTexture.loadFromFile("assets/bolt_cutters.png")) {
        std::cout << "Loaded Bolt Cutters texture." << std::endl;
    } else {
        std::cerr << "Failed to load assets/bolt_Cutters.png" << std::endl;
    }
    if (paperTexture.loadFromFile("assets/paper.png")) {
        std::cout << "Loaded Paper texture." << std::endl;
    } else {
        std::cerr << "Failed to load assets/paper.png" << std::endl;
    }
    if (patternTexture.loadFromFile("assets/buttons.png")) std::cout << "Loaded Pattern texture." << std::endl;
    if (riddleTexture.loadFromFile("assets/riddle.png")) std::cout << "Loaded Riddle texture." << std::endl;
    if (lockTexture.loadFromFile("assets/keypad.png")) std::cout << "Loaded Lock texture." << std::endl;
    if (mathTexture.loadFromFile("assets/calculator.png")) std::cout << "Loaded Math texture." << std::endl;
    if (wireTexture.loadFromFile("assets/wirepuzzle.png")) std::cout << "Loaded Wire texture." << std::endl;
    
    // === Load Room Backgrounds & Solved Backgrounds ===
    for (int i = 1; i <= 7; ++i) {
        // 1. Load Standard Background
        sf::Texture texture;
        std::string filenamePNG = "assets/room" + std::to_string(i) + ".png";
        std::string filenameJPG = "assets/room" + std::to_string(i) + ".jpg";
        
        // Try PNG first, then JPG
        if (texture.loadFromFile(filenamePNG)) {
            roomTextures[i] = texture;
            std::cout << "Loaded background: " << filenamePNG << std::endl;
        } else if (texture.loadFromFile(filenameJPG)) {
            roomTextures[i] = texture;
            std::cout << "Loaded background: " << filenameJPG << std::endl;
        } else {
            std::cerr << "Warning: Could not load background for room " << i << " (checked .png and .jpg)" << std::endl;
        }

        // 2. Load "Open" Background (e.g., room1_open.png)
        sf::Texture openTexture;
        std::string openName = "assets/room" + std::to_string(i) + "_open.png";
        
        if (openTexture.loadFromFile(openName)) {
            solvedRoomTextures[i] = openTexture;
            std::cout << "Loaded open background: " << openName << std::endl;
        }
    }
    
    std::cout << "Assets loaded!" << std::endl;
}

// Create game rooms
void Game::createRooms() {
    // ========================================================================
    // Room 1: Main Entrance (Tutorial)
    // ========================================================================
    auto room1 = std::make_shared<Room>(1, "Main Entrance", 0, 0, 800, 600);
    
    room1->addItem(std::make_shared<Tool>("Flashlight", "flashlight", 
        "Illuminates dark areas", 150.0f, 150.0f, flashlightTexture));
        
    room1->addItem(std::make_shared<BasicItem>("Museum Map", 
        "Shows the layout of the museum", 650.0f, 150.0f, mapTexture));
    
    auto guard1 = std::make_shared<Guard>(400.0f, 200.0f, 100.0f, guardTexture);
    guard1->addPatrolPoint(400.0f, 200.0f);
    guard1->addPatrolPoint(600.0f, 200.0f);
    guard1->addPatrolPoint(600.0f, 400.0f);
    guard1->addPatrolPoint(400.0f, 400.0f);
    room1->addGuard(guard1);
    
    rooms[1] = room1;
    
    // ========================================================================
    // Room 2: Ancient Artifacts Gallery
    // ========================================================================
    auto room2 = std::make_shared<Room>(2, "Ancient Artifacts Gallery", 0, 0, 800, 600);
    
    auto guard2 = std::make_shared<Guard>(400.0f, 450.0f, 110.0f, guardTexture);
    guard2->addPatrolPoint(400.0f, 450.0f);
    guard2->addPatrolPoint(400.0f, 150.0f);
    guard2->addPatrolPoint(600.0f, 150.0f);
    guard2->addPatrolPoint(600.0f, 450.0f);
    room2->addGuard(guard2);
    
    rooms[2] = room2;
    
    // ========================================================================
    // Room 3: Medieval Weapons Hall
    // ========================================================================
    auto room3 = std::make_shared<Room>(3, "Medieval Weapons Hall", 0, 0, 800, 600);
    
    room3->addItem(std::make_shared<Tool>("Bolt Cutters", "bolt_cutters",
        "Cuts through chains and wires", 650.0f, 500.0f, cutterTexture));
        
    room3->addItem(std::make_shared<BasicItem>("Red key",
        "An old security card", 150.0f, 150.0f, keyTexture));
    
    auto guard3 = std::make_shared<Guard>(400.0f, 200.0f, 100.0f, guardTexture);
    guard3->addPatrolPoint(400.0f, 200.0f);
    guard3->addPatrolPoint(600.0f, 200.0f);
    guard3->addPatrolPoint(600.0f, 450.0f);
    guard3->addPatrolPoint(400.0f, 450.0f);
    room3->addGuard(guard3);
    
    rooms[3] = room3;
    
    // ========================================================================
    // Room 4: Security Control Room
    // ========================================================================
    auto room4 = std::make_shared<Room>(4, "Security Control Room", 0, 0, 800, 600);
    
    room4->addItem(std::make_shared<Passcode>("Access Code Note", "4738", 150.0f, 500.0f, paperTexture));
    
    auto guard4a = std::make_shared<Guard>(300.0f, 150.0f, 110.0f, guardTexture);
    guard4a->addPatrolPoint(300.0f, 150.0f);
    guard4a->addPatrolPoint(600.0f, 150.0f);
    room4->addGuard(guard4a);
    
    auto guard4b = std::make_shared<Guard>(600.0f, 450.0f, 110.0f, guardTexture);
    guard4b->addPatrolPoint(600.0f, 450.0f);
    guard4b->addPatrolPoint(300.0f, 450.0f);
    room4->addGuard(guard4b);
    
    rooms[4] = room4;
    
    // ========================================================================
    // Room 5: Dark Archives
    // ========================================================================
    auto room5 = std::make_shared<Room>(5, "Dark Archives", 0, 0, 800, 600);
    
    room5->addItem(std::make_shared<BasicItem>("Encrypted Note",
        "Wire sequence: Primary colors first, then secondary", 650.0f, 150.0f, paperTexture));

    auto guard5 = std::make_shared<Guard>(400.0f, 400.0f, 120.0f, guardTexture);
    guard5->addPatrolPoint(400.0f, 400.0f);
    guard5->addPatrolPoint(600.0f, 400.0f);
    guard5->addPatrolPoint(600.0f, 200.0f);
    guard5->addPatrolPoint(400.0f, 200.0f);
    room5->addGuard(guard5);
    
    rooms[5] = room5;
    
    // ========================================================================
    // Room 6: Laboratory
    // ========================================================================
    auto room6 = std::make_shared<Room>(6, "Laboratory", 0, 0, 800, 600);
    
    room6->addItem(std::make_shared<BasicItem>("Evidence Log",
    "Documents showing illegal experiments", 150.0f, 150.0f, paperTexture)); // <--- Pass paperTexture
    
    auto guard6a = std::make_shared<Guard>(400.0f, 200.0f, 115.0f, guardTexture);
    guard6a->addPatrolPoint(400.0f, 200.0f);
    guard6a->addPatrolPoint(600.0f, 200.0f);
    room6->addGuard(guard6a);
    
    auto guard6b = std::make_shared<Guard>(600.0f, 450.0f, 115.0f, guardTexture);
    guard6b->addPatrolPoint(600.0f, 450.0f);
    guard6b->addPatrolPoint(400.0f, 450.0f);
    room6->addGuard(guard6b);
    
    rooms[6] = room6;
    
    // ========================================================================
    // Room 7: Director's Office
    // ========================================================================
    auto room7 = std::make_shared<Room>(7, "Director's Office", 0, 0, 800, 600);
    
    room7->addItem(std::make_shared<BasicItem>("Evidence File",
    "The proof you need...", 400.0f, 300.0f, paperTexture)); // <--- Pass paperTexture

// Change Journal creation:
    room7->addItem(std::make_shared<BasicItem>("Personal Journal",
    "The Director's personal notes", 650.0f, 150.0f, paperTexture)); // <--- Pass paperTexture
    
    room7->setExitRoom(true);
    rooms[7] = room7;
    
    // ========================================================================
    // Connect Rooms (MANUAL DOOR PLACEMENT)
    // Format: addDoor(make_shared<Door>(X_POS, Y_POS, TARGET_ROOM_ID, ...))
    // ========================================================================
    
    // Room 1 (Main Entrance)
    // Door to Room 2 (Right side)
    room1->addDoor(std::make_shared<Door>(648.0f, 377.0f, 2));
    
    // Room 2 (Ancient Artifacts)
    // Door to Room 1 (Left side)
    room2->addDoor(std::make_shared<Door>(195.0f, 350.0f, 1));
    // Door to Room 3 (Right side)
    room2->addDoor(std::make_shared<Door>(508.0f, 302.0f, 3, true, "blue_key"));
    
    // Room 3 (Medieval Weapons)
    room3->addDoor(std::make_shared<Door>(533.0f, 423.0f, 2)); // Back to 2
    room3->addDoor(std::make_shared<Door>(200.0f, 423.0f, 4)); // Forward to 4
    
    // Room 4 (Security Control)
    room4->addDoor(std::make_shared<Door>(247.0f, 341.0f, 3)); // Back to 3
    room4->addDoor(std::make_shared<Door>(521.0f, 297.0f, 5, true, "yellow_key")); // Forward to 5
    
    // Room 5 (Dark Archives)
    room5->addDoor(std::make_shared<Door>(552.0f, 387.0f, 4)); // Back to 4
    room5->addDoor(std::make_shared<Door>(258.0f, 303.0f, 6, true, "green_key")); // Forward to 6
    
    // Room 6 (Laboratory)
    room6->addDoor(std::make_shared<Door>(182.0f, 308.0f, 5)); // Back to 5
    room6->addDoor(std::make_shared<Door>(488.0f, 270.0f, 7, true, "master_key")); // Forward to 7
    
    // Room 7 (Director's Office)
    room7->addDoor(std::make_shared<Door>(583.0f, 299.0f, 6)); // Back to 6
    room7->addDoor(std::make_shared<Door>(250.0f, 299.0f, 999, true, "Evidence File"));
    
    // ========================================================================
    // APPLY TEXTURES
    // ========================================================================
    for (auto& pair : rooms) {
        int id = pair.first;
        if (roomTextures.find(id) != roomTextures.end()) {
            pair.second->setBackgroundTexture(roomTextures[id]);
        }
        
        if (solvedRoomTextures.find(id) != solvedRoomTextures.end()) {
            pair.second->setSolvedBackgroundTexture(solvedRoomTextures[id]);
        }
    }
    
    std::cout << "\n=== ENHANCED MUSEUM STRUCTURE ===" << std::endl;
}

// Setup puzzles in rooms
void Game::setupPuzzles() {
    auto patternPuzzle = std::make_shared<PatternPuzzle>(std::vector<int>{1, 3, 2, 4});
    patternPuzzle->setFont(mainFont);
    patternPuzzle->setPosition(489.0f, 120.0f);        // <--- Set Position in world
    patternPuzzle->setWorldTexture(patternTexture);
    rooms[2]->addPuzzle(patternPuzzle);
    
    auto riddle = std::make_shared<RiddlePuzzle>(
        "I speak without a mouth and hear without ears.\nI have no body, but come alive with wind.\nWhat am I?",
        "echo"
    );
    riddle->setFont(mainFont);
    riddle->setPosition(13.0f, 456.0f);
    riddle->setWorldTexture(riddleTexture);
    rooms[3]->addPuzzle(riddle);
    
    auto lockPuzzle = std::make_shared<LockPuzzle>("4738");
    lockPuzzle->setFont(mainFont);
    lockPuzzle->setPosition(64.0f, 95.0f);
    lockPuzzle->setWorldTexture(lockTexture);
    rooms[4]->addPuzzle(lockPuzzle);
    
    auto mathPuzzle = std::make_shared<MathPuzzle>("(60 - 12) = ?", "048");
    mathPuzzle->setFont(mainFont);
    mathPuzzle->setPosition(57.0f, 203.0f);
    mathPuzzle->setWorldTexture(mathTexture);
    rooms[5]->addPuzzle(mathPuzzle);
    
    auto wirePuzzle = std::make_shared<WirePuzzle>(
        std::vector<std::string>{"Red", "Yellow", "Blue", "Green", "Purple"}
    );
    wirePuzzle->setFont(mainFont);
    wirePuzzle->setBoltCutters(false);
    wirePuzzle->setPosition(428.0f, 198.0f);
    wirePuzzle->setWorldTexture(wireTexture);
    rooms[6]->addPuzzle(wirePuzzle);
    
    std::cout << "All Puzzles setup complete." << std::endl;
}

// Main game loop
void Game::run() {
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();
        processEvents();
        update();
        render();
    }
}

// Process input events
void Game::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        
        switch (currentState) {
            case GameState::MENU: handleMenuInput(*event); break;
            case GameState::PLAYING: handlePlayingInput(*event); break;
            case GameState::PUZZLE_ACTIVE: handlePuzzleInput(*event); break;
            case GameState::PAUSED: handlePauseInput(*event); break;
            case GameState::GAME_OVER: handleGameOverInput(*event); break;
            case GameState::VICTORY: handleGameOverInput(*event);break;
            default: break;
        }
    }
}

// Handle menu input
void Game::handleMenuInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Enter) {
            currentState = GameState::PLAYING;
            gameTimer->start();
            showStoryText(1);
            std::cout << "\n=== GAME STARTED ===" << std::endl;
        }
    }
}

// Handle playing state input
void Game::handlePlayingInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) pauseGame();
        if (keyPressed->code == sf::Keyboard::Key::I) inventory->toggleVisibility();
        if (keyPressed->code == sf::Keyboard::Key::E) { checkDoorInteraction(); checkItemPickup(); }
        if (keyPressed->code == sf::Keyboard::Key::P) {
            if (rooms.find(currentRoomID) != rooms.end()) {
                // Get the specific puzzle we are touching
                auto puzzle = rooms[currentRoomID]->getIntersectingPuzzle(player->getBounds());
                
                if (puzzle) {
                    // Special Logic for Wire Puzzle (Room 6)
                    if (currentRoomID == 6) {
                        WirePuzzle* wirePuzzle = dynamic_cast<WirePuzzle*>(puzzle.get());
                        if (wirePuzzle) {
                            if (!inventory->hasTool("bolt_cutters")) {
                                showNotification("Need Bolt Cutters to cut wires!", sf::Color::Red, 3.0f);
                                return;
                            }
                            wirePuzzle->setBoltCutters(true);
                        }
                    }

                    activatePuzzle(puzzle);
                    showNotification("Puzzle activated! Press ESC to close", sf::Color::Magenta, 2.0f);
                }
            }
        }
        // ============================
    }
}

// Handle puzzle input
void Game::handlePuzzleInput(const sf::Event& event) {
    if (activePuzzle) {
        bool wasSolved = activePuzzle->isSolvedStatus();
        
        activePuzzle->handleInput(const_cast<sf::Event&>(event));
        
        // Check if puzzle was just solved
        if (!wasSolved && activePuzzle->isSolvedStatus()) {
            gameTimer->addTime(activePuzzle->getTimeBonus());
            showNotification("Puzzle Solved! +" + std::to_string(activePuzzle->getTimeBonus()) + "s", sf::Color::Green, 3.0f);
            
            if (currentRoomID == 2) {
                // Pass keyTexture
                auto blueCard = std::make_shared<Key>("Blue key", "blue_key", 650.0f, 500.0f, keyTexture); 
                rooms[2]->addItem(blueCard);
                // ...
            } else if (currentRoomID == 4) {
                // Pass keyTexture
                auto yellowCard = std::make_shared<Key>("Yellow key", "yellow_key", 650.0f, 500.0f, keyTexture);
                rooms[4]->addItem(yellowCard);
                // ...
            } else if (currentRoomID == 5) {
                // Pass keyTexture
                auto greenCard = std::make_shared<Key>("Green key", "green_key", 650.0f, 500.0f, keyTexture);
                rooms[5]->addItem(greenCard);
                // ...
            } else if (currentRoomID == 6) {
                // Pass keyTexture
                auto masterCard = std::make_shared<Key>("Master key", "master_key", 650.0f, 500.0f, keyTexture);
                rooms[6]->addItem(masterCard);
                // ...
            }
        }
    }
    
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
            activePuzzle = nullptr;
            currentState = GameState::PLAYING;
            gameTimer->resume();
        }
    }
}

// Handle pause input
void Game::handlePauseInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) resumeGame();
    }
}

// Handle input for Game Over and Victory screens
void Game::handleGameOverInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
            window.close();
        }
        // Optional: Press Enter to Restart instead of quit?
        else if (keyPressed->code == sf::Keyboard::Key::Enter) {
            resetGame();
        }
    }
}

// Update game state
void Game::update() {
    switch (currentState) {
        case GameState::MENU: updateMenu(); break;
        case GameState::PLAYING: updatePlaying(); break;
        case GameState::PUZZLE_ACTIVE: updatePuzzle(); break;
        case GameState::GAME_OVER: updateGameOver(); break;
        default: break;
    }
}

void Game::updateMenu() {}

void Game::updatePlaying() {
    gameTimer->update(deltaTime);
    if (notificationTimer > 0) notificationTimer -= deltaTime;
    
    player->handleInput(deltaTime);
    player->update(deltaTime);
    
    if (rooms.find(currentRoomID) != rooms.end()) {
        rooms[currentRoomID]->update(deltaTime);
        
        for (auto& guard : rooms[currentRoomID]->getGuards()) {
            guard->update(deltaTime, *player);
        }
    }
    
    checkCollisions();
    checkGuardDetection();
    checkWinCondition();
    checkLoseCondition();
}

void Game::updatePuzzle() {
    if (activePuzzle) activePuzzle->update(deltaTime);
}

void Game::updateGameOver() {}

// Render everything
void Game::render() {
    window.clear(sf::Color(20, 20, 30));
    switch (currentState) {
        case GameState::MENU: renderMenu(); break;
        case GameState::PLAYING: renderPlaying(); break;
        case GameState::PUZZLE_ACTIVE: renderPuzzle(); break;
        case GameState::GAME_OVER: renderGameOver(); break;
        case GameState::VICTORY: renderVictory(); break;
        default: break;
    }
    window.display();
}

void Game::renderMenu() {
    stateText.setString("MUSEUM ESCAPE - ENHANCED\n\nPress ENTER to Start");
    stateText.setCharacterSize(20);
    stateText.setPosition({150.0f, 150.0f});
    window.draw(stateText);
}

void Game::renderPlaying() {
    // Draw current room
    if (rooms.find(currentRoomID) != rooms.end()) {
        rooms[currentRoomID]->draw(window);
    }
    
    // Draw player
    player->draw(window);
    
    // Draw timer
    gameTimer->draw(window);
    
    // Draw room name (top-left)
    sf::Text roomName(mainFont);
    roomName.setString("Room: " + rooms[currentRoomID]->getRoomName());
    roomName.setCharacterSize(18);
    roomName.setFillColor(sf::Color::White);
    roomName.setPosition({10.0f, 10.0f});
    window.draw(roomName);

    // --- NEW: DEBUG COORDINATES ---
    sf::Vector2f pos = player->getPosition();
    sf::Text coordText(mainFont);
    coordText.setString("X: " + std::to_string((int)pos.x) + "  Y: " + std::to_string((int)pos.y));
    coordText.setCharacterSize(16);
    coordText.setFillColor(sf::Color::Yellow);
    coordText.setPosition({10.0f, 35.0f}); // Display below the room name
    window.draw(coordText);
    // -----------------------------
    if (rooms.find(currentRoomID) != rooms.end()) {
        // Use the function you added to Room.h/cpp
        auto puzzle = rooms[currentRoomID]->getIntersectingPuzzle(player->getBounds());
        
        if (puzzle) {
            sf::Text prompt(mainFont);
            prompt.setString("Press P to Interact");
            prompt.setCharacterSize(18);
            prompt.setFillColor(sf::Color::Yellow);
            prompt.setOutlineColor(sf::Color::Black);
            prompt.setOutlineThickness(1.0f);
            
            // Position prompt slightly above the player
            sf::Vector2f pPos = player->getPosition();
            prompt.setPosition({pPos.x - 20.0f, pPos.y - 30.0f});
            window.draw(prompt);
        }
    }
    // Draw inventory if visible
    if (inventory->getVisible()) {
        inventory->draw(window);
    }
    
    // Draw notification if active (ALWAYS ON TOP)
    if (notificationTimer > 0) {
        notificationText.setString(currentNotification);
        notificationText.setFillColor(notificationColor);
        window.draw(notificationText);
    }
}

void Game::renderPuzzle() {
    renderPlaying();
    window.draw(overlay);
    if (activePuzzle) activePuzzle->display(window);
}

void Game::renderGameOver() {
    window.draw(overlay);
    stateText.setString("GAME OVER\n\nPress ESC to quit");
    stateText.setCharacterSize(30);
    stateText.setPosition({250.0f, 250.0f});
    window.draw(stateText);
}

void Game::renderVictory() {
    window.draw(overlay);
    stateText.setString("MISSION ACCOMPLISHED!\n\nPress ESC to quit");
    stateText.setCharacterSize(26);
    stateText.setPosition({180.0f, 230.0f});
    window.draw(stateText);
}

// Change to a different room
void Game::changeRoom(int newRoomID) {
    if (rooms.find(newRoomID) != rooms.end()) {
        
        float spawnX = 148.0f;
        float spawnY = 289.0f;

        // MANUAL COORDINATE CONFIGURATION (Your custom logic)
        switch (newRoomID) {
            case 1: spawnX = (currentRoomID == 2) ? 632.0f : 148.0f; spawnY = (currentRoomID == 2) ? 299.0f : 289.0f; break;
            case 2: spawnX = (currentRoomID == 1) ? 184.0f : 488.0f; spawnY = (currentRoomID == 1) ? 289.0f : 242.0f; break;
            case 3: spawnX = (currentRoomID == 2) ? 513.0f : 202.0f; spawnY = (currentRoomID == 2) ? 357.0f : 355.0f; break;
            case 4: spawnX = (currentRoomID == 3) ? 264.0f : 490.0f; spawnY = (currentRoomID == 3) ? 276.0f : 259.0f; break;
            case 5: spawnX = (currentRoomID == 4) ? 512.0f : 289.0f; spawnY = (currentRoomID == 4) ? 348.0f : 264.0f; break;
            case 6: spawnX = (currentRoomID == 5) ? 182.0f : 488.0f; spawnY = (currentRoomID == 5) ? 308.0f : 270.0f; break;
            case 7: spawnX = (currentRoomID == 6) ? 583.0f : 250.0f; spawnY = (currentRoomID == 6) ? 299.0f : 299.0f; break;
        }

        currentRoomID = newRoomID;
        rooms[currentRoomID]->setVisited(true);
        player->setPosition(spawnX, spawnY);
        showStoryText(newRoomID);
        
        std::cout << "\n→ Entered: " << rooms[currentRoomID]->getRoomName() << std::endl;
    }
}


void Game::showStoryText(int roomID) {
    std::map<int, std::string> roomStories = {
        {1, "You've infiltrated the museum..."},
        {2, "Ancient artifacts glow eerily..."},
        {3, "Medieval weapons surround you..."},
        {4, "Security monitors flicker..."},
        {5, "Darkness engulfs everything..."},
        {6, "Bubbling beakers and strange equipment..."},
        {7, "The Director's Office..."}
    };
    if (roomStories.find(roomID) != roomStories.end()) {
        showNotification(roomStories[roomID], sf::Color(200, 200, 255), 5.0f);
    }
}

void Game::activatePuzzle(std::shared_ptr<Puzzle> puzzle) {
    activePuzzle = puzzle;
    currentState = GameState::PUZZLE_ACTIVE;
    gameTimer->pause();
}

void Game::checkCollisions() {
    auto bounds = player->getBounds();
    sf::Vector2f pos = player->getPosition();
    if (pos.x < 0) player->setPosition(0, pos.y);
    if (pos.y < 0) player->setPosition(pos.x, 0);
    if (pos.x > 800 - bounds.size.x) player->setPosition(800 - bounds.size.x, pos.y);
    if (pos.y > 600 - bounds.size.y) player->setPosition(pos.x, 600 - bounds.size.y);
}

void Game::checkGuardDetection() {
    for (auto& guard : rooms[currentRoomID]->getGuards()) {
        if (guard->detectPlayer(*player)) {
            if (!player->isPlayerWarned()) {
                player->warn();
                showNotification("WARNING! Caught by guard!", sf::Color::Yellow, 3.0f);
                gameTimer->subtractTime(5.0f);
            } else {
                setGameOver(false);
            }
        }
    }
}

void Game::checkDoorInteraction() {
    auto& doors = rooms[currentRoomID]->getDoors();
    auto playerBounds = player->getBounds();
    
    for (auto& door : doors) {
        if (door->checkCollision(playerBounds)) {
            if (door->getLockedStatus()) {
                bool hasKey = false;
                std::string requiredKey = "";
                
                int targetRoom = door->getTargetRoomID();
                if (targetRoom == 3) {
                    requiredKey = "Blue key";
                } else if (targetRoom == 5) {
                    requiredKey = "Yellow key";
                    // Check for flashlight
                    if (!inventory->hasTool("flashlight")) {
                        showNotification("It's too dark ahead! Need a Flashlight!", sf::Color::Red, 3.0f);
                        std::cout << "Room 5 is DARK! You need a Flashlight first!" << std::endl;
                        return;
                    }
                } else if (targetRoom == 6) {
                    requiredKey = "Green key";
                    // Check for bolt cutters BEFORE checking key
                    if (!inventory->hasTool("bolt_cutters")) {
                        showNotification("Door is chained! Need Bolt Cutters!", sf::Color::Red, 3.0f);
                        std::cout << "Door is chained! You need Bolt Cutters!" << std::endl;
                        return;
                    }
                } else if (targetRoom == 7) {
                    requiredKey = "Master key";
                }
                else if (targetRoom == 999) {
                    requiredKey = "Evidence File";
                }
                
                auto& inv = player->getInventory();
                for (auto* item : inv) {
                    if (item->getName() == requiredKey) {
                        hasKey = true;
                        break;
                    }
                }
                
                if (hasKey) {
                    door->unlock();
                    rooms[currentRoomID]->forceSolvedBackground();
                    showNotification("Door unlocked with " + requiredKey + "!", sf::Color::Green, 2.0f);
                    std::cout << "Door unlocked with " << requiredKey << "!" << std::endl;
                    if (targetRoom == 999) {
                        setGameOver(true);
                        return;
                    }
                    
                    changeRoom(door->getTargetRoomID());
                } else {
                    showNotification("LOCKED! Need " + requiredKey, sf::Color::Red, 2.0f);
                    std::cout << "Door LOCKED! Need: " << requiredKey << std::endl;
                }
            } else {
                if (door->getTargetRoomID() == 999) {
                    setGameOver(true);
                    return;    
                }    

                if (door->getTargetRoomID() > currentRoomID) {
                    rooms[currentRoomID]->forceSolvedBackground();
                }
                changeRoom(door->getTargetRoomID());
            }
            return;
        }
    }
}

void Game::checkItemPickup() {
    auto& items = rooms[currentRoomID]->getItems();
    auto playerBounds = player->getBounds();
    
    for (auto& item : items) {
        if (!item->isItemCollected() && item->checkCollision(playerBounds)) {
            item->collect();
            player->addItem(item.get());
            inventory->addItem(item);
            
            std::string name = item->getName();
            
            if (name == "Blue key") {
                rooms[2]->forceSolvedBackground(); // Open Room 2 Door
                showNotification("Blue Key collected! Door Opened.", sf::Color::Green);
            }
            else if (name == "Yellow key") {
                rooms[4]->forceSolvedBackground(); // Open Room 4 Door
                showNotification("Yellow Key collected! Door Opened.", sf::Color::Green);
            }
            else if (name == "Green key") {
                rooms[5]->forceSolvedBackground(); // Open Room 5 Door
                showNotification("Green Key collected! Door Opened.", sf::Color::Green);
            }
            else if (name == "Master key") {
                rooms[6]->forceSolvedBackground(); // Open Room 6 Door
                showNotification("Master Key collected! Door Opened.", sf::Color::Green);
            }

            // Special messages for important items
            else if (item->getName() == "Flashlight") {
                showNotification("FLASHLIGHT acquired! You can now enter dark areas!", sf::Color::Yellow, 5.0f);
                std::cout << "FLASHLIGHT acquired! Room 5 (Dark Archives) is now accessible!" << std::endl;
            } else if (item->getName() == "Bolt Cutters") {
                showNotification("BOLT CUTTERS acquired! You can now cut chains!", sf::Color::Yellow, 5.0f);
                std::cout << "BOLT CUTTERS acquired! You can access chained areas!" << std::endl;
            } else if (item->getName() == "Evidence File") {
                rooms[7]->forceSolvedBackground();
                showNotification("EVIDENCE FILE acquired! Escape to win!", sf::Color::Green, 5.0f);
                std::cout << "EVIDENCE FILE acquired! Mission complete - now escape!" << std::endl;
            } else if (item->getName() == "Access Code Note") {
                Passcode* passcode = dynamic_cast<Passcode*>(item.get());
                if (passcode) {
                    std::string code = passcode->getCode();
                    showNotification("Security Code: " + code + " (for Lock Puzzle!)", sf::Color::Cyan, 8.0f);
                    std::cout << "Found Access Code: " << code << std::endl;
                }
            } else {
                showNotification("Picked up: " + item->getName(), sf::Color::Cyan, 2.0f);
                std::cout << "Picked up: " << item->getName() << std::endl;
            }
        }
    }
}

// void Game::checkPuzzleInteraction() {
//     auto& puzzles = rooms[currentRoomID]->getPuzzles();
    
//     for (auto& puzzle : puzzles) {
//         if (!puzzle->isSolvedStatus()) {
//             // Special handling for Wire Puzzle - need bolt cutters
//             if (currentRoomID == 6) {
//                 WirePuzzle* wirePuzzle = dynamic_cast<WirePuzzle*>(puzzle.get());
//                 if (wirePuzzle) {
//                     if (!inventory->hasTool("bolt_cutters")) {
//                         showNotification("Need Bolt Cutters to cut wires!", sf::Color::Red, 3.0f);
//                         std::cout << "Wire Puzzle requires Bolt Cutters!" << std::endl;
//                         return;
//                     }
//                     wirePuzzle->setBoltCutters(true);
//                 }
//             }
            
//             activatePuzzle(puzzle);
//             showNotification("Puzzle activated! Press ESC to close", sf::Color::Magenta, 2.0f);
//             return;
//         }
//     }
// }

void Game::checkWinCondition() { 
    // if (inventory->hasItem("Evidence File")) setGameOver(true); 
    }
void Game::checkLoseCondition() { if (gameTimer->isExpired()) setGameOver(false); }
void Game::setGameOver(bool victory) {
    currentState = victory ? GameState::VICTORY : GameState::GAME_OVER;
    gameTimer->stop();
}
void Game::pauseGame() { currentState = GameState::PAUSED; gameTimer->pause(); }
void Game::resumeGame() { currentState = GameState::PLAYING; gameTimer->resume(); }
void Game::resetGame() {
    std::cout << "Resetting Game World..." << std::endl;

    // 1. Return to Menu
    currentState = GameState::MENU;
    
    // 2. Reset Global Variables
    currentRoomID = 1;
    activePuzzle = nullptr;
    
    // 3. Re-create Player (Reset position, health, etc.)
    player = std::make_unique<Player>(148.0f, 289.0f, playerTexture);
    
    // 4. Reset Timer
    gameTimer = std::make_unique<Timer>(600.0f);
    gameTimer->setDisplayPosition(650.0f, 20.0f);
    gameTimer->setFont(mainFont);
    
    // 5. Clear Inventory
    inventory = std::make_unique<Inventory>(15);
    inventory->setFont(mainFont);
    
    // 6. Reset Rooms (This resets Items, Doors, Guards, and Puzzles)
    rooms.clear(); // Clear old rooms
    createRooms(); // Create fresh rooms
    
    // 7. Setup Puzzles again (since rooms were re-created)
    setupPuzzles();
}
void Game::showNotification(const std::string& message, const sf::Color& color, float duration) {
    currentNotification = message;
    notificationColor = color;
    notificationTimer = duration;
}