/*
 * Explosion.cpp
 *
 */

#include "Explosion.h"

unsigned char colors[5][3] = {{181, 40, 65}, {255, 192, 81}, {255, 137, 57}, {232, 95, 77}, {89, 0, 81}};

/**
 * Constructs an Explosion object and initializes all the Particle objects.
 *
 * @param _x
 * 		The x location of the explosion
 * @param _y
 * 		The y coord of the explosion
 */
Explosion::Explosion(float _x, float _y) {
	overloaded = false;
	pos.x = _x;
	pos.y = _y;
	for(int i = 0; i < numParticles; i++)
		particles.push_back(Particle(pos.x, pos.y, 360.0/numParticles*i));
	dead = false;
	type = EXPLOSION;
	team = OTHER;
}
/**
 * The update method updates the all the particles based on the dt since last update, and removes
 * dead particles
 *
 * @param dt
 * 		The time in seconds since update was last called
 */
void Explosion::update(float dt){
	for(unsigned i = 0; i < particles.size(); i++){
		particles[i].update(dt);
		if(particles[i].dead){
			particles.erase(particles.begin()+i);
			i--;
		}
	}
	if(particles.size() == 0){
		dead = true;
	}
}
/**
 * Renders all the particles
 */
void Explosion::render(bool threed){

	glBegin(GL_LINES);
	for(unsigned i = 0; i < particles.size(); i++)
		particles[i].render(threed);
	glEnd();
}

/**
 * Destructor, frees all the memory used by Particle objects and the particles[] array
 */
Explosion::~Explosion() {
}

/**
 * Particle constructor, creates particle object
 *
 * @param _x
 * 		The initial x coord
 * @param _y
 * 		The initial y coord
 * @param angle
 * 		The angle of movement of the particle
 */
Particle::Particle(float _x, float _y, float angle){
	pos.x = _x;
	pos.y = _y;
	life = 30;
	dead = false;
	const int maxVel = 15;
	vel = Vec3f(cos(angle)*(rand()%maxVel), sin(angle)*(rand()%maxVel), 0.0f);
	int random = rand()%5;
	r = colors[random][0];
	g = colors[random][1];
	b = colors[random][2];
}

/**
 * The update method updates the particles position and velocity based on the dt parameter
 *
 * @param dt
 * 		The time in seconds since update was last called
 */
void Particle::update(float dt){
	vel *= 0.99;//Air resistance or friction? Even in space... it looks cooler this way.
	pos += vel;
	life--;
	if(life < 0)
		dead = true;
}

void Particle::render(bool threed){
	float z = threed?0:0;
	glColor4f((r)/250.0, (g)/250.0, (b)/250.0, life/30.0);//Set the color with adjusted alpha so it fades out
	glVertex3f(pos.x, pos.y, z);
	glVertex3f(pos.x+(vel.x), pos.y+(vel.y), z);
}

Particle::~Particle(){

}
