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


enum class EntityType {
    Ship,
    Spawner,
    Boid,
    Bullet,
    AttackBoid,
    Explosion
};

class Entity {
    protected:
        Renderer *renderer;
    public:
	glm::vec3 pos, oldpos, vel, acc, dir, angvel;
        glm::vec4 color;
	float damage, health, maxHealth;
	bool dead, explodes, selected;
	EntityType type;
        std::string team;
	float value;
	std::string name;

	std::list<Entity*> entityStack;

	Entity() : dead(false), explodes(false), selected(false) { }

	Entity(Renderer *r, const glm::vec3 &_pos) : renderer(r), pos(_pos) {}
	virtual ~Entity(){}

	virtual void die(){ dead = true; }
	virtual void interact(Entity* e) = 0;
	virtual void update(float dt) = 0;
	virtual void render() = 0;
	double toRadians(float angle){
		return angle * PIDIV180;
	}
};

#endif /* ENTITY_H_ */
