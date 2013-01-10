/*
 * UserInterface.h
 *
 *  Created on: Feb 7, 2011
 *      Author: Nathan
 */

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#include "../util/includes.h"
#include "boost/signal.hpp"
#include "boost/bind.hpp"
#include <string>
using std::vector;

struct Button;

struct Widget{
	Widget(){}
	Widget(Vec2f _pos, Vec2f _size, Vec3f _colour, float _a){
		pos = _pos;
		size = _size;
		colour = _colour;
		alpha = _a;
		visible = true;
	}
	Vec2f pos;
	Vec2f size;
	Vec3f colour;
	float alpha;
	bool visible;
	vector<Widget *> widgets;

	virtual void draw();
	void fadeTo (float finalAlpha);
	Widget* addWidget(Vec2f pos, Vec2f size, Vec3f colour, float a);
	void addProgressBar(Vec2f pos, Vec2f size, Vec3f colour, float a, float &maxVal,float &var, bool axis);
	Button* addButton(Vec2f _pos, Vec2f _size, Vec3f colour, float a, char* text, float scale);
	void addTextBox(Vec2f _pos, Vec2f _size, Vec3f colour, float a, int &number, float scale);
	void addTextBox(Vec2f _pos, Vec2f _size, Vec3f colour, float a, std::string s, float scale);
	virtual bool checkHover(int mousex, int mousey);
	virtual bool mouseUp();
	virtual void update();
	~Widget();
};

struct ProgressBar:Widget{
	ProgressBar(Vec2f _pos, Vec2f _size, Vec3f _colour, float _a, float &_maxVal, float &_var, bool yaxis):Widget(_pos, _size, _colour, _a){
		maxVal = &_maxVal;
		var = &_var;
		axis = yaxis;
	}
	float* maxVal;
	float* var;
	bool axis;
	void draw();
};

struct TextBox:Widget{
	TextBox( Vec2f _pos, Vec2f _size, Vec3f _colour, float _a, std::string _text, float _scale);
	TextBox(Vec2f _pos, Vec2f _size, Vec3f _colour, float _a, int &_number, float _scale):Widget(_pos, _size, _colour, _a){
		number = &_number;
		scale = _scale;
	}
	int* number;
	std::string text;
	void draw();
	float scale;
};

struct Callback {
	virtual ~Callback(){}
	virtual void operator()() = 0;
};

struct Button:Widget{
	Button(Vec2f _pos, Vec2f _size, Vec3f _colour, float _a, char* _text, float _scale);

	template <class T>
	void hook(void(T::*_callback)(void), T* _object){
		cb = boost::bind(_callback, _object);
	}
	boost::function<void ()> cb;
	bool hover;
	int hoverTime;
	float scale;
	int padding;
	char* text;
	GLuint textDl;
	bool checkHover(int mousex, int mousey);
	bool mouseUp();
	void draw();
};

class UserInterface {
public:
	UserInterface();
	void reset();
	void draw();
	void update();
	void mouseUp();
	Widget* createWidget(Vec2f pos, Vec2f size, Vec3f colour, float a);
	void createProgressBar(Vec2f pos, Vec2f size, Vec3f colour, float a, float &maxVal,float &var, bool axis);
	void createButton(Vec2f pos, Vec2f size, Vec3f colour, float a);
	vector<Widget*> widgets;
	virtual ~UserInterface();
};

#endif /* USERINTERFACE_H_ */
