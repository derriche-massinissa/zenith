/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include <string>
#include <map>
#include <vector>
#include "glyph.hpp"
#include "text_style.hpp"
#include "../ecs/entity.hpp"
#include "const.hpp"
#include "../geom/types/rectangle.hpp"
#include "../renderer/types/gl_types.hpp"

// FreeType 2
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Zen {

/**
 * @struct FontAtlasRowData
 * @since 0.0.0
 */
struct FontAtlasRowData {
	// Row is locked when another row is created after it
	// If locked, the largest height cannot change any more
	bool locked = false;

	int width = 0;

	int	largestHeight = 0;
};

/**
 * @struct FontAtlasData
 * @since 0.0.0
 */
struct FontAtlasData {
	/**
	 * The SDL surface stored on the RAM. Used to update the texture when new
	 * glyphs get added to the atlas cache.
	 *
	 * @since 0.0.0
	 */
	SDL_Surface *surface = nullptr;

	/**
	 * The OpenGL texture stored on the GPU.
	 *
	 * @since 0.0.0
	 */
	GL_texture texture = 0;

	/**
	 * Initial size of 128px.
	 *
	 * @since 0.0.0
	 */
	int width = 128;

	/**
	 * Initial size of 128px.
	 *
	 * @since 0.0.0
	 */
	int	height = 128;

	/**
	 * @since 0.0.0
	 */
	std::vector<FontAtlasRowData> rows;

	/**
	 * Index in the atlasList vector.
	 *
	 * @since 0.0.0
	 */
	size_t index = 0;

	/**
	 * The line spacing of this font. Automaticaly set from the font configuration.
	 * Only used if no user defined line spacing has been specified on the text
	 * object.
	 *
	 * @since 0.0.0
	 */
	int lineSpacing = -1;
};

/**
 * @class TextManager
 * @since 0.0.0
 */
class TextManager
{
public:
	/**
	 * Destructor
	 */
	~TextManager ();

	/**
	 * Initializes the FreeType Library.
	 *
	 * @since 0.0.0
	 */
	void boot ();

	/**
	 * Loads up a font from a file and associates to it the given key.
	 *
	 * @since 0.0.0
	 */
	void addFont (std::string key, std::string path);

	/**
	 * Removes a font from the text manager and destroys it.
	 *
	 * @since 0.0.0
	 *
	 * @param key The key of the Font to remove.
	 */
	void removeFont (std::string key);

	/**
	 * Scans a string, and extracts all newly used characters to cache them for
	 * future use.
	 *
	 * @since 0.0.0
	 *
	 * @param text The text to scan.
	 *
	 * @return Returns the number of characters that have been cached.
	 */
	int scanText (Entity text);

	/**
	 * @param characters A vector of characters to add as glyphs to the font cache.
	 *
	 * @since 0.0.0
	 */
	void addGlyphs (std::vector<int> characters, TextStyle style);

	/**
	 * @return The change in size multiplier of the glyph atlas surface.
	 */
	int packGlyphs (std::vector<Glyph*> *glyphs, FontAtlasData *atlas);

	bool sortGlyphsByHeight(Glyph *a, Glyph *b);

	std::vector<int> stringToUnicodes (std::string text);

	Rectangle getTextBoundingBox (std::vector<int> &characters, TextStyle &style);

	std::vector<Rectangle> getLinesBoundingBox (std::vector<int> &characters,
			TextStyle &style);

	std::vector<int> wrapText (std::vector<int> text, TextStyle style);

	/**
	 * The freetype library instance.
	 *
	 * @since 0.0.0
	 */
	FT_Library ft;

	/**
	 * A map of loaded font files and their respective keys.
	 *
	 * @since 0.0.0
	 */
	std::map<std::string, FT_Face> fonts;

	/**
	 * ```cpp
	 * glyphCache[fontFamily][fontSize][color][style][outline][character]
	 * glyphCache["Arial"][16][0xff00ff][TTF_STYLE_NORMAL][0]['j']
	 * ```
	 *
	 * @since 0.0.0
	 */
	std::map<std::string, std::map<						// Font Family
		int, std::map<									// Font Size
			TEXT_DECORATION, std::map<					// Style
				int, std::map<							// Outline
					int, Glyph							// Character
					>
				>
			>
		>
	> glyphCache;

	/**
	 * @since 0.0.0
	 */
	std::map<std::string, std::map<						// Font Family
		int, std::map<									// Font Size
			TEXT_DECORATION, std::map<					// Style
				int, FontAtlasData						// Outline
				>
			>
		>
	> fontsAtlas;

	/**
	 * A vector of pointers to all created atlases for easier iteration through all
	 * of them (For freeing resources for example).
	 *
	 * @since 0.0.0
	 */
	std::vector<FontAtlasData*> atlasList;
};

}	// namespace Zen
