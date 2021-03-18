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

	src/cameras/2d/camera.cpp
	src/cameras/2d/camera.fwd.h
	src/cameras/2d/camera.h
	src/cameras/2d/camera_config.h
	src/cameras/2d/camera_manager.cpp
	src/cameras/2d/camera_manager.fwd.h
	src/cameras/2d/camera_manager.h
	src/cameras/2d/effects/fade.h
	src/cameras/2d/effects/flash.h
	src/cameras/2d/effects/pan.h
	src/cameras/2d/effects/rotate_to.h
	src/cameras/2d/effects/shake.h
	src/cameras/2d/effects/zoom.h

	src/core/config.cpp
	src/core/config.fwd.h
	src/core/config.h
	src/core/game.cpp
	src/core/game.fwd.h
	src/core/game.h
	src/core/time_step.cpp
	src/core/time_step.h

	src/display/color.cpp
	src/display/color.h

	src/event/event_emitter.cpp
	src/event/event_emitter.h
	src/event/event_listener.h

	src/gameobjects/display_list.h
	src/gameobjects/gameobject.fwd.h
	src/gameobjects/gameobject.h
	src/gameobjects/components/alpha.h
	src/gameobjects/components/flip.h
	src/gameobjects/components/tint.h
	src/gameobjects/components/transform_matrix.cpp
	src/gameobjects/components/transform_matrix.h
	src/gameobjects/components/visible.h
	src/gameobjects/group/group.h
	src/gameobjects/rendertexture/render_texture.fwd.h
	src/gameobjects/rendertexture/render_texture.h

	src/geom/line.cpp
	src/geom/line.h
	src/geom/point.cpp
	src/geom/point.h
	src/geom/rectangle.cpp
	src/geom/rectangle.h

	src/input/pointer.fwd.h
	src/input/pointer.h

	src/loader/loader_plugin.cpp
	src/loader/loader_plugin.fwd.h
	src/loader/loader_plugin.h

	src/math/math.cpp
	src/math/math.h
	src/math/random_data_generator.cpp
	src/math/random_data_generator.h
	src/math/vector2.cpp
	src/math/vector2.h

	src/renderer/renderer.cpp
	src/renderer/renderer.fwd.h
	src/renderer/renderer.h

	src/scale/scale_manager.cpp
	src/scale/scale_manager.fwd.h
	src/scale/scale_manager.h

	src/scene/scene.cpp
	src/scene/scene.fwd.h
	src/scene/scene.h
	src/scene/scene_config.cpp
	src/scene/scene_config.fwd.h
	src/scene/scene_config.h
	src/scene/scene_manager.cpp
	src/scene/scene_manager.fwd.h
	src/scene/scene_manager.h
	src/scene/scene_plugin.cpp
	src/scene/scene_plugin.fwd.h
	src/scene/scene_plugin.h
	src/scene/scene_settings.cpp
	src/scene/scene_settings.fwd.h
	src/scene/scene_settings.h
	src/scene/scene_settings_config.h
	src/scene/scene_systems.cpp
	src/scene/scene_systems.fwd.h
	src/scene/scene_systems.h
	src/scene/scene_transition_config.h

	src/structs/size.cpp
	src/structs/size.h

	src/texture/crop_data.h
	src/texture/frame.cpp
	src/texture/frame.fwd.h
	src/texture/frame.h
	src/texture/frame_data.h
	src/texture/sprite_sheet_config.h
	src/texture/texture.cpp
	src/texture/texture.fwd.h
	src/texture/texture.h
	src/texture/texture_manager.cpp
	src/texture/texture_manager.fwd.h
	src/texture/texture_manager.h
	src/texture/texture_source.cpp
	src/texture/texture_source.fwd.h
	src/texture/texture_source.h
	src/texture/parsers/json_array.cpp
	src/texture/parsers/json_array.h
	src/texture/parsers/json_hash.cpp
	src/texture/parsers/json_hash.h
	src/texture/parsers/sprite_sheet.cpp
	src/texture/parsers/sprite_sheet.h
	src/texture/parsers/sprite_sheet_atlas.cpp
	src/texture/parsers/sprite_sheet_atlas.h

	src/window/window.cpp
	src/window/window.fwd.h
	src/window/window.h
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
