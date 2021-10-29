/*
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "text_manager.hpp"
#include "../window/window.hpp"
#include "../renderer/renderer.hpp"
#include "../scale/scale_manager.hpp"
#include "../utils/map/contains.hpp"
#include "../utils/vector/contains.hpp"
#include "../components/text.hpp"
#include "../components/position.hpp"
#include "../components/origin.hpp"
#include "../components/size.hpp"
#include "../display/types/color.hpp"
#include "../display/color.hpp"
#include "../systems/size.hpp"
#include "../systems/alpha.hpp"
#include "../systems/transform_matrix.hpp"
#include "../systems/position.hpp"
#include "../systems/rotation.hpp"
#include "../systems/scroll.hpp"
#include "../systems/scroll_factor.hpp"
#include "../math/rad_to_deg.hpp"
#include <algorithm>
#include <set>

// Padding to use between glyphs on the font atlas cache
#define GLYPH_PADDING 6
#define GLYPH_PADDING_X GLYPH_PADDING
#define GLYPH_PADDING_Y GLYPH_PADDING

namespace Zen {

extern entt::registry g_registry;
extern Window g_window;
extern ScaleManager g_scale;
extern Renderer g_renderer;

TextManager::~TextManager ()
{
	// Free all the loaded fonts
	for (auto font : fonts) {
		FT_Face face = font.second;
		FT_Done_Face(face);
	}

	// Free the library instance
	FT_Done_FreeType(ft);

	// Free all atlas surfaces and textures
	for (auto atlas : atlasList) {
		g_renderer.deleteTexture(atlas->texture);
		SDL_FreeSurface(atlas->surface);
	}
}

void TextManager::boot ()
{
	// Create a new library
	if (FT_Init_FreeType(&ft)) {
		MessageError("FREETYPE: Could not init FreeType Library");
		return ;
	}
}

void TextManager::addFont (std::string key, std::string path)
{
	if (!Contains(fonts, key)) {
		// Load font
		FT_Face face;
		if (FT_New_Face(ft, path.c_str(), 0, &face)) {
			MessageError("FREETYPE: Failed to load font");
			return;
		}

		FT_Select_Charmap(face, FT_ENCODING_UNICODE);

		fonts[key] = face;
	}
}

int TextManager::scanText (Entity text_)
{
	auto text = g_registry.try_get<Components::Text>(text_);
	ZEN_ASSERT(text, "The entity has no 'Text' component.");

	text->lastFontSize = text->fontSize;

	// Get all unique characters that appeared in the text
	std::vector<std::uint32_t> charactersCodes = stringToUnicodes(text->text);
	std::set<std::uint32_t> characters
		(charactersCodes.begin(), charactersCodes.end());

	// Get glyph data for this style
	auto &data = glyphCache
		[text->style.fontFamily]
		[text->fontSize]
		[text->style.decoration]
		[text->style.outline];

	// Get new characters not yet cached with the given style configuration
	std::vector<std::uint32_t> newCharacters;
	for (auto character : characters) {
		if (data.contains(character))
			continue;

		newCharacters.emplace_back(character);
	}

	// Create glyphs for the new characters
	addGlyphs(newCharacters, text->style, text->fontSize);

	// Check and deal with text wrapping
	if (text->style.wrapWidth > 0) {
		charactersCodes = wrapText(charactersCodes, text->style, text->fontSize);
	}

	// Update the formated text content
	text->content = charactersCodes;

	// Update the text's information (size/bounding box)
	Rectangle bbox = getTextBoundingBox(charactersCodes, text->style,
			text->fontSize);
	SetSize(text_, bbox.width / g_scale.displayScale.x,
			bbox.height / g_scale.displayScale.y);

	return newCharacters.size();
}

/**
 * Calculates the closest multiple of `n` to `x`.
 */
static int ClosestMultiple (const int n, int x)
{
	if (x < n)
		return n;

	x += n / 2;
	x -= x % n;

	return x;
}

