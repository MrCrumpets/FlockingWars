/*
 * Init.h
 *
 * My attempt to remove some of the kind of boilerplate initialisation
 * code from FlockingMain.cpp. There isn't all that much here right now,
 * but it does make the other file a lot cleaner.
 */
#ifndef INIT_H_
#define INIT_H_

#include "GL/glew.h"
#include <string>
#include <iostream>

using std::cout;
using std::endl;

/*
 * Load Image method. Not used, but keeping it for reference.
 *
 */
GLuint loadTexture(std::string filename);

bool init_GL(int width, int height);

//void generateGlyphs(TTF_Font* mainFont, int numChars, Glyph* glyphs);

#endif
