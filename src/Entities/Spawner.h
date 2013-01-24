/*
 * Spawner.h
 *
 * This entity acts somewhat like the homebase. Again, this is leftover
 * from the old version. I'm not sure what it's purpose will be.
 */
class Flock;

#ifndef SPAWNER_H_
#define SPAWNER_H_

#include "Entity.h"
#include "Flock.h"
#include "../Util/includes.h"
#include "../Systems/Graphics/Renderer.h"
#include <queue>

const int numLines = 10;

class Line : public Entity{
public:
	Line(){}
	Line(Vec3f, Vec3f);
	void render(bool);
	void update(float dt);
	void spawnBoids(){};
	Vec3f center;
	Vec3f constVel;
};

class Spawner : public Entity {
public:
	Spawner(Vec3f, int);
	void render(bool);
	void render(Renderer* r);
	void update(float dt);
	virtual ~Spawner();
	std::vector<Line> lines;
	Vec3f center;
	int timer;
	std::queue<unsigned char> queue;
	float money;
	float moneyRate;
	int lastSpawned;
	Vec3f screenPos;
};

#endif /* SPAWNER_H_ */
