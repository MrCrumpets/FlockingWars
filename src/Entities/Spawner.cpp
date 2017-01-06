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

        for(size_t i = 0; i < numLines; ++i){
	    lines.emplace_back(Line{glm::ballRand(radius), glm::ballRand(radius)});
        }

        glGenVertexArrays(1, &_vertex_array);
        glBindVertexArray(_vertex_array);

        // Copy Vertex Buffer Data
        glGenBuffers(1, &_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER,
                lines.size() * sizeof(Line),
                nullptr, GL_STREAM_DRAW);

        // Set Shader Attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid *) 0);
        glEnableVertexAttribArray(0); // Vertex Positions
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
    glBindVertexArray(_vertex_array);
    glDrawArrays(GL_LINES, 0, 2 * lines.size());
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
    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(Line), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, lines.size() * sizeof(Line), &lines[0]);

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
