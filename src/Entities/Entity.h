/*
 * Entity.h
 *
 * Abstract base class for all in-game objects. A lot of the member variables
 * in this class are now either completely unnecessary or somewhat redundant
 * so I need to come back and clean this up.
 *
 * I'm hoping eventually to have most entities be described in XML files unless
 * it requires something more fancy.
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include "../Util/includes.h"
#include "../Systems/Graphics/Renderer.h"
#include <list>

class Entity {
    protected:
        Renderer *renderer;
    public:
	glm::vec3 pos, oldpos, vel, acc, dir, angvel;
        glm::vec3 color;
	float damage, health, maxHealth;
	bool dead, explodes, selected;
	unsigned char type;
	unsigned char team;
	float value;
	std::string name;

	std::list<Entity*> entityStack;

	Entity(){
		dead = false;
	}

	Entity(Renderer *r, glm::vec3 _pos) : renderer(r), pos(_pos) {}
	virtual ~Entity(){}

	virtual void die(){dead = true;}
	virtual void interact(Entity* e){}
	virtual void update(float dt){}

	// Default renderer for debugging purposes. Draws a white square
	// at the position of the entity.
	virtual void render(){
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
