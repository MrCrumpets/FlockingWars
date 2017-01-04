#ifndef SHADERS_H_
#define SHADERS_H_

#include "GL/glew.h"
#include <iostream>

class Shader {
public:
	Shader(const std::string &vertFilename, const std::string &fragFilename, int width, int height);
	GLuint makeShader(GLenum type, const std::string &filename);
	GLuint makeProgram(GLuint vertex_shader, GLuint fragment_shader);
	GLuint makeTexture(const std::string &filename);
	GLuint makeBuffer(GLenum, const void *buffer_data, GLsizei buffer_size);
	GLuint setUniform(const std::string &name);
	GLuint setAttribute(const std::string &name);
	int enable();
	int disable();
	int buildShaders(const std::string & vertFileName, const std::string &fragFileName);
	~Shader();

	static int initShaders(); // Initializes GLEW

	int width, height;
	struct {
	    GLuint vertex_buffer, element_buffer;

	    GLuint vertex_shader, fragment_shader, program;

	    struct {
	        GLint resolution;
	        GLint time;
	    } uniforms;

	    struct {
	        GLint position;
	    } attributes;

	    GLfloat time;
	} g_resources;
};
#endif
