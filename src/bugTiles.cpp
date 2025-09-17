#include "bugTiles.h"
#include "common.h"
#include "hexUtilities.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <vector>

possibleMovesSet BaseBugTile::Place(const hexTileMap& gameMap, const possibleMovesSet& possibleGeneralMoves,
                                    const int IDOfPlayer, const bool isZeroTurn) {
	if (isZeroTurn) {
		return possibleGeneralMoves;
	} else {
		return SpacesSurrondedByTheirColor(gameMap, possibleGeneralMoves, IDOfPlayer);
	}
}
possibleMovesSet BaseBugTile::RemovePossibleMovesAroundTile(const hexTileMap& gameMap,
                                                            const possibleMovesSet& possibleMoves,
                                                            const HexCords& tile) {
	auto neighbors = GetEmptyNeighborsOfTile(gameMap, tile);

	auto result = possibleMovesSet(possibleMoves);
	for (auto iter = neighbors.begin(); iter != neighbors.end();) {
		if (possibleMoves.contains(*iter)) {
			auto temp = GetOccupiedNeighborsOfTile(gameMap, *iter);
			if (temp.size() <= 1) {
				result.erase(*iter);
			}
			iter++;
		} else {
			neighbors.erase(iter);
		}
	}

	return result;
}
possibleMovesSet& BaseBugTile::RemovePossibleMovesAroundTile(const hexTileMap& gameMap, possibleMovesSet& possibleMoves,
                                                             const HexCords& tile) {
	auto neighbors = GetEmptyNeighborsOfTile(gameMap, tile);

	for (auto iter = neighbors.begin(); iter != neighbors.end();) {
		if (possibleMoves.contains(*iter)) {
			auto temp = GetOccupiedNeighborsOfTile(gameMap, *iter);
			if (temp.size() <= 1) {
				possibleMoves.erase(*iter);
			}
			iter++;
		} else {
			neighbors.erase(iter);
		}
	}

	return possibleMoves;
}

possibleMovesSet BaseBugTile::SpacesSurrondedByTheirColor(const hexTileMap& gameMap,
                                                          const possibleMovesSet& possibleGeneralMoves,
                                                          const int IDOfPlayer) {
	possibleMovesSet result;
	for (const auto& move : possibleGeneralMoves) {
		auto neighbors = GetOccupiedNeighborsOfTile(gameMap, move);
		for (const auto& neighbor : neighbors) {
			auto tile = gameMap.find(neighbor);
			if (tile != gameMap.end() && tile->second->GetPlayerID() == IDOfPlayer) {
				result.insert(move);
				break;
			}
		}
	}
	return result;
}

bool BaseBugTile::CheckIntegrityOfHiveWithoutOneTile(hexTileMap gameMap, const HexCords& cordsOfRemovedHex) {
	bool result = false;
	auto it = gameMap.find(cordsOfRemovedHex);
	possibleMovesSet queeToProcess;
	HexCords someElement;

	if (it != gameMap.end()) {
		it->second = nullptr;
		auto neighbors = GetOccupiedNeighborsOfTile(gameMap, cordsOfRemovedHex);
		if (!neighbors.empty()) {
			someElement = *neighbors.begin();
			queeToProcess.insert(someElement);
		}
		while (!queeToProcess.empty()) {
			someElement = *queeToProcess.begin();
			gameMap[someElement] = nullptr;
			queeToProcess.erase(queeToProcess.begin());

			neighbors = GetOccupiedNeighborsOfTile(gameMap, someElement);
			queeToProcess.merge(neighbors);
		}

		result = IsMapClear(gameMap);
	}

	return result;
}

bool BaseBugTile::IsMapClear(const hexTileMap& gameMap) const {
	for (const auto& temp : gameMap) {
		if (temp.second != nullptr) {
			return false;
		}
	}
	return true;
}

bool BaseBugTile::FreedomToMove(const hexTileMap& gameMap, const HexCords& cordsOfRemovedHex) {
	return !IsSpaceSurrounded(gameMap, cordsOfRemovedHex);
}

bool BaseBugTile::IsSpaceSurrounded(const hexTileMap& gameMap, const HexCords& cordsOfHex) {
	auto neighbors = GetOccupiedNeighborsOfTile(gameMap, cordsOfHex);
	return neighbors.size() > 4;
}

// QueenBee
QueenBee::QueenBee(const TileData* const tileData) : BaseBugTile(tileData) {}

