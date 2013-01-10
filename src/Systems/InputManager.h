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

#ifndef INPUTMANGER_H_
#define INPUTMANGER_H_

#include "SDL/SDL.h"
#include "InputConstants.h"
#include <iostream>
#include <map>
#include <string>
#include <list>
#include <set>

using std::map;
using namespace Input;
class InputContext;


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

		MappedInput currentMap;
		bool keysDown[323];
		int mousex, mousey;
};

class InputContext {
	public:
		InputContext (std::string fileName);
		bool mapButtonToAction(int button, Action& out) const;
		bool mapButtonToState(int button, State& out) const;
		virtual ~InputContext();
	public:
		std::map<int, Action> actionMap;
		std::map<int, State> stateMap;
};

#endif /* INPUTMANGER_H_ */
