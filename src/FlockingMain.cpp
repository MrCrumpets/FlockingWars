/*
 * FlockingMain.cpp
 *
 * Entry point for the game. Initializes SFML, OpenGL. Handles input,
 * updates and renders the current game state in main game loop.
 *
 */

#include "main.h"
#include <SFML/Window.hpp>
#include "Systems/InputManager.h"
#include "Systems/Graphics/Shaders/shaders.h"
#include "GameStates/GameState.h"
#include "GameStates/GameLevel.h"
#include "GameStates/Menu.h"

#include "stdlib.h"
#include "Util/init.h"

sf::Window sfmlWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "FlockingWars", 
        sf::Style::Default, sf::ContextSettings(32, 0, 0, 3, 3));

bool fullscreen = false;

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 800;
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


int volume = 0;

GameState* currentState = NULL;

InputManager inputManager;

GLuint bullet_mesh;
GLuint particle_DL;
GLuint glowTex;
GLuint buttonTex;
GLuint shipMesh;
GLuint attackBoidMesh;
GLuint boidSquadronDL;
GLuint logoTex;
GLuint logoDL;

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
	while (sfmlWindow.pollEvent(event)) {
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
            std::cerr << "Init failed, exiting" << std::endl;
            return 1;
	}
	cout << "Init: complete" << endl;

	sf::Clock clock;
	currentState = new Menu(SCREEN_WIDTH, SCREEN_HEIGHT);
	currentState->init();
	sf::Time deltatime;
	cout << "Entering main loop" << endl;
	while (quit == false) {
		clock.restart();
		GameState* newState = currentState->update(deltatime.asSeconds());
		pollEvents();
		inputManager.dispatchInput();
		inputManager.clear();
		if(newState != currentState){
			delete currentState;
			currentState = newState;
			currentState->init();
		}
		currentState->render();
                // Check for OpenGL errors
                GLenum err;
                while ((err = glGetError()) != GL_NO_ERROR) {
                    std::cerr << "Post-render OpenGL error: " << err << std::endl;
                }

		deltatime = clock.getElapsedTime();

		//Sleep the time remaining for a constant framerate to be maintained
		sfmlWindow.display();
		float target_fps = 30.0f;
		deltatime = sf::seconds(1.0f/target_fps) - deltatime;
		sf::sleep(deltatime);
	}
	cout << "All done here." << endl;
	return 0;
}
