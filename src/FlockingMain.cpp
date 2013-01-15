#define GLEW_STATIC

#include "main.h"
#include <SFML/Window.hpp>
#include "Systems/InputManager.h"
#include "Systems/Graphics/Shaders/shaders.h"
#include "Systems/ResourceManager/Manager.h"
#include "GameStates/GameState.h"
#include "GameStates/GameLevel.h"
#include "GameStates/Menu.h"

#include "stdlib.h"
#include "Util/init.h"
#include "Util/Mat4.h"

sf::Window sfmlWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "FlockingWars");

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

bool init() {
	//Initialize OpenGL
	if (init_GL(SCREEN_WIDTH, SCREEN_HEIGHT) == false) {
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

void inputCallback (MappedInput& inputs){
	if(inputs.actions.find("quit") != inputs.actions.end()){
		quit = true;
	}
	if(currentState != NULL){
		currentState->handleInput(inputs, mousex, mousey, false);
	}
}

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

int main() {

	Resource::Manager m("res/config.xml");
	if (init() == false) {
		return 1;
	}
	cout << "Init: complete" << endl;

	sf::Clock clock;
	currentState = new Menu(SCREEN_WIDTH, SCREEN_HEIGHT);
	currentState->init();
	sf::Time deltatime;
	cout << "Entering main loop" << endl;
	while (quit == false) {
		pollEvents();
		inputManager.dispatchInput();
		inputManager.clear();
		clock.restart();
		GameState* newState = currentState->update(deltatime.asSeconds());
		if(newState != currentState){
			delete currentState;
			currentState = newState;
			currentState->init();
		}
		currentState->render();
		deltatime = clock.getElapsedTime();

		//Sleep the time remaining for a constant framerate to be maintained
		sfmlWindow.display();
//		cout << deltatime.asMicroseconds() << " " << deltatime.asMilliseconds() << " " << deltatime.asSeconds() << endl;
		float target_fps = 30.0f;
		sf::Time sleeptime = sf::seconds(1.0f/target_fps) - deltatime;
		sf::sleep(sleeptime);
	}
	cout << "All done here." << endl;
	return 0;
}
