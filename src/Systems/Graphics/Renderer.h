/*
 * Renderer.h
 *
 *  Created on: 2012-12-02
 *      Author: Nathan
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "GL/glew.h"
#include "../../Util/Vec3f.h"
#include "../../Util/Mat4.h"
#include <map>
#include <vector>

class Renderer {
public:
	Renderer(int width, int height);
	void loadShader(const char* filename);
	void useShader(int id);
	GLuint makeBuffer(GLenum target, const void *buffer_data, GLsizei buffer_size);
	void setMatrix();
	void renderVArray(int varray);
	GLuint setUniform(const char* name);
	void disableShader();
	Vec3f unProject(int x, int y, double z);
	void setColor(Vec3f c);
	void setColor(float r, float g, float b, float a);
	Vec3f getCursorPos(int x, int y);

	// Matrix functions
	Mat4 setProjection(int width, int height);
	void uploadModelView();
	void setCamera(float x, float y, float z);
	void applyCamera();
	void pushMatrix();
	Mat4& currentMatrix();
	Mat4 translate(float x, float y , float z);
	Mat4 rotate(float angle, float x, float y, float z);
	Mat4 scale(float x, float y, float z);
	void loadIdentity();
	void popMatrix();

	virtual ~Renderer();
	struct {
		GLuint projMat, modelMat, color;
	} uniforms;
private:
	std::vector<Mat4> matrixStack;
	Mat4 camera;
	int width, height;

};

#endif /* RENDERER_H_ */
