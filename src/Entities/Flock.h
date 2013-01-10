/*
 * Flock.h
 *
 *  Created on: Jan 23, 2011
 *      Author: Nathan
 */
class Spawner;
#ifndef FLOCK_H_
#define FLOCK_H_


#include "Entity.h"
#include "../Util/includes.h"
#include "../Systems/Graphics/Renderer.h"
using std::vector;



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
