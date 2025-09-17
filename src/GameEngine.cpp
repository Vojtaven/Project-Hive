#include "GameEngine.h"
#include "hexUtilities.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <algorithm>
#include <format>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

GameEngine::GameEngine() {
	hexagonHorizontalCount = renderer.GetHexagonHorizontalCount();
	gameMap = GenerateBaseMap();
	PlayerNameConfiguration();

	// Center of board is the first possible move
	borderOfHive.emplace(hexagonHorizontalCount / 2, (HEXAGON_VERTICAL_COUNT - hexagonHorizontalCount / 2) / 2);
}
void GameEngine::CheckInputs() {
	if (!gameInterupted) {
		if (IsMouseButtonPressed(0)) {
			if (renderer.IsMouseInPlayersFields()) {
				CheckInputInPlayerField();
			} else {
				CheckInputInHexMap();
			}
			UpdatePossibleMovesOnSelectedTile();
		}
	}
}

void GameEngine::CheckInputInPlayerField() {
	indexOfPlayerTileSelected = renderer.GetIndexOfHexInPlayerField(idOfPlayerOnTurn);
	isPlayerTileSelected = indexOfPlayerTileSelected != -1;

	if (isPlayerTileSelected) {
		auto playerAvaiblePieces = players[idOfPlayerOnTurn].GetPlayerAvaiblepieces();
		auto selectedPiece = playerAvaiblePieces[indexOfPlayerTileSelected];
		if (selectedPiece.second > 0 &&
		    (turn != 4 || indexOfPlayerTileSelected == 0 || players[idOfPlayerOnTurn].HasPlacedQueen())) {
			switch (selectedPiece.first) {
				case bugType::QUEEN_BEE:
					pointerToSelectedTile =
					    std::make_shared<QueenBee>(players[idOfPlayerOnTurn].GetTileDataAtIndex(indexOfPlayerTileSelected));
					break;
				case bugType::BEETLE:
					pointerToSelectedTile =
					    std::make_shared<Beetle>(players[idOfPlayerOnTurn].GetTileDataAtIndex(indexOfPlayerTileSelected));
					break;
				case bugType::SOLDIER_ANT:
					pointerToSelectedTile = std::make_shared<SoldierAnt>(
					    players[idOfPlayerOnTurn].GetTileDataAtIndex(indexOfPlayerTileSelected));
					break;
				case bugType::SPIDER:
					pointerToSelectedTile =
					    std::make_shared<Spider>(players[idOfPlayerOnTurn].GetTileDataAtIndex(indexOfPlayerTileSelected));
					break;
				case bugType::GRASS_HOPPER:
					pointerToSelectedTile = std::make_shared<GrassHopper>(
					    players[idOfPlayerOnTurn].GetTileDataAtIndex(indexOfPlayerTileSelected));
					break;
			}
		} else {
			InvalidateSelectedTileVariables();
		}
	} else {
		InvalidateSelectedTileVariables();
	}
}

void GameEngine::CheckInputInHexMap() {
	auto tempIterator = FindIteratorOfHexUnderCursor();
	if (tempIterator != gameMap.end()) {
		if (possibleMovesOfSelectedTile.contains(tempIterator->first) && pointerToSelectedTile != nullptr) {
			MoveHex(tempIterator);
		} else if (tempIterator->second != nullptr && tempIterator->second->GetPlayerID() == idOfPlayerOnTurn) {
			InvalidateSelectedTileVariables();
			if (turn != 4 || players[idOfPlayerOnTurn].HasPlacedQueen()) {
				pointerToSelectedTile = tempIterator->second;
				originalCordsOfSelectedTile = tempIterator->first;
			}
		}
	} else {
		InvalidateSelectedTileVariables();
	}
}

void GameEngine::MoveHex(hexTileMap::iterator& mapIterator) {
	if (isPlayerTileSelected) {
		mapIterator->second = pointerToSelectedTile;
		players[idOfPlayerOnTurn].ModifieAvaiblePiecesCount(indexOfPlayerTileSelected, -1);
		ModifyBorderOfHive(mapIterator->first);
	} else {
		auto tempIt = gameMap.find(originalCordsOfSelectedTile);
		if (tempIt != gameMap.end()) {
			if (auto beetle = dynamic_pointer_cast<Beetle>(pointerToSelectedTile)) {
				tempIt->second = beetle->GetTileUnderBeetle();
				beetle->SetTileUnderBeetle(mapIterator->second);
			} else {
				tempIt->second = nullptr;
			}
		}
		mapIterator->second = pointerToSelectedTile;

		ModifyBorderOfHive(mapIterator->first, originalCordsOfSelectedTile);
	}
	InvalidateSelectedTileVariables();
	ChangeTurn();
}

void GameEngine::ChangeTurn() {
	switch (CheckGameStatus()) {
		case GameStatus::NORMAL:
			if (startingPlayer != idOfPlayerOnTurn) {
				turn++;
			}
			idOfPlayerOnTurn = (idOfPlayerOnTurn + 1) % 2;
			return;
		case GameStatus::DRAW:
			messageToDisplay = DRAW_MESSAGE;
			break;
		case GameStatus::FIRST_PLAYER_WON:
			messageToDisplay = std::format(WINNING_MESSAGE, players[0].GetName());
			break;
		case GameStatus::SECOND_PLAYER_WON:
			messageToDisplay = std::format(WINNING_MESSAGE, players[1].GetName());
			break;
	}
	gameInterupted = true;
}

