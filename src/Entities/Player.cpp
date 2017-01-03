/*
 * Player.cpp
 *
 *	The player object class. Contains and manages all the bullets spawned by this ship.
 *	Manages the updating and rendering of the player's ship.
 *  Created on: May 25, 2010
 *      Author: Nathan
 */

#include "Player.h"

static const std::vector<vertex> vertices {
    {{ -0.5f, -0.5f, 0.0f }},
    {{ 0.5f,  0.0f, 0.0f }},
    {{ 0.0f,  0.0f, 0.4f }},
    {{-0.5f,  0.5f, 0.0f }},
    {{-0.3f,  0.0f, 0.0f }}
};

static const std::vector<unsigned int> indices { 0, 1, 2, 1, 2, 3, 3, 2, 4, 4, 2, 0 };

const float decay = 0.95;


Player::Player(Renderer *r, Vec3f _pos, float _size) 
    : Entity(r, _pos), size(_size), _mesh(vertices, indices) {
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
        dir = Vec3f(1, 0, 0);
        dead = false;
        rotspeed = 0;
        velSpeed = 2500;
        //RGB values of the ship mesh
        color = Vec3f(129, 204, 60);
    }

void Player::render(){
    renderer->pushMatrix();
    renderer->setColor(1.0f, 0.0f, 0.0f, 1.0f);
    renderer->rotate(dir.theta(), 0.0f, 0.0f, 1.0f);
    renderer->translate(pos.x, pos.y, 0.0f);
    renderer->uploadModelView();
    _mesh.draw();
    renderer->popMatrix();
    renderer->setColor(1.0f, 1.0f, 1.0f, 1.0f);
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
    rotspeed *= 0.95;
    acc*=0.9;
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
        entityStack.push_back(new Bullet (renderer, pos, dir, vel, bulletRange, team));
        gunHeat += 5;
        gunTimer = 30.0/1000;
    }
}

/**
 * Accelerates the ship in the it is facing
 */
void Player::accelerate(float speed){
    acc += dir.normalized() * 40;
}

/**
 *
 */
void Player::rotate(float dir){
    rotspeed += dir*maxRotSpeed*PIDIV180;
}

Player::~Player() {
}
