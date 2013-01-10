#define GLEW_STATIC
#include "main.h"
#include "Systems/InputManager.h"
#include "GameStates/GameState.h"
#include "GameStates/GameLevel.h"
#include "GameStates/Menu.h"

#include <sstream>
#include <fstream>
#include <direct.h>
#include "stdlib.h"
#include "time.h"
#include "Util/init.h"
#include "Util/text.h"
#include "Util/Mat4.h"
#include "Systems/Graphics/Shaders/shaders.h"

bool fullscreen = false;

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
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
SDL_Event event;
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

Mix_Music *music = NULL;
TTF_Font *mainFont = NULL;

void inputCallback (MappedInput& inputs);

using namespace std;

//ofstream ctt("CON");

/**
 * init() initalizes SDL, creating a fullscreen window, and initalizes all the game objects
 * @return
 * 		bool flag stating whether the init succeeded with errors or not.
 */
bool init() {
	//Initialise all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		return false;
	}
//	freopen( "CON", "w", stdout );
//	freopen( "CON", "w", stderr );
	const SDL_VideoInfo* vidInfo = SDL_GetVideoInfo();
	width = 2048;
	height = 1200;
	SCREEN_WIDTH = fullscreen?vidInfo->current_w:vidInfo->current_w/1.25;
	SCREEN_HEIGHT = fullscreen?vidInfo->current_h:vidInfo->current_h/1.25;
	cout << SCREEN_WIDTH << " " << SCREEN_HEIGHT << endl;
	//Create Window
	unsigned int flags = SDL_OPENGL | SDL_HWSURFACE | (fullscreen ? SDL_FULLSCREEN:0);
	if (SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, flags)
			== NULL) {
		return false;
	}
	cout << "Created SDL Window" << endl;
	//Initialize OpenGL
	if (init_GL(SCREEN_WIDTH, SCREEN_HEIGHT) == false) {
		return false;
	}
	cout << "Initialized OpenGL." << endl;
	//Set the window caption
	SDL_WM_SetCaption("Flocking Wars", NULL );

	//Init audio
	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16;
	int audio_channels = 2;
	int audio_buffers = 4096;
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
		printf("Unable to open audio!\n");
		exit(1);
	}
	Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	music = Mix_LoadMUS("res/music.mp3");
	if(music)
		Mix_FadeInMusic(music, -1, 5000);
	Mix_VolumeMusic(volume);
	//Text
	TTF_Init();
	//Load font
	cout <<  TTF_WasInit() << endl;
	mainFont = TTF_OpenFont("res/inconsolata.ttf", 72);
	if(!mainFont) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		// handle error
	}
	int numChars = 91;
	Glyph* glyphs = new Glyph[numChars];
	generateGlyphs(mainFont, numChars, glyphs);
	initFont(glyphs, numChars);
	//Game Variables
	srand(time(NULL));//Initialize random generator

	//DisplayLists/Textures
	cout << "Init: textures & display lists" << endl;
	cout << "glow" << endl;
	glowTex = loadTexture("res/glow.png");
	cout << "glow" << endl;
	buttonTex = loadTexture("res/buttonTexture.png");
	cout << "glogo!" << endl;
	logoTex = loadTexture("res/logo.png");
	cout << logoTex << endl;
	//Bullet
	short int size = 3;
	float constMeshX[] = {-size/2, size/2, size/2, -size/2};
	float constMeshY[] = {-size, -size, size, size};
	bullet_mesh = glGenLists(1);
	glNewList(bullet_mesh, GL_COMPILE);
	glBegin(DRAWMODE);
	for(int i = 0; i < 4; i++){
		glVertex3f(constMeshX[i], constMeshY[i], 0);
	}
	glEnd();
	glEndList();
	//3D ship
	{
		size = 1;
		float meshX[] = {0.5, -0.5, -0.3, -0.5};
		float meshY[] = {0, -0.5,  0,  0.5};
		float meshZ[] = {0,  0,    0,  0};

		shipMesh = glGenLists(1);
		glNewList(shipMesh, GL_COMPILE);
		glBegin(GL_LINE_LOOP);
		for(int i = 0; i < 4; i++){
			glVertex3f(meshX[i]*size, meshY[i]*size, meshZ[i]*size);
		}
		glEnd();
		glBegin(GL_LINES);
		for(int i = 0; i < 4; i++){
			glVertex3f(0, 0, 0.7);
			glVertex3f(meshX[i]*size, meshY[i]*size, meshZ[i]*size);
		}
		glEnd();
		glEndList();
	}
	{
		float meshX[] = {1.5, 0, -1, 0, 1.5,   0,   0};
		float meshY[] = {0.75, 1,  0,  -1, -0.75, -0.25, 0.25};
		float meshZ[] = {0,  0,  0,  0, 0, 0, 0};
		//Attack Boid
		attackBoidMesh = glGenLists(1);
		glNewList(attackBoidMesh, GL_COMPILE);
		glBegin(GL_LINE_LOOP);
		for(int i = 0; i < 7; i++){
			glVertex3f(meshX[i]*size, meshY[i]*size, meshZ[i]*size);
		}
		glEnd();
		glEndList();
	}
	// Load Shaders
	Shader::initShaders();

	// Input Setup
	inputManager.registerCallback(inputCallback, 0);
	inputManager.pushContext("gameplay");
	return true;
}
/**
 * clean_up() releases all the memory that isn't automatically released and shuts SDL down.
 */
