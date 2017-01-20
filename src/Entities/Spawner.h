#pragma once
/*
 * Spawner.h
 *
 * This entity acts somewhat like the homebase. Again, this is leftover
 * from the old version. I'm not sure what it's purpose will be.
 */
class Flock;

#include "Entity.h"
#include "Flock.h"
#include "../Systems/Graphics/Renderer.h"
#include "../Systems/Graphics/Mesh.hpp"
#include <queue>
#include <memory>

struct Line {
    glm::vec3 last_pos, pos;
};

class Spawner : public Entity {
    private:
        std::unique_ptr<Mesh> _mesh;
        std::vector<vertex> _vertices;
    public:
        Spawner(Renderer *r, glm::vec3 pos, const std::string &team);
        void render();
        void update(float dt);
        void interact(Entity *) {};
        virtual ~Spawner();
        std::vector<Line> lines;
        int timer;
        std::queue<EntityType> queue;
        float money;
        float moneyRate;
        int lastSpawned;
        glm::vec3 screenPos;
};
