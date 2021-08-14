/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "text_manager.hpp"
#include "../window/window.hpp"
#include "../utils/map/contains.hpp"
#include "../components/text.hpp"
#include "../components/position.hpp"
#include <algorithm>
#include <set>
#include "../display/types/color.hpp"
#include "../display/color.hpp"

// Padding to use between glyphs on the font atlas cache
#define GLYPH_PADDING 6
#define GLYPH_PADDING_X GLYPH_PADDING
#define GLYPH_PADDING_Y GLYPH_PADDING

namespace Zen {

extern entt::registry g_registry;
extern Window g_window;

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
		SDL_DestroyTexture(atlas->texture);
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

		fonts[key] = face;

		MessageNote("Added font: ", key, " from file :", path);
	}
}

int TextManager::scanText (Entity text_)
{
	auto text = g_registry.try_get<Components::Text>(text_);
	ZEN_ASSERT(text, "The entity has no 'Text' component.");

	// Get all unique characters that appeared in the text
	std::vector<int> charactersCodes = StringToUnicodes(text->text);
	std::set<int> characters (charactersCodes.begin(), charactersCodes.end());

	// Get glyph data for this style
	auto &data = glyphCache
		[text->style.fontFamily]
		[text->style.fontSize]
		[text->style.color]
		[text->style.decoration]
		[text->style.outline];

	// Get new characters not yet cached with the given style configuration
	std::vector<int> newCharacters;
	for (auto character : characters) {
		if (Contains(data, character))
			continue;

		newCharacters.emplace_back(character);
	}

	// Create glyphs for the new characters
	addGlyphs(newCharacters, text->style);

	// Check and deal with text wrapping
	if (text->style.wrapWidth > 0) {
		auto wrappedText = WrapText(charactersCodes, text->style);
		text->text = std::string(wrappedText.begin(), wrappedText.end());
	}

	return newCharacters.size();
}

void TextManager::addGlyphs (std::vector<int> characters, TextStyle style)
{
	// Check if the requested font is already loaded
	if (!Contains(fonts, style.fontFamily))
	{
		MessageError("There are no loaded fonts with the key: ", style.fontFamily);
		return;
	}

	FT_Face face = fonts[style.fontFamily];

	// Get the font atlas (Created automatically if non existent)
	FontAtlasData &atlas = fontsAtlas
		[style.fontFamily]
		[style.fontSize]
		[style.color]
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
	}

	atlas.index = atlasList.size();
	atlasList.emplace_back(&atlas);

	// Auto width, set height
	FT_Set_Pixel_Sizes(face, 0, style.fontSize);

	// Save the line height of this font
	if (atlas.lineSpacing < 0)
		atlas.lineSpacing = face->size->metrics.height / 64;

	// Create a glyph for each character
	std::vector<Glyph*> glyphs;

	for (int character : characters) {
		Glyph &glyph = glyphCache
			[style.fontFamily]
			[style.fontSize]
			[style.color]
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
			MessageError("Couldn't copy the old font atlas to the new larger one: ", SDL_GetError());
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

		// Text color
		Color textColor;
		SetHex(&textColor, style.color);

		// Convert from indexed to RGBA
		SDL_Color colors[256];
		for (int i = 0; i < 256; i++) {
			colors[i].r = colors[i].g = colors[i].b = colors[i].a = i;

			colors[i].r *= textColor.gl[0];
			colors[i].g *= textColor.gl[1];
			colors[i].b *= textColor.gl[2];
			colors[i].a *= textColor.gl[3];
		}
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
		SDL_DestroyTexture(atlas.texture);

	atlas.texture = SDL_CreateTextureFromSurface(g_window.renderer, atlas.surface);

	if (!atlas.texture)
		MessageError("Unable to create a texture from the rendered font atlas! SDL Error: ", SDL_GetError());
}

void TextManager::render (Entity textEntity)
{
	auto [text, position] = g_registry.try_get<Components::Text,
		 Components::Position>(textEntity);
	ZEN_ASSERT(text, "The entity has no 'Text' component.");

	// Get the glyph atlas for this style
	auto &atlas = fontsAtlas
		[text->style.fontFamily]
		[text->style.fontSize]
		[text->style.color]
		[text->style.decoration]
		[text->style.outline];
	SDL_SetTextureBlendMode(atlas.texture, SDL_BLENDMODE_BLEND);

	// Convert all characters to unicodes
	std::vector<int> characters = StringToUnicodes(text->text);

	// Get the bounding box of each lines of this text object
	std::vector<Rectangle> bboxes = GetLinesBoundingBox(characters, text->style);
	size_t line = 0;

	// Get the widest line
	int largestLineWidth = 0;
	for (Rectangle bbox : bboxes) {
		if (bbox.width > largestLineWidth)
			largestLineWidth = bbox.width;
	}

	int posX = position->x, posY = position->y;
	int penX = posX, penY = posY;

	// Setup initial position of the pen
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
			[text->style.color]
			[text->style.decoration]
			[text->style.outline]
			[c];

		// Taken from this rectangle from the glyph atlas
		SDL_Rect glyphSrc {glyph.cacheX, glyph.cacheY, glyph.cacheW, glyph.cacheH};

		int x = penX + glyph.bearingX,
			y = penY - glyph.bearingY;
		// Drawn to this rectangle on the screen
		SDL_Rect glyphDst {x, y, glyph.cacheW, glyph.cacheH};

		SDL_RenderCopyEx(
				g_window.renderer,
				atlas.texture,
				&glyphSrc,
				&glyphDst,
				0,
				nullptr,
				SDL_FLIP_NONE
				);

		penX += glyph.advanceX;
	}
}

