/*
 * Menu.h
 *
 * Implementation of the abstract GameState class. It should be used for fullscreen
 * menus, like the main menu, settings menus.
 *
 * It may make sense to allow certain game states to overlay on each other
 * e.g. have a menu rendering overtop of a paused game. Currently not
 * necessary though.
 */

#ifndef MENU_H_
#define MENU_H_

#include "GameState.h"
#include "GameLevel.h"
#include "../Systems/UserInterface.h"

// See top of file
class Menu: public GameState {
public:

	// All functions documented in GameState.h
	Menu(int width, int height);
	void init();
	void pause() {};
	void resume() {};
	void handleInput(MappedInput& input, int x, int y, bool mouseDown);
	void render();
	GameState* update(float dt);
	virtual ~Menu();

	// Callback for the button which starts a game leve.
	void newGame();
private:
	UserInterface* ui;
	int width, height;
};

#endif /* MENU_H_ */
