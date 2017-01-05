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

static const float decay = 0.95;
static const float ACCEL = 10.f;


Player::Player(Renderer *r, glm::vec3 _pos, float _size)
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
        dir = glm::vec3(1, 0, 0);
        dead = false;
        rotspeed = 0;
        velSpeed = 2500;
        if(team == PLAYER) {
            color = glm::vec3(129, 204, 60);
        } else {
            color = glm::vec3(181, 40, 65);
        }
    }

void Player::render(){
    renderer->pushMatrix();
    renderer->setColor(1.0f, 0.0f, 0.0f, 1.0f);
    renderer->translate(pos.x, pos.y, 0.0f);
    renderer->rotate(glm::vec3(1.f, 0.f, 0.f), dir);
    _mesh.draw();
    renderer->popMatrix();
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
    dir += glm::vec3(-dir.y, dir.x, 0.0f)*rotspeed*dt;
    dir = glm::normalize(dir);
    rotspeed *= 0.95;
    acc*=0.9;
    if(glm::length(vel) > maxVel){
        vel = glm::normalize(vel)*maxVel;
    }
}

void Player::respawn(glm::vec3 _pos){
    health = maxHealth;
    dead = false;
    pos = _pos;
    gunHeat = 0;
}

void Player::interact(Entity * e){
    glm::vec3 dist = pos - e->pos;
    float distance = glm::dot(dist,dist);
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
        entityStack.push_back(new Bullet (renderer, pos, vel + dir*15.f, dir, bulletRange, team));
        gunHeat += 5;
        gunTimer = 30.0/1000;
    }
}

/**
 * Accelerates the ship in the it is facing
 */
void Player::accelerate(float speed){
    acc += glm::normalize(dir) * ACCEL;
}

/**
 *
 */
void Player::rotate(float dir){
    rotspeed += dir*maxRotSpeed*PIDIV180;
}

Player::~Player() {
}
