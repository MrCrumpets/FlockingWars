/*
 * Menu.cpp
 *
 *  Created on: 2012-11-18
 *      Author: Nathan
 */

#include "Menu.h"

bool startGame = false;

Menu::Menu(int width, int height) {
	this->width = width;
	this->height = height;
}

void Menu::init(){
	ui = new UserInterface();
	Widget* menu = ui->createWidget(glm::vec2(width/2, height/2), glm::vec2(128, 64), glm::vec3(150, 150, 150), 1.f);
	Button* newGameButton = menu->addButton(glm::vec2(0, 0), glm::vec2(128, 64), glm::vec3(129, 204, 60), 255, "Start", 0.4);
	newGameButton->hook(&Menu::newGame, this);
}

void Menu::newGame(){
	startGame = true;
}

GameState* Menu::update(float dt){
	if(true){
		return new GameLevel(width, height);
	}

	ui->update();
	return this;
}

void Menu::render(){
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	ui->draw();
}

void Menu::handleInput(MappedInput& inputs, int x, int y, bool _mouseDown){
	if(inputs.actions.find("left_mouse_down") != inputs.actions.end()){
		ui->mouseUp();
	}
}

Menu::~Menu() {
	delete ui;
}

