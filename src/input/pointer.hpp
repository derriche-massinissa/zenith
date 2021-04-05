/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_POINTER_HPP
#define ZEN_INPUT_POINTER_HPP

#include "../ecs/entity.hpp"

#include <SDL2/SDL_types.h>
#include "../math/vector2.hpp"
#include "types/event.hpp"

namespace Zen {

class Pointer
{
public:
	int id = -1;

	/**
	 * The most recent event this Pointer has processed.
	 *
	 * @since 0.0.0
	 */
	InputEvent event;

	/**
	 * The camera the Pointer interacted with during its last update.
	 *
	 * A Pointer can only ever interact with one camera at once, which will be the
	 * top-most camera in the list should multiple cameras be positioned on-top of
	 * each other.
	 *
	 * @since 0.0.0
	 */
	Entity camera = entt::null;

	/**
	 * Indicates which button was pressed, or released, on the pointer during the
	 * most recent event. It is only set during `up` and `down` events.
	 *
	 * On Touch devices the value is always 0.
	 *
	 * Users may change the configuration of buttons on their pointing device so
	 * that if an event's button property is zero, it may not have been caused by
	 * the button that is physically left–most on the pointing device; however, it
	 * should behave as if the left button was clicked in the standard button
	 * layout.
	 *
	 * @since 0.0.0
	 */
	int button = 0;

	/**
	 * 0b 00000: No button or un-initialized
	 * 0b 00001: Left button
	 * 0b 00010: Right button
	 * 0b 00100: Wheel button or middle button
	 * 0b 01000: 4th button (typically the "Back" button)
	 * 0b 10000: 5th button (typically the "Forward" button)
	 *
	 * For a mouse configured for left-handed use, the button actions are reversed.
	 * In this case, the values are read from right to left.
	 *
	 * @since 0.0.0
	 */
	int buttons = 0;
	
	/**
	 * The position of the Pointer in screen space.
	 *
	 * @since 0.0.0
	 */
	Math::Vector2 position;

	/**
	 * The previous position of the Pointer in screen space.
	 *
	 * The old x and y values are stored in here during the
	 * InputManager::transformPointer call.
	 *
	 * Use the properties `velocity`, `angle` and `distance` to create your own
	 * gesture recognition.
	 *
	 * @since 0.0.0
	 */
	Math::Vector2 prevPosition;

	/**
	 * An internal vector used for calculations of the pointer speed and angle.
	 *
	 * @since 0.0.0
	 */
	Math::Vector2 midPoint {-1, -1};

	/**
	 * The current velocity of the Pointer, based on its current and previous
	 * positions.
	 *
	 * This value is smoothed out each frame, according to the `motionFactor`
	 * property.
	 *
	 * This property is updated whenever the Pointer moves, regardless of any
	 * button states. In other words, it changes based on movement alone - a button
	 * doesn't have to be pressed first.
	 *
	 * @since 0.0.0
	 */
	Math::Vector2 velocity;

	/**
	 * The current angle the Pointer is moving, in radians, based on its previous
	 * and current position.
	 *
	 * The angle is based on the old position facing to the current position.
	 *
	 * This property is updated whenever the Pointer moves, regardless of any
	 * button states. In other words, it changes based on movement alone - a button
	 * doesn't have to be pressed first.
	 *
	 * @since 0.0.0
	 */
	double angle = 0;

	/**
	 * The distance the Pointer has moved, based on its previous and current
	 * position.
	 *
	 * This value is smoothed out each frame, according to the `motionFactor`
	 * property.
	 *
	 * This property is updated whenever the Pointer moves, regardless of any
	 * button states. In other words, it changes based on movement alone - a button
	 * doesn't have to be pressed first.
	 *
	 * If you need the total distance travelled since the primary buttons was
	 * pressed down, then use the `Pointer::getDistance` method.
	 *
	 * @since 0.0.0
	 */
	double distance = 0;