GameStatus GameEngine::CheckGameStatus() {
	auto playerOnePlacedQueen = players[0].HasPlacedQueen();
	auto playerTwoPlacedQueen = players[1].HasPlacedQueen();

	bool playerTwoWon = false, playerOneWon = false;

	if (playerOnePlacedQueen) {
		playerTwoWon = CheckIfPlayerWon(1);
	}
	if (playerTwoPlacedQueen) {
		playerOneWon = CheckIfPlayerWon(0);
	}

	if (playerOneWon && playerTwoWon) {
		return GameStatus::DRAW;
	} else if (playerOneWon) {
		return GameStatus::FIRST_PLAYER_WON;
	} else if (playerTwoWon) {
		return GameStatus::SECOND_PLAYER_WON;
	}
	return GameStatus::NORMAL;
}

bool GameEngine::CheckIfPlayerWon(const int IDOfPlayer) {
	for (const auto& tile : gameMap) {
		if (tile.second != nullptr) {
			if (tile.second->GetBugType() == bugType::QUEEN_BEE && tile.second->GetPlayerID() == (IDOfPlayer + 1) % 2) {
				if (GetOccupiedNeighborsOfTile(gameMap, tile.first).size() == 6) {
					return true;
				}
			}
		}
	}
	return false;
}

void GameEngine::ModifyBorderOfHive(const HexCords& presentCordsOfModifiedTile,
                                    const HexCords& originalPositionOfModifiedTile) {
	ModifyBorderOfHive(presentCordsOfModifiedTile);

	auto it = gameMap.find(originalPositionOfModifiedTile);
	if (it != gameMap.end() && it->second == nullptr) {
		borderOfHive.insert(originalCordsOfSelectedTile);
		EraseUnnecessarydHexesFromBorder(originalPositionOfModifiedTile);
	}
}

void GameEngine::ModifyBorderOfHive(const HexCords& presentCordsOfModifiedTile) {
	borderOfHive.erase(presentCordsOfModifiedTile);
	auto tempNeihgbours = GetEmptyNeighborsOfTile(gameMap, presentCordsOfModifiedTile);
	borderOfHive.merge(tempNeihgbours);
}

void GameEngine::EraseUnnecessarydHexesFromBorder(const HexCords& originalPositionOfModifiedTile) {
	auto candidates = GetEmptyNeighborsOfTile(gameMap, originalPositionOfModifiedTile);

	for (const auto& candidate : candidates) {
		if (!HasSomeBugHexNeighbor(candidate)) {
			borderOfHive.erase(candidate);
		}
	}
}

bool GameEngine::HasSomeBugHexNeighbor(const HexCords& hexcords) {
	return GetOccupiedNeighborsOfTile(gameMap, hexcords).size() > 0;
}

void GameEngine::InvalidateSelectedTileVariables() {
	isPlayerTileSelected = false;
	indexOfPlayerTileSelected = -1;
	pointerToSelectedTile = nullptr;
	originalCordsOfSelectedTile = { std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };
}

void GameEngine::UpdatePossibleMovesOnSelectedTile() {
	if (pointerToSelectedTile != nullptr) {
		if (isPlayerTileSelected) {
			possibleMovesOfSelectedTile =
			    pointerToSelectedTile->Place(gameMap, borderOfHive, idOfPlayerOnTurn, turn == 0);
		} else if (players[idOfPlayerOnTurn].HasPlacedQueen()) {
			possibleMovesOfSelectedTile =
			    pointerToSelectedTile->Move(gameMap, borderOfHive, originalCordsOfSelectedTile);
		} else {
			possibleMovesOfSelectedTile = possibleMovesSet();
		}
	}
}

void GameEngine::RenderBaseLayout() {
	renderer.RenderBaseLayout(gameMap, players, idOfPlayerOnTurn);
	if (turn == 4 && !players[idOfPlayerOnTurn].HasPlacedQueen()) {
		renderer.DisplayQueenMessage();
	}
}

void GameEngine::RenderRest() {
	if (isPlayerTileSelected) {
		renderer.HighLightSelectedHex(std::make_pair(idOfPlayerOnTurn, indexOfPlayerTileSelected));
	} else if (pointerToSelectedTile != nullptr) {
		renderer.HighLightSelectedHex(originalCordsOfSelectedTile);
	}

	if (pointerToSelectedTile == nullptr) {
		renderer.HighLightPossibleMoves(borderOfHive);
	} else {
		renderer.HighLightPossibleMoves(possibleMovesOfSelectedTile);
	}

	if (gameInterupted) {
		renderer.DisplayCenteredTextBaner(messageToDisplay, FONT_SIZE * 2);
	}
}

void GameEngine::PlayerNameConfiguration() {
	players[0].SetName("BLACK");
	players[1].SetName("GRAY");
}

hexTileMap GameEngine::GenerateBaseMap() {
	hexTileMap map;
	for (int i = 0; i < hexagonHorizontalCount; i++) {
		for (int j = 0; j < HEXAGON_VERTICAL_COUNT; j++) {
			map.insert(std::make_pair(HexCords(i, j - i / 2), nullptr));
		}
	}
	return map;
}

hexTileMap::iterator GameEngine::FindIteratorOfHexUnderCursor() {
	HexCords cord = renderer.FindCordsOfHexUnderCursor();
	auto it = gameMap.find(cord);
	return it;
}