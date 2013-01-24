/*
 * Vec3f.h
 *
 * Performs all the important vector operations.
 */

#ifndef VEC3F_H_
#define VEC3F_H_

#include "math.h"


class Vec3f {
public:
	Vec3f(){x = y = z = 0.0f;
			r = &x; g = &y;b = &z;}
	Vec3f(float _x, float _y, float _z){x = _x; y = _y; z = _z;}

	Vec3f operator=(const Vec3f& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	Vec3f operator+(const Vec3f &rhs) const
	{
		return Vec3f(rhs.x + x, rhs.y + y, rhs.z + z);
	}
	Vec3f operator-(const Vec3f &rhs) const
	{
		return Vec3f(x - rhs.x, y - rhs.y, z - rhs.z);
	}
	Vec3f operator*(Vec3f &a)
	{
		return Vec3f(a.x * x, a.y * y, a.z * z);
	}
	Vec3f operator/(Vec3f &a)
	{
		return Vec3f(x / a.x, y/a.y, z/a.z);
	}
	Vec3f operator*(const float a) const
	{
		return Vec3f(x*a, y*a, z*a);
	}

	Vec3f operator/(float a)
	{
		return Vec3f(x/a, y/a, z/a);
	}
	Vec3f& operator*=(float a){
			x *= a; y *= a; z *= a;
			return *this;
	}
	Vec3f& operator+=(Vec3f a){
			x += a.x; y+=a.y; z += a.z;
			return *this;
	}
	Vec3f& operator-=(Vec3f a){
				x -= a.x;
				y -= a.y;
				z -= a.z;
				return *this;
	}

	bool operator==(const Vec3f &a) const {
		return a.x == x && a.y == y && a.z == z;
	}

	bool operator!=(const Vec3f &a) const {
		return !(*this == a);
	}

	float dot(Vec3f a){
		return x*a.x + y*a.y + z*a.z;
	}
	Vec3f cross(Vec3f a){
		return Vec3f(y*a.z - z*a.y, z*a.x - x * a.z, x*a.y-y*a.x);//[a2b3-a3b2, a3b1-a1b3, a1b2-a2b1]
	}
	void zero(){
		x = y = z = 0.0f;
	}
	float lengthSq(){
		return x*x + y*y + z*z;
	}
	float length(){
		return sqrt(x*x + y*y + z*z);
	}
	void normalize(){
		float l = length();
		if(l > 0){
			x /= l;
			y /= l;
			z /= l;
		}
	}
	float theta(){
			return atan2(y, x);
		}
	Vec3f normalized(){
		float l = length();
		return (l > 0 ? Vec3f(x/l, y/l, z/l): *this);
	}
	float x, y, z;
	float *r, *g, *b;
};

#endif /* VEC3F_H_ */
