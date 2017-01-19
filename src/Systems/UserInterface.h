/*
 * UserInterface.h
 *
 *  Created on: Feb 7, 2011
 *      Author: Nathan
 */

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <functional>
#include <string>
#include <vector>

struct Button;

struct Widget{
	Widget(){}
	Widget(glm::vec2 _pos, glm::vec2 _size, glm::vec3 _colour, float _a){
		pos = _pos;
		size = _size;
		colour = _colour;
		alpha = _a;
		visible = true;
	}
	glm::vec2 pos;
	glm::vec2 size;
	glm::vec3 colour;
	float alpha;
	bool visible;
        std::vector<Widget *> widgets;

	virtual void draw();
	void fadeTo (float finalAlpha);
	Widget* addWidget(glm::vec2 pos, glm::vec2 size, glm::vec3 colour, float a);
	void addProgressBar(glm::vec2 pos, glm::vec2 size, glm::vec3 colour, float a, float &maxVal,float &var, bool axis);
	Button* addButton(glm::vec2 _pos, glm::vec2 _size, glm::vec3 colour, float a, std::string text, float scale);
	void addTextBox(glm::vec2 _pos, glm::vec2 _size, glm::vec3 colour, float a, int &number, float scale);
	void addTextBox(glm::vec2 _pos, glm::vec2 _size, glm::vec3 colour, float a, std::string s, float scale);
	virtual bool checkHover(const glm::vec2 &mouse);
	virtual bool mouseUp(const glm::vec2 &mouse);
	virtual void update();
	~Widget();
};

struct ProgressBar:Widget{
	ProgressBar(glm::vec2 _pos, glm::vec2 _size, glm::vec3 _colour, float _a, float &_maxVal, float &_var, bool yaxis):Widget(_pos, _size, _colour, _a){
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
	TextBox( glm::vec2 _pos, glm::vec2 _size, glm::vec3 _colour, float _a, std::string _text, float _scale);
	TextBox(glm::vec2 _pos, glm::vec2 _size, glm::vec3 _colour, float _a, int &_number, float _scale):Widget(_pos, _size, _colour, _a){
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
	Button(glm::vec2 _pos, glm::vec2 _size, glm::vec3 _colour, float _a, std::string _text, float _scale);

	template <class T>
	void hook(void(T::*_callback)(void), T* _object){
		cb = std::bind(_callback, _object);
	}
	std::function<void ()> cb;
	bool hover;
	int hoverTime;
	float scale;
	int padding;
	std::string text;
	GLuint textDl;
	bool checkHover(const glm::vec2 &mouse);
	bool mouseUp(const glm::vec2 &mouse);
	void draw();
};

class UserInterface {
public:
	UserInterface();
	void reset();
	void draw();
	void update(const glm::vec2 &mouse);
	void mouseUp(const glm::vec2 &mouse);
	Widget* createWidget(glm::vec2 pos, glm::vec2 size, glm::vec3 colour, float a);
	void createProgressBar(glm::vec2 pos, glm::vec2 size, glm::vec3 colour, float a, float &maxVal,float &var, bool axis);
	void createButton(glm::vec2 pos, glm::vec2 size, glm::vec3 colour, float a);
        std::vector<Widget*> widgets;
	virtual ~UserInterface();
};

#endif /* USERINTERFACE_H_ */
