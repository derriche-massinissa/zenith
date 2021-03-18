/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CAMERAS_SCENE2D_CAMERA_MANAGER_H
#define ZEN_CAMERAS_SCENE2D_CAMERA_MANAGER_H

#include <vector>
#include <functional>
#include <string>

#include "camera_config.h"
#include "camera.h"
#include "../../scene/scene.fwd.h"
#include "../../scene/scene_systems.h"
#include "../../geom/rectangle.h"
#include "../../structs/size.h"
#include "../../input/pointer.h"
#include "../../gameobjects/gameobject.h"
#include "../../gameobjects/display_list.h"
#include "../../renderer/renderer.fwd.h"

namespace Zen {
namespace Cameras {
namespace Scene2D {

/**
 * The Camera Manager is a plugin that belongs to a Scene and is responsible for 
 * managing all of the Scene Cameras.
 *
 * By default you can access the Camera Manager from within a Scene using `cameras`, although this can be changed
 * in your game config.
 *
 * Create new Cameras using the `add` method.
 *
 * Cameras provide a view into your game world, and can be positioned, rotated, zoomed and scrolled accordingly.
 *
 * A Camera consists of two elements: The viewport and the scroll values.
 *
 * The viewport is the physical position and size of the Camera within your game. Cameras, by default, are
 * created the same size as your game, but their position and size can be set to anything. This means if you
 * wanted to create a camera that was 320x200 in size, positioned in the bottom-right corner of your game,
 * you'd adjust the viewport to do that (using methods like `setViewport` and `setSize`).
 *
 * If you wish to change where the Camera is looking in your game, then you scroll it. You can do this
 * via the properties `scrollX` and `scrollY` or the method `setScroll`. Scrolling has no impact on the
 * viewport, and changing the viewport has no impact on the scrolling.
 *
 * By default a Camera will render all Game Objects it can see. You can change this using the `ignore` method,
 * allowing you to filter Game Objects out on a per-Camera basis. The Camera Manager can manage up to 31 unique
 * 'Game Object ignore capable' Cameras. Any Cameras beyond 31 that you create will all be given a Camera ID of
 * zero, meaning that they cannot be used for Game Object exclusion. This means if you need your Camera to ignore
 * Game Objects, make sure it's one of the first 31 created.
 *
 * A Camera also has built-in special effects including Fade, Flash, Camera Shake, Pan and Zoom.
 *
 * @class CameraManager
 * @since 0.0.0
 */
class CameraManager
{
public:
	/**
	 * @since 0.0.0
	 *
	 * @param scene_ The Scene that owns the Camera Manager plugin.
	 */
	CameraManager (Scene* scene_);

	/**
	 * @since 0.0.0
	 */
	~CameraManager ();

	/**
	 * A reference to the Scene that owns the Camera Manager plugin.
	 *
	 * @since 0.0.0
	 */
	Scene* scene;

	/**
	 * A reference to the SceneSystems handler for the Scene that owns the
	 * CameraManager.
	 *
	 * @since 0.0.0
	 */
	Scenes::SceneSystems& systems;

	/**
	 * A vector of the Camera objects being managed by this Camera Manager.
	 * The Cameras are updated and rendered in the same order in which they appear in this array.
	 * Do not directly add or remove entries to this array. However, you can move the contents
	 * around the array should you wish to adjust the display order.
	 *
	 * @since 0.0.0
	 */
	std::vector<Camera> cameras;

	/**
	 * A handy pointer to the 'main' camera. By default this is the first Camera the
	 * Camera Manager creates. You can also set it directly, or use the `makeMain` argument
	 * in the `add` methods. It allows you to access it from your game:
	 *
	 * ```c++
	 * // Reference
	 * Zen::Camera& cam = *cameras.main;
	 * // Pointer
	 * Zen::Camera* cam = cameras.main;
	 * ```
	 *
	 * Also see the properties `camera1`, `camera2` and so on.
	 *
	 * @since 0.0.0
	 */
	Camera *main;

