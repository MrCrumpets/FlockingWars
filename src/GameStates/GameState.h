/*
 * GameState.h
 *
 *  Created on: 2012-11-17
 *      Author: Nathan
 */

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "../Systems/InputManager.h"

class GameState {
public:
	GameState(){};
	virtual void init() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual void handleInput(MappedInput& input, int x, int y, bool mouseDown) = 0;
	virtual GameState* update(float dt) = 0;
	virtual void render() = 0;
	virtual ~GameState(){};
};

#endif /* GAMESTATE_H_ */
