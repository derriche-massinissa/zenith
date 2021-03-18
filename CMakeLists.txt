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
	src/const.h
	src/data.h
	src/defs.h
	src/messages.h
	src/messages.cpp

	src/cameras/2d/base_camera.h
	src/cameras/2d/base_camera.cpp
	src/cameras/2d/camera.h
	src/cameras/2d/camera.cpp
	src/cameras/2d/camera_config.h
	src/cameras/2d/camera_manager.h
	src/cameras/2d/camera_manager.cpp

	src/core/game.fwd.h
	src/core/game.h
	src/core/game.cpp
	src/core/config.h
	src/core/config.cpp
	src/core/time_step.h
	src/core/time_step.cpp

	src/display/color.h
	src/display/color.cpp
	src/display/masks/mask.h
	src/display/masks/bitmap_mask.h
	src/display/masks/bitmap_mask.cpp

	src/event/event_emitter.h
	src/event/event_emitter.cpp
	src/event/event_listener.h

	src/gameobjects/components/alpha.h
	src/gameobjects/components/transform_matrix.h
	src/gameobjects/components/transform_matrix.cpp
	src/gameobjects/components/visible.h

	src/geom/line.h
	src/geom/line.cpp
	src/geom/point.h
	src/geom/point.cpp
	src/geom/rectangle.h
	src/geom/rectangle.cpp

	src/loader/loader_plugin.h
	src/loader/loader_plugin.cpp

	src/math/math.h
	src/math/math.cpp
	src/math/random_data_generator.h
	src/math/random_data_generator.cpp
	src/math/vector2.h
	src/math/vector2.cpp

	src/renderer/renderer.h
	src/renderer/renderer.cpp

	src/scale/scale_manager.h
	src/scale/scale_manager.cpp

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

	srd/structs/size.h
	srd/structs/size.cpp

	src/textures/crop_data.h
	src/textures/frame.h
	src/textures/frame.cpp
	src/textures/frame_data.h
	src/textures/sprite_sheet_config.h
	src/textures/texture.h
	src/textures/texture.cpp
	src/textures/texture_manager.h
	src/textures/texture_manager.cpp
	src/textures/texture_source.h
	src/textures/texture_source.cpp

	src/textures/parsers/json_array.h
	src/textures/parsers/json_array.cpp
	src/textures/parsers/json_hash.h
	src/textures/parsers/json_hash.cpp
	src/textures/parsers/sprite_sheet.h
	src/textures/parsers/sprite_sheet.cpp
	src/textures/parsers/sprite_sheet_atlas.h
	src/textures/parsers/sprite_sheet_atlas.cpp

	src/window/window.h
	src/window/window.cpp
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

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(includes/json)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# External includes
#target_include_directories(${PROJECT} PUBLIC
#	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/includes"
#	)

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
	PATTERN "*.hpp"
	)
