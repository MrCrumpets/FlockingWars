/*
 * GameLevel.cpp
 *
 * GameState for the actual game world. It stores, updates, and renders
 * all the entities in the level.
 *
 * I am planning on making it load a level configuration from a text
 * file in order to have pre-designed and varied levels.
 */

#include "GameLevel.h"
#include "../Util/Mat4.h"
#include "../Systems/Graphics/Renderer.h"

Renderer* renderer;
Flock* selectedFlock;

struct {
	Vec3f pos;
	Vec3f pressed;
	bool dragging;
	float r,g,b,a;
} cursor;

GameLevel::GameLevel(int width, int height) {
	camera.x = -width/2;
	camera.y = -width/2;
	camera.z = -1450;
	this->width = width;
	this->height = height;
	cursor.r = cursor.g = cursor.b = cursor.a = 1.0f;
	cursor.dragging = false;
}

void GameLevel::init(){
	spawner = new Spawner(Vec3f(1, 1, 1), PLAYER);
	player = new Player(spawner->pos, 25);
	entities.push_back(player);
	entities.push_back(spawner);
	Flock* f = new Flock();
	selectedFlock = f;
	for(int i = 0; i < 25; i++){
		Boid* p = new Boid(Vec3f(rand()%25, rand()%25, 0.0f), PLAYER);
		entities.push_back(p);
		f->addEntity(p);
	}
	flocks.push_back(f);

	renderer = new Renderer(width, height);
	time = renderer->setUniform("utime");
	offsetUniform = renderer->setUniform("uoffset");
	glUniform2f(offsetUniform, 0.5f, 0.25f);
	renderer->setCamera(0.5f, -0.25f, 150.0f);
	renderer->setColor(1.0f, 1.0f, 1.0f, 0.5f);

	// Load Font
	renderer->loadFont("res/Inconsolata.ttf");
}

GameState* GameLevel::update(float dt){
	renderer->useShader(0);
	glUniform1f(time, dt);
	for(vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it){
		(*it)->update(dt);
		if(cursor.dragging){
			if(contains((*it)->pos, cursor.pos, cursor.pressed))
				(*it)->g = 0.0f;
			else
				(*it)->g = 1.0f;
		}
	}
	for(vector<Flock*>::iterator it = flocks.begin(); it != flocks.end(); ++it)
		(*it)->update(dt);

	for(unsigned int i = 0; i < entities.size(); i++){
		for(unsigned int j = i; j < entities.size(); j++){
			if(j !=i){
				entities[i]->interact(entities[j]);
				entities[j]->interact(entities[i]);
			}
		}
	}
	// Delete Dead Objects, Add New Ones
	std::vector<Entity*> newObjects;
	for(vector<Entity*>::iterator it = entities.begin(); it != entities.end();){
		while(!(*it)->entityStack.empty()){
			newObjects.push_back((*it)->entityStack.front());
			(*it)->entityStack.pop_front();
		}
		if((*it)->dead){
			if((*it)->type == SHIP){
				player->respawn(spawner->pos);
				it++;
			}
			else{
				delete *it;
				it = entities.erase(it);
			}
		}
		else {
			it++;
		}
	}
	for(unsigned int i = 0; i < newObjects.size(); i++){
		entities.push_back(newObjects[i]);
	}
	return this;
}

void GameLevel::render(){
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	renderer->pushMatrix();
	renderer->setCamera(-player->pos.x, -player->pos.y, 150);
	renderCursor(renderer);
	renderer->renderString(Vec3f(0,0,0), "Player 1");
	vector<Entity*>::iterator it;
	for(it = entities.begin(); it != entities.end(); it++){
		(*it)->render(renderer);
	}
	renderer->popMatrix();
}

bool GameLevel::contains(Vec3f in, Vec3f a, Vec3f b){
	if((in.x - a.x)*(in.x - b.x) < 0 && (in.y - a.y)*(in.y - b.y) < 0)
		return true;
	return false;
}
void GameLevel::renderCursor(Renderer* r){
	r->pushMatrix();
	r->applyCamera();
	r->setColor(cursor.r, cursor.g, cursor.b, cursor.a);
	r->uploadModelView();
	glBegin(GL_QUADS);
	float h = 1;
	glVertex3f(cursor.pos.x - h/2, cursor.pos.y - h/2, 0);
	glVertex3f(cursor.pos.x + h/2, cursor.pos.y - h/2, 0);
	glVertex3f(cursor.pos.x + h/2, cursor.pos.y + h/2, 0);
	glVertex3f(cursor.pos.x - h/2, cursor.pos.y + h/2, 0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(cursor.pressed.x - h/2, cursor.pressed.y - h/2, 0);
	glVertex3f(cursor.pressed.x + h/2, cursor.pressed.y - h/2, 0);
	glVertex3f(cursor.pressed.x + h/2, cursor.pressed.y + h/2, 0);
	glVertex3f(cursor.pressed.x - h/2, cursor.pressed.y + h/2, 0);
	glEnd();
	if(cursor.dragging){
		glBegin(GL_QUADS);
		glVertex3f(cursor.pressed.x, cursor.pressed.y, 0);
		glVertex3f(cursor.pos.x, cursor.pressed.y, 0);
		glVertex3f(cursor.pos.x, cursor.pos.y, 0);
		glVertex3f(cursor.pressed.x, cursor.pos.y, 0);
		glEnd();
	}
	r->popMatrix();
}

void GameLevel::handleInput(MappedInput& inputs, int x, int y, bool mouseDown){
	if(inputs.states.find("shoot") != inputs.states.end()){
		player->shoot();
	}
	if(inputs.states.find("thrust") != inputs.states.end()){
		player->accelerate(1.f);
	}
	if(inputs.states.find("rotate_left") != inputs.states.end()){
		player->rotate(1.f);
	}
	if(inputs.states.find("rotate_right") != inputs.states.end()){
		player->rotate(-1.f);
	}

	cursor.pos = renderer->getCursorPos(x, y);
	if(inputs.actions.find("left_mouse_press") != inputs.actions.end())
		cursor.pressed = cursor.pos;

	if(inputs.states.find("left_mouse_down") != inputs.states.end() && cursor.pos != cursor.pressed)
		cursor.dragging = true;
	else
		cursor.dragging = false;

	if(inputs.actions.find("left_mouse_release") != inputs.actions.end()){
		selectedFlock->setObjective(cursor.pos);
	}

}

GameLevel::~GameLevel() {
	delete renderer;
}