void TextManager::preBatch (Entity text_)
{
	auto text = g_registry.try_get<Components::Text>(text_);
	ZEN_ASSERT(text, "The entity has no 'Text' component.");

	if (g_scale.scaleMode != SCALE_MODE::RESIZE &&
			text->lastDisplayScale != g_scale.displayScale.x
	) {
		text->fontSize = text->style.fontSize * g_scale.displayScale.x;

		if (text->style.baseSize > 0) {
			text->fontSize =
				ClosestMultiple(text->style.baseSize, text->fontSize);
		}

		// Save the current display size
		text->lastDisplayScale = g_scale.displayScale.x;

		if (text->fontSize != text->lastFontSize)
			scanText(text_);
	}
}

void TextManager::addGlyphs (std::vector<std::uint32_t> characters, TextStyle style,
		int fontSize)
{
	// Check if the requested font is already loaded
	if (!fonts.contains(style.fontFamily)) {
		MessageError("There are no loaded fonts with the key: ", style.fontFamily);
		return;
	}

	if (fontSize < 0)
		fontSize = style.fontSize;

	FT_Face face = fonts[style.fontFamily];

	// Get the font atlas (Created automatically if non existent)
	FontAtlasData &atlas = fontsAtlas
		[style.fontFamily]
		[fontSize]
		[style.decoration]
		[style.outline];

	// Create a surface if this atlas is new
	if (!atlas.surface) {
		atlas.surface = SDL_CreateRGBSurface(
				0,
				atlas.width,
				atlas.height,
				32,
				0xff,
				0xff00,
				0xff0000,
				0xff000000
		);

		atlas.index = atlasList.size();
		atlasList.emplace_back(&atlas);
	}

	// Auto width, set height
	FT_Set_Pixel_Sizes(face, 0, fontSize);

	// Save the line height of this font
	if (atlas.lineSpacing < 0)
		atlas.lineSpacing = face->size->metrics.height / 64;

	// Create a glyph for each character
	std::vector<Glyph*> glyphs;

	for (std::uint32_t character : characters) {
		Glyph &glyph = glyphCache
			[style.fontFamily]
			[fontSize]
			[style.decoration]
			[style.outline]
			[character];

		// Load character glyph
		if (FT_Load_Char(face, character, FT_LOAD_DEFAULT)) {
			MessageError("FREETYPE: Failed to load glyph");
			return;
		}

		// Get glyph attributes
		glyph.cacheX = 0;
		glyph.cacheY = 0;
		glyph.cacheW = face->glyph->bitmap.width;
		glyph.cacheH = face->glyph->bitmap.rows;

		glyph.advanceX = face->glyph->advance.x / 64;
		glyph.advanceY = face->glyph->advance.y / 64;
		glyph.bearingX = face->glyph->metrics.horiBearingX / 64;
		glyph.bearingY = face->glyph->metrics.horiBearingY / 64;

		glyph.ascender = face->size->metrics.ascender / 64;
		glyph.descender = face->size->metrics.descender / 64;

		glyphs.push_back(&glyph);
	}

	// Pack the glyphs on the atlas
	int resizeMultiplier = packGlyphs(&glyphs, &atlas);

	// Resize the surface if necessary
	if (resizeMultiplier > 1) {
		// Create a new surface larger than the older
		SDL_Surface *newAtlas = SDL_CreateRGBSurface(
				0,
				atlas.surface->w * resizeMultiplier,
				atlas.surface->h * resizeMultiplier,
				atlas.surface->format->BitsPerPixel,
				atlas.surface->format->Rmask,
				atlas.surface->format->Gmask,
				atlas.surface->format->Bmask,
				atlas.surface->format->Amask
		);

		// Copy the content of the older surface to the new one
		SDL_SetSurfaceBlendMode(atlas.surface, SDL_BlendMode::SDL_BLENDMODE_BLEND);
		SDL_Rect dstRect {0, 0, atlas.surface->w, atlas.surface->h};
		if (SDL_BlitSurface(atlas.surface, nullptr, newAtlas, &dstRect)) {
			MessageError("Couldn't copy the old font atlas to the new larger "
					"one: ", SDL_GetError());
		}

		// Delete old surface and replace it with the new one
		SDL_FreeSurface(atlas.surface);

		atlas.surface = newAtlas;
		atlas.width *= resizeMultiplier;
		atlas.height *= resizeMultiplier;
	}

	// Blit the glyphs on the atlas surface
	for (size_t i = 0; i < glyphs.size(); i++) {
		// Load character glyph
		if (FT_Load_Char(face, characters[i], FT_LOAD_RENDER)) {
			MessageError("FREETYPE: Failed to load glyph");
			return;
		}

		// Convert glyph to surface
		SDL_Surface *glyphSurface = SDL_CreateRGBSurfaceFrom(
				face->glyph->bitmap.buffer,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				8,
				face->glyph->bitmap.pitch,
				//0xff,
				//0xff00,
				//0xff0000,
				//0xff000000
				0, 0, 0, 0xff
		);

		// Convert from indexed to RGBA
		SDL_Color colors[256];
		for (int i = 0; i < 256; i++)
			colors[i].r = colors[i].g = colors[i].b = colors[i].a = i;
		SDL_SetPaletteColors(glyphSurface->format->palette, colors, 0, 256);

		// Blit the glyph to the atlas
		auto &glyph = glyphs[i];
		SDL_SetSurfaceBlendMode(glyphSurface, SDL_BlendMode::SDL_BLENDMODE_NONE);
		SDL_Rect dst {glyph->cacheX, glyph->cacheY, glyph->cacheW, glyph->cacheH};
		SDL_BlitSurface(glyphSurface, nullptr, atlas.surface, &dst);

		// Free the glyph surface
		SDL_FreeSurface(glyphSurface);
	}

	// Regenerate the texture and reupload it to the GPU
	if (atlas.texture)
		g_renderer.deleteTexture(atlas.texture);

	// Here min/mag filters do not matter, as glyphs never get scaled, only
	// regenerated at new sizes.
	atlas.texture = g_renderer.createTexture2D(0, GL_LINEAR, GL_LINEAR,
			GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_RGBA, atlas.surface);

	if (!atlas.texture) {
		MessageError("Unable to create a texture from the rendered font atlas!"
				"SDL Error: ", SDL_GetError());
	}
}

