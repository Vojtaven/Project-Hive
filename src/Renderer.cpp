#include "raylib.h"
#include "raymath.h"
#include "Renderer.h"
#include "rlgl.h"

#include <exception>
#include <iostream>

Renderer::Renderer() {
	WindowInitialization();
	VariableInitialization();
}
void Renderer::RenderBaseLayout(const hexTileMap& map, const Player players[2], const int idOfPlayerOnTurn) {
	ClearBackground(BLACK);
	RenderHexMap(map);
	RenderPlayerFields();
	RenderPlayers(players, idOfPlayerOnTurn);

	// Debug
	if (DEBUG_MODE) DisplayFrameTime();
}

void Renderer::RenderPlayerFields() {
	// Player1
	DrawRectangle(0, 0, (int)sideSize, (int)windowSize.y, PLAYER_BACKGROUND_COLOR);
	DrawLineEx(Vector2(sideSize, 0), Vector2(sideSize, windowSize.y), 4, TEXT_COLOR);

	// Player2
	DrawRectangle((int)(windowSize.x - sideSize), 0, (int)sideSize, (int)windowSize.y, PLAYER_BACKGROUND_COLOR);
	DrawLineEx(Vector2(windowSize.x - sideSize, 0), Vector2(windowSize.x - sideSize, windowSize.y), 4, TEXT_COLOR);
}

void Renderer::RenderHexMap(const hexTileMap& map) {
	Vector2 hexScreenPos;

	for (const auto& hex : map) {
		hexScreenPos = CalculateScreenPos(hex.first);
		RenderHexOnPosition(hex.second.get(), hexScreenPos);
	}
}
void Renderer::RenderHexOnPosition(const BaseBugTile* hex, Vector2 hexScreenPos) {
	if (hex == nullptr) {
		DrawDefaultHex(hexScreenPos);
	} else {
		DrawBugHex(hexScreenPos, hex);
	}
}

void Renderer::HighLightSelectedHex(std::variant<std::pair<int, int>, HexCords> selectedHex) {
	if (auto cords = std::get_if<HexCords>(&selectedHex)) {
		auto hexScreenPos = CalculateScreenPos(*cords);
		DrawPolyLinesEx(hexScreenPos, HEXAGON_SIDES_COUNT, hexSize - lineThickness, 0, lineThickness, HIGHLIGHT_COLOR);
	} else {
		auto playersIndexes = std::get<0>(selectedHex);
		float x = (windowSize.x - sideSize) * playersIndexes.first + sideSize / 2;
		float y = offsetOfHexInPlayerField + spacingOfHexInPlayerField * playersIndexes.second;
		DrawPolyLinesEx(Vector2(x, y), HEXAGON_SIDES_COUNT, hexSize - lineThickness, 0, lineThickness, HIGHLIGHT_COLOR);
	}
}

void Renderer::HighLightPossibleMoves(const possibleMovesSet& possibleMoves) {
	for (const auto& cord : possibleMoves) {
		auto hexScreenPos = CalculateScreenPos(cord);
		DrawPolyLinesEx(hexScreenPos, HEXAGON_SIDES_COUNT, hexSize - lineThickness, 0, lineThickness,
		                POSSIBLE_MOVES_HIGHLIGHT_COLOR);
	}
}

bool Renderer::IsMouseInPlayersFields() {
	const auto mousePosition = GetMousePosition();
	return mousePosition.x < sideSize || mousePosition.x > windowSize.x - sideSize;
}

int Renderer::GetIndexOfHexInPlayerField(int playerId) {
	auto mousePosition = GetMousePosition();
	int tempIndexOfHex = (int)round((mousePosition.y - offsetOfHexInPlayerField) / spacingOfHexInPlayerField);

	if (tempIndexOfHex > 5) {
		return -1;
	}

	float x = (windowSize.x - sideSize) * playerId + sideSize / 2;
	float y = offsetOfHexInPlayerField + spacingOfHexInPlayerField * tempIndexOfHex;
	if (CheckCollisionPointCircle(mousePosition, Vector2(x, y), hexSize * SQRT_OF_THREE / 2)) {
		return tempIndexOfHex;
	} else {
		return -1;
	}
}

std::pair<std::string, Color> Renderer::GetColorAndNameFromBugType(bugType type) {
	switch (type) {
		case bugType::QUEEN_BEE:
			return std::make_pair("Queen Bee", QUEEN_BEE_COLOR);
			break;
		case bugType::BEETLE:
			return std::make_pair("Beetle", BEETLE_COLOR);
			break;
		case bugType::SOLDIER_ANT:
			return std::make_pair("Soldier Ant", SOLDIER_ANT_COLOR);
			break;
		case bugType::SPIDER:
			return std::make_pair("Spider", SPIDER_COLOR);
			break;
		case bugType::GRASS_HOPPER:
			return std::make_pair("Grass Hopper", GRASS_HOPPER_COLOR);
			break;
		default:
			throw std::runtime_error("Not supported bug type");
			break;
	}
}

