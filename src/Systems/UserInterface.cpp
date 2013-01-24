/*
 * UserInterface.cpp
 *
 *  Created on: Feb 7, 2011
 *      Author: Nathan
 */

#include "UserInterface.h"

UserInterface::UserInterface() {
}

void UserInterface::reset(){
	while(!widgets.empty())
	{
		delete widgets.back();
		widgets.pop_back();
	}
}
void UserInterface::update(){
	for(unsigned int i = 0; i < widgets.size(); i++){
		if(widgets[i]->checkHover(mousex, mousey))
			break;
	}
}
void UserInterface::draw(){
	glLoadIdentity();
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0, uiWidth, 0, uiHeight);
	glDisable(GL_TEXTURE_2D);
	for(unsigned int i = 0; i < widgets.size(); i++){
		if(widgets[i]->visible)
			widgets[i]->draw();
	}
}

void UserInterface::mouseUp(){
	for(unsigned int i = 0; i < widgets.size(); i++){
		if(widgets[i]->mouseUp())
			break;
	}
}

Widget* UserInterface::createWidget(Vec2f _pos, Vec2f _size, Vec3f colour, float a){
	Widget* w = new Widget(_pos, _size, colour/255.f, a/255.0f);
	widgets.push_back(w);
	return w;
}

void UserInterface::createProgressBar(Vec2f _pos, Vec2f _size, Vec3f colour, float a, float &maxVal, float &var, bool yaxis){
	ProgressBar* pb = new ProgressBar(_pos, _size, colour/255.f, a/255.0f, maxVal, var, yaxis);
	widgets.push_back(pb);
}

UserInterface::~UserInterface() {}

TextBox::TextBox( Vec2f _pos, Vec2f _size, Vec3f _colour, float _a, std::string _text, float _scale):Widget(_pos, _size, _colour, _a){
	text = _text;
	scale = _scale;
//	size.x = stringWidth(text, scale);
	pos.x -= size.x/2;
}

void TextBox::draw(){
	glColor4f(colour.x, colour.y, colour.z, alpha);
//	if(text != "")
//		renderString(text, pos.x, pos.y, 0, scale);
//	else
//		renderNum(*number, pos.x, pos.y, 0, scale);
}