/*
void TextManager::render (Entity textEntity, Entity camera)
{
	MessageNote("Implement a text pipeline");
	return;

	auto [text, position, origin, size] = g_registry.try_get<Components::Text,
		 Components::Position, Components::Origin, Components::Size>(textEntity);
	ZEN_ASSERT(text, "The entity has no 'Text' component.");

	double alpha = GetAlpha(camera) * GetAlpha(textEntity);

	if (!alpha)
		// Nothing to see, so abort early
		return;

	auto &camMatrix = tempMatrix1;
	auto &textMatrix = tempMatrix2;

	// Get the glyph atlas for this style
	auto &atlas = fontsAtlas
		[text->style.fontFamily]
		[text->style.fontSize]
		[text->style.decoration]
		[text->style.outline];

	// Text color (Tint)
	Color textColor;
	SetHex(&textColor, text->style.color);
	SDL_SetTextureColorMod(
		atlas.texture,
		textColor.red,
		textColor.green,
		textColor.blue
		);

	// Convert all characters to unicodes
	std::vector<int> characters = StringToUnicodes(text->content);

	// Get the bounding box of each lines of this text object
	std::vector<Rectangle> bboxes = GetLinesBoundingBox(characters, text->style);
	size_t line = 0;

	// Get the widest line
	int largestLineWidth = 0;
	for (Rectangle bbox : bboxes) {
		if (bbox.width > largestLineWidth)
			largestLineWidth = bbox.width;
	}

	// Initial pen position
	int posX = position->x - origin->x * size->width,
		posY = position->y - origin->y * size->height;

	// Transform matrices
	ApplyITRS(&textMatrix,
		posX, posY,
		GetRotation(textEntity),
		1., 1.
	);
	camMatrix = GetTransformMatrix(camera);

	// Scroll factor
	textMatrix.e -= GetScrollX(camera) * GetScrollFactorX(textEntity);
	textMatrix.f -= GetScrollY(camera) * GetScrollFactorY(textEntity);

	// Multiply by the Text matrix
	Multiply(&camMatrix, textMatrix);

	// ScaleManager values
	Math::Vector2 sScale = g_scale.displayScale;
	Math::Vector2 sOffset = g_scale.displayOffset;

	int penX, penY;
	penX = penY = posX = posY = 0;

	// Move the pen to take into account the text align configuration
	switch (text->style.alignment) {
		case TEXT_ALIGNMENT::LEFT:
			penX = posX;
			break;
		case TEXT_ALIGNMENT::RIGHT:
			penX = posX + largestLineWidth - bboxes[0].width;
			break;
		case TEXT_ALIGNMENT::CENTER:
			penX = posX + (largestLineWidth/2.) -
				(bboxes[0].width/2);
			break;
	}

	// Blit each character from the atlas
	for (auto c : characters) {
		// Check if special character
		if (c == '\n') {
				// Increment the line index
				line++;

				// Move down the pen by the line spacing
				if (text->style.lineSpacing >= 0)
					penY += text->style.lineSpacing;
				else
					penY += atlas.lineSpacing;

				// Move the pen to take into account the text align configuration
				switch (text->style.alignment) {
					case TEXT_ALIGNMENT::LEFT:
						penX = posX;
						break;
					case TEXT_ALIGNMENT::RIGHT:
						penX = posX + largestLineWidth - bboxes[line].width;
						break;
					case TEXT_ALIGNMENT::CENTER:
						penX = posX + (largestLineWidth/2.) -
							(bboxes[line].width/2);
						break;
				}

				// Move on to the next character
				continue;
		}

		auto &glyph = glyphCache
			[text->style.fontFamily]
			[text->style.fontSize]
			[text->style.decoration]
			[text->style.outline]
			[c];

		// Taken from this rectangle from the glyph atlas
		SDL_Rect glyphSrc {
			glyph.cacheX,
			glyph.cacheY,
			glyph.cacheW,
			glyph.cacheH
		};

		auto &glyphMatrix = tempMatrix3;
		ApplyITRS(&glyphMatrix,
				penX + glyph.bearingX,
				penY - glyph.bearingY + atlas.lineSpacing - glyph.ascender + 2,
				//			Gives better results with a margin of 2 pixels   ^
				0,
				1., 1.
		);

		auto &viewMatrix = tempMatrix4;
		viewMatrix = camMatrix;
		Multiply(&viewMatrix, glyphMatrix);

		// Decompose the transform matrix
		DecomposedMatrix dm = DecomposeMatrix(viewMatrix);

		SDL_FRect glyphDst;

		// Position
		glyphDst.x = dm.translateX * sScale.x + sOffset.x;
		glyphDst.y = dm.translateY * sScale.y + sOffset.y;

		// Scale
		glyphDst.w = glyph.cacheW * dm.scaleX * sScale.x;
		glyphDst.h = glyph.cacheH * dm.scaleY * sScale.y;

		// Rotation
		float angle = Math::RadToDeg(dm.rotation);

		// Origin
		SDL_FPoint orig {0.f, 0.f};

		// Alpha (Transparency)
		if (alpha < 1.0)
		{
			SDL_SetTextureAlphaMod(
				atlas.texture,
				alpha * 255
				);
		}

		SDL_RenderCopyExF(
				g_window.renderer,
				atlas.texture,
				&glyphSrc,
				&glyphDst,
				angle,
				&orig,
				SDL_FLIP_NONE
				);

		// Move on to the next character
		penX += glyph.advanceX;
	}
}
*/

