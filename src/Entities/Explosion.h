#pragma once

#include "Entity.h"
#include "../Systems/Graphics/Renderer.h"
#include "../Systems/Graphics/Mesh.hpp"

class Explosion : public Entity{
    struct Particle {
        glm::vec3 last_pos, pos;
        float energy;
    };

    public:
        Explosion(Renderer* r, glm::vec3 pos);
        void update(float);
        void render();
        void interact(Entity* e){}
        virtual ~Explosion();
    private:
        int _numDead;
        int _energy;
        std::vector<Particle> particles;
        std::unique_ptr<Mesh> _mesh;
        std::vector<vertex> _vertices;
};
