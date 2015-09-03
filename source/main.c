#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "main.h"
#include "FileExplorer.h"
#include "TextView.h"
#include "draw.h"
#include "gui.h"
#include "input.h"
#include "rendering.h"

//FPS Counter
bool GW_MODE;
int mode = 0;

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
	while (aptMainLoop())
	{

		//As nop90 suggested
		getFB();

		//Gets input (keys and touch)
		getInput();

		switch (mode)
		{
		case 0:
			//Main screen
			FileExplorer_Main();
			GUI_MenuView();
			GUI_StatusBar("");
			GUI_FileExplorer();
			break;

		case 1:
			//Text screen
			TextView_Main();
			GUI_TextView(path, page, greyScale);
			GUI_StatusBar(title);
			if (!hideUI)GUI_QuickUi();
			if (!hideOptions)GUI_OptionsUi(greyScale);
			break;

		case 2:
			//Exit 
			goto close;
			break;
		}

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
	}

	close:

	// Exit services
	gfxExit();
	ptmExit();
	acExit();
	hidExit();
	aptExit();
	srvExit();
	return 0;
}