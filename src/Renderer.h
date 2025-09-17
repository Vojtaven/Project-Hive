/**
 * @file Renderer.h
 * @author Vojtech Venzara (Vojtaven@seznam.cz)
 * @brief Contains class that manage rendering of game
 * @version 0.1
 * @date 2024-04-11
 *
 * @copyright Copyright (c) 2024
 */

#ifndef Renderer_H
#define Renderer_H

#include "bugTiles.h"
#include "common.h"
#include "hexUtilities.h"
#include "Player.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <iostream>
#include <map>
#include <variant>

/**
 * @brief Manages rendering of the game elements.
 *
 * The Renderer class handles rendering of various game elements such as the game map, players, hex tiles,
 * messages, and other UI components.
 */
class Renderer {
public:
	/**
	 * @brief Constructs a Renderer object.
	 *
	 * Constructs a Renderer object and initializes rendering parameters.
	 */
	Renderer();

	/**
	 * @brief Renders the base layout of the game.
	 *
	 * Renders the base layout of the game, including the game map, players, and other elements.
	 *
	 * @param map The game map represented as a hexTileMap.
	 * @param players An array containing the players in the game.
	 * @param idOfPlayerOnTurn The ID of the player currently on turn.
	 */
	void RenderBaseLayout(const hexTileMap& map, const Player players[2], const int idOfPlayerOnTurn);

	/**
	 * @brief Finds the coordinates of the hex tile under the cursor.
	 *
	 * @return The coordinates of the hex tile under the cursor.
	 */
	HexCords FindCordsOfHexUnderCursor() const;

	/**
	 * @brief Gets the count of hexagons horizontally based on hexagon size and screen size.
	 *
	 * @return The count of hexagons horizontally on the game map.
	 */
	int GetHexagonHorizontalCount();

	/**
	 * @brief Checks if the mouse cursor is in the players' fields.
	 *
	 * @return True if the mouse cursor is in the players' fields, false otherwise.
	 */
	bool IsMouseInPlayersFields();

	/**
	 * @brief Gets the index of the hex tile in the player's field.
	 *
	 * Retrieves the index of the hex tile in the player field based on the player ID.
	 *
	 * @param playerId The ID of the player.
	 * @return The index of the hex tile in the player's field or -1 if no hex is under cursor
	 */
	int GetIndexOfHexInPlayerField(int playerId);

	/**
	 * @brief Renders the players on the game screen.
	 *
	 * Renders the players on the game screen, including their names, pieces etc.
	 *
	 * @param players An array containing the players in the game.
	 * @param idOfPlayerOnTurn The ID of the player currently on turn.
	 */
	void RenderPlayers(const Player players[2], const int idOfPlayerOnTurn);

	/**
	 * @brief Highlights the selected hex tile.
	 *
	 * @param selectedHex A variant representing the player and piece index or selected hex tile coordinates.
	 */
	void HighLightSelectedHex(std::variant<std::pair<int, int>, HexCords> selectedHex);

	/**
	 * @brief Highlights the possible moves on the game map.
	 *
	 * Highlights the possible moves on the game map based on the provided set of possible moves.
	 *
	 * @param possibleMoves The set of possible moves to highlight.
	 */
	void HighLightPossibleMoves(const possibleMovesSet& possibleMoves);

	/**
	 * @brief Displays a message indicating the requirement to place the Queen.
	 *
	 * Displays a message on the game screen indicating that the player must place the Queen.
	 */
	void DisplayQueenMessage();

	/**
	 * @brief Displays a centered text banner on the game screen.
	 *
	 * Displays a centered text banner with the specified message on the game screen.
	 *
	 * @param message The message to display in the banner.
	 * @param font_size The font size of the text (optional, defaults to FONT_SIZE).
	 */
	void DisplayCenteredTextBaner(const std::string& message, const int font_size = FONT_SIZE);

private:
	/**
	 * @brief Renders a hex tile at a specified position.
	 *
	 * Renders a hex tile at the specified screen position.
	 *
	 * @param hex A pointer to the hex tile to render.
	 * @param hexScreenPos The screen position at which to render the hex tile.
	 */
	void RenderHexOnPosition(const BaseBugTile* hex, Vector2 hexScreenPos);

	/**
	 * @brief Renders centered text on the game screen.
	 *
	 * Renders centered text with the specified properties on the game screen.
	 *
	 * @param text The text to render.
	 * @param startPosition The position to start rendering the text.
	 * @param availableSpace The available space for rendering the text.
	 * @param textColor The color of the text (optional, defaults to TEXT_COLOR).
	 * @param fontSize The font size of the text (optional, defaults to FONT_SIZE).
	 */
	void RenderCenteredText(const char* text, const Vector2& startPosition, const float avaibleSpace,
	                        Color textColor = TEXT_COLOR, int fontSize = FONT_SIZE);

	/**
	 * @brief Displays a text banner on the game screen.
	 *
	 * Displays a text banner with the specified message and properties on the game screen.
	 *
	 * @param message The message to display in the banner.
	 * @param position The position to display the banner.
	 * @param fontSize The font size of the text (optional, defaults to FONT_SIZE).
	 * @param textColor The color of the text (optional, defaults to TEXT_COLOR).
	 * @param bannerColor The background color of the banner (optional, defaults to WHITE).
	 * @param borderColor The color of the banner border (optional, defaults to RED).
	 */
	void DisplayTextBanner(const std::string& message, const Vector2& position, const int font_size = FONT_SIZE,
	                       const Color textColor = TEXT_COLOR, const Color bannerColor = WHITE,
	                       const Color borderColor = RED);
	/**
	 * @brief Displays the frame time on the game screen.
	 *
	 * Displays the frame time (frames per second) on the game screen in left bottom corner for debugging purposes.
	 */
	void DisplayFrameTime();

