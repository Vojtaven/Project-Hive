/**
 * @file bugTiles.h
 * @author Vojtech Venzara (Vojtaven@seznam.cz)
 * @brief Contains classes for bug pieces and their functions
 * @version 0.1
 * @date 2024-04-11
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef BUG_TILES_H
#define BUG_TILES_H

#include "common.h"
#include "hexUtilities.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

/**
 * @brief Enumeration representing types of bugs in the game.
 */
enum class bugType {
	QUEEN_BEE,   /**< Represents the Queen Bee bug type. */
	BEETLE,      /**< Represents the Beetle bug type. */
	SOLDIER_ANT, /**< Represents the Soldier Ant bug type. */
	SPIDER,      /**< Represents the Spider bug type. */
	GRASS_HOPPER /**< Represents the Grasshopper bug type. */
};


struct TileData {
	TileData(bugType type, Color bugColor, int playerId) : type(type), bugColor(bugColor), playerId(playerId) {}
	bugType type;
	Color bugColor;
	int playerId;
};

/**
 * @brief Base class representing a bug tile in the game.
 *
 * This class provides a base representation for a bug tile in the game. It includes methods
 * for accessing properties of the bug tile and defining common behavior for all bug types.
 */
class BaseBugTile {
public:
	/**
	 * @brief Constructs a BaseBugTile object.
	 *
	 * Constructs a BaseBugTile object with the specified player ID, color, and bug type.
	 *
	 * @param playerId The ID of the player owning the bug tile.
	 * @param color The color of the bug tile.
	 * @param type The type of bug represented by the tile.
	 */
	BaseBugTile(const TileData* const tileData)
	    : tileData(tileData){}

	/**
	 * @brief Deleted default constructor.
	 *
	 * The default constructor is deleted to prevent instantiation without required parameters.
	 */
	BaseBugTile() = delete;

	/**
	 * @brief Get the Bug Color object
	 *
	 * @return const Color&
	 */
	const Color& GetBugColor() const { return tileData->bugColor; }

	/**
	 * @brief Get the Player ID object
	 *
	 * @return const int&
	 */
	const int& GetPlayerID() const { return tileData->playerId; }

	/**
	 * @brief Get the Bug Type object
	 *
	 * @return const bugType&
	 */
	const bugType& GetBugType() const { return tileData->type; }

	/**
	 * @brief Define rules for moving the bug tile.
	 *
	 * This pure virtual function must be implemented by derived classes to define
	 * the specific movement rules of the bug tile.
	 * Base rules for all tiles is: The removal must not violate the integrity of the hive.
	 *
	 * @param gameMap The game map represented as a hexTileMap.
	 * @param possibleGeneralMoves The set of possible general moves.
	 * @param originalCords The original coordinates of the bug tile.
	 * @return A set of HexCords representing the possible moves for the tile.
	 */
	virtual possibleMovesSet Move(const hexTileMap& gameMap, const possibleMovesSet& possibleGeneralMoves,
	                              const HexCords& originalCords) = 0;

	/**
	 * @brief Define rules for placing the bug tile.
	 *
	 * This function return possible places to places the bug tile. It is called when bug tile is placed from player
	 * hand.
	 *
	 * @param gameMap The game map represented as a hexTileMap.
	 * @param possibleGeneralMoves The set of possible general moves.
	 * @param IDOfPlayer The ID of the player placing the bug tile.
	 * @param isZeroTurn Flag indicating if it's the zero turn (optional, defaults to false). If set to true rule about
	 * placing near bug tile with same color doesn't apply
	 * @return A set of HexCords representing the possible places to place the tile
	 */
	possibleMovesSet Place(const hexTileMap& gameMap, const possibleMovesSet& possibleGeneralMoves,
	                       const int IDOfPlayer, const bool isZeroTurn = false);

