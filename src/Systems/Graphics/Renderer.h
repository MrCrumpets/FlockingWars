/*
 * Renderer.h
 *
 *  Created on: 2012-12-02
 *      Author: Nathan
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <memory>
#include "Font.h"

class Renderer {
    public:
	Renderer(int width, int height);
	void loadShader(const std::string &filename);
	void useShader(int id);
	GLuint makeBuffer(GLenum target, const void *buffer_data, GLsizei buffer_size);
	void setMatrix();
	void renderVArray(int varray);
	GLuint setUniform(const std::string& );
	void disableShader();
	glm::vec3 unProject(int x, int y, double z);
	void setColor(const glm::vec4 &c);
	void setColor(float r, float g, float b, float a);
	glm::vec3 getCursorPos(int x, int y);
	void loadFont(const std::string &name);
	void renderString(glm::vec3 pos, const std::string &text);
	void bindTexture(int texture);
	void drawVertexArray(const std::vector<float> &vertices);

	// Matrix functions
	glm::mat4 setProjection(int width, int height);
	void uploadModelView();
	void setCamera(float x, float y, float z);
	void pushMatrix();
	glm::mat4& currentMatrix();
	glm::mat4 translate(const glm::vec3 &pos);
	glm::mat4 rotate(float angle, float x, float y, float z);
        glm::mat4 rotate(const glm::vec3 &from, const glm::vec3 &to);
	glm::mat4 scale(float x, float y, float z);
	void loadIdentity();
	void popMatrix();

	virtual ~Renderer();
	struct {
		GLuint projection, camera, model, color, texture;
	} uniforms;

    private:
	std::vector<glm::mat4> matrixStack;
        glm::mat4 camera;
        std::unique_ptr<Font> _font;
	int width, height;
};

#endif /* RENDERER_H_ */
