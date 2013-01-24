/*
 * Flock.h
 *
 * This was originally (way back when) a class used to simulate a flocking
 * algorithm. My current plan is to use it as a "squad" class.
 *
 * It will handle orders/commands and formations for a number of entities.
 *
 */
class Spawner;
#ifndef FLOCK_H_
#define FLOCK_H_


#include "Entity.h"
#include "../Util/includes.h"
#include "../Systems/Graphics/Renderer.h"
using std::vector;


// These are tiny little ships that fly around and don't do much else. I
// am not sure how much longer they'll be around before I replace them
// with something more abstract.
class Boid : public Entity {
public:
	Boid();
	Boid(Vec3f pos, int team);
	void update(float dt);
	void interact(Entity* e);
	void render(Renderer*);
	void die();
	virtual ~Boid();
	float minSpeed, maxSpeed, minSq, maxSq;
	GLuint meshDisplayList;
	int rank;
	float fear;
};

/**
 * Groups entities into a squad for convenient ordering and commanding.
 */

class Flock {
public:
	char tag; // For identification (ie. Alpha, Beta)
	Flock();
	void setTag(char t);
	void addEntity(Entity* e);
	void removeEntity(Entity* e);
	void setObjective(Vec3f o);
	bool update(float dt);
	virtual ~Flock(){}
	vector<Entity*> members;
};

#endif /* FLOCK_H_ */
