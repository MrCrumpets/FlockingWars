#include "shaders.h"
#include <stdio.h>
#include <stdlib.h>

using std::cout;
using std::endl;

static const GLfloat g_vertex_buffer_data[] = {
	-1.f, -1.f,
	 1.f, -1.f,
	-1.f,  1.f,
	 1.f,  1.f
};

static const GLushort g_element_buffer_data[] = { 0, 1, 2, 3 };

void *fileContents(const char *filename, GLint *length)
{
    FILE *f = fopen(filename, "r");
    void *buffer;

    if (!f) {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    *length = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer = malloc(*length+1);
    *length = fread(buffer, 1, *length, f);
    fclose(f);
    ((char*)buffer)[*length] = '\0';

    return buffer;
}

static void showInfoLog(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{


	cout << "LOG:" << endl;
    GLint log_length;
    char *log;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    log = (char*) malloc(log_length);
    glGet__InfoLog(object, log_length, NULL, log);
    fprintf(stdout, "%s", log);
    free(log);
}

Shader::Shader(const char *vertFilename, const char *fragFilename, int _width, int _height){
	buildShaders(vertFilename, fragFilename);
	width = _width;
	height = _height;
}

int Shader::enable(){
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    // Activate our shader program
    glUseProgram(g_resources.program);

//    // Assign our uniform attributes
//
//    // Time
//    glUniform1f(g_resources.uniforms.time, g_resources.time);
//
//    // Resolution
//    glUniform2f(g_resources.uniforms.resolution, width, height);
//
    // Set up vertex array
//    glBindBuffer(GL_ARRAY_BUFFER, g_resources.vertex_buffer);
//    glVertexAttribPointer(
//            g_resources.attributes.position /* attribute */,
//            2,                              /* size */
//            GL_FLOAT,                       /* type */
//            GL_FALSE,                       /* normalized */
//            sizeof(GLfloat)*2,              /* stride */
//            (void*)0                        /* array buffer offset */
//    );
//    // Bind vertex array attributes
//    glEnableVertexAttribArray(g_resources.attributes.position);
//
//    // Send draw call
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_resources.element_buffer);
//    glDrawElements(
//            GL_TRIANGLE_STRIP,  /* Draw mode */
//            4,                  /* count */
//            GL_UNSIGNED_SHORT,  /* type */
//            (void*)0            /* element array buffer offset */
//    );
//
//
//    glDisableVertexAttribArray(g_resources.attributes.position);

	return 0;
}

GLuint Shader::setUniform(const char* name){
	return glGetUniformLocation(g_resources.program, name);
}

GLuint Shader::setAttribute(const std::string &name){
    return glGetAttribLocation(g_resources.program, name.c_str());
}

int Shader::disable(){
	glUseProgram(0);
	return 0;
}

GLuint Shader::makeShader (GLenum type, const char *filename){
    GLint length;
    GLchar *source = (GLchar*) fileContents(filename, &length);
    GLuint shader;
    GLint shader_ok;

    if(!source)
        return 0;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&source, &length);
    free (source);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
    if(!shader_ok) {
        cout << "Failed to compile " << filename << endl;
        showInfoLog(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint Shader::makeProgram(GLuint vertex_shader, GLuint fragment_shader)
{
    GLint program_ok;
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);

    if(!program_ok) {
        cout << "Failed to link shader program" << endl;
        showInfoLog(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}


int Shader::buildShaders(const char *vertFilename, const char *fragFilename){

    // Shaders
    g_resources.vertex_shader = makeShader (GL_VERTEX_SHADER, vertFilename);
    if (g_resources.vertex_shader == 0)
        return 0;
    g_resources.fragment_shader = makeShader (GL_FRAGMENT_SHADER, fragFilename);
    if (g_resources.fragment_shader == 0)
        return 0;

    g_resources.program = makeProgram(g_resources.vertex_shader, g_resources.fragment_shader);

    if(g_resources.program == 0)
        return 0;
    return 1;
}

int Shader::initShaders() {
    glewInit();
    if(!GLEW_VERSION_2_0)
    {
        cout << "OpenGL 2.0 not avaible" << endl;
        return 1;
    }
    return 0;
}
