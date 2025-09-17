/**
 * @file Player.h
 * @author Vojtech Venzara (Vojtaven@seznam.cz)
 * @brief Contains definition for player class
 * @version 0.1
 * @date 2024-04-11
 *
 * @copyright Copyright (c) 2024
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "bugTiles.h"
#include "common.h"
#include "hexUtilities.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

/**
 * @brief Type alias for player piece and his amount, that player has
 */
using playerPiece = std::pair<bugType, int>;

class Player {
public:
	/**
	 * @brief Construct a new Player object
	 */
	Player() = default;

	Player(int playerId) : playerId(playerId) {}

	/**
	 * @brief Get the Name of player
	 *
	 * @return const std::string
	 */
	const std::string& GetName() const { return name; }

	/**
	 * @brief Set the Name of player
	 *
	 * @param name
	 */
	void SetName(const std::string& name) { this->name = name; }

	/**
	 * @brief Get the Player Avaible pieces
	 *
	 * @return const std::vector<playerPiece>
	 */
	const std::array<playerPiece, DIFFERENT_PIECES_COUNT>& GetPlayerAvaiblepieces() const { return avaiblePlayerpieces; }

	/**
	 * @brief Modifie count of piece at index by amount
	 *
	 * @param index
	 * @param ammunt
	 */
	void ModifieAvaiblePiecesCount(const int& index, const int& amount) { avaiblePlayerpieces[index].second += amount; }
	/**
	 * @brief Check if player has placed queen
	 *
	 * @return true If player has placed queen
	 * @return false If player has not placed queen
	 */
	bool HasPlacedQueen() { return avaiblePlayerpieces[0].second == 0; }

	const TileData* const GetTileDataAtIndex(int index) const { return &(bugTileData[index]); }

private:
	const int playerId = std::numeric_limits<int>::max();
	/**
	 * @brief Initializes the name variable
	 */
	std::string name = "";

	/**
	 * @brief Initializes the vector of available player pieces.
	 *
	 * This initializes the vector `avaiblePlayerpieces` with specific numbers of different player pieces.
	 *
	 * The initialization includes:
	 * - 1 Queen Bee
	 * - 2 Spiders
	 * - 2 Beetles
	 * - 3 Grasshoppers
	 * - 3 Soldier Ants
	 */
	std::array<playerPiece, DIFFERENT_PIECES_COUNT> avaiblePlayerpieces = { { { bugType::QUEEN_BEE, 1 },
		                                                                      { bugType::SPIDER, 2 },
		                                                                      { bugType::BEETLE, 2 },
		                                                                      { bugType::GRASS_HOPPER, 3 },
		                                                                      { bugType::SOLDIER_ANT, 3 } } };

	const std::array<TileData, DIFFERENT_PIECES_COUNT> bugTileData = {
		{ { bugType::QUEEN_BEE, QUEEN_BEE_COLOR, playerId },
		  { bugType::SPIDER, SPIDER_COLOR, playerId },
		  { bugType::BEETLE, BEETLE_COLOR, playerId },
		  { bugType::GRASS_HOPPER, GRASS_HOPPER_COLOR, playerId },
		  { bugType::SOLDIER_ANT, SOLDIER_ANT_COLOR, playerId } }
	};
};

#endif  // !PLAYER_H