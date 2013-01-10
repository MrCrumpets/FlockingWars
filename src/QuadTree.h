/*
 * QuadTree.h
 *
 *  Created on: Mar 15, 2011
 *      Author: Nathan
 */

#ifndef QUADTREE_H_
#define QUADTREE_H_

#include "Util/includes.h"
#include "Entities/Entity.h"
using std::vector;

class QuadTree {
public:
	QuadTree(Vec2f min, Vec2f max, vector<Entity*> entities, unsigned int depth);
	void addEntity(Entity e);
	void split();
	void render();
	vector<Entity*> findEntities(Vec2f pos, float r);
	Vec2f _min, _max, _mid;
	vector<Entity*> _entities;
	QuadTree* children[4];
	unsigned int _depth;
	bool _split;
	virtual ~QuadTree();
};

#endif /* QUADTREE_H_ */
