#include "input.h"

//For ctrulib
#include <3ds.h>

//For sprintF
#include <stdio.h>

//Variables
u32 old_input;
u32 input;
u16 posX;
u16 posY;

void getInput()
{
	//Touch input
	touchPosition myTouchPosition;
	hidTouchRead(&myTouchPosition);

	posX = myTouchPosition.px;
	posY = myTouchPosition.py;

	//HID Input
	old_input = input;
	hidScanInput();
	input = hidKeysDown();

	if (!(!(input & KEY_TOUCH) && (old_input & KEY_TOUCH))){ posX = 0; posY = 0; }
	
}

void InputWait() {
	u32 pad_state_old = hidKeysDown();
	while (true) {
		u32 pad_state = hidKeysDown();
		if (pad_state ^ pad_state_old)
			input = ~pad_state;
	}
}