possibleMovesSet QueenBee::Move(const hexTileMap& gameMap, const possibleMovesSet& possibleGeneralMoves,
                                const HexCords& originalCords) {
	if (!CheckIntegrityOfHiveWithoutOneTile(gameMap, originalCords) || !FreedomToMove(gameMap, originalCords)) {
		return possibleMovesSet();
	}
	possibleMovesSet result;

	auto possibleMoves = RemovePossibleMovesAroundTile(gameMap, possibleGeneralMoves, originalCords);
	auto neigbors = GetEmptyNeighborsOfTile(gameMap, originalCords);

	for (const auto& item : neigbors) {
		if (possibleGeneralMoves.contains(item)) {
			result.insert(item);
		}
	}

	RemovePossibleMovesAroundTile(gameMap, result, originalCords);

	return result;
}

// Spider
Spider::Spider(const TileData* const tileData) : BaseBugTile(tileData) {}

possibleMovesSet Spider::Move(const hexTileMap& gameMap, const possibleMovesSet& possibleGeneralMoves,
                              const HexCords& originalCords) {
	if (!CheckIntegrityOfHiveWithoutOneTile(gameMap, originalCords) || !FreedomToMove(gameMap, originalCords)) {
		return possibleMovesSet();
	}
	auto result = FindPositionThreeSpacesFromOrigin(gameMap, possibleGeneralMoves, originalCords);
	// Placeholder
	return result;
}

possibleMovesSet Spider::FindPositionThreeSpacesFromOrigin(const hexTileMap& gameMap,
                                                           const possibleMovesSet& possibleGeneralMoves,
                                                           const HexCords& originalCords) {
	std::queue<std::pair<HexCords, int>> queue;
	possibleMovesSet result;
	std::set<HexCords> visited;
	// Origin
	queue.emplace(originalCords, 0);

	while (!queue.empty()) {
		auto itemToProcess = queue.front();
		visited.insert(itemToProcess.first);

		auto neighbors = GetEmptyNeighborsOfTile(gameMap, itemToProcess.first);
		for (const auto& neighbor : neighbors) {
			if (!visited.contains(neighbor) && possibleGeneralMoves.contains(neighbor)) {
				visited.insert(neighbor);
				if (itemToProcess.second == 2) {
					result.insert(neighbor);
				} else {
					queue.emplace(neighbor, itemToProcess.second + 1);
				}
			}
		}

		queue.pop();
	}

	return result;
}

// Beetle
Beetle::Beetle(const TileData* const tileData) : BaseBugTile(tileData) {}

possibleMovesSet Beetle::Move(const hexTileMap& gameMap, const possibleMovesSet& possibleGeneralMoves,
                              const HexCords& originalCords) {
	if (bugTileUnderBeetle == nullptr && !CheckIntegrityOfHiveWithoutOneTile(gameMap, originalCords)) {
		return possibleMovesSet();
	}

	auto neighbors = GetNeighborsOfTile(gameMap, originalCords);
	if (bugTileUnderBeetle == nullptr) {
		RemovePossibleMovesAroundTile(gameMap, neighbors, originalCords);
	}

	return neighbors;
}

// GrassHopper
GrassHopper::GrassHopper(const TileData* const tileData)
    : BaseBugTile(tileData) {}

possibleMovesSet GrassHopper::Move(const hexTileMap& gameMap, const possibleMovesSet& possibleGeneralMoves,
                                   const HexCords& originalCords) {
	if (!CheckIntegrityOfHiveWithoutOneTile(gameMap, originalCords)) {
		return possibleMovesSet();
	}

	possibleMovesSet result;
	auto tempPosition = originalCords;
	bool moved = false;

	for (const auto& vector : AXIAL_DIRECTION_VECTORS) {
		tempPosition = originalCords + vector;
		moved = false;
		auto it = gameMap.find(tempPosition);
		while (it != gameMap.end() && it->second != nullptr) {
			tempPosition += vector;
			it = gameMap.find(tempPosition);
			moved = true;
		}

		if (moved) {
			result.insert(tempPosition);
		}
	}
	return result;
}
// SolfierAnt
SoldierAnt::SoldierAnt(const TileData* const tileData)
    : BaseBugTile(tileData) {}

possibleMovesSet SoldierAnt::Move(const hexTileMap& gameMap, const possibleMovesSet& possibleGeneralMoves,
                                  const HexCords& originalCords) {
	if (!CheckIntegrityOfHiveWithoutOneTile(gameMap, originalCords) || !FreedomToMove(gameMap, originalCords)) {
		return possibleMovesSet();
	}

	possibleMovesSet result;
	for (const auto& hex : possibleGeneralMoves) {
		if (!IsSpaceSurrounded(gameMap, hex)) {
			result.insert(hex);
		}
	}

	return result;
}