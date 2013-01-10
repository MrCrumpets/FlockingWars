#ifndef INIT_H_
#define INIT_H_

#include "GL/glew.h"
#include "SDL/SDL_ttf.h"
#include <string>
#include <iostream>
#include "text.h"

using std::cout;
using std::endl;

/*
 * Load Image method. Not used, but keeping it for reference.
 *
 */
GLuint loadTexture(std::string filename);

bool init_GL(int width, int height);

void generateGlyphs(TTF_Font* mainFont, int numChars, Glyph* glyphs);

#endif
