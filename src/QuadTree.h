/*
 * QuadTree.h
 *
 * Outline for a first attempt at more efficient entity-entity interactions.
 * I started this awhile ago, I'm not sure if I'm to use a QuadTree anymore,
 * but I'm keeping this here for now.
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
