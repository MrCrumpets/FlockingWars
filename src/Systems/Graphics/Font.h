/*
 * Text.h
 *
 *  Created on: 2013-01-15
 *      Author: Nathan
 */

#ifndef TEXT_H_
#define TEXT_H_

#include <map>
#include <string>
#include "ft2build.h"
#include FT_FREETYPE_H

struct Glyph{
	int adv; // Horizontal offset to next character
	int w, h; // Dimensions of glyph
	int left, top; // Bounds of glyph
	int texWidth, texHeight;
	unsigned int texture;
};

class Font {
public:
	Font(const std::string &fileName);
	Glyph getGlyph(const char c);
	Glyph loadGlyph(char c);
	virtual ~Font();

	// Returns next largest power of two.
	inline int npw2(int a) const {
		int result = 1;
		while(result < a)
			result <<= 1;
		return result;
	}
private:
	FT_Library library;
	FT_Face face;
	std::map<char, Glyph> glyphs;
};

#endif /* TEXT_H_ */
