#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "main.h"
#include "app.h"
#include "draw.h"
#include "gui.h"
#include "input.h"
#include "rendering.h"

//FPS Counter
bool GW_MODE;


int main()
{
	// Initialize services
	srvInit();
	aptInit();
	acInit();
	ptmInit();
	hidInit(NULL);
	gfxInitDefault();

	//As nop90 suggested
	getFB();

	//The app code:
	MenuView();

	// Exit services
	gfxExit();
	ptmExit();
	acExit();
	hidExit();
	aptExit();
	srvExit();
	return 0;
}