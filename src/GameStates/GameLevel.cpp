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
#include "../Systems/Graphics/Renderer.h"

Renderer* renderer;
Flock* selectedFlock;

struct {
    glm::vec3 pos;
    glm::vec3 pressed;
    bool dragging;
    float r,g,b,a;
} cursor;

GameLevel::GameLevel(int width, int height) 
    : width(width), height(height) {
    camera.x = -width/2;
    camera.y = -width/2;
    camera.z = -1450;
    cursor.r = cursor.g = cursor.b = cursor.a = 1.0f;
    cursor.dragging = false;
}

void GameLevel::init(){

    renderer = new Renderer(width, height);
    renderer->setColor(1.0f, 1.0f, 1.0f, 0.5f);

    // Load Font
    renderer->loadFont("res/Inconsolata.ttf");

    spawner = new Spawner(renderer, glm::vec3(1, 1, 1), PLAYER);
    player = new Player(renderer, spawner->pos, 25);
    entities.push_back(player);
    entities.push_back(spawner);
    /*
       Flock* f = new Flock();
       selectedFlock = f;
       for(int i = 0; i < 25; i++){
       Boid* p = new Boid(glm::vec3(rand()%25, rand()%25, 0.0f), PLAYER);
       entities.push_back(p);
       f->addEntity(p);
       }
       flocks.push_back(f);
       */
}

GameState* GameLevel::update(float dt){
    for(auto e : entities){
        e->update(dt);
        if(cursor.dragging){
            e->selected = contains(e->pos, cursor.pos, cursor.pressed);
        }
    }

    for(auto f : flocks) {
        f->update(dt);
    }

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
    auto it = std::begin(entities);
    while(it != std::end(entities)){
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
    renderer->setCamera(player->pos.x, player->pos.y, glm::length(player->vel) / 10.f + 30.f);

    renderer->pushMatrix();
    //renderCursor(renderer);
    //renderer->renderString(glm::vec3(0,0,0), "Player 1");
    for(auto e : entities){
        e->render();
    }
    renderer->popMatrix();
}

bool GameLevel::contains(glm::vec3 in, glm::vec3 a, glm::vec3 b){
    if((in.x - a.x)*(in.x - b.x) < 0 && (in.y - a.y)*(in.y - b.y) < 0)
        return true;
    return false;
}

void GameLevel::renderCursor(Renderer* r){
    r->pushMatrix();
    r->setColor(cursor.r, cursor.g, cursor.b, cursor.a);
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
    if(inputs.actions.find("left_mouse_press") != inputs.actions.end()) {
        cursor.pressed = cursor.pos;
    }

    if(inputs.states.find("left_mouse_down") != inputs.states.end() && cursor.pos != cursor.pressed) {
        cursor.dragging = true;
    }
    else {
        cursor.dragging = false;
    }

    if(inputs.actions.find("left_mouse_release") != inputs.actions.end()){
        selectedFlock->setObjective(cursor.pos);
    }

}

GameLevel::~GameLevel() {
    delete renderer;
}

