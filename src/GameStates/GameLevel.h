#pragma once
#include <vector>
#include <glm/fwd.hpp>

#include "GameState.h"
#include "../EntityManager.hpp"
#include "../Systems/InputManager.h"

class GameLevel: public GameState {
    public:
        GameLevel();
        void pause() {};
        void resume() {};
        void handleInput(MappedInput& input, int x, int y, bool mouseDown);
        void init();
        void render();
        GameState* update(float dt);
        virtual ~GameLevel();
    private:
        EntityManager _em;
};