int TextManager::packGlyphs (std::vector<Glyph*> *glyphs,
		FontAtlasData *atlas)
{
	// Was the size of the atlas texture increased? This is a multiplier of this
	// change:
	// - 1 is no change in size
	// - 2 is size doubled
	// - 3 is size tripled
	// - ...etc
	int sizeIncrease = 1;

	// Sort by a heuristic
	//std::sort(glyphs->begin(), glyphs->end(), SortGlyphsByHeight);

	// Loop over all the glyphs
	for (auto glyph : *glyphs) {
		int yPos = GLYPH_PADDING_Y;
		bool done = false;
		int i = -1;

		while (!done) {
			i++;
			if (i > static_cast<int>(atlas->rows.size() - 1))
				atlas->rows.emplace_back();

			auto &row = atlas->rows[i];

			// If this glyph will go past the width of the atlas then loop around to
			// the next row, using the largest height from the previous row
			if ((row.width + glyph->cacheW + GLYPH_PADDING_X) >
				(atlas->width * sizeIncrease)) {
				yPos += row.largestHeight + GLYPH_PADDING_Y;
				row.locked = true;	// Lock this row's largest height
				continue;
			}

			// If this glyph will go past the height of this row and another
			// row already exists after it, move to the next
			if (row.locked && glyph->cacheH > row.largestHeight) {
				yPos += row.largestHeight + GLYPH_PADDING_Y;
				continue;
			}

			// If we go off the bottom of the atlas, make a new texture with double
			// the dimensions of the present atlas and copy to it current content
			if ((yPos + glyph->cacheH) > (atlas->height * sizeIncrease)) {
				sizeIncrease++;
			}

			// This is the position of the glyph
			glyph->cacheX = row.width + GLYPH_PADDING_X;
			glyph->cacheY = yPos;

			// Update the row's width
			row.width += glyph->cacheW + GLYPH_PADDING_X;

			// Save the largest height of the new row
			if (glyph->cacheH > row.largestHeight)
				row.largestHeight = glyph->cacheH;

			// Success! This glyph is done!
			done = true;
		}
	}

	return sizeIncrease;
}

