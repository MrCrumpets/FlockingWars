/*
 * Vec2f.h
 *
 *  Created on: Jan 24, 2011
 *      Author: Nathan
 */

#ifndef VEC2F_H_
#define VEC2F_H_

#include "math.h"

class Vec2f {
public:
	Vec2f(){x = y = 0.0f;}
	Vec2f(float _x, float _y){x = _x; y = _y;}
	Vec2f operator+(Vec2f a)
	{
		return Vec2f(a.x + x, a.y + y);
	}
	Vec2f operator-(Vec2f a)
	{
		return Vec2f(x - a.x, y - a.y);
	}
	Vec2f operator*(Vec2f a)
	{
		return Vec2f(a.x * x, a.y * y);
	}
	Vec2f operator*(float a)
	{
		return Vec2f(x*a, y*a);
	}
	Vec2f operator/(float a)
	{
		return Vec2f(x/a, y/a);
	}
	Vec2f& operator*=(float a){
			x *= a; y *= a;
			return *this;
	}
	Vec2f& operator+=(Vec2f a){
			x += a.x; y+=a.y;
			return *this;
	}
	Vec2f& operator-=(Vec2f a){
				x -= a.x;
				y -= a.y;
				return *this;
	}
	float dot(Vec2f a){
		return x*a.x + y*a.y;
	}
	void zero(){
		x = y= 0.0f;
	}
//	void random(int max){
//		x = rand()%max; y = rand()%max;
//	}
	float lengthSq(){
		return x*x + y*y;
	}
	float length(){
		return sqrt(x*x + y*y);
	}
	void normalize(){
		float l = length();
		if(l > 0){
			x /= l;
			y /= l;
		}
	}
	Vec2f perp(){
		return Vec2f(-y, x);
	}
	Vec2f normalized(){
		float l = length();
		return (l > 0 ? Vec2f(x/l, y/l): *this);
	}
	float theta(){
		return atan2(y, x);
	}
	float x, y;
};

#endif /* VEC2F_H_ */
