#ifndef INPUT_H
#define INPUT_H
#include <3ds.h>

void getInput();
void InputWait();

extern u32 old_input;
extern u32 input;
extern u16 posX;
extern u16 posY;

#endif