bool TextManager::sortGlyphsByHeight(Glyph *a, Glyph *b) {
	return (a->cacheH < b->cacheH);
}

std::vector<std::uint32_t> TextManager::stringToUnicodes (std::string text)
{
	std::vector<std::uint32_t> characters;

	for (size_t i = 0; i < text.length();) {
		int cplen = 1;
		if ((text[i] & 0xf8) == 0xf0)
			cplen = 4;
		else if ((text[i] & 0xf0) == 0xe0)
			cplen = 3;
		else if ((text[i] & 0xe0) == 0xc0)
			cplen = 2;

		if ((i + cplen) > text.length())
			cplen = 1;

		std::uint32_t c = 0;
		//for (int j = 0; j < cplen; j++) {
		for (int j = cplen-1; j >= 0; j--) {
			std::uint8_t ch = text[i + j];
			c <<= 8;
			c |= ch;
		}

		characters.emplace_back(c);

		i += cplen;
	}

	return characters;
}

Rectangle TextManager::getTextBoundingBox (std::vector<std::uint32_t> &characters,
		TextStyle &style, int fontSize)
{
	if (fontSize < 0)
		fontSize = style.fontSize;

	Rectangle bbox {0., 0., 0., 0.};
	int lineWidth = 0;

	int lineSpacing;
	if (style.lineSpacing < 0) {
		auto &atlas = fontsAtlas
			[style.fontFamily]
			[fontSize]
			[style.decoration]
			[style.outline];

		lineSpacing = atlas.lineSpacing;
	} else {
		lineSpacing = style.lineSpacing;
	}

	bbox.height = lineSpacing;

	for (int character : characters) {
		if (character == '\n') {
			bbox.height += lineSpacing;

			if (bbox.width < lineWidth)
				bbox.width = lineWidth;

			lineWidth = 0;
		} else {
			auto &glyph = glyphCache
				[style.fontFamily]
				[fontSize]
				[style.decoration]
				[style.outline]
				[character];

			lineWidth += glyph.advanceX;
		}
	}

	// Test width of last line
	if (bbox.width < lineWidth)
		bbox.width = lineWidth;

	/*
	// Adjust bounding box according to the scale mode
	bbox.width /= g_scale.displayScale.x;
	bbox.height /= g_scale.displayScale.y;
	*/

	return bbox;
}

