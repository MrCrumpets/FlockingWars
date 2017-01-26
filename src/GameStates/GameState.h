#pragma once
/*
 * GameState.h
 *
 * Abstract base class for any state of the game. This is an implementation of
 * a finite state machine. The main gameloop holds a pointer to one GameState
 * at any time, which it will update and render. If the gamestate needs to
 * change, then the update function will return a pointer to a new GameState.
 *
 * Types of GameStates (currently):
 *      - GameLevel
 *      - Menu
 */
#include "../Systems/InputManager.h"

// See top of file
class GameState {
    public:
        GameState() { };

        // Should perform any tasks that need to happen before the first update/render
        virtual void init() = 0;

        // Lets the gamestate know that it will be paused
        virtual void pause() = 0;

        // Lets the gamestate know that updates/render will resume
        virtual void resume() = 0;

        // Handles the mapped inputs that correspond to this context
        virtual void handleInput(MappedInput& input, int x, int y, bool mouseDown) = 0;

        // Returns pointer to self normally
        // Returns pointer to a new GameState if the game state should change
        virtual GameState* update(float dt) = 0;

        // Renders the game state
        virtual void render() = 0;
        virtual ~GameState(){};
};
