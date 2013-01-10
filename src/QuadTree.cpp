/*
 * QuadTree.cpp
 *
 *  Created on: Mar 15, 2011
 *      Author: Nathan
 */

#include "QuadTree.h"
#include "Util/text.h"

unsigned int MAX_DEPTH = 15;
unsigned int MAX_POP = 5;

QuadTree::QuadTree(Vec2f min, Vec2f max, vector<Entity*> entities, unsigned int depth) {
	_min = min;
	_max = max;
	_mid = (_max + _min)/2;
	_entities = entities;
	_depth = depth;
	_split = false;
	for(int i = 0; i < 4; i++)
		children[i] = NULL;
	if(_depth < MAX_DEPTH){
		if(_entities.size() > MAX_POP){
			split();
		}
	}
}


vector<Entity*> QuadTree::findEntities(Vec2f pos, float r){
	if(_split){
		int index = (pos.x > _mid.x) | ((pos.y > _mid.y) << 1);
		children[index]->findEntities(pos, r);
	}
}

void QuadTree::render(){
	glColor4ub(255, 255, 255, 50);
	renderNum(_entities.size(), _mid.x, _mid.y, 0, 0.5);
	glBegin(GL_LINES);
	glVertex2f(_min.x, _min.y);
	glVertex2f(_min.x, _max.y);

	glVertex2f(_min.x, _max.y);
	glVertex2f(_max.x, _max.y);

	glVertex2f(_max.x, _max.y);
	glVertex2f(_max.x, _min.y);

	glVertex2f(_max.x, _min.y);
	glVertex2f(_min.x, _min.y);
	glEnd();
	if(_split){
		for(int i = 0; i < 4; i++){
			children[i]->render();
		}
	}
}

void findEntities(Vec2f pos, float r){

}

void QuadTree::split(){
	int depth = _depth + 1;
	Vec2f hypotenuse = _max - _min;
	double halfWidth = hypotenuse.x/2;
	double halfHeight = hypotenuse.y/2;
	vector<Entity*> childEntities[4];
	vector<Entity*>::iterator it;
	for(it = _entities.begin(); it != _entities.end(); it++){
		int xon = (((*it)->pos.x > _min.x + halfWidth));
		int yon = (((*it)->pos.y > _min.y + halfHeight)<<1);
		int index = xon | yon;
		childEntities[index].push_back((*it));
	}
	for(int i = 0; i < 4; i++){
		int xq = !!(i&1);
		int yq = !!(i&2);
		Vec2f min = Vec2f(_min.x + (halfWidth*xq), _min.y + (halfHeight*yq));
		Vec2f max = Vec2f(_max.x - (halfWidth*(!xq)), _max.y - (halfHeight*(!yq)));
		children[i] = new QuadTree(min, max, childEntities[i], depth);
	}
	_split = true;
}

QuadTree::~QuadTree() {
	if(_split){
		for(int i = 0; i < 4; i++)
			delete children[i];
	}
	_split = false;
}
