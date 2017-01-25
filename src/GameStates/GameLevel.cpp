/*
 * GameLevel.cpp
 *
 * GameState for the actual game world. It stores, updates, and renders
 * all the entities in the level.
 *
 * I am planning on making it load a level configuration from a text
 * file in order to have pre-designed and varied levels.
 */

#include <imgui/imgui.h>
#include "glm/ext.hpp"

#include "GameLevel.h"

float color[3] = { 0.f, 0.f, 0.f };
std::vector<char> buff;


GameLevel::GameLevel() {
}

void GameLevel::init(){
}

GameState* GameLevel::update(float dt){
    return this;
}

void GameLevel::render(){
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glClearColor(color[0], color[1], color[2], 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::ShowTestWindow();
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

