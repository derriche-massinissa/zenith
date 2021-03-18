# Set the minimum version required of CMake
cmake_minimum_required(VERSION 3.19.4)

# GCC warnings
add_compile_options(-w) #Wall -Wextra -Wpedantic)

# Set up the project
project(
	"Zenith"
	VERSION 0.0.0
	LANGUAGES CXX
	)

# Source files
set(zenith_SRCS
	src/zenith.h
	src/defs.h
	src/const.h
	src/data.h
	src/messages.h
	src/messages.cpp
	src/math.h
	src/math.cpp

	src/core/game.fwd.h
	src/core/game.h
	src/core/game.cpp
	src/core/game_config.h
	src/core/game_config.cpp
	src/core/time_step.h
	src/core/time_step.cpp

	src/window/window.h
	src/window/window.cpp

	src/event/event_emitter.h
	src/event/event_emitter.cpp
	src/event/event_listener.h
	src/event/event_listener.cpp

	src/scene/scene.fwd.h
	src/scene/scene.h
	src/scene/scene.cpp
	src/scene/scene_config.h
	src/scene/scene_manager.fwd.h
	src/scene/scene_manager.h
	src/scene/scene_manager.cpp
	src/scene/scene_plugin.h
	src/scene/scene_plugin.cpp
	src/scene/scene_settings.h
	src/scene/scene_settings.cpp
	src/scene/scene_settings_config.h
	src/scene/scene_systems.h
	src/scene/scene_systems.cpp
	src/scene/scene_transition_config.h
	)

# Find external libraries in the system
#find_package(SDL2 REQUIRED)
	#SDL2_image REQUIRED
	#SDL2_ttf REQUIRED
	#SDL2_mixer REQUIRED

# Create the library
add_library(
	${PROJECT_NAME}
	STATIC
	${zenith_SRCS}
	)

# Require C++17
target_compile_features(${PROJECT_NAME}
	PUBLIC cxx_std_17
	)

# Impose the use of C++ for the linker
set_target_properties(${PROJECT_NAME}
	PROPERTIES LINKER_LANGUAGE CXX
	)

# External includes
#target_include_directories(${PROJECT_NAME} PUBLIC
#	${SDL2_INCLUDE_DIRS}
#	#${SDL2IMAGE_INCLUDE_DIRS}
#	#${SDL2TTF_INCLUDE_DIRS}
#	#${SDL2MIX_INCLUDE_DIRS}
#	)

# Link libraries
#target_link_libraries(${PROJECT_NAME} PUBLIC
#	${SDL2_LIBRARIES}
#	#${SDL2IMAGE_LIBRARIES}
#	#${SDL2TTF_LIBRARIES}
#	#${SDL2MIX_LIBRARIES}
#	)

# Installation
# Library
install(
	TARGETS "${PROJECT_NAME}"
	DESTINATION "lib"
	)
# Header files
install(
	DIRECTORY "${CMAKE_SOURCE_DIR}/src/"
	TYPE INCLUDE
	FILES_MATCHING
	PATTERN "*.h"
	)
