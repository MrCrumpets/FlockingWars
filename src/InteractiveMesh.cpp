/*
 * InteractiveMesh.cpp
 *
 *	This was just a little experiment initially but turned into a fully-fledged feature.
 *	It is a grid/mesh the lies in the background and reacts to input from the game. Explosions
 *	and ship movement send ripples through the mesh causing some cool effects.
 *
 *	It is really just a container for a list of vertices.
 *
 *  Created on: Jun 12, 2010
 *      Author: Nathan
 *
 */

#include "InteractiveMesh.h"

InteractiveMesh* mesh = NULL;
int ROWS = 0;
int COLS = 0;
/**
 * Mesh Constructor. Initializes all the points and lines in the mesh, and calculates the offset and
 * grid spacing
 */
InteractiveMesh::InteractiveMesh(int rows, int cols) {
	mesh = this;
	ROWS = rows;
	COLS = cols;
	points = new MeshPoint*[COLS * ROWS];
	int xoffset = 0;
	int yoffset = -((height * 1.0)/rows);
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			points[r * COLS + c] = new MeshPoint((((width * 1.0)/(COLS-2)) * c) + xoffset, (((height*1.0)/(ROWS-2)) * 1.0 * r) + yoffset, 0);
			if (c > 0) {
				if(r != 0 && r != ROWS-1)
					points[r* COLS + c]->moveable = true;
				else
					points[r* COLS + c]->moveable = false;
			}
			if (r > 0) {
				if(c != 0 && c != COLS-1)
					points[r* COLS + c]->moveable = true;
				else
					points[r* COLS + c]->moveable = false;
			}
			if(r == 0 || r == ROWS-1){
				points[r* COLS + c]->moveable = false;
			}
		}
	}

//	std::cout << "Constructed Mesh." << std::endl;
}
/*
 *	Render() renders the mesh. Simple two vertex GL_LINES, nothing fancy.
 */
void InteractiveMesh::render(bool threed){
	glColor4ub(100, 129, 255, 100);
	if(threed){
		glBegin(GL_LINES);
			for(int i = 0; i < ROWS; i++){
				for(int j = 0; j < COLS; j++){
					if(j < COLS-1)
					{
						glVertex3f(points[i*COLS+j]->ix, points[i*COLS+j]->iy,  points[i*COLS+j]->z);
						glVertex3f(points[i*COLS+j+1]->ix, points[i*COLS+j+1]->iy,  points[i*COLS+j+1]->z);
					}
					if(i < ROWS-1)
					{
						glVertex3f(points[i*COLS+j]->ix, points[i*COLS+j]->iy,  points[i*COLS+j]->z);
						glVertex3f(points[(i+1)*COLS+j]->ix, points[(i+1)*COLS+j]->iy,  points[(i+1)*COLS+j]->z);
					}
				}
			}
		glEnd();
	}
	else
	{
		glBegin(GL_LINES);
			for(int i = 0; i < ROWS; i++){
				for(int j = 0; j < COLS; j++){
					if(j < COLS-1)
					{
						glVertex3f(points[i*COLS+j]->x, points[i*COLS+j]->y, 0);
						glVertex3f(points[i*COLS+j+1]->x, points[i*COLS+j+1]->y, 0);
					}
					if(i < ROWS-1)
					{
						glVertex3f(points[i*COLS+j]->x, points[i*COLS+j]->y, 0);
						glVertex3f(points[(i+1)*COLS+j]->x, points[(i+1)*COLS+j]->y, 0);
					}
				}
			}
		glEnd();
	}
}
/**
 * The update method updates all the points and lines in the mesh
 */
void InteractiveMesh::update(){
	for(int i = 0; i < COLS * ROWS; i++){
		points[i]->update();
	}
}

/**
 * Creates an "impulse" at the coordinates specified with the strenght specified. Sends a wave through the mesh
 * from the impulse point (x, y) with a value of (strength^2/sqrt(dist_from_point), which gives a nice decay with
 * distance.
 *
 * @param x
 * 		The x coordinate of the impulse
 * @param y
 * 		The y coordinate of the impulse
 * @param strength
 * 		The strength of the impulse
 * @return
 * 		void
 */
void InteractiveMesh::createImpulse (float x, float y, float strength){
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			float px = points[i * COLS + j]->x;//X coordinate of point
			float py = points[i * COLS + j]->y;//Y coordinate of point
			float dx = px - x;//The distance from the point to the impulse
			float dy = py - y;//The distance from the point to the impulse
			float offx = 0;
			float offy = 0;
			float offz = 0;
//			if(x < r*r+25){
//				float invsqrt = inverseSqrt(x);
//				if(dx < r){
//					float c = cos((dx/r)*(PI/2));
//					offx = (invsqrt*(dx*(c>0?c:0)))*strength;//The desired offset
//				}
//				else
//					std::cout << dx << std::endl;
//				if(dy < r){
//					float c = cos((dy/r)*(PI/2));
//					offy = (invsqrt*(dy*(c>0?c:0)))*strength;
//				}
//			}
			float is = inverseSqrt(dx * dx + dy * dy);//1/dist
			offx = (strength*(is)*(dx/25));//The desired offset
		    offy = (strength*(is)*(dy/25));
		    offz = std::min((strength*strength*(is)), 3.0f);
			float maxH = 10;
			offz = offz > maxH ? maxH : offz;
			points[i * COLS + j]->x += offx;
			points[i * COLS + j]->y += offy;
			points[i * COLS + j]->z += offz;
		}
	}
}

