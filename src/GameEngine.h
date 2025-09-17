/**
 * @file GameEngine.h
 * @author Vojtech Venzara (Vojtaven@seznam.cz)
 * @brief Contains class with the main logic of the game
 * @version 0.1
 * @date 2024-04-11
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "bugTiles.h"
#include "common.h"
#include "hexUtilities.h"
#include "Player.h"
#include "raylib.h"
#include "raymath.h"
#include "Renderer.h"
#include "rlgl.h"

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Enumeration representing the status of the game.
 */
enum class GameStatus {
	NORMAL,           /**< Normal state of the game. */
	DRAW,             /**< The game ended in a draw. */
	FIRST_PLAYER_WON, /**< The first player won the game. */
	SECOND_PLAYER_WON /**< The second player won the game. */
};

/**
 * @brief Represents the game engine.
 *
 * This class manages the game logic and rendering.
 */
class GameEngine {
public:
	/**
	 * @brief Constructs a GameEngine object.
	 *
	 * Constructs a GameEngine object and initializes the game.
	 */
	GameEngine();

	/**
	 * @brief Checks player inputs.
	 *
	 * Checks for player inputs and processes them accordingly.
	 */
	void CheckInputs();

	/**
	 * @brief Renders the base layout of the game.
	 *
	 * Renders the base layout of the game including the game map, player information.
	 */
	void RenderBaseLayout();

	/**
	 * @brief Configures player names.
	 *
	 * Allows players to configure their names before starting the game.
	 */
	void PlayerNameConfiguration();

	/**
	 * @brief Renders the rest of the game elements.
	 *
	 * Renders the remaining elements of the game such as messages, possible moves, hightlighted tiles etc.
	 */
	void RenderRest();

private:
	/**
	 * @brief Checks the status of the game.
	 *
	 * Checks the status of the game to determine if it's in a normal state, a draw, or if a player has won.
	 *
	 * @return The current status of the game.
	 */
	GameStatus CheckGameStatus();

	/**
	 * @brief Checks if a player has won.
	 *
	 * Checks if the specified player has won the game. Meaning that their Queen is surrounded on all sides;
	 *
	 * @param IDOfPlayer The ID of the player to check.
	 * @return True if the player has won, false otherwise.
	 */
	bool CheckIfPlayerWon(const int IDOfPlayer);

	/**
	 * @brief Updates possible moves on the selected tile.
	 *
	 * Updates the set of possible moves for the currently selected tile based on player input.
	 */
	void UpdatePossibleMovesOnSelectedTile();

	/**
	 * @brief Checks input in the player field.
	 *
	 * Checks for player inputs in the player field and processes them accordingly.
	 */
	void CheckInputInPlayerField();

	/**
	 * @brief Checks input in the hex map.
	 *
	 * Checks for player inputs in the hex map and processes them accordingly.
	 */
	void CheckInputInHexMap();

	/**
	 * @brief Invalidates selected tile variables.
	 *
	 * Resets the variables related to the selected tile to their default state.
	 * This variables are reseted: isPlayerTileSelected, indexOfPlayerTileSelected, pointerToSelectedTile,
	 * originalCordsOfSelectedTile.
	 */
	void InvalidateSelectedTileVariables();

	/**
	 * @brief Moves/Places the selected tile on game map.
	 *
	 * Moves or places the selected hex tileto the new position specified by the map iterator.
	 *
	 * @param mapIterator An iterator pointing to places where to put the selected tile.
	 */
	void MoveHex(hexTileMap::iterator& mapIterator);

	/**
	 * @brief Changes the turn in the game.
	 *
	 * Changes the turn to the next player in the game and checks the status of the game.
	 */
	void ChangeTurn();

	/**
	 * @brief Modifies the border of the hive based on the move of a tile.
	 *
	 * Modifies the border of the hive based on the move of a tile, considering both
	 * the present position and the original position of the modified tile.
	 *
	 * @param presentPositionOfModifiedTile The present position of the modified tile.
	 * @param originalPositionOfModifiedTile The original position of the modified tile.
	 */
	void ModifyBorderOfHive(const HexCords& presentPositionOfModifiedTile,
	                        const HexCords& originalPositionOfModifiedTile);
	/**
	 * @brief Modifies the border of the hive based on the move of a tile.
	 *
	 * Modifies the border of the hive based on the move of a tile, considering only
	 * the present position of the modified tile.
	 *
	 * @param presentPositionOfModifiedTile The present position of the modified tile.
	 */
	void ModifyBorderOfHive(const HexCords& presentPositionOfModifiedTile);

	/**
	 * @brief Erases unnecessary hex tiles from the border of the hive.
	 *
	 * Removes hex tiles from the border of the hive that are no longer needed
	 * after a modification.
	 *
	 * @param originalPositionOfModifiedTile The original position of the modified tile.
	 */
	void EraseUnnecessarydHexesFromBorder(const HexCords& originalPositionOfModifiedTile);

	/**
	 * @brief Checks if the tile have some occupied neighbors.
	 *
	 * Checks if the tile at specified cordinates have atleast 1 occupied neighbors.
	 *
	 * @param hexcords The coordinates to check.
	 * @return True if there are bug hex tile neighbors, false otherwise.
	 */
	bool HasSomeBugHexNeighbor(const HexCords& hexcords);

	/**
	 * @brief Generates the empty base map layout for the game.
	 *
	 * @return The generated base map.
	 */
	hexTileMap GenerateBaseMap();

	/**
	 * @brief Finds the iterator of the hex tile under the cursor.
	 *
	 * Finds the iterator of the hex tile under the cursor in the game map.
	 *
	 * @return The iterator of the hex tile under the cursor.
	 */
	hexTileMap::iterator FindIteratorOfHexUnderCursor();

	Renderer renderer = Renderer(); /**< The renderer object for rendering graphics. */
	hexTileMap gameMap;             /**< The game map representing hex tiles. */

	possibleMovesSet borderOfHive;                /**< The set of hex tiles representing the border of the hive. */
	possibleMovesSet possibleMovesOfSelectedTile; /**< The set of possible moves for the currently selected tile. */

	int hexagonHorizontalCount = 0; /**< The number of hexagons horizontally on the game map. */

	// Selected tile variables
	bool isPlayerTileSelected = false; /**< Flag indicating whether a player tile is currently selected. */
	int indexOfPlayerTileSelected = 0; /**< The index of the player tile currently selected. */

	std::shared_ptr<BaseBugTile> pointerToSelectedTile = nullptr; /**< A pointer to the currently selected tile. */
	HexCords originalCordsOfSelectedTile = {
		std::numeric_limits<int>::max(), std::numeric_limits<int>::max()
	}; /**< The original coordinates of the selected tile. */

	// If (turn = 3 || turn = 4 ) <=> I am stupid and left it set after debugging :]
	int turn = 0; /**< The current turn in the game. */
	int idOfPlayerOnTurn = 0; /**< The ID of the player currently on turn. */
	int startingPlayer = 0; /**< The ID of the starting player. */
	Player players[2] = { Player(0), Player(1) }; /**< An array containing the players in the game. */
	bool gameInterupted = false; /**< Flag indicating whether the game has been interrupted. */
	std::string messageToDisplay = ""; /**< The message to display in the game. */
};
#endif