/*
 * Mat4.h
 *
 * Representation of a 4x4 matrix. Computes matrix multiplication, addition,
 * translation, scaling, and rotation matrices for OpenGL.
 *
 */

#ifndef MAT4_H_
#define MAT4_H_

#include "math.h"
#include <iostream>

class Mat4 {
public:
	Mat4(){
		Mat4(1.0f);
	}
	Mat4(float x){
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				m[i*4 + j] = i == j ? x : 0;
	}
	Mat4 operator*(Mat4 a)
	{
		Mat4 res(0);
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				for(int k = 0; k < 4; k++)
					res.m[i*4 + j] += m[i*4 + k] * a.m[k*4 + j];

		return res;
	}

	void print(){
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				std::cout << m[i*4 + j] << " ";
			}
			std::cout << std::endl;
		}
	}

	// Returns a 4x4 translation matrix for OpenGL
	static Mat4 translation(double x, double y, double z){
		Mat4 translate(1.0f);
		translate.m[3*4 + 0] = x;
		translate.m[3*4 + 1] = y;
		translate.m[3*4 + 2] = z;
		return translate;
	}

	// Returns a 4x4 scaling matrix for OpenGL
	static Mat4 scale(double x, double y, double z){
		Mat4 scale(1.0f);
		scale.m[0*4 + 0] = x;
		scale.m[1*4 + 1] = y;
		scale.m[3*4 + 3] = z;
		return scale;
	}

	// Currently only performs rotations around the Z-axis...
	static Mat4 rotation(double angle, double x, double y, double z){
		Mat4 rotate(1.0f);
		float cz = cos(angle);
		float sz = sin(angle);
		rotate.m[0*4 + 0] = cz;
		rotate.m[1*4 + 0] = -sz;
		rotate.m[0*4 + 1] = sz;
		rotate.m[1*4 + 1] = cz;
		return rotate;
	}

	/**
	 * MESA's implementation of gluInvertMatrix. Found on StackOverflow.
	 *
	 */
	static bool invert(const float m[16], float invOut[16]){
		float inv[16], det;
		int i;

		inv[0] = m[5]  * m[10] * m[15] -
				 m[5]  * m[11] * m[14] -
				 m[9]  * m[6]  * m[15] +
				 m[9]  * m[7]  * m[14] +
				 m[13] * m[6]  * m[11] -
				 m[13] * m[7]  * m[10];

		inv[4] = -m[4]  * m[10] * m[15] +
				  m[4]  * m[11] * m[14] +
				  m[8]  * m[6]  * m[15] -
				  m[8]  * m[7]  * m[14] -
				  m[12] * m[6]  * m[11] +
				  m[12] * m[7]  * m[10];

		inv[8] = m[4]  * m[9] * m[15] -
				 m[4]  * m[11] * m[13] -
				 m[8]  * m[5] * m[15] +
				 m[8]  * m[7] * m[13] +
				 m[12] * m[5] * m[11] -
				 m[12] * m[7] * m[9];

		inv[12] = -m[4]  * m[9] * m[14] +
				   m[4]  * m[10] * m[13] +
				   m[8]  * m[5] * m[14] -
				   m[8]  * m[6] * m[13] -
				   m[12] * m[5] * m[10] +
				   m[12] * m[6] * m[9];

		inv[1] = -m[1]  * m[10] * m[15] +
				  m[1]  * m[11] * m[14] +
				  m[9]  * m[2] * m[15] -
				  m[9]  * m[3] * m[14] -
				  m[13] * m[2] * m[11] +
				  m[13] * m[3] * m[10];

		inv[5] = m[0]  * m[10] * m[15] -
				 m[0]  * m[11] * m[14] -
				 m[8]  * m[2] * m[15] +
				 m[8]  * m[3] * m[14] +
				 m[12] * m[2] * m[11] -
				 m[12] * m[3] * m[10];

		inv[9] = -m[0]  * m[9] * m[15] +
				  m[0]  * m[11] * m[13] +
				  m[8]  * m[1] * m[15] -
				  m[8]  * m[3] * m[13] -
				  m[12] * m[1] * m[11] +
				  m[12] * m[3] * m[9];

		inv[13] = m[0]  * m[9] * m[14] -
				  m[0]  * m[10] * m[13] -
				  m[8]  * m[1] * m[14] +
				  m[8]  * m[2] * m[13] +
				  m[12] * m[1] * m[10] -
				  m[12] * m[2] * m[9];

		inv[2] = m[1]  * m[6] * m[15] -
				 m[1]  * m[7] * m[14] -
				 m[5]  * m[2] * m[15] +
				 m[5]  * m[3] * m[14] +
				 m[13] * m[2] * m[7] -
				 m[13] * m[3] * m[6];

		inv[6] = -m[0]  * m[6] * m[15] +
				  m[0]  * m[7] * m[14] +
				  m[4]  * m[2] * m[15] -
				  m[4]  * m[3] * m[14] -
				  m[12] * m[2] * m[7] +
				  m[12] * m[3] * m[6];

		inv[10] = m[0]  * m[5] * m[15] -
				  m[0]  * m[7] * m[13] -
				  m[4]  * m[1] * m[15] +
				  m[4]  * m[3] * m[13] +
				  m[12] * m[1] * m[7] -
				  m[12] * m[3] * m[5];

		inv[14] = -m[0]  * m[5] * m[14] +
				   m[0]  * m[6] * m[13] +
				   m[4]  * m[1] * m[14] -
				   m[4]  * m[2] * m[13] -
				   m[12] * m[1] * m[6] +
				   m[12] * m[2] * m[5];

		inv[3] = -m[1] * m[6] * m[11] +
				  m[1] * m[7] * m[10] +
				  m[5] * m[2] * m[11] -
				  m[5] * m[3] * m[10] -
				  m[9] * m[2] * m[7] +
				  m[9] * m[3] * m[6];

		inv[7] = m[0] * m[6] * m[11] -
				 m[0] * m[7] * m[10] -
				 m[4] * m[2] * m[11] +
				 m[4] * m[3] * m[10] +
				 m[8] * m[2] * m[7] -
				 m[8] * m[3] * m[6];

		inv[11] = -m[0] * m[5] * m[11] +
				   m[0] * m[7] * m[9] +
				   m[4] * m[1] * m[11] -
				   m[4] * m[3] * m[9] -
				   m[8] * m[1] * m[7] +
				   m[8] * m[3] * m[5];

		inv[15] = m[0] * m[5] * m[10] -
				  m[0] * m[6] * m[9] -
				  m[4] * m[1] * m[10] +
				  m[4] * m[2] * m[9] +
				  m[8] * m[1] * m[6] -
				  m[8] * m[2] * m[5];

		det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		if (det == 0)
			return false;

		det = 1.0 / det;

		for (i = 0; i < 16; i++)
			invOut[i] = inv[i] * det;

		return true;
	}

	// The actual matrix data
	float m[16];
};

#endif /* VEC3F_H_ */
