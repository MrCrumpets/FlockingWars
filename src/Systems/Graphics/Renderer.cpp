/*
 * Renderer.cpp
 *
 *  Created on: 2012-12-02
 *      Author: Nathan
 */

#include "Renderer.h"
#include "Shaders/shaders.h"

Shader* shader;
Mat4 projection;
Mat4 invProjection;

Renderer::Renderer(int width, int height) {
	this->height = height;
	this->width = width;
	matrixStack.push_back(Mat4(1.0f));
}

void Renderer::loadShader(const char* filname){
	shader = new Shader("res/shaders/vert.glsl", "res/shaders/frag.glsl", width, height);
}

GLuint Renderer::makeBuffer(GLenum target, const void *buffer_data, GLsizei buffer_size){
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, buffer_size, buffer_data, GL_STATIC_DRAW);
    return buffer;
}

Vec3f Renderer::unProject(int x, int y, double z){
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	double mv[16], p[16];
	for(int i = 0; i < 16; i++){
		mv[i] = (double)(currentMatrix()).m[i];
		p[i] = (double)projection.m[i];
	}
	double wx = x, wy = y, wz = z;
	double vx = -1, vy = -1, vz = 0-1;
	gluUnProject(wx, wy, wz, mv, p, viewport, &vx, &vy, &vz);
	return Vec3f(vx, vy, vz);
}

void Renderer::setColor(float r, float g, float b, float a){
	glUniform4f(uniforms.color, r, g, b, a);
}

Vec3f Renderer::getCursorPos(int x, int y){
	pushMatrix();
	applyCamera();
	Vec3f p0 = unProject(x, y, 0.0);
	Vec3f p1 = unProject(x, y, 1.0);
	float u = -p0.z/(p1.z - p0.z);
	popMatrix();
	return p0 + (p1 - p0)*u;
}

#define PROJECTION_FOV_RATIO 0.7f
#define PROJECTION_NEAR_PLANE 0.0625f
#define PROJECTION_FAR_PLANE 1024.0f

Mat4 Renderer::setProjection(int width, int height){
	Mat4 result;
	GLdouble wf = (GLdouble)width, hf = (GLdouble)height;
	GLdouble
		r_xy_factor = fminf(wf, hf) * 1.0f/PROJECTION_FOV_RATIO,
		r_x = r_xy_factor/wf,
		r_y = r_xy_factor/hf,
		r_zw_factor = 1.0f/(PROJECTION_FAR_PLANE - PROJECTION_NEAR_PLANE),
		r_z = (PROJECTION_NEAR_PLANE + PROJECTION_FAR_PLANE)*r_zw_factor,
		r_w = -2.0f*PROJECTION_NEAR_PLANE*PROJECTION_FAR_PLANE*r_zw_factor;

	result.m[0*4 + 0] = r_x;  result.m[0*4 + 1] = 0.0f; result.m[0*4 + 2] = 0.0f; result.m[0*4 + 3] = 0.0f;
	result.m[1*4 + 0] = 0.0f; result.m[1*4 + 1] = r_y;  result.m[1*4 + 2] = 0.0f; result.m[1*4 + 3] = 0.0f;
	result.m[2*4 + 0] = 0.0f; result.m[2*4 + 1] = 0.0f; result.m[2*4 + 2] = r_z;  result.m[2*4 + 3] = 1.0f;
	result.m[3*4 + 0] = 0.0f; result.m[3*4 + 1] = 0.0f; result.m[3*4 + 2] = r_w;  result.m[3*4 + 3] = 0.0f;
	projection = result;
	return result;
}

void Renderer::uploadModelView(){
    glUniformMatrix4fv(uniforms.modelMat, 1, GL_FALSE, currentMatrix().m);
}

void Renderer::setCamera(float x, float y, float z){
	camera = Mat4::translation(x, y, z);
}

void Renderer::applyCamera(){
	matrixStack.back() = matrixStack.back() * camera;
}

void Renderer::pushMatrix(){
	matrixStack.push_back(matrixStack.back());
}

void Renderer::popMatrix(){
	matrixStack.pop_back();
}

Mat4 Renderer::translate(float x, float y, float z){
	Mat4 t = Mat4::translation(x, y, z);
	matrixStack.back() = matrixStack.back() * t;
	return matrixStack.back();
}

Mat4 Renderer::rotate(float angle, float x, float y, float z){
	Mat4 t = Mat4::rotation(angle, x, y, z);
	matrixStack.back() = matrixStack.back() * t;
	return matrixStack.back();
}

Mat4 Renderer::scale(float x, float y, float z){
	Mat4 t = Mat4::scale(x, y, z);
	matrixStack.back() = matrixStack.back() * t;
	return matrixStack.back();
}

Mat4& Renderer::currentMatrix(){
	return matrixStack.back();
}

void Renderer::loadIdentity(){

}

GLuint Renderer::setUniform(const char* name){
	return shader->setUniform(name);
}

void Renderer::useShader(int id){
	shader->enable();
}

void Renderer::disableShader(){
	shader->disable();
}


Renderer::~Renderer() {}
