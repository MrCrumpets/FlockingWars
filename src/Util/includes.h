/*
 * includes.h
 *
 *  Created on: May 31, 2010
 *      Author: Nathan
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

#define DRAWMODE GL_LINE_LOOP
#define PIDIV180 0.0174532925
#define PI 3.14159

#include "GL/glew.h"
#include "math.h"
#include "Vec2f.h"
#include "Vec3f.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>

extern bool DEBUG;

extern int width;
extern int height;
extern int uiWidth, uiHeight;
extern int mousex;
extern int mousey;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern GLuint bullet_mesh;
extern GLuint particle_DL;
extern GLuint glowTex;
extern GLuint buttonTex;
extern GLuint shipMesh;
extern GLuint attackBoidMesh;
extern int damaged;
const unsigned char OTHER = 0, PLAYER = 1, ENEMY = 2;
const unsigned char SHIP = 0, SPAWNER = 1, BOID = 2, BULLET = 3, ATTACKBOID = 4, EXPLOSION = 5;

float inverseSqrt(float x);

extern double* getRotationMatrix(double x, double y);

using std::cout;
using std::endl;
#endif /* INCLUDES_H_ */
