/*
 * Bullet.h
 *
 * This class manages state of bullet objects as well as provides functions
 * for accurate interpolated collision detection. This is a remnant of
 * old code, I may replace it soon.
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

	// Determines if this bullet intersected with the entity parameter
	// in the last time step.
	void interact(Entity* e);
	bool isDead(void);
		virtual ~Bullet();
private:
	float size;
	float energy;
	float maxEnergy;
};

#endif /* BULLET_H_ */
