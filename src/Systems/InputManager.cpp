/*
 * InputManager.cpp
 *
 *	Taken largely
 *
 *  Created on: Jun 7, 2012
 *      Author: Nathan
 */

#include "InputManager.h"
#include "ResourceManager/FileIO.h"
#include <fstream>

using namespace Input;
using std::cout;
using std::endl;

InputManager::InputManager() {
	keys = { false };
	unsigned count = 0;
	std::ifstream infile("res/config/contexts");
	count = readData<int>(infile);
	cout << count << endl;
	for(unsigned i = 0; i < count; i++){
		std::string name = readData<std::string>(infile);
		std::string fileName = readData<std::string>(infile);
		inputContexts[name] = new InputContext(fileName);
	}
}

void InputManager::clear(){
	currentMap.actions.clear();
	currentMap.states.clear();
}

void InputManager::setRawInputState(int button, bool pressed){
	Action a;
	State s;
	if(pressed && !keys[button])
	{
		if(mapButtonToAction(button, a)){
			currentMap.actions.insert(a);
		}
	}
	if(pressed)
	{
		if (mapButtonToState(button, s)){
			currentMap.states.insert(s);
			return;
		}
	}
	else
		mapAndEat(button);
}

void InputManager::dispatchInput() const {
	MappedInput input = currentMap;
    for(std::multimap<int, InputCallback>::const_iterator iter = callbackTable.begin(); iter != callbackTable.end(); ++iter) {
    	(*iter->second)(input);
    }
}

void InputManager::registerCallback(InputCallback callback, int priority){
	callbackTable.insert(std::make_pair(priority, callback));
}

void InputManager::pushContext(const std::string& name){
	std::map<std::string, InputContext*>::iterator iter = inputContexts.find(name);
	if(iter == inputContexts.end()){
		std::cout << "Invalid InputContext* pushed." << std::endl;
	}
	activeContexts.push_front(iter->second);
}

void InputManager::popContext(){
	if(activeContexts.empty())
		std::cout << "Active Context stack is empty; cannot pop context." << std::endl;
	else
		activeContexts.pop_front();
}

bool InputManager::mapButtonToAction(int button, Action& action) const {
	for(std::list<InputContext*>::const_iterator iter = activeContexts.begin(); iter != activeContexts.end(); ++iter){
		const InputContext* context = *iter;
		if(context->mapButtonToAction(button, action)){
			return true;
		}
	}
	return false;
}

bool InputManager::mapButtonToState(int button, State& state) const {
	for(std::list<InputContext*>::const_iterator iter = activeContexts.begin(); iter != activeContexts.end(); ++iter){
		const InputContext* context = *iter;
		if(context->mapButtonToState(button, state)){
			return true;
		}
	}
	return false;
}

void InputManager::mapAndEat(int button){
	Action action;
	State state;

	if(mapButtonToAction(button, action))
		currentMap.EatAction(action);
	if(mapButtonToState(button, state))
		currentMap.EatState(state);
	keys[button] = false;
}

InputContext::InputContext(std::string fileName){
	std::ifstream contextFile((std::string("res/config/") + fileName).c_str());
	// Read States
	unsigned numStates = readData<unsigned>(contextFile);
	for(unsigned i = 0; i < numStates; i++)
	{
		int button = static_cast<int>(readData<int>(contextFile));
		State state = static_cast<State>(readData<int>(contextFile));
		stateMap[button] = state;
	}
	// Read Actions
	unsigned numActions = readData<unsigned>(contextFile);
	for(unsigned i = 0; i < numActions; i++)
	{
		int button = static_cast<int>(readData<int>(contextFile));
		Action action = static_cast<Action>(readData<int>(contextFile));
		actionMap[button] = action;
	}
}

bool InputContext::mapButtonToAction(int button, Action& out) const {
	std::map<int, Action>::const_iterator iter = actionMap.find(button);
	if(iter == actionMap.end())
		return false;
	out = iter->second;
	return true;
}

bool InputContext::mapButtonToState(int button, State& out) const {
	std::map<int, State>::const_iterator iter = stateMap.find(button);
	if(iter == stateMap.end())
		return false;
	out = iter->second;
	return true;
}


InputContext::~InputContext(){
}

InputManager::~InputManager() {
	for(std::map<std::string, InputContext*>::iterator iter = inputContexts.begin(); iter != inputContexts.end(); iter++)
		delete iter->second;
}
