#include "common.h"
#include "GameEngine.h"
#include "hexUtilities.h"
#include "raylib.h"
#include "raymath.h"
#include "Renderer.h"
#include "rlgl.h"

#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main() {
	GameEngine gameEngine;

	// Main game loop
	while (!WindowShouldClose()) {
		try {
			gameEngine.RenderBaseLayout();
			gameEngine.CheckInputs();
			gameEngine.RenderRest();
			EndDrawing();
		} catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
	CloseWindow();
	return 0;
}
