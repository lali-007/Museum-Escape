Year 2089. You are an investigative journalist who discovered that 
the National History Museum is hiding a dark secret: ancient artifacts with 
unknown power are being used for illegal experiments.

Tonight, you've broken into the museum after hours to find evidence. But the 
advanced AI security system has detected your entry and alerted the authorities.

You have 150 seconds before the police arrive. Find the Evidence File in the 
Director's Office and escape before it's too late! Make sure to avoid the guards
in the premises. You cannot risk getting caught by them twice.

Your only advantage: you hacked the puzzle systems. Solve them to unlock doors 
and delay the police.

## IMPORTANT INSTRUCTIONS ##
    1. Download/clone the entire repository
    2. Open the src folder in you IDE of choice
    3. Compile using: "g++ main.cpp Game.cpp Room.cpp Player.cpp Timer.cpp Guard.cpp 
        Puzzle.cpp Item.cpp  -I "...\Museum-Escape\include" -L "...\Museum-Escape\lib" 
        -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -o game"
        Make sure to edit the paths for include and lib depeding on where you folder is.
    4. Run the exe. Goodluck on your mission! 


## CONTROLS ##
    Move Up: W or Up Arrow
    Move Down: S or Down Arrow
    Move Left: A or Left Arrow
    Move Right: D or Right Arrow
    Pick Up Items: E (While being near to the item)
    Enter Rooms: E (While standing on doors)
    Solve Puzzles: P (While being near to the puzzle)
    Pause/Unpause: Escape Key
    When Game is over: Escape Key (Exit)
                       Enter Key (Restart)

## ROOM STRUCTURE & GAME FLOW ##
    Main Entrance:
        1 Guard
        Flashlight 
        Map
        Unlocked Door to room 2
    Ancient Artifact Gallery:
        1 Guard
        Pattern Puzzle (Solve to get Key)
        Use Key to unlock door to room 3
    Medieval Weapons Hall:
        1 Guard
        Bolt Cutters
        Red Key
        Riddle (Solve to get additial time)
        Unlocked door to room 4
    Security Control Room:
        2 Guards
        Access Code (Needed to solve puzzle)
        Lock Puzzle (Solve to get Key)
        Use Key to unlock door to room 5
    Dark Archives:
        FLASHLIGHT IS NECCESARY TO ENTER
        1 Guard
        Encrypted Note
        Math Puzzle (Solve to get Key)
        Use Key to unlock door to room 6
    Laboratory:
        2 Guards
        Evidence Log
        Wire Puzzle (Bolt Cutters needed to solve)
        Solve wire puzzle for Key
        Use Key to unlock door to Room 7
    Director's Office:
        No Guards
        Personal Journal
        Evidence File (Needed to unlock Final Door)
        Escape using the final exit door