// PRIVATE

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
		int yPos = 0;
		bool done = false;
		int i = -1;

		while (!done) {
			i++;
			if (i > (int)(atlas->rows.size() - 1))
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
			glyph->cacheX = row.width;
			glyph->cacheY = yPos;

			// Update the row's width
			row.width += glyph->cacheW;

			// Save the largest height of the new row
			if (glyph->cacheH > row.largestHeight)
				row.largestHeight = glyph->cacheH;

			// Success! This glyph is done!
			done = true;
		}
	}

	return sizeIncrease;
}

bool TextManager::SortGlyphsByHeight(Glyph *a, Glyph *b) {
	return (a->cacheH < b->cacheH);
}

std::vector<int> TextManager::StringToUnicodes (std::string text)
{
	std::vector<int> characters;

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

		unsigned int c = 0;
		for (int j = 0; j < cplen; j++) {
			unsigned char ch = text[i + j];
			c <<= 8;
			c |= ch;
		}

		characters.emplace_back(c);

		i += cplen;
	}

	return characters;
}

Rectangle TextManager::GetTextBoundingBox (std::vector<int> &characters, TextStyle &style)
{
	Rectangle bbox {0., 0., 0., 0.};
	int lineWidth = 0;

	int lineSpacing;
	if (style.lineSpacing < 0) {
		auto &atlas = fontsAtlas
			[style.fontFamily]
			[style.fontSize]
			[style.color]
			[style.decoration]
			[style.outline];

		lineSpacing = atlas.lineSpacing;
	} else {
		lineSpacing = style.lineSpacing;
	}

	bbox.height = lineSpacing;

	for (auto character : characters) {
		if (character == '\n') {
			bbox.height += lineSpacing;

			if (bbox.width < lineWidth)
				bbox.width = lineWidth;

			lineWidth = 0;
		} else {
			auto &glyph = glyphCache
				[style.fontFamily]
				[style.fontSize]
				[style.color]
				[style.decoration]
				[style.outline]
				[character];

			lineWidth += glyph.advanceX;
		}
	}

	return bbox;
}

std::vector<Rectangle> TextManager::GetLinesBoundingBox (
		std::vector<int> &characters, TextStyle &style)
{
	std::vector<Rectangle> linesBbox;

	int lineSpacing;
	if (style.lineSpacing < 0) {
		auto &atlas = fontsAtlas
			[style.fontFamily]
			[style.fontSize]
			[style.color]
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
				[style.fontSize]
				[style.color]
				[style.decoration]
				[style.outline]
				[character];

			linesBbox.back().width += glyph.advanceX;
		}
	}

	return linesBbox;
}

std::vector<int> TextManager::WrapText (std::vector<int> text, TextStyle style)
{
	if (style.wrapWidth <= 0)
		return text;

	std::vector<int> wrappedText;
	std::vector<int> word;
	int width = 0;

	bool previouslySpace = false;

	for (auto character : text) {
		// Gather characters that form a single word
		if (character != ' ' && character != '\t') {
			word.emplace_back(character);
			continue;
		}

		auto &glyphData = glyphCache
			[style.fontFamily]
			[style.fontSize]
			[style.color]
			[style.decoration]
			[style.outline];

		/*
		if (style.advancedWrap) {
			// Collapse neighboring spaces into a single one
			if (character == ' ' && previouslySpace)
				continue;

			if (character == ' ')
				previouslySpace = true;
			else
				previouslySpace = false;

			// Remove white spaces at the beginning of lines

			// Remove white spaces at the end of lines

			// Break long words
		}
		*/

		// If We reach the wrap width, add a new line
		int wordWidth = 0;
		for (auto& c : word)
			wordWidth += glyphData[c].advanceX;

		if ((wordWidth + width) > style.wrapWidth) {
			wrappedText.emplace_back('\n');
			width = 0;
		}

		wrappedText.insert(wrappedText.end(), word.begin(), word.end());
		width += wordWidth;
		word.clear();

		// Add the pending non word character
		wrappedText.emplace_back(character);
		width += glyphData[character].advanceX;
	}

	return wrappedText;
}

}	// namespace Zen
