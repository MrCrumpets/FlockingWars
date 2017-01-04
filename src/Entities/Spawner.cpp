/*
 * Spawner.cpp
 *
 *  Created on: Jan 28, 2011
 *      Author: Nathan
 */

#include "Spawner.h"

int radius = 5;

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
	for(float b = 0.0; b < 2*PI; b+=(2*PI)/numLines){//Beta
		for(float t = 0; t < PI; t += PI/numLines){//Theta
			float y = rand()%radius*sin(b) * sin(t);
			Line a(glm::vec3(center.x+rand()%radius*sin(t)*cos(b), center.y+y, rand()%radius*cos(t)), glm::vec3(center.x, center.y, 0));
			lines.push_back(a);
		}
	}
}

void Spawner::render(){
    renderer->pushMatrix();
    renderer->uploadModelView();
    glColor4ub(color.x, color.y, color.z, 255);
    float z = 10;
    float x = pos.x + 50;
    float y = pos.y + 50;
    float h = (health/maxHealth)*100;
    float w = 20;
    glBegin(GL_QUADS);
    glVertex3f(x, y, z);
    glVertex3f(x, y, z+h);
    glVertex3f(x+w, y, z+h);
    glVertex3f(x+w, y, z);
    glEnd();
    glColor4ub(color.x, color.y, color.z, (sin(((timer/50.0)*(2*PI))/4)*0.2 + 0.8f)*255);
    glBegin(GL_LINES);
    for(int i = 0; i < lines.size(); i++)
    {
        lines[i].render();
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, glowTex );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    float half = 256;
    glBegin( GL_QUADS );
    //Bottom-left vertex (corner)
    glTexCoord2i( 0, 0 );
    glVertex3f(pos.x-half, pos.y-half, 0.0f );

    //Bottom-right vertex (corner)
    glTexCoord2i( 1, 0 );
    glVertex3f(pos.x+half, pos.y-half, 0.f );

    //Top-right vertex (corner)
    glTexCoord2i( 1, 1 );
    glVertex3f(pos.x+half,pos.y+half, 0.f );

    //Top-left vertex (corner)
    glTexCoord2i( 0, 1 );
    glVertex3f(pos.x-half, pos.y+half, 0.f );
    glEnd();
    glDisable(GL_TEXTURE_2D);
    renderer->popMatrix();
}

void Spawner::update(float dt){
	if(health <= 0)
		dead = true;
	for(int i = 0; i < lines.size(); i++){
		lines[i].update(dt);
	}
	if(team == ENEMY){
		money+= moneyRate;
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

Spawner::~Spawner() {
	// TODO Auto-generated destructor stub
}

Line::Line(glm::vec3 _pos, glm::vec3 _center){
	pos = _pos;
	center = _center;
	vel = glm::vec3(rand()%radius*2 - radius, rand()%radius*2 - radius, rand()%radius*2 - radius);
	_constVel = glm::length(vel);
}

void Line::render()
{
    glVertex3f(pos.x, pos.y, pos.z);
    glVertex3f(vel.x + pos.x, vel.y + pos.y, vel.z + pos.z);
}

void Line::update(float dt){
	acc = glm::normalize(center-pos)*_constVel;
	vel += acc*dt;
	pos += vel*dt;
}
