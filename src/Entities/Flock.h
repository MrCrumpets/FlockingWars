#pragma once
class Spawner;

#include "Entity.h"
#include "../Systems/Graphics/Renderer.h"
#include "../Systems/Graphics/Mesh.hpp"

// These are tiny little ships that fly around and don't do much else. I
// am not sure how much longer they'll be around before I replace them
// with something more abstract.
class Boid : public Entity {
    public:
        Boid();
        Boid(Renderer *r, const glm::vec3 &pos, const std::string &team);
        void update(float dt);
        void interact(Entity* e);
        void render();
        void die();
        virtual ~Boid();
        float minSpeed, maxSpeed, minSq, maxSq;
        int rank;
        float fear;
};

/**
 * Groups entities into a squad for convenient ordering and commanding.
 */

class Flock {
    public:
        char tag; // For identification (ie. Alpha, Beta)
        Flock();
        void setTag(char t);
        void addEntity(Entity* e);
        void removeEntity(Entity* e);
        void setObjective(const glm::vec3 &o);
        int update(float dt);
        virtual ~Flock(){}
        std::vector<Entity*> members;
};

