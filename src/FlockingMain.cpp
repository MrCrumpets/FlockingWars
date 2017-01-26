/*
 * FlockingMain.cpp
 *
 * Entry point for the game. Initializes SFML, OpenGL. Handles input,
 * updates and renders the current game state in main game loop.
 *
 */

#include <SFML/Window.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui-sfml.h>
#include <sol.hpp>

#include "Systems/InputManager.h"
#include "Systems/console.hpp"
#include "Systems/Graphics/Shaders/shaders.h"
#include "GameStates/GameState.h"
#include "GameStates/GameLevel.h"
#include "GameStates/Menu.h"

#include "stdlib.h"
#include "Util/init.h"

static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;
static bool show_console = false;
bool quit = false;
int mousex = 0, mousey = 0;
static bool keys[323] = { false };

sf::Window window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "FlockingWars",
        sf::Style::Default, sf::ContextSettings(32, 0, 0, 3, 3));

std::unique_ptr<GameState> currentState;
static sol::state lua;

InputManager inputManager;
void inputCallback (MappedInput& inputs);
std::unique_ptr<Console> console;

// Initializes OpenGl, Shader Extensions.
bool init() {
    //Initialize OpenGL
    if (init_GL(SCREEN_WIDTH, SCREEN_HEIGHT) == false) {
        std::cerr << "init_GL failed" << std::endl;
        return false;
    }

    srand(time(NULL));//Initialize random generator

    // Load Shaders
    Shader::initShaders();

    // Fire up dear imgui
    ImGui::SFML::Init(window);
    console = std::make_unique<Console>();
    console->registerCallback(
	    [] (const std::string &command) {
		try {
		    lua.script(command);
		}
		catch (const sol::error& e) {
		    std::cerr << "Caught: " << e.what() << std::endl;
		}
	    });
    // Input Setup
    inputManager.registerCallback(inputCallback, 0);
    inputManager.pushContext("gameplay");
    return true;
}

// Currently I only have the one main input callback which sends the
// input to the current GameState.
void inputCallback (MappedInput& inputs){
    if(inputs.actions.find("quit") != inputs.actions.end()){
        quit = true;
    }
    if(currentState != nullptr){
        currentState->handleInput(inputs, mousex, mousey, false);
    }
}

// Gets events from SFML and passes them off to the InputManager
void pollEvents(){
    sf::Event event;
    while (window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(event);
        switch(event.type){
            case sf::Event::Closed:
                quit = true;
                break;
            case sf::Event::MouseMoved:
                mousex = event.mouseMove.x;
                mousey = SCREEN_HEIGHT - event.mouseMove.y;
                break;
            case sf::Event::KeyPressed:
                keys[event.key.code] = true;
                if(event.key.code == 56) show_console = !show_console;
                break;
            case sf::Event::KeyReleased:
                inputManager.setRawInputState(event.key.code, false); // Tell inputManager that the key was released.
                keys[event.key.code] = false;
                break;
            default:
                break;
        }
    }
    // Register input for all keys currently down.
    for(int i = 0; i < 323; i++){
        if(keys[i]){
            inputManager.setRawInputState(i, true);
        }
    }
}


// Entry point.
int main() {
    if (init() == false) {
        return 1;
    }

    currentState = std::make_unique<GameLevel>(lua);
    currentState->init();
    sf::Time deltatime;
    sf::Clock clock;
    while (!quit) {
        sf::Time timeDelta = clock.restart();
        pollEvents();
        ImGui::SFML::Update(window, timeDelta);
        inputManager.dispatchInput();
        inputManager.clear();

        GameState* newState = currentState->update(timeDelta.asSeconds());
        if(newState != currentState.get()){
            currentState.reset(newState);
            currentState->init();
        }

        // Check for OpenGL errors
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "Post-render OpenGL error: " << err << std::endl;
        }


        currentState->render();

        if(show_console) {
            console->draw("console", &show_console);
        }

        ImGui::Render();

        //Sleep the time remaining for a constant framerate to be maintained
        window.display();
        sf::sleep(sf::seconds(1.f / 30.f) - clock.getElapsedTime());
    }
    cout << "All done here." << endl;
    return 0;
}
