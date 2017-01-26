#pragma once
#include <vector>
#include <glm/fwd.hpp>
#include <sol.hpp>

#include "GameState.h"
#include "../EntityManager.hpp"
#include "../Systems/InputManager.h"

class GameLevel: public GameState {
    public:
        GameLevel(sol::state &lua);
        void pause() {};
        void resume() {};
        void handleInput(MappedInput& input, int x, int y, bool mouseDown);
        void init();
        void render();
        GameState* update(float dt);
        virtual ~GameLevel();
    private:
        EntityManager _em;
        std::map<std::string, std::unique_ptr<System>> _systems;
        sol::state &_lua;
};
