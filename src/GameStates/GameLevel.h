/*
 * GameLevel.h
 *
 *  Created on: 2012-11-17
 *      Author: Nathan
 */

#ifndef GAMELEVEL_H_
#define GAMELEVEL_H_

#include "GameState.h"
#include "../Entities/Entity.h"
#include "../Entities/Spawner.h"
#include "../Entities/Player.h"
#include "../Entities/Flock.h"
#include "../Util/Vec3f.h"
#include "../Systems/InputManager.h"
#include <vector>

using std::vector;

class GameLevel: public GameState {
public:
	GameLevel(int width, int height);
	void pause() {};
	void resume() {};
	void handleInput(MappedInput& input, int x, int y, bool mouseDown);
	void init();
	void render();
	GameState* update(float dt);
	virtual ~GameLevel();
private:
	void renderCursor(Renderer* r);
	bool contains(Vec3f x, Vec3f a, Vec3f b);
	std::vector<Entity*> entities;
	std::vector<Flock*> flocks;
	Player* player;
	Spawner* spawner;
	Vec3f camera;
	GLuint time, offsetUniform;
	int width, height;
};

#endif /* GAMELEVEL_H_ */
