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
#include "../components/transform_matrix.hpp"

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
	SDL_Surface *surface = nullptr;

	SDL_Texture *texture = nullptr;

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
	 * @since 0.0.0
	 */
	void render (Entity textEntity, Entity camera);

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

	/**
	 * A temporary Transform Matrix, re-used internally during batching.
	 *
	 * @since 0.0.0
	 */
	Components::TransformMatrix tempMatrix1;

	/**
	 * A temporary Transform Matrix, re-used internally during batching.
	 *
	 * @since 0.0.0
	 */
	Components::TransformMatrix tempMatrix2;

	/**
	 * A temporary Transform Matrix, re-used internally during batching.
	 *
	 * @since 0.0.0
	 */
	Components::TransformMatrix tempMatrix3;

	/**
	 * A temporary Transform Matrix, re-used internally during batching.
	 *
	 * @since 0.0.0
	 */
	Components::TransformMatrix tempMatrix4;

private:
	/**
	 * @return The change in size multiplier of the glyph atlas surface.
	 */
	int packGlyphs (std::vector<Glyph*> *glyphs, FontAtlasData *atlas);

	static bool SortGlyphsByHeight(Glyph *a, Glyph *b);

	static std::vector<int> StringToUnicodes (std::string text);

	Rectangle GetTextBoundingBox (std::vector<int> &characters, TextStyle &style);

	std::vector<Rectangle> GetLinesBoundingBox (std::vector<int> &characters,
			TextStyle &style);

	std::vector<int> WrapText (std::vector<int> text, TextStyle style);
};

}	// namespace Zen
