/*
 * includes.h
 *
 * Originally written because a lot of files had the same includes, and
 * I eventually got lazy and started putting global variables here. I
 * will need to get rid of this one as well.
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

#define DRAWMODE GL_LINE_LOOP
#define PIDIV180 0.0174532925
#define PI 3.14159

#include "GL/glew.h"
#include "math.h"
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

float inverseSqrt(float x);

extern double* getRotationMatrix(double x, double y);

using std::cout;
using std::endl;
#endif /* INCLUDES_H_ */
