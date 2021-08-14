# Set the minimum version required of CMake
cmake_minimum_required(VERSION 3.20.0)

# Export compile commands for clangd (Code Completion)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "" FORCE)

# Build type
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)

# Debug mode (GDB) + Disable optimization (for debugging) + Warnings
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -O0 -Wall -Wpedantic -Wextra" CACHE STRING "" FORCE)

# Release mode
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE STRING "" FORCE)

# Install directory
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist" CACHE FILEPATH "" FORCE)

# Set up the project
project(
	"Zenith"
	VERSION 0.0.0
	LANGUAGES CXX
	)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FT REQUIRED freetype2)

# Source files
set(zenith_SRCS
	src/tweens/tween/tween.cpp
	src/tweens/tween_manager.cpp





	src/text/text_manager.cpp
	src/systems/sources/text.cpp







	src/tweens/easing.cpp

	src/math/easing/linear/linear.cpp
	src/math/easing/stepped/stepped.cpp

	src/math/easing/back/in.cpp
	src/math/easing/bounce/in.cpp
	src/math/easing/circular/in.cpp
	src/math/easing/cubic/in.cpp
	src/math/easing/elastic/in.cpp
	src/math/easing/expo/in.cpp
	src/math/easing/quadratic/in.cpp
	src/math/easing/quartic/in.cpp
	src/math/easing/quintic/in.cpp
	src/math/easing/sine/in.cpp

	src/math/easing/back/out.cpp
	src/math/easing/bounce/out.cpp
	src/math/easing/circular/out.cpp
	src/math/easing/cubic/out.cpp
	src/math/easing/elastic/out.cpp
	src/math/easing/expo/out.cpp
	src/math/easing/quadratic/out.cpp
	src/math/easing/quartic/out.cpp
	src/math/easing/quintic/out.cpp
	src/math/easing/sine/out.cpp

	src/math/easing/back/in_out.cpp
	src/math/easing/bounce/in_out.cpp
	src/math/easing/circular/in_out.cpp
	src/math/easing/cubic/in_out.cpp
	src/math/easing/elastic/in_out.cpp
	src/math/easing/expo/in_out.cpp
	src/math/easing/quadratic/in_out.cpp
	src/math/easing/quartic/in_out.cpp
	src/math/easing/quintic/in_out.cpp
	src/math/easing/sine/in_out.cpp






	src/structs/list.cpp
	src/audio/audio_manager.cpp
	src/audio/tools/ogg.cpp
	src/audio/tools/al_utility.cpp
	src/audio/tools/alc_utility.cpp
	src/input/keyboard/keyboard_plugin.cpp
	src/input/keyboard/combo/key_combo.cpp
	src/input/keyboard/keys/key.cpp
	src/input/keyboard/keyboard_manager.cpp
	src/cameras/2d/camera_manager.cpp
	src/cameras/2d/systems/camera.cpp
	src/core/config.cpp
	src/core/game.cpp
	src/core/handle_sdl_events.cpp
	src/core/time_step.cpp
	src/display/color.cpp
	src/event/event_emitter.cpp
	src/gameobjects/display_list.cpp
	src/gameobjects/gameobject_factory.cpp
	src/gameobjects/update_list.cpp
	src/geom/line.cpp
	src/geom/circle.cpp
	src/geom/ellipse.cpp
	src/geom/rectangle.cpp
	src/geom/triangle.cpp
	src/input/mouse/mouse_manager.cpp
	src/input/pointer.cpp
	src/input/input_plugin.cpp
	src/input/input_manager.cpp
	src/loader/loader_plugin.cpp
	src/math/angle/wrap_degrees.cpp
	src/math/angle/wrap_radians.cpp
	src/math/angle/between.cpp
	src/math/clamp.cpp
	src/math/deg_to_rad.cpp
	src/math/distance/distance_between.cpp
	src/math/fuzzy/fuzzy_ceil.cpp
	src/math/fuzzy/fuzzy_equal.cpp
	src/math/fuzzy/fuzzy_floor.cpp
	src/math/fuzzy/fuzzy_greater_than.cpp
	src/math/fuzzy/fuzzy_less_than.cpp
	src/math/interpolation/smooth_step_interpolation.cpp
	src/math/linear.cpp
	src/math/rad_to_deg.cpp
	src/math/random_data_generator.cpp
	src/math/rotate_around.cpp
	src/math/smooth_step.cpp
	src/math/snap/snap_ceil.cpp
	src/math/snap/snap_floor.cpp
	src/math/snap/snap_to.cpp
	src/math/transform_xy.cpp
	src/math/vector2.cpp
	src/math/wrap.cpp
	src/renderer/blend_modes.cpp
	src/renderer/renderer.cpp
	src/scale/scale_manager.cpp
	src/scene/config.cpp
	src/scene/scene.cpp
	src/scene/scene_manager.cpp
	src/scene/plugin.cpp
	src/scene/systems.cpp
	src/structs/size.cpp
	src/systems/sources/actor.cpp
	src/systems/sources/alpha.cpp
	src/systems/sources/background_color.cpp
	src/systems/sources/blend_mode.cpp
	src/systems/sources/bounds.cpp
	src/systems/sources/deadzone.cpp
	src/systems/sources/depth.cpp
	src/systems/sources/dirty.cpp
	src/systems/sources/flip.cpp
	src/systems/sources/follow.cpp
	src/systems/sources/get_bounds.cpp
	src/systems/sources/id.cpp
	src/systems/sources/input.cpp
	src/systems/sources/mask.cpp
	src/systems/sources/name.cpp
	src/systems/sources/origin.cpp
	src/systems/sources/position.cpp
	src/systems/sources/renderable.cpp
	src/systems/sources/rotation.cpp
	src/systems/sources/scale.cpp
	src/systems/sources/scroll.cpp
	src/systems/sources/scroll_factor.cpp
	src/systems/sources/size.cpp
	src/systems/sources/textured.cpp
	src/systems/sources/tint.cpp
	src/systems/sources/transform.cpp
	src/systems/sources/transform_matrix.cpp
	src/systems/sources/transparent.cpp
	src/systems/sources/viewport.cpp
	src/systems/sources/visible.cpp
	src/systems/sources/zoom.cpp
	src/texture/parsers/json_array.cpp
	src/texture/parsers/json_hash.cpp
	src/texture/parsers/sprite_sheet_atlas.cpp
	src/texture/parsers/sprite_sheet.cpp
	src/texture/systems/frame.cpp
	src/texture/systems/source.cpp
	src/texture/systems/texture.cpp
	src/texture/texture_manager.cpp
	src/utils/base64/base64_decode.cpp
	src/utils/base64/base64_encode.cpp
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

# Require C++20
target_compile_features(${PROJECT_NAME}
	PUBLIC cxx_std_20
	)

# Impose the use of C++ for the linker
set_target_properties(${PROJECT_NAME}
	PROPERTIES LINKER_LANGUAGE CXX
	)

# External includes
target_include_directories(${PROJECT_NAME} PRIVATE
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/includes"
	${FT_INCLUDE_DIRS}
	)

target_link_libraries(${PROJECT_NAME} PUBLIC
	"${CMAKE_SOURCE_DIR}/lib/libvorbisfile.so"
	"${CMAKE_SOURCE_DIR}/lib/libopenal.so"
	${FT_LIBRARIES}
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
	DIRECTORY "${CMAKE_SOURCE_DIR}/includes/"
	TYPE INCLUDE
	FILES_MATCHING
	PATTERN "*.h"
	PATTERN "*.hpp"
	)
