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
appMode_t mode = MENU;

int main()
{
	// Initialize services
	srvInit();
	aptInit();
	acInit();
	ptmuInit();
	hidInit();
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
		case MENU:
			//Main screen
			FileExplorer_Main();
			GUI_MenuView();
			GUI_StatusBar("");
			GUI_FileExplorer();
			break;

		case BOOK_INTRO:
		    if(!GUI_BookIntro()) mode = TEXT;
		    break;

		case TEXT:
			//Text screen
			TextView_Main();
			GUI_TextView(path, page, greyScale);
			GUI_StatusBar(title);
			if (!hideUI)GUI_QuickUi();
			if (!hideOptions)GUI_OptionsUi(greyScale);
			break;

		case EXIT:
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
	ptmuExit();
	acExit();
	hidExit();
	aptExit();
	srvExit();
	return 0;
}