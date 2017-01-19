#pragma once

#include "Entity.h"
#include "../Systems/Graphics/Renderer.h"

class Explosion : public Entity{
    struct Particle {
        glm::vec3 last_pos, pos;
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
        std::vector<int> life;
};