	/**
	 * @brief Virtual destructor for BaseBugTile.
	 *
	 * Defines a virtual destructor to allow proper cleanup in derived classes.
	 */
	virtual ~BaseBugTile(){};

protected:
	/**
	 * @brief Retrieves tiles with atleast one neighbor of same color.
	 *
	 * Retrieves tiles from possibleGeneralMoves with atleast one neighbor, that have same ID as IDOfPlayer.
	 *
	 * @param gameMap The game map represented as a hexTileMap.
	 * @param possibleGeneralMoves The set of possible moves.
	 * @param IDOfPlayer The ID of the player.
	 * @return A set of HexCords with atleast one neighbor of same color.
	 */
	possibleMovesSet SpacesSurrondedByTheirColor(const hexTileMap& gameMap,
	                                             const possibleMovesSet& possibleGeneralMoves, const int IDOfPlayer);

	/**
	 * @brief Checks the integrity of the hive without one tile.
	 *
	 * This function checks the integrity of the hive without considering one specific tile.
	 *
	 * @param map The game map represented as a hexTileMap.
	 * @param cordsOfRemovedHex The coordinates of the removed hex tile. Passed as copy intentionally.
	 * @return True if the hive remains intact after removing the specified tile, false otherwise.
	 */
	bool CheckIntegrityOfHiveWithoutOneTile(hexTileMap map, const HexCords& cordsOfRemovedHex);

	/**
	 * @brief Checks if the bug tile has freedom to move.
	 *
	 * This function checks if the bug tile isn't surrounded.
	 *
	 * @param gameMap The game map represented as a hexTileMap.
	 * @param cordsOfRemovedHex The coordinates of the bug tile.
	 * @return True if the bug tile has freedom to move, false otherwise.
	 */

	bool FreedomToMove(const hexTileMap& gameMap, const HexCords& cordsOfRemovedHex);

	/**
	 * @brief Checks if a tile is surrounded by tiles.
	 *
	 * This function checks if a space on the game map is surrounded by atleast 5 tiles.
	 *
	 * @param gameMap The game map represented as a hexTileMap.
	 * @param cordsOfHex The coordinates of the space.
	 * @return True if the space is surrounded by tiles, false otherwise.
	 */
	bool IsSpaceSurrounded(const hexTileMap& gameMap, const HexCords& cordsOfHex);

	/**
	 * @brief Removes possible moves around a tile.
	 *
	 * This function removes possible moves around a specified tile from a set of possible moves.
	 *
	 * @param gameMap The game map represented as a hexTileMap.
	 * @param possibleMoves The set of possible moves passed as const reference.
	 * @param tile The coordinates of the tile.
	 * @return A set of HexCords representing the possible moves with moves around the tile removed.
	 */
	possibleMovesSet RemovePossibleMovesAroundTile(const hexTileMap& gameMap, const possibleMovesSet& possibleMoves,
	                                               const HexCords& tile);

	/**
	 * @brief Removes possible moves around a tile.
	 *
	 * This function removes possible moves around a specified tile from a set of possible moves.
	 *
	 * @param gameMap The game map represented as a hexTileMap.
	 * @param possibleMoves The set of possible moves passed as reference.
	 * @param tile The coordinates of the tile.
	 * @return A reference to the modified set of possible moves with moves around the tile removed.
	 */
	possibleMovesSet& RemovePossibleMovesAroundTile(const hexTileMap& gameMap, possibleMovesSet& possibleMoves,
	                                                const HexCords& tile);

	/**
	 * @brief Checks if the game map is clear.
	 *
	 * This function checks if the game map is clear, i.e., no bugs are present on the map.
	 *
	 * @param gameMap The game map represented as a hexTileMap.
	 * @return True if the game map is clear, false otherwise.
	 */
	bool IsMapClear(const hexTileMap& gameMap) const;


	const TileData* const tileData = nullptr;


	//bugType type;           /**< The type of bug represented by the tile. */
	//Color bugColor = PINK;  /**< The color of the bug tile. */
	//int playerId = INT_MAX; /**< The ID of the player owning the bug tile. */
};


/**
 * @brief Represents a Queen Bee bug tile in the game.
 *
 * This class inherits from BaseBugTile and represents a Queen Bee bug tile in the game.
 * It provides specific behavior for Queen Bee tiles.
 */
