# Set the minimum version required of CMake
cmake_minimum_required(VERSION 3.20.0)

# Set up the project
project(
	"Zenith"
	VERSION 0.0.0
	LANGUAGES CXX
	)

# Source files
set(zenith_SRCS
	)
	#src/math/angle/wrap_degrees.cpp
	#src/math/angle/wrap_radians.cpp
	#src/math/angle/wrap_degrees.hpp
	#src/math/angle/wrap_radians.hpp
	#src/math/fuzzy/fuzzy_ceil.cpp
	#src/math/fuzzy/fuzzy_equal.cpp
	#src/math/fuzzy/fuzzy_floor.cpp
	#src/math/fuzzy/fuzzy_greater_than.cpp
	#src/math/fuzzy/fuzzy_less_than.cpp
	#src/math/fuzzy/fuzzy_ceil.hpp
	#src/math/fuzzy/fuzzy_equal.hpp
	#src/math/fuzzy/fuzzy_floor.hpp
	#src/math/fuzzy/fuzzy_greater_than.hpp
	#src/math/fuzzy/fuzzy_less_than.hpp
	#src/math/snap/snap_ceil.cpp
	#src/math/snap/snap_floor.cpp
	#src/math/snap/snap_to.cpp
	#src/math/snap/snap_ceil.hpp
	#src/math/snap/snap_floor.hpp
	#src/math/snap/snap_to.hpp
	#src/math/types/vector2.hpp
	#src/math/clamp.cpp
	#src/math/deg_to_rad.cpp
	#src/math/linear.cpp
	#src/math/rad_to_deg.cpp
	#src/math/random_data_generator.cpp
	#src/math/rotate_around.cpp
	#src/math/transform_xy.cpp
	#src/math/vector2.cpp
	#src/math/wrap.cpp
	#src/math/clamp.hpp
	#src/math/const.hpp
	#src/math/deg_to_rad.hpp
	#src/math/linear.hpp
	#src/math/rad_to_deg.hpp
	#src/math/random.hpp
	#src/math/random_data_generator.hpp
	#src/math/rotate_around.hpp
	#src/math/transform_xy.hpp
	#src/math/vector2.hpp
	#src/math/wrap.hpp
	#)




	#src/zenith.h

	#src/const.h
	#src/data.h
	#src/defs.h
	#src/messages.h
	#src/messages.cpp
	#
	#	src/cameras/2d/camera.cpp
	#	src/cameras/2d/camera.fwd.h
	#	src/cameras/2d/camera.h
	#	src/cameras/2d/camera_config.h
	#	src/cameras/2d/camera_manager.cpp
	#	src/cameras/2d/camera_manager.fwd.h
	#	src/cameras/2d/camera_manager.h
	#	src/cameras/2d/effects/fade.h
	#	src/cameras/2d/effects/flash.h
	#	src/cameras/2d/effects/pan.h
	#	src/cameras/2d/effects/rotate_to.h
	#	src/cameras/2d/effects/shake.h
	#	src/cameras/2d/effects/zoom.h
	#
	#	src/core/config.cpp
	#	src/core/config.fwd.h
	#	src/core/config.h
	#	src/core/game.cpp
	#	src/core/game.fwd.h
	#	src/core/game.h
	#	src/core/time_step.cpp
	#	src/core/time_step.h
	#
	#	src/display/color.cpp
	#	src/display/color.h
	#
	#	src/event/event_emitter.cpp
	#	src/event/event_emitter.h
	#	src/event/event_listener.h
	#
	#	src/gameobjects/components/alpha.h
	#	src/gameobjects/components/blend_mode.h
	#	src/gameobjects/components/depth.h
	#	src/gameobjects/components/flip.h
	#	src/gameobjects/components/get_bounds.h
	#	src/gameobjects/components/mask.h
	#	src/gameobjects/components/origin.h
	#	src/gameobjects/components/scroll_factor.h
	#	src/gameobjects/components/size.h
	#	src/gameobjects/components/texture_crop.h
	#	src/gameobjects/components/tint.h
	#	src/gameobjects/components/transform.h
	#	src/gameobjects/components/transform_matrix.cpp
	#	src/gameobjects/components/transform_matrix.h
	#	src/gameobjects/components/visible.h
	#	src/gameobjects/group/group.fwd.h
	#	src/gameobjects/group/group.h
	#	src/gameobjects/image/image.cpp
	#	src/gameobjects/image/image.h
	#	src/gameobjects/rendertexture/render_texture.fwd.h
	#	src/gameobjects/rendertexture/render_texture.h
	#	src/gameobjects/sprite/
	#	src/gameobjects/display_list.cpp
	#	src/gameobjects/display_list.h
	#	src/gameobjects/gameobject.cpp
	#	src/gameobjects/gameobject.fwd.h
	#	src/gameobjects/gameobject.h
	#	src/gameobjects/gameobject_factory.cpp
	#	src/gameobjects/gameobject_factory.h
	#	src/gameobjects/update_list.cpp
	#	src/gameobjects/update_list.h
	#
	#	src/geom/line.cpp
	#	src/geom/line.h
	#	src/geom/point.cpp
	#	src/geom/point.h
	#	src/geom/rectangle.cpp
	#	src/geom/rectangle.fwd.h
	#	src/geom/rectangle.h
	#
	#	src/input/pointer.fwd.h
	#	src/input/pointer.h
	#
	#	src/loader/loader_plugin.cpp
	#	src/loader/loader_plugin.fwd.h
	#	src/loader/loader_plugin.h
	#
	#	src/math/angle/wrap.h
	#	src/math/angle/wrap.cpp
	#	src/math/angle/wrap_degrees.h
	#	src/math/angle/wrap_degrees.cpp
	#	src/math/fuzzy/ceil.cpp
	#	src/math/fuzzy/ceil.h
	#	src/math/fuzzy/equal.cpp
	#	src/math/fuzzy/equal.h
	#	src/math/fuzzy/floor.cpp
	#	src/math/fuzzy/floor.h
	#	src/math/fuzzy/greater_than.cpp
	#	src/math/fuzzy/greater_than.h
	#	src/math/fuzzy/less_than.cpp
	#	src/math/fuzzy/less_than.h
	#	src/math/snap/snap_ceil.cpp
	#	src/math/snap/snap_ceil.h
	#	src/math/snap/snap_floor.cpp
	#	src/math/snap/snap_floor.h
	#	src/math/snap/snap_to.cpp
	#	src/math/snap/snap_to.h
	#	src/math/clamp.h
	#	src/math/const.h
	#	src/math/deg_to_rad.h
	#	src/math/linear.h
	#	src/math/random.h
	#	src/math/rad_to_deg.h
	#	src/math/random_data_generator.h
	#	src/math/rotate_around.h
	#	src/math/transform_xy.h
	#	src/math/vector2.h
	#	src/math/wrap.h
	#	src/math/clamp.cpp
	#	src/math/deg_to_rad.cpp
	#	src/math/linear.cpp
	#	src/math/rad_to_deg.cpp
	#	src/math/random_data_generator.cpp
	#	src/math/rotate_around.cpp
	#	src/math/transform_xy.cpp
	#	src/math/vector2.cpp
	#	src/math/wrap.cpp
	#
	#	src/renderer/blend_modes.cpp
	#	src/renderer/renderer.cpp
	#	src/renderer/renderer.fwd.h
	#	src/renderer/renderer.h
	#
	#	src/scale/scale_manager.cpp
	#	src/scale/scale_manager.fwd.h
	#	src/scale/scale_manager.h
	#
	#	src/scene/scene.cpp
	#	src/scene/scene.fwd.h
	#	src/scene/scene.h
	#	src/scene/scene_config.cpp
	#	src/scene/scene_config.fwd.h
	#	src/scene/scene_config.h
	#	src/scene/scene_manager.cpp
	#	src/scene/scene_manager.fwd.h
	#	src/scene/scene_manager.h
	#	src/scene/scene_plugin.cpp
	#	src/scene/scene_plugin.fwd.h
	#	src/scene/scene_plugin.h
	#	src/scene/scene_settings.cpp
	#	src/scene/scene_settings.fwd.h
	#	src/scene/scene_settings.h
	#	src/scene/scene_settings_config.h
	#	src/scene/scene_systems.cpp
	#	src/scene/scene_systems.fwd.h
	#	src/scene/scene_systems.h
	#	src/scene/scene_transition_config.h
	#
	#	src/structs/size.cpp
	#	src/structs/size.h
	#
	#	src/texture/crop_data.h
	#	src/texture/frame.cpp
	#	src/texture/frame.fwd.h
	#	src/texture/frame.h
	#	src/texture/frame_data.h
	#	src/texture/sprite_sheet_config.h
	#	src/texture/texture.cpp
	#	src/texture/texture.fwd.h
	#	src/texture/texture.h
	#	src/texture/texture_manager.cpp
	#	src/texture/texture_manager.fwd.h
	#	src/texture/texture_manager.h
	#	src/texture/texture_source.cpp
	#	src/texture/texture_source.fwd.h
	#	src/texture/texture_source.h
	#	src/texture/parsers/json_array.cpp
	#	src/texture/parsers/json_array.h
	#	src/texture/parsers/json_hash.cpp
	#	src/texture/parsers/json_hash.h
	#	src/texture/parsers/sprite_sheet.cpp
	#	src/texture/parsers/sprite_sheet.h
	#	src/texture/parsers/sprite_sheet_atlas.cpp
	#	src/texture/parsers/sprite_sheet_atlas.h
	#
	#	src/utils/base64_encode.h
	#	src/utils/base64_encode.cpp
	#	src/utils/base64_decode.h
	#	src/utils/base64_decode.cpp
	#
	#	src/window/window.cpp
	#	src/window/window.fwd.h
	#	src/window/window.h
	#	)

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

# Export compile commands for clangd (Code Completion)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "" FORCE)

# Build type
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)

# Debug mode (GDB) + Disable optimization (for debugging) + Warnings
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -Og -Wall -Wpedantic -Wextra" CACHE STRING "" FORCE)

# Releas mode
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE STRING "" FORCE)

# Install directory
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist" CACHE FILEPATH "" FORCE)

# External includes
target_include_directories(${PROJECT_NAME} PRIVATE
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/includes"
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
	PATTERN "*.hpp"
	)
