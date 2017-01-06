#pragma once
#include "GL/glew.h"
#include <glm/glm.hpp>

#include <vector>

struct vertex {
    glm::vec3 position;
};

enum class DrawType {
    Triangles = GL_TRIANGLES,
    LineStrip = GL_LINE_STRIP,
    Lines = GL_LINES
};

enum class MeshType {
    Static = GL_STATIC_DRAW,
    Dynamic = GL_DYNAMIC_DRAW,
    Streaming = GL_STREAM_DRAW
};

class Mesh {
    public:
         Mesh() { glGenVertexArrays(1, &_vertex_array); }
        ~Mesh() { glDeleteVertexArrays(1, &_vertex_array); }

         Mesh(const std::vector<vertex> &vertices, const std::vector<GLuint> &indices, DrawType = DrawType::Triangles, MeshType = MeshType::Static);
        void draw();
        void update(const std::vector<vertex> &updatedVertices);
   private:
        std::vector<vertex> _vertices;
        std::vector<GLuint> _indices;
        GLuint _vertex_array;
        GLuint _vertex_buffer;
        GLuint _element_buffer;
        DrawType _drawType;
        MeshType _meshType;
};