Button::Button(Vec2f _pos, Vec2f _size, Vec3f _colour, float _a, char* _text, float _scale):Widget(_pos, _size, _colour, _a){
	text = _text;
	hover = false;
	hoverTime = 0;
	scale = _scale;
	padding = 10;
	//size.x = stringWidth(text, scale)+2*padding;
	//size.y = stringHeight(text, scale)+padding;
	pos.x -= size.x/2;
//	textDl = glGenLists(1);
//	glNewList(textDl, GL_COMPILE);
//	renderString(text, pos.x+padding, pos.y+size.y/2-padding, 0, scale);
//	glEndList();
}
void Button::draw(){
	if(hover && hoverTime < 30)
		hoverTime++;
	else if(!hover && hoverTime > 0)
		hoverTime*= 0.5;
	float halpha = (sin(((float)hoverTime/30)*PI/2)*0.4+0.3)*alpha;
	float x = pos.x;
	float y = pos.y;
	glPushMatrix();
	glCallList(textDl);
	glPopMatrix();
	glColor4ub(255, 255, 255, 255);
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, buttonTex );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glColor4f(colour.x, colour.y, colour.z, halpha);
	glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex3f(x, y, 0);
		glTexCoord2i(0, 1); glVertex3f(x, pos.y+size.y, 0);
		glTexCoord2i(1, 1); glVertex3f(x+size.x, pos.y+size.y, 0);
		glTexCoord2i(1, 0); glVertex3f(x+size.x, y, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void ProgressBar::draw(){
	glColor4f(colour.x, colour.y, colour.z, alpha);
	GLdouble z = 0, x = pos.x, y = pos.y;
	if(axis)//If animation should be on y-axis
	{
		glBegin(GL_QUADS);
			glVertex3f(x, y, 0);
			glVertex3f(x, y+(size.y*((float)*var/(float)*maxVal)), 0);
			x += size.x;
			glVertex3f(x, y+(size.y*((float)*var/(float)*maxVal)), 0);
			glVertex3f(x, y, 0);
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
			glVertex3f(pos.x, pos.y, 0);
			glVertex3f(pos.x, pos.y+size.y, 0);
			glVertex3f(pos.x+(size.x*((float)*var/(float)*maxVal)), pos.y+size.y, 0);
			glVertex3f(pos.x+(size.x*((float)*var/(float)*maxVal)), pos.y, 0);
		glEnd();
	}
}

bool Button::mouseUp(){
	if(mousex > pos.x && mousex < pos.x+size.x && mousey > pos.y && mousey < pos.y+size.y){
		cb();
		return true;
	}
	return false;
}

bool Button::checkHover(int mousex, int mousey){
	if(mousex > pos.x && mousex < pos.x+size.x && mousey > pos.y && mousey < pos.y+size.y){
		hover = true;
		return true;
	}
	else
		hover = false;
	return false;
}

bool Widget::mouseUp(){
	if(mousex > pos.x && mousex < pos.x+size.x && mousey > pos.y && mousey < pos.y+size.y){
		for(unsigned int i = 0; i < widgets.size(); i++){
			if(widgets[i]->mouseUp())
				return true;
		}
		return true;
	}
	else
		return false;
}

bool Widget::checkHover(int mousex, int mousey){
	if(mousex > pos.x && mousex < pos.x+size.x && mousey > pos.y && mousey < pos.y+size.y){
		for(unsigned int i = 0; i < widgets.size(); i++){
			widgets[i]->checkHover(mousex, mousey);
		}
		return true;
	}
	else
		return false;
}

void Widget::update(){
	for(unsigned int i = 0; i < widgets.size(); i++){
		widgets[i]->update();
	}
}

void Widget::draw(){
	glColor4f(colour.x, colour.y, colour.z, alpha);
	glBegin(GL_QUADS);
		glVertex3f(pos.x, pos.y, 0);
		glVertex3f(pos.x, pos.y+size.y, 0);
		glVertex3f(pos.x+size.x, pos.y+size.y, 0);
		glVertex3f(pos.x+size.x, pos.y, 0);
	glEnd();
	for(unsigned int i = 0; i < widgets.size(); i++){
		widgets[i]->draw();
	}
}

Widget* Widget::addWidget(Vec2f _pos, Vec2f _size, Vec3f colour, float a){
	Widget* w = new Widget(pos + _pos, _size, colour/255.f, a/255.0f);
	widgets.push_back(w);
	return w;
}

void Widget::addTextBox(Vec2f _pos, Vec2f _size, Vec3f colour, float a, std::string text, float scale){
	TextBox* tb = NULL;
	tb = new TextBox(pos + _pos, _size, colour/255.f, a/255.0f, text, scale);
	widgets.push_back(tb);
}

void Widget::addTextBox(Vec2f _pos, Vec2f _size, Vec3f colour, float a, int &number, float scale){
	TextBox* tb = NULL;
	tb = new TextBox(pos + _pos, _size, colour/255.0f, a/255.0f, number, scale);
	widgets.push_back(tb);
}


void Widget::addProgressBar(Vec2f _pos, Vec2f _size, Vec3f colour, float a, float &maxVal, float &var, bool yaxis){
	ProgressBar* pb = new ProgressBar(pos + _pos, _size, colour/255.f, a/255.0f, maxVal, var, yaxis);
	widgets.push_back(pb);
}

Button* Widget::addButton(Vec2f _pos, Vec2f _size, Vec3f colour, float a, char* text, float scale){
	Button* pb = new Button(pos + _pos, _size, colour/255.0f, a/255.0f, text, scale);
	widgets.push_back(pb);
	return pb;
}

Widget::~Widget(){
	while(!widgets.empty())
	{
		delete widgets.back();
		widgets.pop_back();
	}
}