class QueenBee : public BaseBugTile {
public:
	/**
	 * @brief Constructs a QueenBee object.
	 *
	 * Constructs a QueenBee object with the specified player ID, color, and type.
	 *
	 * @param playerId The ID of the player owning the Queen Bee tile.
	 * @param color The color of the Queen Bee tile (optional, defaults to QUEEN_BEE_COLOR).
	 * @param type The type of bug represented by the tile (optional, defaults to bugType::QUEEN_BEE).
	 */
	QueenBee(const TileData* const tileData);

	/**
	 * @brief Default destructor for QueenBee.
	 *
	 * Defines a default destructor for QueenBee objects.
	 */
	~QueenBee() = default;

	/**
	 * @brief Define rules for moving the Queen Bee tile.
	 *
	 * Implements the movement rules for Queen Bee tile.
	 * Aditional rules for Queen bee are: Freedom to move, only 1 space per turn,
	 *
	 * @param gameMap The game map represented as a hexTileMap.
	 * @param possibleGeneralMoves The set of possible general moves.
	 * @param originalCords The original coordinates of the Queen Bee tile.
	 * @return A set of HexCords representing the possible moves after the rules were applied.
	 */

	possibleMovesSet Move(const hexTileMap& gameMap, const possibleMovesSet& possibleGeneralMoves,
	                      const HexCords& originalCords);
};
class Spider : public BaseBugTile {
public:
	/**
	 * @brief Constructs a Spider object.
	 *
	 * Constructs a Spider object with the specified player ID, color, and type.
	 *
	 * @param playerId The ID of the player owning the Spider tile.
	 * @param color The color of the Spider tile (optional, defaults to SPIDER_COLOR).
	 * @param type The type of bug represented by the tile (optional, defaults to bugType::SPIDER).
	 */
	Spider(const TileData* const tileData);

	/**
	 * @brief Default destructor for Spider.
	 *
	 * Defines a default destructor for Spider objects.
	 */
	~Spider() = default;

	/**
	 * @brief Define rules for moving the Spider tile.
	 *
	 * Implements the movement rules for Spider tile.
	 * Aditional rules for Spider are: Freedom to move, only 3 spaces per turn without backtracing.
	 *
	 * @param gameMap The game map represented as a hexTileMap.
	 * @param possibleGeneralMoves The set of possible general moves.
	 * @param originalCords The original coordinates of the Spider tile.
	 * @return A set of HexCords representing the possible moves after the rules were applied.
	 */
	possibleMovesSet Move(const hexTileMap& gameMap, const possibleMovesSet& possibleGeneralMoves,
	                      const HexCords& originalCords);

private:
	/**
	 * @brief Finds positions three tiles away from the origin tile.
	 *
	 * This function searches for positions on the game map that are three tiles away from the original tile and the
	 * path must only contain tiles from possibleGeneralMoves.
	 *
	 * @param gameMap The game map represented as a hexTileMap.
	 * @param possibleGeneralMoves The set of possible general moves.
	 * @param originalCords The coordinates of the original tile.
	 * @return A set of HexCords representing positions three tiles away from the original tile.
	 */
	possibleMovesSet FindPositionThreeSpacesFromOrigin(const hexTileMap& gameMap,
	                                                   const possibleMovesSet& possibleGeneralMoves,
	                                                   const HexCords& originalCords);
};

class Beetle : public BaseBugTile {
public:
	/**
	 * @brief Constructs a Beetle object.
	 *
	 * Constructs a Beetle object with the specified player ID, color, and type.
	 *
	 * @param playerId The ID of the player owning the Beetle tile.
	 * @param color The color of the Beetle tile (optional, defaults to BEETLE_COLOR).
	 * @param type The type of bug represented by the tile (optional, defaults to bugType::BEETLE).
	 */
	Beetle(const TileData* const tileData);

	/**
	 * @brief Default destructor for Beetle.
	 *
	 * Defines a default destructor for Beetle objects.
	 */
	~Beetle() = default;

