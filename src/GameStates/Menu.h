/*
 * Menu.h
 *
 *  Created on: 2012-11-18
 *      Author: Nathan
 */

#ifndef MENU_H_
#define MENU_H_

#include "GameState.h"
#include "GameLevel.h"
#include "../Systems/UserInterface.h"

class Menu: public GameState {
public:
	Menu(int width, int height);
	void init();
	void pause() {};
	void resume() {};
	void handleInput(MappedInput& input, int x, int y, bool mouseDown);
	void render();
	GameState* update(float dt);
	virtual ~Menu();

	void newGame();
private:
	UserInterface* ui;
	int width, height;
};

#endif /* MENU_H_ */
