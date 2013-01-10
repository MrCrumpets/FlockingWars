/*
 * text.h
 *
 *  Created on: 2012-11-26
 *      Author: Nathan
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "GL/glew.h"
#include <sstream>

struct Glyph{
	float w;
	float h;
	float ls;//Left Spacing
	float rs;//Right Spacing
	float spacing;
	GLuint dl;
};

void initFont(Glyph*, int);

float stringHeight(std::string ch, float size);

float stringWidth(std::string ch, float size);

void renderString(std::string ch, float x, float y, float z, float size);

void renderNum(int number, float x, float y, float z, float size);

#endif /* TEXT_H_ */
