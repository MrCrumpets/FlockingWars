/*
 * Spawner.h
 *
 * This entity acts somewhat like the homebase. Again, this is leftover
 * from the old version. I'm not sure what it's purpose will be.
 */
class Flock;

#ifndef SPAWNER_H_
#define SPAWNER_H_

#include "Entity.h"
#include "Flock.h"
#include "../Util/includes.h"
#include "../Systems/Graphics/Renderer.h"
#include <queue>

const int numLines = 10;

class Line : public Entity{
    private:
        float _constVel;
    public:
        Line(){}
        Line(glm::vec3, glm::vec3);
        void render();
        void update(float dt);
        void spawnBoids(){};
        glm::vec3 center;
};

class Spawner : public Entity {
    public:
        Spawner(Renderer *r, glm::vec3, int);
        void render();
        void update(float dt);
        virtual ~Spawner();
        std::vector<Line> lines;
        glm::vec3 center;
        int timer;
        std::queue<unsigned char> queue;
        float money;
        float moneyRate;
        int lastSpawned;
        glm::vec3 screenPos;
};

#endif /* SPAWNER_H_ */
