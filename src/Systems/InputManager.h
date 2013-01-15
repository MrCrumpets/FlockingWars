/*
 * InputManager.h
 *
 * Uses a modified version of ApochPiQ's example code and article on GameDev.
 *
 * Input Mapping Demo
 * Copyright (c) 2011, Michael A. Lewis
 * All rights reserved.
 *
 *  Created on: Jun 7, 2012
 *      Author: Nathan
 */

#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

#include "InputConstants.h"
#include "rapidxml.hpp"
#include <iostream>
#include <map>
#include <string>
#include <list>
#include <set>

using std::map;
using rapidxml::xml_node;
using namespace Input;
class InputContext;

typedef std::string Action;
typedef std::string State;

/**
 * Current input mapping. Holds all the states and actions that have occurred since last poll.
 */
struct MappedInput
{
	std::set<Action> actions;
	std::set<State> states;

	void EatAction(Action action)
	{
		actions.erase(action);
	}
	void EatState(State state)
	{
		states.erase(state);
	}
};

typedef void (*InputCallback)(MappedInput& inputs);

class InputManager {
	bool keys[323];
	// Constructor / Destructor
	public:
		InputManager();
		~InputManager();
	// Event Polling
	public:
		void pollEvents();
	// Input Dispatching
	public:
		void dispatchInput() const;
		void clear(void);
		void registerCallback(InputCallback callback, int priority);
	// Context Management
	public:
		void pushContext(const std::string& name);
		void popContext();
		void setRawInputState(int button, bool pressed);
	// Buttons
	private:
		bool mapButtonToAction(int, Action& ) const;
		bool mapButtonToState(int, State&) const;
		void mapAndEat (int button);
	private:
		std::map<std::string, InputContext*> inputContexts;
		std::list<InputContext*> activeContexts;
		std::multimap<int, InputCallback> callbackTable;

		void registerButtonStrings();
		int getKey(const std::string &string);
		MappedInput currentMap;
		bool keysDown[323];
		int mousex, mousey;
};

class InputContext {
	public:
		InputContext (xml_node<>* context);
		bool mapButtonToAction(int button, Action& out) const;
		bool mapButtonToState(int button, State& out) const;
		virtual ~InputContext();
	public:
		std::map<int, Action> actionMap;
		std::map<int, State> stateMap;
};

#endif /* INPUTMANAGER_H_ */
