#pragma once
/*
 * Player.h
 *
 *  Created on: May 25, 2010
 *      Author: Nathan
 */

#include "Entity.h"
#include "Bullet.h"
#include "../Systems/Graphics/Renderer.h"
#include "../Systems/Graphics/Mesh.hpp"
#include "Explosion.h"

class Player : public Entity{
public:
	Player(Renderer *r, glm::vec3, float);
	void render();
	void update(float);
	void accelerate(float speed);
	void rotate(float dir);
	void shoot(void);
	void respawn(glm::vec3);
	void interact(Entity* e);
	void removeBullet(unsigned int);
	bool intersectSphere(float, float, float);
	bool bulletsIntersectS(glm::vec3, glm::vec3, float);
	virtual ~Player();
	float rotspeed;
	float velSpeed;
	float maxVel;
	float maxRotSpeed;
	float gunHeat;
	float maxHeat;
	float gunTimer;
	int bulletRange;
private:
	static const int meshSize = 4;
	int size;
        Mesh _mesh;
};
