/*
 * Spawner.cpp
 *
 *  Created on: Jan 28, 2011
 *      Author: Nathan
 */

#include "Spawner.h"
#include <glm/gtc/random.hpp>

static const float radius = 15.f;
static const int numLines = 100;
static GLuint _vertex_array;
static GLuint _vertex_buffer;

Spawner::Spawner(Renderer *r, glm::vec3 _pos, int _team)
    : Entity(r, _pos) {
        center = pos;
        team = _team;
        type = SPAWNER;
        timer = 0;
        maxHealth = 120;
        health = maxHealth;
        money = 50;
        damage = 500;
        moneyRate = 0.25;
        lastSpawned = 0;
        if(team == PLAYER)
            color = glm::vec3(129, 204, 60);
        else {
            color = glm::vec3(181, 40, 65);
        }

        std::vector<glm::uint32> indices;
        for(size_t i = 0; i < numLines; ++i){
	    lines.emplace_back(Line{glm::ballRand(radius), glm::ballRand(radius)});
            indices.push_back(indices.size());
        }
        std::vector<vertex> vertices;
        for(const auto &l : lines) {
            vertices.emplace_back(vertex{l.last_pos});
            vertices.emplace_back(vertex{l.pos});
        }
        _mesh = std::make_unique<Mesh>(vertices, indices, DrawType::Lines, MeshType::Streaming);
    }

void Spawner::render(){
    renderer->pushMatrix();
    float z = 10;
    float x = pos.x + 50;
    float y = pos.y + 50;
    float h = (health/maxHealth)*100;
    float w = 20;
    // Draw Health Bar
    // Draw lines
    renderer->translate(pos.x, pos.y, 0.0f);
    renderer->setColor(color.r, color.g, color.b, (sin(((timer/50.0)*(2*PI))/4)*0.2 + 0.8f)*255);
    _mesh->draw();
    renderer->popMatrix();
}

void Spawner::update(float dt){
    if(health <= 0)
        dead = true;
    for(auto &l : lines){
        auto vel = l.pos - l.last_pos;
	auto acc = glm::normalize(pos-l.pos);
	vel = glm::normalize(vel + acc*dt) * 15.f;
        l.last_pos = l.pos;
	l.pos += vel*dt;
    }
    std::vector<vertex> vertices;
    for(const auto &l : lines) {
        vertices.emplace_back(vertex{l.last_pos});
        vertices.emplace_back(vertex{l.pos});
    }
    _mesh->update(vertices);

    if(team == ENEMY){
        money += moneyRate;
        moneyRate += 0.00025;
        timer++;
        if(money > 50){
            if((rand()%10) > 5){
                queue.push(ATTACKBOID);
                money -= 50;
            }
            else{
                money -= 50;
                for(int i = 0; i < 5; i++)
                    queue.push(BOID);
            }
        }
    }
    if(queue.size()){
        if(lastSpawned > 5){
            lastSpawned = 0;
            switch(queue.front()){
                case BOID:
                    entityStack.push_back(new Boid(renderer, pos, team));
                    break;
            }
            queue.pop();
        }
        lastSpawned++;
    }
}

Spawner::~Spawner() {}
