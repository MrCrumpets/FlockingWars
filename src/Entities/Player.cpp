/*
 * Player.cpp
 *
 *	The player object class. Contains and manages all the bullets spawned by this ship.
 *	Manages the updating and rendering of the player's ship.
 *  Created on: May 25, 2010
 *      Author: Nathan
 */

#include "Player.h"


static GLuint vertexBufferObject;
GLuint offsetLocation;

const float vertexData[] = {
		-0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.0f, 0.0f, 1.0f,
		 0.0f,  0.0f, 0.4f, 1.0f,

		 0.5f,  0.0f, 0.0f, 1.0f,
		 0.0f,  0.0f, 0.4f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f,

		-0.5f,  0.5f, 0.0f, 1.0f,
		 0.0f,  0.0f, 0.4f, 1.0f,
		-0.3f,  0.0f, 0.0f, 1.0f,

		-0.3f,  0.0f, 0.0f, 1.0f,
		 0.0f,  0.0f, 0.4f, 1.0f,
		-0.5f, -0.5f, 0.0f, 1.0f,
};

const float decay = 0.91;

Player::Player(Vec3f _pos, float _size) {
	type = SHIP;
	team = PLAYER;
	maxHealth = 500;
	maxHeat = 500;
	maxRotSpeed = 20;
	maxVel = 750;
	bulletRange = 100;
	gunHeat = 0;
	gunTimer = 1;
	health = maxHealth;
	explodes = true;
	pos = _pos;
	dir = Vec3f(1, 0, 0);
	size = _size;
	dead = false;
	rotspeed = 0;
	velSpeed = 2500;
	//RGB values of the ship mesh
	r = 129;
	g = 204;
	b = 60;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    glFrontFace(GL_CW);
}

void Player::render(Renderer* r){
	r->pushMatrix();
    size_t colorData = sizeof(vertexData) / 2;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorData);
//	r->translate(pos.x, pos.y, 0.0f);
	r->rotate(dir.theta(), 0.0f, 0.0f, 1.0f);
	r->applyCamera();
	r->translate(pos.x, pos.y, 0.0f);
	r->uploadModelView();
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertexData)/4);
	glDisableVertexAttribArray(0);
//	glDisableVertexAttribArray(1);
	r->popMatrix();
}

/*
 * Updates the ship coordinates, velocities, and angle based on the deltaTime since the last update
 *
 * @param dt
 * 		The time in seconds since update was last called
 */
void Player::update(float dt){
	if(health <= 0){
		dead = true;
		entityStack.push_back(new Explosion(pos.x, pos.y));
	}
	if(gunHeat > 0)
		gunHeat -= 33*dt;
	gunTimer -= dt;
	vel *= decay;
	vel += acc * dt;
	pos += vel * dt;
	dir += Vec3f(-dir.y, dir.x, 0.0f)*rotspeed*dt;
	dir.normalize();
	rotspeed *= 0.89;
	acc*=0.5;
	if(vel.length() > maxVel){
		vel = vel.normalized()*maxVel;
	}
}

void Player::respawn(Vec3f _pos){
	health = maxHealth;
	dead = false;
	pos = _pos;
	gunHeat = 0;
}

void Player::interact(Entity * e){
	Vec3f dist = pos - e->pos;
	float distance = dist.lengthSq();
	if(e->team == team){
	}
	else if(e->team != OTHER)
	{
		if(distance < 100){
			health -= e->damage;
			entityStack.push_back(new Explosion(pos.x, pos.y));
//			e->health -= damage;
		}
	}
}

/*
 * Creates a new bullet and adds it to the bullet list
 */
void Player::shoot(){
	if(gunHeat < maxHeat && gunTimer <= 0){
		entityStack.push_back(new Bullet (pos, dir, vel, bulletRange, team));
		gunHeat += 5;
		gunTimer = 30.0/1000;
	}
}

/**
 * Accelerates the ship in the it is facing
 */
void Player::accelerate(float speed){
	acc += dir.normalized() * 100;
}

/**
 *
 */
void Player::rotate(float dir){
	rotspeed += dir*maxRotSpeed*PIDIV180;
}

Player::~Player() {
}
