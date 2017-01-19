/*
 * Flock.cpp
 *
 *  Created on: Jan 23, 2011
 *      Author: Nathan
 */

#include "Flock.h"

float decay = 0.998;
glm::vec3 orbit;

static const std::vector<vertex> boidVertices {
    {{ -0.5f, -0.5f, 0.0f }},
    {{ 0.5f,  0.0f, 0.0f }},
    {{ 0.0f,  0.0f, 0.4f }},
    {{-0.5f,  0.5f, 0.0f }},
    {{-0.3f,  0.0f, 0.0f }}
};

static const std::vector<unsigned int> boidIndices { 0, 1, 2, 1, 2, 3, 3, 2, 4, 4, 2, 0 };

static Mesh *boidMesh;

Flock::Flock(){}

void Flock::addEntity(Entity* e){
    members.push_back(e);
}

void Flock::removeEntity(Entity* e){
}

int Flock::update(float dt){
    std::remove_if(
            members.begin(),
            members.end(),
            [](Entity *e) { return e->dead; }
            );
    for(auto &m : members){
        m->acc -= (m->pos - orbit) * static_cast<float>((rand()%10)/10.f);
    }
    return members.size();
}

void Flock::setObjective(const glm::vec3 &o){
    orbit = o;
}

Boid::Boid(Renderer *r, const glm::vec3 &_pos, const std::string& _team)
    : Entity(r, _pos) {
        if(!boidMesh) {
            boidMesh = new Mesh(boidVertices, boidIndices);
        }
        team = _team;
        type = EntityType::Boid;
        explodes = true;
        minSpeed = 5;
        maxSpeed = rand()%45 + 30;
        minSq = minSpeed*minSpeed;
        maxSq = maxSpeed*maxSpeed;
        pos = _pos;
        vel = glm::vec3(rand()%(int)maxSpeed-maxSpeed/2, rand()%(int)maxSpeed-maxSpeed/2, 0.0f);
        dead = false;
        dir = glm::vec3(0, 1, 0);
        value = 5;
        damage = 30;
        health = maxHealth = 90;
        fear = 0;
    }

void Boid::render(){
    boidMesh->draw();
}

void Boid::update(float dt){
    if(health < 0) {
        die();
    }
    maxSpeed = maxSpeed + fear;
    vel += acc * 33.f * dt;

    float dist = glm::dot(vel, vel);
    if(dist < minSq){
        vel = glm::normalize(vel)*minSpeed;
    }
    else if(dist > maxSq){
        vel = glm::normalize(vel)*maxSpeed;
    }
    pos += vel * dt;
    vel *= decay;
    acc = glm::vec3();
    dir = glm::normalize(vel);
}

//Thresholds
float zone = 15;
float interactZone = zone*zone;
float repelThresh = 0.25f;
float alignThresh = 0.5f;
float repelWeight = 5.f;
float coheseWeight = 8;
float alignWeight = 1;

void Boid::interact(Entity* e){
    glm::vec3 dist = pos - e->pos;
    float dsq = glm::dot(dist, dist);
    dist = glm::normalize(dist);

    if(e->team != team) {
        health -= e->damage;
    }
    else if(dsq <= interactZone && e->type == EntityType::Boid)
    {
        float ratio = dsq/interactZone;
        if(ratio < repelThresh){
            //Repel
            float force = (repelThresh/ratio - 1.0f)*repelWeight;
            acc += dist*force;
        }
        else if(ratio < alignThresh){
            float weight = (ratio - alignThresh)/(1.0f - alignThresh);
            float force = (1.0f - (cos(weight*PI*2) * -0.5f + 0.5f))*alignWeight;
            acc += glm::normalize(e->vel + glm::vec3(0.0001)) * force;
        }
        else {
            //Attract
            float weight = (ratio - repelThresh)/(1.0f - repelThresh);
            float force = (1.0f - (cos(weight*PI*2) * -0.5f + 0.5f))*coheseWeight;
            acc -= dist*force;
        }
    }
}

void Boid::die(){
    dead = true;
}

Boid::~Boid(){

}

