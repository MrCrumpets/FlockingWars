/*
 * Spawner.cpp
 *
 *  Created on: Jan 28, 2011
 *      Author: Nathan
 */

#include "Spawner.h"
#include <glm/gtc/random.hpp>
#include <iostream>

static const float radius = 15.f;
static const int numLines = 100;
static GLuint _vertex_array;
static GLuint _vertex_buffer;

Spawner::Spawner(Renderer *r, glm::vec3 pos, const std::string &team)
    : Entity(r, pos) {
        this->team = team;
        type = EntityType::Spawner;
        timer = 0;
        maxHealth = 120;
        health = maxHealth;
        money = 50;
        damage = 500;
        moneyRate = 0.25;
        lastSpawned = 0;

        std::vector<glm::uint32> indices;
        for(size_t i = 0; i < numLines; ++i){
            indices.push_back(indices.size());
            indices.push_back(indices.size());
	    lines.emplace_back(Line{pos + glm::ballRand(radius), pos + glm::ballRand(radius)});
            _vertices.push_back({lines.back().last_pos});
            _vertices.push_back({lines.back().pos});
        }

        _mesh = std::make_unique<Mesh>(_vertices, indices, DrawType::Lines, MeshType::Streaming);
    }

void Spawner::render(){
    _mesh->draw();
}

void Spawner::update(float dt){
    if(health <= 0)
        dead = true;

    int line_idx = 0;
    // Update lines
    for(auto &l : lines){
        auto vel = l.pos - l.last_pos;
	auto acc = glm::normalize(pos-l.pos);
	vel = glm::normalize(vel + acc*dt) * 15.f;
        l.last_pos = l.pos;
	l.pos += vel*dt;
        _vertices[line_idx++] = { l.last_pos };
        _vertices[line_idx++] = { l.pos };
    }

    _mesh->update(_vertices);

    if(team == "Enemy"){
        money += moneyRate;
        moneyRate += 0.00025;
        timer++;
        if(money > 50){
            if((rand()%10) > 5){
                queue.push(EntityType::AttackBoid);
                money -= 50;
            }
            else{
                money -= 50;
                for(int i = 0; i < 5; i++)
                    queue.push(EntityType::Boid);
            }
        }
    }
    if(queue.size()){
        if(lastSpawned > 5){
            lastSpawned = 0;
            switch(queue.front()){
                case EntityType::Boid:
                    entityStack.push_back(new Boid(renderer, pos, team));
                    break;
            }
            queue.pop();
        }
        lastSpawned++;
    }
}

Spawner::~Spawner() {}
