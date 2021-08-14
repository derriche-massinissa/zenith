/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_GAMEOBJECTFACTORY_H
#define ZEN_GAMEOBJECTS_GAMEOBJECTFACTORY_H

#include <string>
#include "../scene/scene.fwd.hpp"
#include "../ecs/entity.hpp"
#include "../text/text_style.hpp"
//#include "image/image.fwd.h"

namespace Zen {

/**
 * @class GameObjectFactory
 * @since 0.0.0
 */
class GameObjectFactory
{
public:
	/**
	 * Constructor.
	 *
	 * @since 0.0.0
	 */
	GameObjectFactory (Scene* scene_);

	/**
	 * @since 0.0.0
	 */
	Scene* scene;

	/**
	 * @since 0.0.0
	 */
	void boot ();

	/**
	 * @since 0.0.0
	 */
	Entity image (double x, double y, std::string key, std::string frame = "");

	/**
	 * ```cpp
	 * auto text = this.add.text(100, 150, "Score: 0", {
	 *		.color = 0xff0000,
	 *		.fontFamily = "Helvetica",
	 *		.fontSize = 64,
	 *		.align = "center",
	 *		.backgroundColor = 0x00ffff
	 * });
	 *
	 * SetColor(text, 0x00ff00);
	 * SetFontSize(text, 32);
	 * SetFontFamily(text, "Arial");
	 * SetText(text, "New Best Score!");
	 * SetAlign(text, "left");
	 * SetBackgroundColor(text, 0xffff00);
	 * ```
	 *
	 * @since 0.0.0
	 * @param x The position of the text on the x axis
	 * @param y The position of the text on the y axis
	 * @param text The text content of the text object
	 * @param style The style of the text object
	 */
	Entity text (double x, double y, std::string text, TextStyle style);

	/**
	 * Create a text object by passing an array of lines that will be automatically
	 * separated by new lines characters (\n).
	 *
	 * @since 0.0.0
	 */
	Entity text (double x, double y, std::vector<std::string> text, TextStyle style);
};

}	//namespace Zen

#endif
