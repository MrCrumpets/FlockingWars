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
Spawner* playerSpawn;
Player* player;

static const glm::vec4 playerColor = { 129.f / 255.f, 204.f / 255.f, 60.f / 255.f, 1.f };
static const glm::vec4 enemyColor = { 181.f / 255.f, 40.f / 255.f, 65.f / 255.f, 1.f };

struct {
    glm::vec3 pos;
    glm::vec3 pressed;
    bool dragging;
    float r,g,b,a;
} cursor;

GameLevel::GameLevel(int width, int height) 
    : width(width), height(height), _im(50, 50, 1.5f) {
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

    playerSpawn = new Spawner(renderer, glm::vec3(1, 1, 0), "Player");
    player = new Player(renderer, playerSpawn->pos, 25);
    entities.push_back(player);
    entities.push_back(playerSpawn);
    entities.push_back(new Spawner(renderer, glm::vec3(50, 50, 0), "Enemy"));
    Flock* f = new Flock();
    selectedFlock = f;
    for(int i = 0; i < 25; i++){
        Boid* p = new Boid(renderer, glm::vec3(rand()%50 + 50, rand()%50 + 50, 0.0f), "Enemy");
        entities.push_back(p);
        f->addEntity(p);
    }
    flocks.push_back(f);
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
    for(auto e : entities){
        while(!e->entityStack.empty()){
            newObjects.push_back(e->entityStack.front());
            e->entityStack.pop_front();
        }
        if(e->dead){
            if(e->type == EntityType::Ship){
                player->respawn(playerSpawn->pos);
            }
        }
    }

    entities.erase( std::remove_if(
                entities.begin(), 
                entities.end(), 
                [](Entity *e) { return e->dead; }
                ), entities.end() );

    for(unsigned int i = 0; i < newObjects.size(); i++){
        entities.push_back(newObjects[i]);
    }
    return this;
}

void GameLevel::render(){
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glClearColor(0.f,0.f,0.f,1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    renderer->setCamera(player->pos.x, player->pos.y, glm::length(player->vel) / 10.f + 80.f);

    renderer->pushMatrix();
    for(auto e : entities){
        renderer->pushMatrix();
            if(e->team == "Player") {
                renderer->setColor(playerColor);
            } else {
                renderer->setColor(enemyColor);
            }
            renderer->translate(e->pos);
            renderer->rotate(glm::vec3(1.f, 0.f, 0.f), e->dir);
            e->render();
        renderer->popMatrix();
    }
    renderer->popMatrix();
}

bool GameLevel::contains(glm::vec3 in, glm::vec3 a, glm::vec3 b){
    if((in.x - a.x)*(in.x - b.x) < 0 && (in.y - a.y)*(in.y - b.y) < 0)
        return true;
    return false;
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

