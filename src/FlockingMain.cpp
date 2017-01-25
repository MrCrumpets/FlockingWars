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

#include "Systems/InputManager.h"
#include "Systems/Graphics/Shaders/shaders.h"
#include "GameStates/GameState.h"
#include "GameStates/GameLevel.h"
#include "GameStates/Menu.h"

#include "stdlib.h"
#include "Util/init.h"


bool fullscreen = false;

int SCREEN_WIDTH = 1920;
int SCREEN_HEIGHT = 1080;
int uiWidth = 1024, uiHeight = 600;
const int SCREEN_BPP = 24;
const int FPS = 30;
bool quit = false;
bool DEBUG = false;
bool joystick = false;
bool mainMenuBuilt = false;
const int MAINMENU = 0, GAMEMODE = 1, STOREMODE = 0;
int gameMode = 0;
int mousex = 0, mousey = 0;
bool keys[323] = {false};

sf::Window window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "FlockingWars",
        sf::Style::Default, sf::ContextSettings(32, 0, 0, 3, 3));

int volume = 0;

GameState* currentState = NULL;

InputManager inputManager;
void inputCallback (MappedInput& inputs);

using namespace std;

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
    if(currentState != NULL){
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

    currentState = new GameLevel();
    currentState->init();
    sf::Time deltatime;
    sf::Clock clock;

    while (quit == false) {
        sf::Time timeDelta = clock.restart();
        pollEvents();
        GameState* newState = currentState->update(timeDelta.asSeconds());
        ImGui::SFML::Update(window, timeDelta);
        inputManager.dispatchInput();
        inputManager.clear();

        if(newState != currentState){
            delete currentState;
            currentState = newState;
            currentState->init();
        }

        // Check for OpenGL errors
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "Post-render OpenGL error: " << err << std::endl;
        }

        currentState->render();
        ImGui::Render();

        //Sleep the time remaining for a constant framerate to be maintained
        window.display();
        sf::sleep(sf::seconds(1.f / 30.f) - clock.getElapsedTime());
    }
    cout << "All done here." << endl;
    return 0;
}