	/**
	 * The smoothing factor to apply to the Pointer position.
	 *
	 * Due to their nature, pointer positions are inherently noisy. While this is
	 * fine for lots of games, if you need cleaner positions then you can set this
	 * value to apply an automatic smoothing to the positions as they are recorded.
	 *
	 * The default value of zero means 'no smoothing'.
	 * Set to a small value, such as 0.2, to apply an average level of smoothing
	 * between positions. You can do this by changing this value directly, or by
	 * setting the `input.smoothFactor` property in the Game Config.
	 *
	 * Positions are only smoothed when the pointer moves. If the primary button on
	 * this Pointer enters an Up or Down state, then the position is always
	 * precise, and not smoothed.
	 *
	 * @since 0.0.0
	 */
	double smoothFactor = 0;

	/**
	 * The factor applied to the motion smoothing each frame.
	 *
	 * This value is passed to the Smooth Step Interpolation that is used to
	 * calculate the velocity, angle and distance of the Pointer. It's applied
	 * every frame, until the midPoint reaches the current position of the Pointer.
	 *
	 * 0.2 provides a good average but can be increased if you need a quicker
	 * update and are working in a high performance environment. Never set this
	 * value to zero.
	 *
	 * @since 0.0.0
	 */
	double motionFactor = 0.2;

	/**
	 * The x position of this Pointer, translated into the coordinate space of the
	 * most recent Camera it interacted with.
	 *
	 * If you wish to use this value _outside_ of an input event handler then you
	 * should update it first by calling the Pointer::updateWorldPoint method.
	 *
	 * @since 0.0.0
	 */
	double worldX = 0.;

	/**
	 * The y position of this Pointer, translated into the coordinate space of the
	 * most recent Camera it interacted with.
	 *
	 * If you wish to use this value _outside_ of an input event handler then you
	 * should update it first by calling the Pointer::updateWorldPoint method.
	 *
	 * @since 0.0.0
	 */
	double worldY = 0.;

	/**
	 * Time when this Pointer was most recently moved (regardless of the state of
	 * its buttons, if any)
	 *
	 * @since 0.0.0
	 */
	Uint32 moveTime = 0;

	/**
	 * X coordinate of the Pointer when Button 1 (left button), or Touch, was
	 * pressed, used for dragging objects.
	 *
	 * @since 0.0.0
	 */
	double downX = 0.;

	/**
	 * Y coordinate of the Pointer when Button 1 (left button), or Touch, was
	 * pressed, used for dragging objects.
	 *
	 * @since 0.0.0
	 */
	double downY = 0.;

	/**
	 * The Event timestamp when the first button, or Touch input, was pressed. Used
	 * for dragging objects.
	 *
	 * @since 0.0.0
	 */
	Uint32 downTime = 0;

	/**
	 * X coordinate of the Pointer when Button 1 (left button), or Touch, was
	 * released, used for dragging objects.
	 *
	 * @since 0.0.0
	 */
	double upX = 0.;

	/**
	 * Y coordinate of the Pointer when Button 1 (left button), or Touch, was
	 * released, used for dragging objects.
	 *
	 * @since 0.0.0
	 */
	double upY = 0.;

	/**
	 * The Event timestamp when the final button, or Touch input, was released.
	 * Used for dragging objects.
	 *
	 * @since 0.0.0
	 */
	Uint32 upTime = 0;

	/**
	 * Is the primary button down? (usually button 0, the left mouse button)
	 *
	 * @since 0.0.0
	 */
	bool primaryDown = false;

	/**
	 * Is _any_ button on this pointer considered as being down?
	 *
	 * @since 0.0.0
	 */
	bool isDown = false;

	/**
	 * Did the previous input event come from a Touch input (true) or Mouse? (false)
	 *
	 * @since 0.0.0
	 */
	bool wasTouch = false;

	/**
	 * Did this Pointer get cancelled by a `touchcancel` event?
	 *
	 * @since 0.0.0
	 */
	bool wasCanceled = false;

	/**
	 * If the mouse is locked, the horizontal relative movement of the Pointer in
	 * pixels since last frame.
	 *
	 * @since 0.0.0
	 */
	double movementX = 0.;

	/**
	 * If the mouse is locked, the vertical relative movement of the Pointer in
	 * pixels since last frame.
	 *
	 * @since 0.0.0
	 */
	double movementY = 0.;

	/**
	 * An active Pointer is one that is currently pressed down on the display.
	 * A Mouse is always considered as active.
	 *
	 * @since 0.0.0
	 */
	bool active = false;

