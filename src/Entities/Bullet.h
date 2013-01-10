/*
 * Bullet.h
 *
 *  Created on: May 28, 2010
 *      Author: Nathan
 */

#ifndef BULLET_H_
#define BULLET_H_

#include "Entity.h"
#include "../Util/includes.h"
#include "../Systems/Graphics/Renderer.h"


class Bullet: public Entity {
public:
	Bullet(Vec3f, Vec3f, Vec3f, int bulletRange, unsigned char team);
	void render(Renderer*);
	void update(float);
	void interact(Entity* e);
	bool isDead(void);
	bool intersectsSphere(Vec3f pos, Vec3f vel, float);
	virtual ~Bullet();
private:
	float size;
	float energy;
	float maxEnergy;
};

#endif /* BULLET_H_ */
