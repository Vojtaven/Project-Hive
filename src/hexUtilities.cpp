#include "hexUtilities.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <iostream>

bool HexCords::operator<(const HexCords& other) const {
	return this->q < other.q || (this->q == other.q && this->r < other.r);
}

HexCords HexCords::operator+(const HexCords& other) const { return { this->q + other.q, this->r + other.r }; }

HexCords HexCords::operator+=(const HexCords& other) {
	this->q += other.q;
	this->r += other.r;
	return *this;
}

possibleMovesSet GetNeighborsOfTile(const hexTileMap& gameMap, const HexCords& tile) {
	possibleMovesSet result;
	HexCords tempHexCord = { std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };
	for (int i = 0; i < SIZE_OF_AXIAL_VECTORS; i++) {
		tempHexCord = tile + AXIAL_DIRECTION_VECTORS[i];
		result.insert(tempHexCord);
	}
	return result;
}

possibleMovesSet GetEmptyNeighborsOfTile(const hexTileMap& gameMap, const HexCords& tile) {
	auto neighbors = GetNeighborsOfTile(gameMap, tile);
	possibleMovesSet result;
	for (const auto& x : neighbors) {
		auto it = gameMap.find(x);
		if (it != gameMap.end() && it->second == nullptr) {
			result.insert(x);
		}
	}

	return result;
}

possibleMovesSet GetOccupiedNeighborsOfTile(const hexTileMap& gameMap, const HexCords& tile) {
	auto neighbors = GetNeighborsOfTile(gameMap, tile);
	possibleMovesSet result;
	for (const auto& x : neighbors) {
		auto it = gameMap.find(x);
		if (it != gameMap.end() && it->second != nullptr) {
			result.insert(x);
		}
	}

	return result;
}

Vector3 Cube_round(Vector3 frac) {
	int q, r, s;
	q = (int)round(frac.x);
	r = (int)round(frac.y);
	s = (int)round(frac.z);

	float qDiff = abs(q - frac.x);
	float rDiff = abs(r - frac.y);
	float sDiff = abs(s - frac.z);

	if (qDiff > rDiff && qDiff > sDiff) {
		q = -r - s;
	} else if (rDiff > sDiff) {
		r = -q - s;
	} else {
		s = -q - r;
	}

	return Vector3((float)q, (float)r, (float)s);
}

Vector2 CubeToAxial(Vector3 cube) { return Vector2(cube.x, cube.y); }

Vector3 AxialToCube(Vector2 axial) {
	float q, r, s;
	q = axial.x;
	r = axial.y;
	s = -q - r;
	return Vector3(q, r, s);
}

Vector2 RoundAxial(Vector2 axialFrac) { return CubeToAxial(Cube_round(AxialToCube(axialFrac))); }