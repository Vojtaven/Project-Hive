cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(ProjectHive)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 4.5.0)
find_package(raylib ${RAYLIB_VERSION} REQUIRED) # QUIET or REQUIRED

# Our Project
set( project_sources	
	src/projectHive.cpp
	src/hexUtilities.h 
	src/hexUtilities.cpp
	src/Renderer.h
	src/Renderer.cpp
	src/GameEngine.h
	src/GameEngine.cpp
	src/common.h
	src/bugTiles.h
	src/bugTIles.cpp
	src/Player.h
	src/Player.cpp
)


add_executable(${PROJECT_NAME} ${project_sources})
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