	/**
	 * @brief Renders the player fields on the game screen.
	 *
	 * Renders the player fields base and base layout.
	 */
	void RenderPlayerFields();

	/**
	 * @brief Renders the hex map on the game screen.
	 *
	 * Renders the hex map. Color of pieces is determined by their type base is black and white.
	 *
	 * @param map The game map represented as a hexTileMap.
	 */
	void RenderHexMap(const hexTileMap& map);

	/**
	 * @brief Renders a panel displaying information about a player's piece.
	 *
	 * Renders a panel displaying information about the specified player's piece at the specified position.
	 * Information like name, remaining amount and draw picture of the hex tile.
	 *
	 * @param piece The player's piece to display information about.
	 * @param startPosition The position to start rendering the panel.
	 * @param availableSpace The available space for rendering the panel.
	 * @param textColor The color of the text (optional, defaults to TEXT_COLOR).
	 * @param fontSize The font size of the text (optional, defaults to FONT_SIZE).
	 * @param outlineColor The color of the panel outline (optional, defaults to WHITE).
	 * @param hexBaseColor The base color of the hex tile (optional, defaults to BLACK).
	 * @return The vertical size of the panel;
	 */
	float RenderPlayersPiecePanel(const playerPiece& piece, const Vector2& startPosition, const float avaibleSpace,
	                              Color textColor = TEXT_COLOR, int fontSize = FONT_SIZE, Color outlineColor = WHITE,
	                              Color hexBaseColor = BLACK);

	/**
	 * @brief Initializes the game window.
	 *
	 * Initializes the game window with specific parameters such as size, position etc
	 */
	void WindowInitialization();

	/**
	 * @brief Initializes the class variables.
	 *
	 * Initializes the class variables to their default values and calculate remaining values.
	 */
	void VariableInitialization();

	/**
	 * @brief Draws a centered bug hex tile on the game screen.
	 *
	 * Draws a centered bug hex tile with the specified properties on the game screen.
	 *
	 * @param startPosition The position to start rendering the hex tile.
	 * @param availableSpace The available space for rendering the hex tile.
	 * @param outlineColor The color of the hex tile outline (optional, defaults to WHITE).
	 * @param hexBaseColor The base color of the hex tile (optional, defaults to BLACK).
	 * @param hexSecondaryColor The secondary color of the hex tile (optional, defaults to BLACK).
	 */
	void DrawCenteredBugHex(const Vector2& startPosition, const float avaibleSpace, Color outlineColor = WHITE,
	                        Color hexBaseColor = BLACK, Color hexSecondaryColor = BLACK);

	/**
	 * @brief Draws a default hex tile at the specified position.
	 *
	 * Default hex tile is hex without smaller hex in middle.
	 *
	 * @param hexScreenPos The screen position to draw the hex tile.
	 * @param outlineColor The color of the hex tile outline (optional, defaults to WHITE).
	 * @param hexBaseColor The base color of the hex tile (optional, defaults to BLACK).
	 */
	void DrawDefaultHex(const Vector2& hexScreenPos, Color outlineColor = WHITE, Color hexBaseColor = BLACK);

	/**
	 * @brief Draws a bug hex tile at the specified position.
	 *
	 * Draws a bug hex tile with the properties of the specified tile at the specified position on the game screen.
	 *
	 * @param hexScreenPos The screen position to draw the hex tile.
	 * @param tile A pointer to the hex tile to draw.
	 */
	void DrawBugHex(const Vector2& hexScreenPos, const BaseBugTile* tile);

	/**
	 * @brief Draws a bug hex tile at the specified position with custom colors.
	 *
	 * Draws a bug hex tile with custom colors at the specified position on the game screen.
	 *
	 * @param hexScreenPos The screen position to draw the hex tile.
	 * @param outlineColor The color of the hex tile outline (optional, defaults to WHITE).
	 * @param hexBaseColor The base color of the hex tile (optional, defaults to BLACK).
	 * @param hexSecondaryColor The secondary color of the hex tile (optional, defaults to BLACK).
	 */
	void DrawBugHex(const Vector2& hexScreenPos, Color outlineColor = WHITE, Color hexBaseColor = BLACK,
	                Color hexSecondaryColor = BLACK);

	/**
	 * @brief Calculates the screen position of a hex tile based on its coordinates.
	 *
	 * @param hexPos The coordinates of the hex tile.
	 * @return The screen position of the hex tile.
	 */
	Vector2 CalculateScreenPos(const HexCords& hexPos);

	/**
	 * @brief Retrieves the color and name associated with a bug type.
	 *
	 * @param type The bug type.
	 * @return A pair containing the name and color associated with the bug type.
	 */
	std::pair<std::string, Color> GetColorAndNameFromBugType(bugType type);

	float lineThickness = 0; /**< The thickness of lines used for rendering. */
	float defaultOffset = 0, verticalOffset = 0, horizontalOffset = 0; /**< Offsets for rendering. */
	int hexagonHorizontalCount = 0;     /**< The number of hexagons horizontally that fits on screen */
	Vector2 windowSize, windowPosition; /**< The size and position of the game window. */
	float hexSize = 0;                  /**< The size of the hexagons used for rendering. */
	int displayIdentifier = std::numeric_limits<int>::max(); /**< Identifier for display purposes. */
	float sideSize = 0;                                      /**< Size of hexagon sides. */
	float offsetOfHexInPlayerField = 0;                      /**< Offset of hexagons in the player's field. */
	float spacingOfHexInPlayerField = 0;                     /**< Spacing of hexagons in the player's field. */
};

#endif