	/**
	 * @brief Define rules for moving the Beetle tile.
	 *
	 * Implements the movement rules for Beetle tile.
	 * Aditional rules for Beetle are: Only 1 spaces per turn and can land on other occupied tiles.
	 *
	 * @param gameMap The game map represented as a hexTileMap.
	 * @param possibleGeneralMoves The set of possible general moves.
	 * @param originalCords The original coordinates of the Beetle tile.
	 * @return A set of HexCords representing the possible moves after the rules were applied.
	 */
	possibleMovesSet Move(const hexTileMap& gameMap, const possibleMovesSet& possibleGeneralMoves,
	                      const HexCords& originalCords);

	/**
	 * @brief Sets the tile under the Beetle.
	 *
	 * Sets the tile under the Beetle to the specified tile.
	 *
	 * @param tile A shared pointer to the tile under the Beetle.
	 */
	void SetTileUnderBeetle(std::shared_ptr<BaseBugTile> tile) { bugTileUnderBeetle = tile; }

	/**
	 * @brief Gets the tile under the Beetle.
	 *
	 * Retrieves the tile under the Beetle.
	 *
	 * @return A shared pointer to the tile under the Beetle.
	 */
	std::shared_ptr<BaseBugTile> GetTileUnderBeetle() { return bugTileUnderBeetle; }

private:
	std::shared_ptr<BaseBugTile> bugTileUnderBeetle = nullptr; /**> Pointer to tile under beetle*/
};
class GrassHopper : public BaseBugTile {
public:
	/**
	 * @brief Constructs a GrassHopper object.
	 *
	 * Constructs a GrassHopper object with the specified player ID, color, and type.
	 *
	 * @param playerId The ID of the player owning the Grass Hopper tile.
	 * @param color The color of the Grass Hopper tile (optional, defaults to GRASS_HOPPER_COLOR).
	 * @param type The type of bug represented by the tile (optional, defaults to bugType::GRASS_HOPPER).
	 */
	GrassHopper(const TileData* const tileData);

	/**
	 * @brief Default destructor for Grass Hopper.
	 *
	 * Defines a default destructor for GrassHopper objects.
	 */
	~GrassHopper() = default;

	/**
	 * @brief Define rules for moving the Grass Hopper tile.
	 *
	 * Implements the movement rules for Grass Hopper tile.
	 * Aditional rules for Grass Hopper are: Any number of spaces per turn, but must jump in straight line over occupied
	 * tiles
	 *
	 * @param gameMap The game map represented as a hexTileMap.
	 * @param possibleGeneralMoves The set of possible general moves.
	 * @param originalCords The original coordinates of the Grass Hopper tile.
	 * @return A set of HexCords representing the possible moves after the rules were applied.
	 */
	possibleMovesSet Move(const hexTileMap& gameMap, const possibleMovesSet& possibleGeneralMoves,
	                      const HexCords& originalCords);
};
class SoldierAnt : public BaseBugTile {
public:
	/**
	 * @brief Constructs a SoldierAnt object.
	 *
	 * Constructs a SoldierAnt object with the specified player ID, color, and type.
	 *
	 * @param playerId The ID of the player owning the Soldier Ant tile.
	 * @param color The color of the Soldier Ant tile (optional, defaults to SOLDIER_ANT_COLOR).
	 * @param type The type of bug represented by the tile (optional, defaults to bugType::SOLDIER_ANT).
	 */
	SoldierAnt(const TileData* const tileData);

	/**
	 * @brief Default destructor for Soldier Ant.
	 *
	 * Defines a default destructor for SoldierAnt objects.
	 */
	~SoldierAnt() = default;

	/**
	 * @brief Define rules for moving the Soldier Ant tile.
	 *
	 * Implements the movement rules for Soldier Ant tile.
	 * Aditional rules for Soldier Ant are: Freedom to move and can move any number of tiles around hive.
	 *
	 * @param gameMap The game map represented as a hexTileMap.
	 * @param possibleGeneralMoves The set of possible general moves.
	 * @param originalCords The original coordinates of the Soldier Ant tile.
	 * @return A set of HexCords representing the possible moves after the rules were applied.
	 */
	possibleMovesSet Move(const hexTileMap& gameMap, const possibleMovesSet& possibleGeneralMoves,
	                      const HexCords& originalCords);
};

#endif  // !BUG_TILES_H