	/**
	 * Is this pointer Pointer Locked?
	 *
	 * Only a mouse pointer can be locked.
	 *
	 * You can set this by calling the `input.mouse.lockPointer()` method.
	 *
	 * @since 0.0.0
	 */
	bool locked = false;

	/**
	 * The horizontal scroll amount that occurred due to the user moving a mouse
	 * wheel or similar input device.
	 *
	 * @since 0.0.0
	 */
	double deltaX = 0.;

	/**
	 * The vertical scroll amount that occurred due to the user moving a mouse
	 * wheel or similar input device.
	 *
	 * @since 0.0.0
	 */
	double deltaY = 0.;

	/**
	 * The z-axis scroll amount that occurred due to the user moving a mouse
	 * wheel or similar input device.
	 *
	 * @since 0.0.0
	 */
	double deltaZ = 0.;

	Pointer ();

    /**
     * Takes a Camera and updates this Pointer's `worldX` and `worldY` values so
	 * they are the result of a translation through the given Camera.
     *
     * Note that the values will be automatically replaced the moment the Pointer is
     * updated by an input event, such as a mouse move, so should be used
	 * immediately.
     *
     * @since 0.0.0
     *
     * @param camera The Camera which is being tested against.
     */
	void updateWorldPoint (Entity camera_);

    /**
     * Takes a Camera and returns a Vector2 containing the translated position of
	 * this Pointer within that Camera. This can be used to convert this Pointers
	 * position into camera space.
     *
     * @since 0.0.0
     *
     * @param camera - The Camera to use for the translation.
     *
     * @return A Vector2 containing the translated coordinates of this Pointer,
	 * based on the given camera.
     */
	Math::Vector2 positionToCamera (Entity camera_);

    /**
     * Calculates the motion of this Pointer, including its velocity and angle of
	 * movement.
     * This method is called automatically each frame by the Input Manager.
     *
     * @since 0.0.0
     */
    void updateMotion ();

    /**
     * Internal method to handle a Mouse Up Event.
     *
     * @since 0.0.0
     *
     * @param event The Mouse Event to process.
     */
    void up (InputEvent event_);

    /**
     * Internal method to handle a Mouse Down Event.
     *
     * @since 0.0.0
     *
     * @param event The Mouse Event to process.
     */
    void down (InputEvent event_);

    /**
     * Internal method to handle a Mouse Move Event.
     *
     * @since 0.0.0
     *
     * @param event The Mouse Event to process.
     */
    void move (InputEvent event_);

    /**
     * Internal method to handle a Mouse Wheel Event.
     *
     * @since 0.0.0
     *
     * @param event The Mouse Event to process.
     */
    void wheel (InputEvent event_);

    /**
     * Internal method to handle a Touch Start Event.
     *
     * @since 0.0.0
     *
     * @param touch The Changed Touch from the Touch Event.
     * @param event The full Touch Event.
     */
    void touchstart (InputEvent touch_, InputEvent event_);

    /**
     * Internal method to handle a Touch Move Event.
     *
     * @since 0.0.0
     *
     * @param touch The Changed Touch from the Touch Event.
     * @param event The full Touch Event.
     */
    void touchmove (InputEvent touch_, InputEvent event_);

    /**
     * Internal method to handle a Touch End Event.
     *
     * @since 0.0.0
     *
     * @param touch The Changed Touch from the Touch Event.
     * @param event The full Touch Event.
     */
    void touchend (InputEvent touch_, InputEvent event_);

    /**
     * Internal method to handle a Touch Cancel Event.
     *
     * @since 0.0.0
     *
     * @param touch The Changed Touch from the Touch Event.
     * @param event The full Touch Event.
     */
    void touchcancel (InputEvent touch_, InputEvent event_);

    /**
     * Checks to see if any buttons are being held down on this Pointer.
     *
     * @since 0.0.0
     *
     * @return `true` if no buttons are being held down.
     */
    bool noButtonDown ();

    /**
     * Checks to see if the left button is being held down on this Pointer.
     *
     * @since 0.0.0
     *
     * @return `true` if the left button is being held down.
     */
    bool leftButtonDown ();

    /**
     * Checks to see if the right button is being held down on this Pointer.
     *
     * @since 0.0.0
     *
     * @return `true` if the right button is being held down.
     */
    bool rightButtonDown ();

