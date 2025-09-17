/**
 * @file common.h
 * @author Vojtech Venzara (Vojtaven@seznam.cz)
 * @brief Contains common constants and definitions used throughout the project.
 * @version 0.1
 * @date 2024-04-11
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef COMMON_H
#define COMMON_H

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

// Defaults constants
constexpr int HEXAGON_SIDES_COUNT = 6;   /**< Number of sides in a hexagon. */
constexpr int SIZE_OF_AXIAL_VECTORS = 6; /**< Size of axial vectors array. */
constexpr bool DEBUG_MODE = false;       /**< Turns on unlimeted FPS and frame time */

// Screen constants
constexpr int FPS = 90; /**< Frames per second for the game. (Only if DEBUG_MODE == false */

// GameEngine constants
constexpr int HEXAGON_VERTICAL_COUNT = 12; /**< Number of hexagons vertically in the game board. */
constexpr int DIFFERENT_PIECES_COUNT = 5;

// Renderer constants
constexpr float WINDOW_SCALING = (float)(2 / 3.0);     /**< Scaling factor for window size. */
const float SQRT_OF_THREE = (float)sqrt(3);            /**< Square root of three. For faster calculation*/
constexpr float SIDE_SIZE_PERCENT = (float)(1.4 / 10); /**< Side size percentage. */
constexpr float TOLERANCE = 5;                         /**< Tolerance value for calculations. */
constexpr int FONT_SIZE = 20;                          /**< Default font size. */

constexpr Color QUEEN_BEE_COLOR = ORANGE;       /**< Color of the Queen Bee. */
constexpr Color BEETLE_COLOR = PURPLE;          /**< Color of the Beetle. */
constexpr Color GRASS_HOPPER_COLOR = DARKGREEN; /**< Color of the Grasshopper. */
constexpr Color SPIDER_COLOR = BROWN;           /**< Color of the Spider. */
constexpr Color SOLDIER_ANT_COLOR = BLUE;       /**< Color of the Soldier Ant. */

constexpr Color TEXT_COLOR = WHITE;         /**< Color of text elements. */
constexpr Color PLAYER_ON_TURN_COLOR = RED; /**< Color indicating the player on turn. */

constexpr Color PLAYER_BACKGROUND_COLOR = { 40, 40, 40, 255 }; /**< Background color for player info. */

constexpr Color HIGHLIGHT_COLOR = LIME; /**< Color for highlighted elements. */
constexpr Color POSSIBLE_MOVES_HIGHLIGHT_COLOR = { 7, 140, 140,
	                                               255 }; /**< Color for highlighting possible moves. It is Dark cyan*/

constexpr std::pair<Color, Color> FIRST_PLAYER_COLORS = { WHITE, BLACK }; /**< Colors for the first player. */
constexpr std::pair<Color, Color> SECOND_PLAYER_COLORS = { WHITE, GRAY }; /**< Colors for the second player. */

static constexpr const char* QUEEN_MESSAGE =
    "!!! You must place Queen on this turn !!!"; /**< Message indicating that the Queen must be placed. */
static constexpr const char* DRAW_MESSAGE = "!!! Game ended in draw !!!"; /**< Message indicating a draw. */
static constexpr const char* WINNING_MESSAGE =
    "!!! Player {} has won !!!"; /**< Message indicating the winning player. */

#endif  // !COMMON_H
