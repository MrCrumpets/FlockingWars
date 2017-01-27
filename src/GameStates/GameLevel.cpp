#include <imgui/imgui.h>
#include "glm/ext.hpp"

#include "GameLevel.h"

float color[3] = { 0.f, 0.f, 0.f };
std::vector<char> buff;

GameLevel::GameLevel(sol::state &lua)
    : _lua(lua) , _em(lua)
{ 
    // Instantiate sub-systems (rendering, physics, ..)
    _systems["physics"] = (std::make_unique<PhysicsSystem>());
    _systems["graphics"] = (std::make_unique<GraphicsSystem>(1920, 1080));

    lua["createEntity"] = [this](){
	auto &e = _em.createEntity();
        e.addComponent<GraphicsComponent*>(_systems["graphics"]->getComponent());
        e.addComponent<PhysicsComponent*>(_systems["physics"]->getComponent());
	return e;
    };
}

void GameLevel::init(){
}

GameState* GameLevel::update(float dt){
    for(auto &system : _systems) {
        system.second->update(dt);
    }
    return this;
}

void GameLevel::render(){
}

void GameLevel::handleInput(MappedInput& inputs, int x, int y, bool mouseDown){
    if(inputs.states.find("shoot") != inputs.states.end()){ }
    if(inputs.states.find("thrust") != inputs.states.end()){ }
    if(inputs.states.find("rotate_left") != inputs.states.end()){ }
    if(inputs.states.find("rotate_right") != inputs.states.end()){ }
    if(inputs.actions.find("left_mouse_press") != inputs.actions.end()) { }
}

GameLevel::~GameLevel() {
}

