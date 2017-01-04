#pragma once

#include "Entity.h"
#include "../Util/includes.h"
#include "../Systems/Graphics/Renderer.h"
#include "../Systems/Graphics/Mesh.hpp"

class Bullet: public Entity {
    public:
        Bullet(Renderer *r, glm::vec3 pos, glm::vec3 vel, int bulletRange, unsigned char team);
        void render();
        void update(float);

        // Determines if this bullet intersected with the entity parameter
        // in the last time step.
        void interact(Entity* e);
        bool isDead(void);
        virtual ~Bullet();
    private:
        float size;
        float energy;
        float maxEnergy;
        Mesh _mesh;
};
