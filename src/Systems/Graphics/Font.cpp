/*
 * Font.cpp
 *
 * Uses the FreeType 2 library to load font faces and generates glyphs
 * for use with OpenGL.
 *
 */

#include "Font.h"
#include "SFML/System.hpp"
#include "GL/glew.h"
#include FT_GLYPH_H
#include <vector>
#include <iostream>

Font::Font(const std::string &filename) {
	std::cout << "Loading font " << filename << std::endl;
	if(FT_Init_FreeType(&library) != 0){
		std::cout << "Error during library initialization" << std::endl;
		return;
	}

	if(FT_New_Face(library, filename.c_str(), 0, &face) != 0){
		std::cout << "Could not create font face" << std::endl;
		return;
	}
    FT_Set_Char_Size( face, 16 << 6, 0, 96, 96);

}

Glyph Font::loadGlyph(char c){
	FT_GlyphSlot glyph = face->glyph;
	Glyph g;
    FT_UInt charIndex = FT_Get_Char_Index( face, c );
	int error = FT_Load_Char(face, charIndex, FT_LOAD_RENDER);
	if(error)
		return g;
	FT_Bitmap& bitmap = glyph->bitmap;
	int width = bitmap.width;
	int height = bitmap.rows;
	int texWidth = npw2(width);
	int texHeight = npw2(height);
	g.adv = glyph->advance.x >> 6;
	g.left = glyph->bitmap_left;
	g.top = glyph->bitmap_top;
	g.w = width;
	g.h = height;
	g.texWidth = texWidth;
	g.texHeight = texHeight;

	if(width > 0 && height > 0){
		std::vector<GLubyte> buf;
		buf.resize(texWidth * texHeight * 4, 255);
		const sf::Uint8* pixels = bitmap.buffer;
		for(int i = 0; i < texHeight; ++i){
			for(int j = 0; j < texWidth; ++j){
				buf[(j + i*texWidth)*4] = (i >= height || j >= width) ? 0 : pixels[i*texWidth + j];
			}
			pixels += bitmap.pitch;
		}
		GLuint tex = static_cast<GLuint>(g.texture);
		g.texture = 0;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, g.texWidth, g.texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}
	return g;
}

Glyph Font::getGlyph(const char c){
	std::map<char, Glyph>::iterator it = glyphs.find(c);
	if(it != glyphs.end()){
		return it->second;
	}
	else{
		Glyph g = loadGlyph(c);
		return (glyphs[c] = g);
	}
}

Font::~Font() {
	FT_Done_Face(face);
	FT_Done_FreeType(library);
}
