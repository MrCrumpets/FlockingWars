/*
 * InputManager.cpp
 *
 *	Taken largely
 *
 *  Created on: Jun 7, 2012
 *      Author: Nathan
 */

#include "InputManager.h"
#include "SFML/Window/Keyboard.hpp"
#include "rapidxml.hpp"
#include <fstream>
#include <vector>

using std::cout;
using std::endl;

using namespace rapidxml;
xml_document<> doc;

std::map<std::string, sf::Keyboard::Key>& keyStrings()
{
  static std::map<std::string, sf::Keyboard::Key> *map = new std::map<std::string, sf::Keyboard::Key>();
  return *map;
}

InputManager::InputManager() {
	std::cout << "Parsing key mapping" << std::endl;
	registerButtonStrings();
	std::ifstream infile("res/config/keys.xml");
	std::vector<char> buffer((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	try
	{
	    doc.parse<0>(&buffer[0]);
	}
	catch(rapidxml::parse_error &e)
	{
		std::cout << "XML parse error." << std::endl;
		std::cout << e.what() << std::endl << e.where<char>() << std::endl;
		throw;
	}
	for (xml_node<> *node = doc.first_node();
			node; node = node->next_sibling())
	{
		inputContexts[node->first_attribute()->value()] = new InputContext(node);
	}
}

void InputManager::registerButtonStrings(){
	keyStrings()["escape"] = sf::Keyboard::Escape;
	keyStrings()["space"] = sf::Keyboard::Space;
	keyStrings()["w"] = sf::Keyboard::W;
	keyStrings()["a"] = sf::Keyboard::A;
	keyStrings()["s"] = sf::Keyboard::S;
	keyStrings()["d"] = sf::Keyboard::D;
}

inline int getKey(const std::string &string){
	if(keyStrings().find(string) != keyStrings().end())
		return keyStrings()[string];
	return static_cast<int>(string[0]);
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

InputContext::InputContext(xml_node<> *context){
	std::cout << "first context: " << context->first_attribute()->value() <<  std::endl;
	xml_node<> *keymap = context->first_node();
	// Read States
	for(xml_node<> *node = keymap->first_node();
			node; node = node->next_sibling())
	{
		int key = getKey(node->first_attribute()->value());
		State state (node->first_attribute()->next_attribute()->value());
		stateMap[key] = state;
	}
	keymap = keymap->next_sibling();
	// Read Actions
	for(xml_node<> *node = keymap->first_node();
			node; node = node->next_sibling())
	{
		std::cout << node->first_attribute()->value() << std::endl;
		int key = getKey(node->first_attribute()->value());
		Action action (node->first_attribute()->next_attribute()->value());
		actionMap[key] = action;
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
