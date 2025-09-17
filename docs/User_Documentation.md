## User Documentation

### Getting Started

The application is an implementation of the board game Hive. The rules are included in the file Hive.pdf or can be found [here](https://themindcafe.com.sg/wp-content/uploads/2018/04/Hive.pdf).
The application should enforce all rules. If you find something that is not being checked correctly, please let me know.

### Installation

You need CMake and Raylib, at least version 4.5.0.
The following example uses the vcpkg package manager.

After that, follow these steps:
- Navigate to your project directory: `cd ~/project`
- Create a build directory: `mkdir build`
- Enter the build directory: `cd build`
- Run CMake: `cmake .. -DCMAKE_TOOLCHAIN_FILE=PATH/vcpkg/scripts/buildsystems/vcpkg.cmake` **Replace "PATH" with the actual path to the vcpkg installation folder**
- Build the application: `cmake --build .`

### First Use

Players take turns, and the name of the player currently on turn is displayed in red.
During their turn, players can place a bug tile from their available pieces or move an already placed bug tile, but only if they have already placed their queen.

Possible places for moving or placing a tile are marked by turquoise highlights. If there is no highlight, the tile cannot be moved or placed.

When moving, players must first select the tile they want to move and then select one of the highlighted places where they want to move the tile. 
If they click elsewhere, the move is canceled.

When placing, the player currently on turn must choose from the bug tiles in their player panel. Each bug tile has a name, remaining quantity, and a picture representing its appearance in the game. A player can only place bug tiles that have more than 0 quantity remaining. The player first selects a bug tile from the panel and then selects one of the highlighted places where they want to place the tile. If they click elsewhere, the placement is canceled.

### Ending the Game

If the conditions for winning or draw are met, the game ends, and a text box is displayed with information about the outcome. Then you need to restart the game if you want to play again.