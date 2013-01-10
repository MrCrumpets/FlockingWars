/*
 * InputConstants.h
 *
 *  Created on: Jun 8, 2012
 *      Author: Nathan
 */

#ifndef INPUTCONSTANTS_H_
#define INPUTCONSTANTS_H_

namespace Input {

	enum RawButton {
		MOUSE_LEFT,
		MOUSE_LEFT_UP,
		MOUSE_RIGHT,
		MOUSE_RIGHT_UP
	};

	enum Action
	{
		A_QUIT_GAME,
		A_LEFT_MOUSE_PRESS,
		A_LEFT_MOUSE_RELEASE,
	};

	enum State
	{
		S_ROTATE_LEFT,	// 0
		S_ROTATE_RIGHT, 	// 1
		S_THRUST,	// 2
		S_SHOOT, 	// 3
		S_REV, 		// 4
		S_DRAGGING, // 5
	};

}

#endif /* INPUTCONSTANTS_H_ */
