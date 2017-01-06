#pragma once
/*
 * InteractiveMesh.h
 *
 *  Created on: Jun 12, 2010
 *      Author: Nathan
 */


#include <glm/glm.hpp>
#include <memory>
#include "Systems/Graphics/Renderer.h"
#include "Systems/Graphics/Mesh.hpp"

class InteractiveMesh {
    private:
        std::unique_ptr<Mesh> _mesh;
        int _rows, _cols;

        std::vector<vertex> _points;
    public:
        InteractiveMesh(int rows, int cols, float gridSize);
        void render();
        void update();
        void createImpulse(float, float, float);
        ~InteractiveMesh() {};
};