void clean_up() {
	Mix_FreeMusic(music);
	TTF_CloseFont( mainFont );
	Mix_CloseAudio();
	SDL_Quit();
}


void inputCallback (MappedInput& inputs){
	if(inputs.actions.find(Input::A_QUIT_GAME) != inputs.actions.end()){
		quit = true;
	}
	if(currentState != NULL){
		currentState->handleInput(inputs, mousex, mousey, false);
	}
}

void pollEvents(){
	while (SDL_PollEvent(&event)) {
		switch(event.type){
		case SDL_QUIT:
			quit = true;
		break;
		case SDL_KEYDOWN:
			keys[event.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			inputManager.setRawInputState(event.key.keysym.sym, false); // Tell inputManager that the key was released.
			keys[event.key.keysym.sym] = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			keys[0] = true;
			keys[1] = false;
			break;
		case SDL_MOUSEBUTTONUP:
			keys[0] = false;
			keys[1] = true;
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

/**
 * Main method. Calls all the init functions, loads events from the event queue, has the main gameloops,
 * calls the update function and renders all the objects.
 */
int main(int argc, char *argv[]) {
//	   char* buffer;
//
//	   // Get the current working directory:
//	   if( (buffer = _getcwd( NULL, 0 )) == NULL )
//	      perror( "_getcwd error" );
//	   else
//	   {
//	      printf( "%s \nLength: %d\n", buffer, strlen(buffer) );
//	      free(buffer);
//	   }

	//Initialize
	if (init() == false) {
		return 1;
	}
	cout << "Init: complete" << endl;
	SDL_ShowCursor(0);
	currentState = new Menu(SCREEN_WIDTH, SCREEN_HEIGHT);
	currentState->init();
	int deltaTime = 1;
	//While the user hasn't quit
	while (quit == false) {
		pollEvents();
		inputManager.dispatchInput();
		inputManager.clear();
		unsigned startTime = SDL_GetTicks();

		// Get mouse position
	    SDL_GetMouseState(&mousex, &mousey);
	    mousey = SCREEN_HEIGHT - mousey;

		startTime = SDL_GetTicks();

		GameState* newState = currentState->update(deltaTime/1000.f);
		if(newState != currentState){
			delete currentState;
			currentState = newState;
			currentState->init();
		}
		currentState->render();

		//Sleep the time remaining for a constant framerate to be maintained
		int dif = SDL_GetTicks() - startTime;
//		renderNum(dif, 50, SCREEN_HEIGHT - 300, 0, 0.5);
		SDL_GL_SwapBuffers();//Swap back buffer
		if ((1000 / FPS) > dif){
			SDL_Delay((1000 / FPS) - dif);
			deltaTime = (1000 / FPS);
		}
		else
			deltaTime = dif;
	}
	//Free memory and quit SDL
	clean_up();
//	ctt.close();
	cout << "All done here." << endl;
	return 0;
}
