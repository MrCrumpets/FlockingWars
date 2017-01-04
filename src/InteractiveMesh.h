/*
 * InteractiveMesh.h
 *
 *  Created on: Jun 12, 2010
 *      Author: Nathan
 */

#ifndef INTERACTIVEMESH_H_
#define INTERACTIVEMESH_H_

#include <glm/glm.hpp>
#include "Util/includes.h"

class MeshPoint{
public:
	float x, y, z, oldx, oldy, oldz, ix, iy, iz;
	glm::vec3 normal;
	bool moveable;
	MeshPoint(float x, float y, float z);
	void setPosition(float, float, float);
	void update();
};
class InteractiveMesh{
public:
	MeshPoint **points;
	InteractiveMesh(int, int);
	void render(bool threed);
	void update();
	double getNormal(double x, double y, glm::vec3 &normal);
	double* getRotationMatrix(double x, double y);
	void createImpulse(float, float, float);
	virtual ~InteractiveMesh();
};
#endif /* INTERACTIVEMESH_H_ */
