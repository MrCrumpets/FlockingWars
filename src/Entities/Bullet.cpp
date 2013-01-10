/*
 * Bullet.cpp
 *
 *	Bullet object class. Manages the updating and rendering of
 *	the bullets, and contains methods for determing their intersection
 *	with spheres.
 *
 *  Created on: May 28, 2010
 *     Author: Nathan
 */

#include "Bullet.h"

/**
 * Bullet constructor, creates the bullet object and initializes
 * the mesh.
 *
 * @param _x
 * 		The initial xcoord of the bullet
 * @param _y
 * 		The initial ycoord of the bullet
 * @param size
 * 		The size of the bullet mesh
 * @param speed
 * 		The velocity of the ship to be added to the bullets initial
 * 			velocity.
 */

Bullet::Bullet(Vec3f _pos, Vec3f _dir, Vec3f _vel, int bulletRange, unsigned char _team) {
	dir = _dir;
	pos = _pos;
	vel = _vel + _dir*750;
	dead = false;
	maxEnergy = energy = (float)bulletRange;
	type = BULLET;
	explodes = false;
	team = _team;
	size = 3;
	switch(team){
	case PLAYER:
		r = 129;
		g = 204;
		b = 60;
	break;
	case ENEMY:
		r = 181;
		g = 40;
		b = 65;
		break;
	}
}

/*
 * Updates the bullets coordinates based on the deltaTime since the last update
 *
 * @param dt
 * 		The time in seconds since update was last called
 */
void Bullet::update(float dt){
	oldpos = pos;
	pos += vel*dt;
	vel += acc*dt;
	energy -= 100*dt;
	damage = energy;
	if(energy < 0)
		dead = true;
	if(pos.x > width || pos.x < 0 || pos.y > height || pos.y < 0){
		dead = true;
	}
}

void Bullet::render(Renderer* re){
//	cout << "getting rot matrix " << pos.x << " " << pos.y << endl;
//	glEnd();
//	glColor4ub(r, g, b, (energy/maxEnergy)*255);
//	glPushMatrix();
//	glTranslatef(pos.x, pos.y, 0);
//	glRotatef(rot, 0, 0, 1);
//	glCallList(bullet_mesh);
//	glPopMatrix();
}
/**
 * Determines whether this bullet intersects with the sphere described in the params
 *
 * @param x
 * 		The xcoord of the sphere
 * @param y
 * 		The y-coord of the sphere
 * @param size
 * 		The radius of the sphere
 * @return
 * 		Whether or not any of the bullets have intersected
 */
void Bullet::interact(Entity* e){
	if(e->team != team){
		if(e->type != BOID){
			float xdif = ((pos.x - e->pos.x));
			float ydif = ((pos.y - e->pos.y));
			if((xdif * xdif) + (ydif * ydif) < 100){
				dead = true;
				return;
			}
			Vec3f vb = pos - oldpos;
			Vec3f va = e->pos - e->oldpos;
			Vec3f ab = oldpos - e->oldpos;
			Vec3f vba = (vb-va);
			if(ab.lengthSq() < 625){
				float rab = 5;
				float c = ab.dot(ab) - (rab*rab);
				float b = 2*(vba.dot(ab));
				float a = vba.dot(vba);
				float desc = (b*b - (4*a*c));
				if(desc > 0){
					float sq = sqrt(desc);
					float a2 = 2.0*a;
					float u0 = (-b - sq)/a2;
					float u1 = (-b + sq)/a2;
					if(u0 > 1 || u0 < 0 || u1 > 1 || u1 < 0)
						return;
					else{
						dead = true;
						return;
					}
				}
			}
		}
	}
}
/*
 * Returns dead
 *
 * @return dead
 * 		The bool stating the condition of the bullet object
 */
bool Bullet::isDead(void){
	return dead;
}
Bullet::~Bullet() {}