	/**
	 * A default un-transformed Camera that doesn't exist on the camera list and doesn't
	 * count towards the total number of cameras being managed. It exists for other
	 * systems, as well as your own code, should they require a basic un-transformed
	 * camera instance from which to calculate a view matrix.
	 *
	 * @since 0.0.0
	 */
	Camera def;

	/**
	 * This method is called automatically, only once, when the Scene is first 
	 * created.
	 * Do not invoke it directly.
	 *
	 * @since 0.0.0
	 */
	void boot ();

	/**
	 * This method is called automatically by the Scene when it is starting up.
	 * It is responsible for creating local systems, properties and listening for 
	 * Scene events.
	 * Do not invoke it directly.
	 *
	 * @since 0.0.0
	 */
	void start ();

	/**
	 * Adds a new Camera into the Camera Manager. The Camera Manager can support up to 31 different Cameras.
	 *
	 * Each Camera has its own viewport, which controls the size of the Camera and its position within the canvas.
	 *
	 * Use the `Camera.scrollX` and `Camera.scrollY` properties to change where the Camera is looking, or the
	 * Camera methods such as `centerOn`. Cameras also have built in special effects, such as fade, flash, shake,
	 * pan and zoom.
	 *
	 * By default Cameras are transparent and will render anything that they can see based on their `scrollX`
	 * and `scrollY` values. Game Objects can be set to be ignored by a Camera by using the `Camera.ignore` method.
	 *
	 * See the Camera class documentation for more details.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The horizontal position of the Camera viewport.
	 * @param y_ The vertical position of the Camera viewport.
	 * @param width_ The width of the Camera viewport. If not given it'll be the game config size.
	 * @param height_ The height of the Camera viewport. If not given it'll be the game config size.
	 * @param makeMain_ Set this Camera as being the 'main' camera. This just makes the property `main` point to it.
	 * @param name_ The name of the Camera.
	 *
	 * @return A pointer to the newly created Camera.
	 */
	Camera* add (
			int x_ = 0,
			int y_ = 0,
			int width_ = 0,
			int height_ = 0,
			bool makeMain_ = false,
			std::string name_ = "");

	/**
	 * Gets the total number of Cameras in this Camera Manager.
	 *
	 * If the optional `isVisible` argument is set it will only count Cameras that are currently visible.
	 *
	 * @since 0.0.0
	 *
	 * @param isVisible_ Set the `true` to only include visible Cameras in the total.
	 *
	 * @return The total number of Cameras in this Camera Manager.
	 */
	int getTotal (bool isVisible_ = false);

	/**
	 * Populates this Camera Manager based on the given vector of config objects.
	 *
	 * See the `Zen::CameraConfig` documentation for details of the object structure.
	 *
	 * @since 0.0.0
	 *
	 * @param config_ A vecor of Camera configuration objects to be added to this Camera Manager.
	 *
	 * @return This Camera Manager instance.
	 */
	CameraManager& fromConfig (std::vector<CameraConfig> config_);

	/**
	 * Gets a Camera based on its name.
	 *
	 * Camera names are optional and don't have to be set, so this method is only of any use if you
	 * have given your Cameras unique names.
	 *
	 * @since 0.0.0
	 *
	 * @param name_ The name of the Camera.
	 *
	 * @return A pointer to the first Camera with a name matching the given string, otherwise `null`.
	 */
	Camera* getCamera (std::string name_);

	/**
	 * Returns an array of all cameras below the given Pointer.
	 *
	 * The first camera in the array is the top-most camera in the camera list.
	 *
	 * @since 0.0.0
	 *
	 * @param pointer_ The Pointer to check against.
	 *
	 * @return A vector of cameras below the Pointer.
	 */
	std::vector<Camera*> getCamerasBelowPointer (Input::Pointer pointer_);

