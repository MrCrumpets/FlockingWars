/*
 * InteractiveMesh.cpp
 *
 *      This was just a little experiment initially but turned into a fully-fledged feature.
 *      It is a grid/mesh the lies in the background and reacts to input from the game. Explosions
 *      and ship movement send ripples through the mesh causing some cool effects.
 *
 *      It is really just a container for a list of vertices.
 *
 *  Created on: Jun 12, 2010
 *      Author: Nathan
 *
 */

#include "InteractiveMesh.h"

static const float gridSize = 5.f;

/**
 * Mesh Constructor. Initializes all the points and lines in the mesh, and calculates the offset and
 * grid spacing
 */
InteractiveMesh::InteractiveMesh(int rows, int cols, float gridSize) : _rows(rows), _cols(cols){
    glm::vec3 pos;
    _points.resize(rows*cols);
    std::vector<glm::uint32> indices;
    for (int r = 0; r < _rows; r++) {
        for (int c = 0; c < _cols; c++) {
            _points[r * _cols + c] = { pos };
            pos.x += gridSize;
        }
        pos.y += gridSize;
        pos.x = 0;
    }

    for (int r = 0; r < _rows; r++) {
        for (int c = 0; c < _cols; c++) {
            // Horizontal
            if(c + 1< _cols) {
                indices.push_back(r * _cols + c);
                indices.push_back(r * _cols + c + 1);
            }
            // Vertical
            if(r + 1 < _rows) {
                indices.push_back(r * _cols + c);
                indices.push_back((r+1) * _cols + c);
            }
        }
    }

    _mesh.reset(new Mesh(_points, indices, MeshType::Lines));
}

/*
 *      Render() renders the mesh. Simple two vertex GL_LINES, nothing fancy.
 */
void InteractiveMesh::render(){
    _mesh->draw();
}
/**
 * The update method updates all the points and lines in the mesh
 */
void InteractiveMesh::update(){
    for(int i = 0; i < _cols * _rows; i++){
        //points[i]->update();
    }
}

/**
 * Creates an "impulse" at the coordinates specified with the strenght specified. Sends a wave through the mesh
 * from the impulse point (x, y) with a value of (strength^2/sqrt(dist_from_point), which gives a nice decay with
 * distance.
 *
 * @param x
 *              The x coordinate of the impulse
 * @param y
 *              The y coordinate of the impulse
 * @param strength
 *              The strength of the impulse
 * @return
 *              void
 */
void InteractiveMesh::createImpulse (float x, float y, float strength){
    /*
    for(int i = 0; i < _rows; i++){
        for(int j = 0; j < _cols; j++){
            float px = points[i * _cols + j]->x;//X coordinate of point
            float py = points[i * _cols + j]->y;//Y coordinate of point
            float dx = px - x;//The distance from the point to the impulse
            float dy = py - y;//The distance from the point to the impulse
            float offx = 0;
            float offy = 0;
            float offz = 0;
            float is = inverseSqrt(dx * dx + dy * dy);//1/dist
            offx = (strength*(is)*(dx/25));//The desired offset
            offy = (strength*(is)*(dy/25));
            offz = std::min((strength*strength*(is)), 3.0f);
            float maxH = 10;
            offz = offz > maxH ? maxH : offz;
            points[i * _cols + j]->x += offx;
            points[i * _cols + j]->y += offy;
            points[i * _cols + j]->z += offz;
        }
    }*/
}
