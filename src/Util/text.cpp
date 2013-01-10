/*
 * text.cpp
 *
 *  Created on: 2012-11-27
 *      Author: Nathan
 */

#include "text.h"


int numChars;
Glyph* glyphs;

void initFont(Glyph* _glyphs, int _numChars){
	glyphs = _glyphs;
	numChars = _numChars;
}

float stringHeight(std::string ch, float size){
	int len = ch.length();
	float h = 0;
	for(int i = 0; i < len;i++){
		Glyph n = glyphs[(int)ch[i]-32];
		h = n.h > h ? n.h: h;
	}
	return h*size;
}

float stringWidth(std::string ch, float size){
	int len = ch.length();
	float x = 0;
	for(int i = 0; i < len;i++){
		Glyph n = glyphs[(int)ch[i]-32];
		x += n.ls*size;
		x += n.rs*size + n.w*size;
	}
	return (x);
}

void renderString(std::string ch, float x, float y, float z, float size){
	glEnable(GL_TEXTURE_2D);
	//glColor4ub(255, 255, 255, 255);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	int len = ch.length();
	glPushMatrix();
	//glTranslatef(x, y, z);
	for(int i = 0; i < len;i++){
		Glyph n = glyphs[(int)ch[i]-32];
		x += n.ls*size;
		glPushMatrix();
		glTranslatef(x, y, z);
		glScalef(size, size, size);
		glCallList(n.dl);
		glPopMatrix();
		x += n.rs*size + n.w*size;
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void renderNum(int number, float x, float y, float z, float size){
	std::stringstream stream;
	stream << number;
	renderString(stream.str(), x, y, z, size);
}
