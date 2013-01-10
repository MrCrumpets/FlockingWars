/*
 * Spawner.cpp
 *
 *  Created on: Jan 28, 2011
 *      Author: Nathan
 */

#include "Spawner.h"

int radius = 5;

Spawner::Spawner(Vec3f _pos, int _team) {
	pos = _pos;
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
	if(team == ENEMY){
		r = 181, g = 40, b = 60;
	}
	else{
		r = 129, g = 204, b = 60;
	}
	for(float b = 0.0; b < 2*PI; b+=(2*PI)/numLines){//Beta
		for(float t = 0; t < PI; t += PI/numLines){//Theta
			float y = rand()%radius*sin(b) * sin(t);
			Line a(Vec3f(center.x+rand()%radius*sin(t)*cos(b), center.y+y, rand()%radius*cos(t)), Vec3f(center.x, center.y, 0));
			lines.push_back(a);
		}
	}
}

void Spawner::render(Renderer* re){
	re->pushMatrix();
	re->applyCamera();
    re->uploadModelView();
	glColor4ub(r,g,b,255);
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
	glColor4ub(r, g, b, (sin(((timer/50.0)*(2*PI))/4)*0.2 + 0.8f)*255);
	glBegin(GL_LINES);
	for(int i = 0; i < lines.size(); i++)
	{
		lines[i].render(true);
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
	re->popMatrix();
}

void Spawner::render(bool threed){

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
				entityStack.push_back(new Boid(pos, team));
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

Line::Line(Vec3f _pos, Vec3f _center){
	pos = _pos;
	center = _center;
	vel = Vec3f(rand()%radius*2 - radius, rand()%radius*2 - radius, rand()%radius*2 - radius);
	constVel = vel;
}

void Line::render(bool threed)
{
	glVertex3f(pos.x, pos.y, pos.z);
	glVertex3f(vel.x + pos.x, vel.y + pos.y, vel.z + pos.z);
}

void Line::update(float dt){
	acc = (center-pos).normalized()*constVel.length();
	vel += acc*dt;
	pos += vel*dt;
}