void Renderer::RenderPlayers(const Player players[2], const int idOfPlayerOnTurn) {
	float startHeight = 0;
	float startWidth = 0;
	std::pair<Color, Color> playerColor;
	for (size_t i = 0; i < 2; i++) {
		float startHeight = 10;
		float startWidth = (windowSize.x - sideSize) * i;

		if (i == idOfPlayerOnTurn) {
			RenderCenteredText(players[i].GetName().c_str(), Vector2(startWidth, 10), sideSize, PLAYER_ON_TURN_COLOR);
		} else {
			RenderCenteredText(players[i].GetName().c_str(), Vector2(startWidth, 10), sideSize);
		}

		startHeight += FONT_SIZE + 10;

		DrawLineEx(Vector2(startWidth, startHeight), Vector2(startWidth + sideSize, startHeight), 4, TEXT_COLOR);

		startHeight += 15;

		auto piecesToRender = players[i].GetPlayerAvaiblepieces();

		i == 0 ? playerColor = FIRST_PLAYER_COLORS : playerColor = SECOND_PLAYER_COLORS;
		for (const auto& piece : piecesToRender) {
			startHeight += RenderPlayersPiecePanel(piece, Vector2(startWidth, startHeight), sideSize, TEXT_COLOR,
			                                       FONT_SIZE, playerColor.first, playerColor.second);
			startHeight += 15;
		}
	}
}

void Renderer::RenderCenteredText(const char* text, const Vector2& startPosition, const float avaibleSpace,
                                  Color textColor, int fontSize) {
	int textWidth = MeasureText(text, fontSize);
	DrawText(text, (int)(startPosition.x + (avaibleSpace - textWidth) / 2), (int)startPosition.y, fontSize, textColor);
}

void Renderer::DrawCenteredBugHex(const Vector2& startPosition, const float avaibleSpace, Color outlineColor,
                                  Color hexBaseColor, Color hexSecondaryColor) {
	float xPosition = startPosition.x + avaibleSpace / 2;
	float yPosition = startPosition.y + SQRT_OF_THREE * hexSize / 2;

	DrawBugHex(Vector2(xPosition, yPosition), outlineColor, hexBaseColor, hexSecondaryColor);
}

float Renderer::RenderPlayersPiecePanel(const playerPiece& piece, const Vector2& startPosition,
                                        const float avaibleSpace, Color textColor, int fontSize, Color outlineColor,
                                        Color hexBaseColor) {
	auto playersPieceToRender = GetColorAndNameFromBugType(piece.first);

	RenderCenteredText(playersPieceToRender.first.c_str(), startPosition, avaibleSpace, textColor, fontSize);
	RenderCenteredText(TextFormat("%i left", piece.second), Vector2(startPosition.x, startPosition.y + fontSize + 5),
	                   avaibleSpace, textColor, fontSize);
	DrawCenteredBugHex(Vector2(startPosition.x, startPosition.y + fontSize * 2 + 10), avaibleSpace, outlineColor,
	                   hexBaseColor, playersPieceToRender.second);

	// How big is the panel
	return fontSize * 2 + 15 + SQRT_OF_THREE * hexSize;
}

void Renderer::WindowInitialization() {
	// Initialization
	//--------------------------------------------------------------------------------------
	// Display fps
	if (!DEBUG_MODE) SetTargetFPS(FPS);

	// Screen Size
	InitWindow(1280, 720, "Project Hive");

	displayIdentifier = GetCurrentMonitor();
	windowSize.x = (GetMonitorWidth(displayIdentifier) * WINDOW_SCALING);
	windowSize.y = (GetMonitorHeight(displayIdentifier) * WINDOW_SCALING);

	windowPosition.x = (GetMonitorWidth(displayIdentifier) - windowSize.x) / 2;
	windowPosition.y = (GetMonitorHeight(displayIdentifier) - windowSize.y) / 2;

	SetWindowPosition((int)windowPosition.x, (int)windowPosition.y);
	SetWindowSize((int)windowSize.x, (int)windowSize.y);

	Camera2D camera = { 0 };
	camera.zoom = 1.0f;
}

void Renderer::VariableInitialization() {
	// Based on 1280 * 720 window size

	hexSize = ((windowSize.y - TOLERANCE) / (HEXAGON_VERTICAL_COUNT * SQRT_OF_THREE + 1));
	lineThickness = hexSize * (float)1.0 / 10;
	defaultOffset = hexSize;
	sideSize = windowSize.x * SIDE_SIZE_PERCENT;

	hexagonHorizontalCount = (int)((windowSize.x - defaultOffset - sideSize * 2) / (hexSize * (3.0 / 2) + 1));
	sideSize = (float)((windowSize.x - TOLERANCE * 2 - hexagonHorizontalCount * (3.0 / 2) * hexSize - hexSize / 2) / 2);

	horizontalOffset = sideSize + TOLERANCE;

	offsetOfHexInPlayerField = FONT_SIZE * 3 + 45 + SQRT_OF_THREE * hexSize / 2;
	spacingOfHexInPlayerField = FONT_SIZE * 2 + 30 + SQRT_OF_THREE * hexSize;
}

