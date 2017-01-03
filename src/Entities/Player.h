#pragma once
/*
 * Player.h
 *
 *  Created on: May 25, 2010
 *      Author: Nathan
 */

#include "Entity.h"
#include "Bullet.h"
#include "../Util/includes.h"
#include "../Systems/Graphics/Renderer.h"
#include "../Systems/Graphics/Mesh.hpp"
#include "Explosion.h"

class Player : public Entity{
public:
	Player(Renderer *r, Vec3f, float);
	void render();
	void update(float);
	void accelerate(float speed);
	void rotate(float dir);
	void shoot(void);
	void respawn(Vec3f);
	void interact(Entity* e);
	void removeBullet(unsigned int);
	bool intersectSphere(float, float, float);
	bool bulletsIntersectS(Vec3f, Vec3f, float);
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