double InteractiveMesh::getNormal(double x, double y, glm::vec3 &normal){
	int row = (y/(float)height)*ROWS;
	int column = (x/(float)width)*COLS;
	int index = row * COLS + column;
	glm::vec3 a = glm::vec3(points[index]->x, points[index]->y, points[index]->z);
	MeshPoint* mb = 0;
	MeshPoint* mc = 0;
	glm::vec3 b;
	glm::vec3 c;

	if(row > 0)
		mb = points[(row-1)*COLS + column];
	else
		mb = points[(row+1)*COLS + column];

	if(column > 0)
		mc = points[row*COLS + column-1];
	else
		mc = points[row*COLS + column+1];
	b = a-glm::vec3(mb->x, mb->y, mb->z);
	c = a-glm::vec3(mc->x, mc->y, mc->z);
	normal = glm::cross(b, c);
	return a.z;
}

double* getRotationMatrix(double x, double y){
	if(x < 0 || x > width || y < 0 || y > height)
		return NULL;
	double* mat = new double[16];
	for(int i = 0; i < 16; i++)
		mat[i] = 1;
	int row = (y/(float)height)*ROWS;
	int column = (x/(float)width)*COLS;
	if(row < 0 || row > ROWS || column < 0 || column > COLS)
		return mat;
	int index = row * COLS + column;
	MeshPoint** points = mesh->points;
	glm::vec3 a = glm::vec3(points[index]->x, points[index]->y, points[index]->z);
	MeshPoint* mb = 0;
	MeshPoint* mc = 0;
	glm::vec3 b;
	glm::vec3 c;

	if(row > 0)
		mb = points[(row-1)*COLS + column];//y
	else
		mb = points[(row+1)*COLS + column];

	if(column > 0)
		mc = points[row*COLS + column-1];//x
	else
		mc = points[row*COLS + column+1];
	b = a-glm::vec3(mb->x, mb->y, mb->z);
	c = a-glm::vec3(mc->x, mc->y, mc->z);
	glm::vec3 normal = glm::normalize(glm::cross(c, b));
	glm::vec3 x1 = glm::normalize(glm::vec3(1.0f, 0, (mc->z-a.z)/(mc->x-a.x)));
	glm::vec3 y1 = glm::normalize(glm::vec3(0, 1.0f, (mb->z-a.z)/(mb->y-a.y)));
//	cout << x1.x << " " << x1.y << " " << x1.z << endl;
//	cout << y1.x << " " << y1.y << " " << y1.z << endl;
//	cout << normal.x << " " << normal.y << " " << normal.z << endl;
	mat[0] = x1.x;
	mat[1] = x1.y;
	mat[2] = x1.z;
	mat[3] = 0;
	mat[4] = y1.x;
	mat[5] = y1.y;
	mat[6] = y1.z;
	mat[7] = 0;
	mat[8] = normal.x;
	mat[9] = normal.y;
	mat[10] = normal.z;
	mat[11] = 0;
	mat[12] = 0;
	mat[13] = 0;
	mat[14] = a.z;
	mat[15] = 1.0;
	return mat;
}

/**
 * InteractiveMesh destructor, frees memory used by all points and lines.
 */
InteractiveMesh::~InteractiveMesh() {
	for(int i = 0; i < COLS * ROWS; i++)
		delete points[i];
	delete points;
}
/*
 * Constructs a grid point or vertex.
 * Stores the initial x and way so we can force the points back to their original positions after they're moved
 *
 * @param x
 * 		The xcoord of the point
 * @param y
 * 		The ycoord of the point
 */
MeshPoint::MeshPoint (float x, float y, float z){
	this->setPosition(x, y, z);
	ix = x;//Initial x
	iy = y;//Initial y
	iz = z;
}

/**
 * SetPosition isn't really used as much as it should be, but it sets the position of the x and y values
 *
 * @param x
 * 		The x coord for the point to be translated to
 * @param y
 * 		The y coord for the point to be translated to
 */
void MeshPoint::setPosition (float x, float y, float z){
	this->x = oldx = x;
	this->y = oldy = y;
	this->z = oldz = z;
}

/*
 * MPoint::update():
 *
 * The update method updates the point's position. It uses verlet integration (http://en.wikipedia.org/wiki/Verlet_integration)
 *  to maintain some velocity with the points, but also attempts to return to it's original position.
 *
 */
void MeshPoint::update(){
	float dx = ix - x;//The delta x from the points original position
	float dy = iy - y;//y dx
	float dz = iz - z;
	if(moveable){//Not really used, the edge points were original prevented from moving.
		float tempx = x;
		float tempy = y;
		float tempz = z;
		//Add a combination of verlet velocity and attempting to move to original position
		x += ((x - oldx) * 0.8 + (dx * 0.05));
		y += ((y - oldy) * 0.8 + (dy * 0.05));
		z += ((z - oldz) * 0.8 + (dz * 0.05));
		oldx = tempx;
		oldy = tempy;
		oldz = tempz;
	}
	else{
		x = ix;
		y = iy;
		z = iz;
	}
}
