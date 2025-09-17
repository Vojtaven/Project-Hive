/**
 * @file hexUtilities.h
 * @author Vojtech Venzara (Vojtaven@seznam.cz)
 * @brief Conteins useful function to wotk with hex
 * @version 0.1
 * @date 2024-04-11
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef hexUtilities_H
#define hexUtilities_H

#include "common.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <iostream>
#include <map>

/**
 * @struct HexCords
 * @brief Represents coordinates in a hexagonal grid.
 * For cordinates we use the axial representation
 */
struct HexCords {
	int q; /**< Q component*/
	int r; /**< R component*/

	/**
	 * @brief Less-than comparison operator.
	 * @param other The other HexCords object to compare against.
	 * @return True if this object is less than the other object, false otherwise.
	 */
	bool operator<(const HexCords& other) const;

	/**
	 * @brief Addition operator.
	 * @param other The other HexCords object to add.
	 * @return The result of the addition.
	 */
	HexCords operator+(const HexCords& other) const;

	/**
	 * @brief Compound addition operator.
	 * @param other The other HexCords object to add.
	 * @return Reference to the modified HexCords object.
	 */
	HexCords operator+=(const HexCords& other);
};

// Type Aliases
class BaseBugTile;

/**
 * @typedef hexTileMap
 * @brief A map of HexCords to shared pointers to BaseBugTile.
 */
using hexTileMap = std::map<HexCords, std::shared_ptr<BaseBugTile>>;

/**
 * @typedef possibleMovesSet
 * @brief A set of HexCords representing possible moves.
 */
using possibleMovesSet = std::set<HexCords>;

/**
 * @brief Array of axial direction vectors.
 *
 * Represents the axial direction vectors used in hexagonal grids.
 * Each vector consists of two integers representing the q and r coordinates respectively.
 * The array size is determined by the SIZE_OF_AXIAL_VECTORS constant.
 */
constexpr HexCords AXIAL_DIRECTION_VECTORS[SIZE_OF_AXIAL_VECTORS] = {
	{ -1, 1 }, /**< Bottom-left direction */
	{ 1, -1 }, /**< Top-right direction */
	{ 1, 0 },  /**< Right direction */
	{ -1, 0 }, /**< Left direction */
	{ 0, -1 }, /**< Top-left direction */
	{ 0, 1 }   /**< Bottom-right direction */
};

/**
 * @brief Get the neighbors of a tile.
 *
 * Retrieves the neighboring tiles of the specified tile.
 *
 * @param gameMap The game map represented as a hexTileMap.
 * @param tile The coordinates of the tile for which neighbors are to be retrieved.
 * @return A set of HexCords representing the neighboring tiles of the specified tile.
 */
possibleMovesSet GetNeighborsOfTile(const hexTileMap& gameMap, const HexCords& tile);

/**
 * @brief Get the emty neighbors of a tile.
 *
 * Retrieves the neighboring tiles of the specified tile, that don't have any Bug piece on them.
 *
 * @param gameMap The game map represented as a hexTileMap.
 * @param tile The coordinates of the tile for which neighbors are to be retrieved.
 * @return A set of HexCords representing the empty neighboring tiles of the specified tile.
 */
possibleMovesSet GetEmptyNeighborsOfTile(const hexTileMap& gameMap, const HexCords& tile);

/**
 * @brief Get the occupied neighbors of a tile.
 *
 * Retrieves the neighboring tiles of the specified tile, that have Bug piece on them.
 *
 * @param gameMap The game map represented as a hexTileMap.
 * @param tile The coordinates of the tile for which neighbors are to be retrieved.
 * @return A set of HexCords representing the occupied neighboring tiles of the specified tile.
 */
possibleMovesSet GetOccupiedNeighborsOfTile(const hexTileMap& gameMap, const HexCords& tile);

/**
 * @brief Rounds the fractional cube coordinates to the nearest cube coordinates.
 *
 * @param frac The fractional cube coordinates.
 * @return The rounded cube coordinates.
 */
Vector3 Cube_round(Vector3 frac);

/**
 * @brief Converts cube coordinates to axial coordinates.
 *
 * @param cube The cube coordinates.
 * @return The corresponding axial coordinates.
 */
Vector2 CubeToAxial(Vector3 cube);

/**
 * @brief Converts axial coordinates to cube coordinates.
 *
 * @param axial The axial coordinates.
 * @return The corresponding cube coordinates.
 */
Vector3 AxialToCube(Vector2 axial);

/**
 * @brief Rounds the fractional axial coordinates to the nearest axial coordinates.
 *
 * @param axialFrac The fractional axial coordinates.
 * @return The axial coordinates rounded to the nearest axial.
 */
Vector2 RoundAxial(Vector2 axialFrac);

#endif