void Renderer::DrawDefaultHex(const Vector2& hexScreenPos, Color outlineColor, Color hexBaseColor) {
	DrawPoly(hexScreenPos, HEXAGON_SIDES_COUNT, hexSize - lineThickness, 0, hexBaseColor);
	DrawPolyLinesEx(hexScreenPos, HEXAGON_SIDES_COUNT, hexSize - lineThickness, 0, lineThickness, outlineColor);
}
void Renderer::DrawBugHex(const Vector2& hexScreenPos, const BaseBugTile* tile) {
	Color bugColor = tile->GetBugColor();
	switch (tile->GetPlayerID()) {
		case 0:
			DrawBugHex(hexScreenPos, FIRST_PLAYER_COLORS.first, FIRST_PLAYER_COLORS.second, bugColor);
			break;
		case 1:
			DrawBugHex(hexScreenPos, SECOND_PLAYER_COLORS.first, SECOND_PLAYER_COLORS.second, bugColor);
			break;
		default:
			throw std::runtime_error("AAaaaaa bad playerrrr");
			break;
	}
}

void Renderer::DrawBugHex(const Vector2& hexScreenPos, Color outlineColor, Color hexBaseColor,
                          Color hexSecondaryColor) {
	DrawDefaultHex(hexScreenPos, outlineColor, hexBaseColor);
	DrawPoly(hexScreenPos, HEXAGON_SIDES_COUNT, hexSize / 2, 0, hexSecondaryColor);
}

Vector2 Renderer::CalculateScreenPos(const HexCords& hexPos) {
	return Vector2(
	    hexSize * 3 / 2 * hexPos.q + defaultOffset + horizontalOffset,
	    hexSize * (SQRT_OF_THREE / 2 * hexPos.q + SQRT_OF_THREE * hexPos.r) + defaultOffset + verticalOffset);
}

HexCords Renderer::FindCordsOfHexUnderCursor() const {
	float q, r;
	Vector2 mousePosition = GetMousePosition();

	mousePosition.x -= defaultOffset + horizontalOffset;
	mousePosition.y -= defaultOffset + verticalOffset;

	q = (mousePosition.x / hexSize) * ((float)2 / 3);
	r = (float)(-(1.0 / 3) * mousePosition.x + (SQRT_OF_THREE / 3) * mousePosition.y) / hexSize;

	Vector2 tempHexCords = RoundAxial(Vector2(q, r));

	return HexCords((int)tempHexCords.x, (int)tempHexCords.y);
}

int Renderer::GetHexagonHorizontalCount() { return hexagonHorizontalCount; }

void Renderer::DisplayFrameTime() {
	DrawText(TextFormat("%02.02f ms", GetFrameTime() * 1000), 10, (int)(windowSize.y - FONT_SIZE), FONT_SIZE, RED);
}

void Renderer::DisplayCenteredTextBaner(const std::string& message, const int font_size) {
	auto bannerSizeX = MeasureText(message.c_str(), font_size) + TOLERANCE * 2 + lineThickness * 2;
	auto bannerStartX = (windowSize.x - bannerSizeX) / 2;

	auto bannerSizeY = lineThickness * 2 + TOLERANCE * 2 + font_size;
	auto bannerStartY = (windowSize.y - bannerSizeY) / 2;

	DisplayTextBanner(message, Vector2(bannerStartX, bannerStartY), font_size, RED);
}

void Renderer::DisplayQueenMessage() {
	auto bannerSizeX = MeasureText(QUEEN_MESSAGE, FONT_SIZE) + TOLERANCE * 2 + lineThickness * 2;
	auto bannerStartX = (windowSize.x - bannerSizeX) / 2;

	DisplayTextBanner(QUEEN_MESSAGE, Vector2(bannerStartX, 0), FONT_SIZE, RED);
}

void Renderer::DisplayTextBanner(const std::string& message, const Vector2& position, const int font_size,
                                 const Color textColor, const Color bannerColor, const Color borderColor) {
	auto rectangleSizeX = MeasureText(message.c_str(), font_size) + TOLERANCE * 2 + lineThickness * 2;
	auto rec = Rectangle(position.x, position.y, rectangleSizeX, font_size + 2 * TOLERANCE + lineThickness * 2);
	DrawRectangleRec(rec, bannerColor);
	DrawRectangleLinesEx(rec, lineThickness, borderColor);
	DrawText(message.c_str(), (int)(position.x + TOLERANCE + lineThickness),
	         (int)(position.y + TOLERANCE + lineThickness), font_size, textColor);
}