std::vector<Rectangle> TextManager::getLinesBoundingBox (
		std::vector<std::uint32_t> &characters, TextStyle &style, int fontSize)
{
	if (fontSize < 0)
		fontSize = style.fontSize;

	std::vector<Rectangle> linesBbox;

	int lineSpacing;
	if (style.lineSpacing < 0) {
		auto &atlas = fontsAtlas
			[style.fontFamily]
			[fontSize]
			[style.decoration]
			[style.outline];

		lineSpacing = atlas.lineSpacing;
	} else {
		lineSpacing = style.lineSpacing;
	}

	linesBbox.emplace_back();
	linesBbox.back().height = lineSpacing;

	for (auto character : characters) {
		if (character == '\n') {
			linesBbox.emplace_back();
			linesBbox.back().height = lineSpacing;
		} else {
			auto &glyph = glyphCache
				[style.fontFamily]
				[fontSize]
				[style.decoration]
				[style.outline]
				[character];

			linesBbox.back().width += glyph.advanceX;
		}
	}

	return linesBbox;
}

std::vector<std::uint32_t> TextManager::wrapText (std::vector<std::uint32_t> text,
		TextStyle style, int fontSize)
{
	if (style.wrapWidth <= 0)
		return text;

	if (fontSize < 0)
		fontSize = style.fontSize;

	std::vector<std::uint32_t> wrappedText;
	std::vector<std::uint32_t> word;
	int width = 0;
	bool previouslyBlank = false;

	std::vector<std::uint32_t> nonWordCharacters {' ', '\t', '-'};
	std::vector<std::uint32_t> blankCharacters {' ', '\t'};

	auto &glyphData = glyphCache
		[style.fontFamily]
		[fontSize]
		[style.decoration]
		[style.outline];

	for (size_t i = 0; i < text.size(); i++) {
		std::uint32_t character = text[i];

		// Gather characters that form a single word
		if (!Contains(nonWordCharacters, character)) {
			word.emplace_back(character);
			previouslyBlank = false;
		} else {
			if (style.advancedWrap) {
				// Collapse neighboring blanks into a single one
				if (Contains(nonWordCharacters, character) && previouslyBlank)
					continue;

				if (Contains(nonWordCharacters, character))
					previouslyBlank = true;
			}
		}

		// If outside word or done with text
		if (Contains(nonWordCharacters, character) || (i == text.size() - 1)) {
			// If We reach the wrap width, add a new line
			int wordWidth = 0;
			for (int c : word)
				wordWidth += glyphData[c].advanceX;

			if ((wordWidth + width) > style.wrapWidth) {
				wrappedText.emplace_back('\n');
				width = 0;

				// If advanced wrap is on, break word if still larger than
				// wrap width
				if (style.advancedWrap && wordWidth > style.wrapWidth) {
					wordWidth = 0;
					std::vector<std::uint32_t> brokenWord;
					for (int c : word) {
						brokenWord.emplace_back(c);
						wordWidth += glyphData[c].advanceX;

						if (wordWidth > style.wrapWidth) {
							brokenWord.emplace_back('\n');
							wordWidth = 0;
						}
					}
					
					word = brokenWord;
				}
			}

			// Insert word into wrapped text
			wrappedText.insert(wrappedText.end(), word.begin(), word.end());
			width += wordWidth;
			word.clear();
		}

		// Add the pending non word character if any
		if (Contains(nonWordCharacters, character)) {
			wrappedText.emplace_back(character);
			width += glyphData[character].advanceX;
		}
	}

	if (!style.advancedWrap)
		return wrappedText;

	// Remove white spaces around newlines (\n)
	std::vector<std::uint32_t> buffer;
	std::vector<std::uint32_t> blankBuffer;
	bool start = true;
	bool ignore = false;
	for (std::uint32_t character : wrappedText) {
		// Ignore characters from start of text
		if (start) {
			if (Contains(blankCharacters, character))
				continue;
			else
				start = false;
		}

		// Ignore all blank that come after a newline
		if (ignore && Contains(blankCharacters, character))
			continue;

		// Setup blank ignore
		if (character == '\n')
			ignore = true;
		else
			ignore = false;

		// BACK
		if (Contains(blankCharacters, character)) {
			blankBuffer.emplace_back(character);
		} else if (character == '\n') {
			blankBuffer.clear();
			buffer.emplace_back(character);
		} else {
			buffer.insert(buffer.end(), blankBuffer.begin(), blankBuffer.end());
			blankBuffer.clear();
			buffer.emplace_back(character);
		}
	}

	wrappedText = buffer;

	return wrappedText;
}

}	// namespace Zen
