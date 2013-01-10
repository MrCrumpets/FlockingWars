#ifndef SHADERS_H_
#define SHADERS_H_

#include "GL/glew.h"
#define NO_SDL_GLEXT
#include <iostream>

class Shader {
public:
	Shader(const char *vertFilename, const char *fragFilename, int width, int height);
	GLuint makeShader(GLenum type, const char *filename);
	GLuint makeProgram(GLuint vertex_shader, GLuint fragment_shader);
	GLuint makeTexture(const char *filename);
	GLuint makeBuffer(GLenum, const void *buffer_data, GLsizei buffer_size);
	GLuint setUniform(const char* name);
	int enable();
	int disable();
	int buildShaders(const char * vertFileName, const char *fragFileName);
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