	/**
	 * Removes the given vector of Cameras, from this Camera Manager.
	 *
	 * If found in the Camera Manager it will be immediately removed from the local cameras array.
	 * If also currently the 'main' camera, 'main' will be reset to be camera 0.
	 *
	 * @since 0.0.0
	 *
	 * @param camerasToRemove_ A vector of Camera pointers to be removed from this 
	 * CameraManager.
	 *
	 * @return The total number of Cameras removed.
	 */
	int remove (std::vector<Camera*> camerasToRemove_);

	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param cameraToRemove_ A Camera pointer to be removed from this 
	 * CameraManager.
	 *
	 * @return The total number of Cameras removed.
	 */
	int remove (Camera* cameraToRemove_);

	/**
	 * The internal render method. This is called automatically by the Scene and should not be invoked directly.
	 *
	 * It will iterate through all local cameras and render them in turn, as long as they're visible and have
	 * an alpha level > 0.
	 *
	 * @since 0.0.0
	 *
	 * @param renderer_ The renderer that will render the children to this camera.
	 * @param displayList_ The Display List for the Scene.
	 */
	void render (Renderer& renderer_, GameObjects::DisplayList& displayList_);

	/**
	 * Takes an array of Game Objects and a Camera and returns a new array
	 * containing only those Game Objects that pass the `willRender` test
	 * against the given Camera.
	 *
	 * @since 0.0.0
	 *
	 * @param children_ A vector of Game Objects to be checked against the camera.
	 * @param camera_ A reference to the camera to filter the Game Objects against.
	 *
	 * @return A filtered list of only Game Objects within the Scene that will 
	 * render against the given Camera.
	 */
	std::vector<GameObjects::GameObject*> getVisibleChildren (
			std::vector<GameObjects::GameObject*>& children_,
			Camera& camera_);

	/**
	 * Resets this Camera Manager.
	 *
	 * This will iterate through all current Cameras, destroying them all, then it 
	 * will reset the cameras vector, reset the ID counter and create 1 new single 
	 * camera using the default values.
	 *
	 * @since 0.0.0
	 *
	 * @return A pointer to the freshly created main Camera.
	 */
	Camera* resetAll ();

	/**
	 * The main update loop. Called automatically when the Scene steps.
	 *
	 * @since 0.0.0
	 *
	 * @param time_ The amount of time since the game started, in ms.
	 * @param delta_ The delta time, in ms, elapsed since the last update.
	 */
	void update (Uint32 time_, Uint32 delta_);

	/**
	 * The event handler that manages the `resize` event dispatched by the Scale Manager.
	 *
	 * @since 0.0.0
	 *
	 * @param data_ A data object containing the following integers:
	 * @param gameSize_ The default game size, as defined in the Game Config.
	 * @param displaySize_ The display size. Not used here.
	 * @param previousWidth_ The previous base width before resize.
	 * @param previousHeight_ The previous base height before resize.
	 */
	void onResize (Structs::Size gameSize_, Structs::Size displaySize_, int previousWidth_, int previousHeight_);

	/**
	 * Resizes all cameras to the given dimensions.
	 *
	 * @since 0.0.0
	 *
	 * @param width_ The new width of the camera.
	 * @param height_ The new height of the camera.
	 */
	void resize (int width_, int height_);

	/**
	 * The Scene that owns this plugin is shutting down.
	 * We need to kill and reset all internal properties as well as stop listening to Scene events.
	 *
	 * @since 0.0.0
	 */
	void shutdown ();

private:
	/**
	 * Gets the next available Camera ID number.
	 *
	 * The Camera Manager supports up to 31 unique cameras, after which the ID returned will always be zero.
	 * You can create additional cameras beyond 31, but they cannot be used for Game Object exclusion.
	 *
	 * @since 0.0.0
	 *
	 * @return The next available Camera ID, or 0 if they're all already in use.
	 */
	int getNextID ();
};

}	// namespace Scene2D
}	// namespace Cameras
}	// namespace Zen

#include "../../scene/scene_manager.h"
#include "../../scene/scene.h"

#endif
