/*
 * Entity.h
 *
 *  Created on: May 21, 2010
 *      Author: Nathan
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include "../Util/includes.h"
#include "../Systems/Graphics/Renderer.h"
#include <list>

class Entity {
public:
	Vec3f pos, oldpos, vel, acc, dir, angvel;
	float damage, health, maxHealth;
	unsigned char r, g, b;
	bool dead, explodes;
	unsigned char type;
	unsigned char team;
	float value;
	std::string name;

	std::list<Entity*> entityStack;

	Entity(){
		dead = false;
	}

	Entity(Vec3f _pos){pos = _pos;}
	~Entity(){}

	virtual void die(){dead = true;}
	virtual void interact(Entity* e){}
	virtual void update(float dt){}
	virtual void render(Renderer*){
		int rsize = 2;
		glColor4ub(255, 255, 255, 255);
		glBegin(GL_QUADS);
		glVertex2f(pos.x-rsize, pos.y-rsize);
		glVertex2f(pos.x-rsize, pos.y+rsize);
		glVertex2f(pos.x+rsize, pos.y+rsize);
		glVertex2f(pos.x+rsize, pos.y-rsize);
		glEnd();
	}
	double toRadians(float angle){
		return angle * PIDIV180;
	}
};

#endif /* ENTITY_H_ */