    /**
     * Checks to see if the middle button is being held down on this Pointer.
     *
     * @since 0.0.0
     *
     * @return `true` if the middle button is being held down.
     */
    bool middleButtonDown ();

    /**
     * Checks to see if the back button is being held down on this Pointer.
     *
     * @since 0.0.0
     *
     * @return `true` if the back button is being held down.
     */
    bool backButtonDown ();

    /**
     * Checks to see if the forward button is being held down on this Pointer.
     *
     * @since 0.0.0
     *
     * @return `true` if the forward button is being held down.
     */
    bool forwardButtonDown ();

    /**
     * Checks to see if the left button was just released on this Pointer.
     *
     * @since 0.0.0
     *
     * @return `true` if the left button was just released.
     */
    bool leftButtonReleased ();

    /**
     * Checks to see if the right button was just released on this Pointer.
     *
     * @since 0.0.0
     *
     * @return `true` if the right button was just released.
     */
    bool rightButtonReleased ();

    /**
     * Checks to see if the middle button was just released on this Pointer.
     *
     * @since 0.0.0
     *
     * @return `true` if the middle button was just released.
     */
    bool middleButtonReleased ();

    /**
     * Checks to see if the back button was just released on this Pointer.
     *
     * @since 0.0.0
     *
     * @return `true` if the back button was just released.
     */
    bool backButtonReleased ();

    /**
     * Checks to see if the forward button was just released on this Pointer.
     *
     * @since 0.0.0
     *
     * @return `true` if the forward button was just released.
     */
    bool forwardButtonReleased ();

    /**
     * If the Pointer has a button pressed down at the time this method is called,
	 * it will return the distance between the Pointer's `downX` and `downY` values
	 * and the current position.
     *
     * If no button is held down, it will return the last recorded distance, based
	 * on where the Pointer was when the button was released.
     *
     * If you wish to get the distance being travelled currently, based on the
	 * velocity of the Pointer, then see the `Pointer::distance` property.
     *
     * @since 0.0.0
     *
     * @return The distance the Pointer moved.
     */
    double getDistance ();

    /**
     * If the Pointer has a button pressed down at the time this method is called,
	 * it will return the horizontal distance between the Pointer's `downX` and
	 * `downY` values and the current position.
     *
     * If no button is held down, it will return the last recorded horizontal
	 * distance, based on where the Pointer was when the button was released.
     *
     * @since 0.0.0
     *
     * @return The horizontal distance the Pointer moved.
     */
    double getDistanceX ();

    /**
     * If the Pointer has a button pressed down at the time this method is called,
	 * it will return the vertical distance between the Pointer's `downX` and
	 * `downY` values and the current position.
     *
     * If no button is held down, it will return the last recorded vertical
	 * distance, based on where the Pointer was when the button was released.
     *
     * @since 0.0.0
     *
     * @return The vertical distance the Pointer moved.
     */
    double getDistanceY ();

    /**
     * If the Pointer has a button pressed down at the time this method is called,
	 * it will return the duration since the button was pressed down.
     *
     * If no button is held down, it will return the last recorded duration, based
	 * on the time the last button on the Pointer was released.
     *
     * @since 0.0.0
     *
     * @return The duration the Pointer was held down for in milliseconds.
     */
    Uint32 getDuration ();

    /**
     * If the Pointer has a button pressed down at the time this method is called,
	 * it will return the angle between the Pointer's `downX` and `downY` values
	 * and the current position.
     *
     * If no button is held down, it will return the last recorded angle, based on
	 * where the Pointer was when the button was released.
     *
     * The angle is based on the old position facing to the current position.
     *
     * If you wish to get the current angle, based on the velocity of the Pointer,
	 * then see the `Pointer::angle` property.
     *
     * @since 0.0.0
     *
     * @return The angle between the Pointer's coordinates in radians.
     */
    double getAngle ();

    /**
     * Time when this Pointer was most recently updated.
     * This comes directly from the `event.timeStamp` property.
     * If no event has yet taken place, it will return zero.
     *
     * @since 0.0.0
     */
	Uint32 getTime ();
};

}	// namespace Zen

#endif
