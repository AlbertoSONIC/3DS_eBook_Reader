#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "app.h"
#include "draw.h"
#include "input.h"
#include "rendering.h"
#include "gui.h"
#include "fileexplorer.h"

int page;
bool greyScale = true;
bool hideUI = true;
bool hideOptions = true;


void MenuView()
{
	//This is the main menu
	FileExplorerMain();
}

void TextView(char* title, char* path) //This is the text screen
{
	hideUI = true;
	page = 0;
	while (page != GetBookmark(path))
	{
		drawTextView(path, page, greyScale);
		page++;
	}

	while (aptMainLoop())
	{
		getFB();

		drawTextView(path, page, greyScale);
		drawStatusBar(title);

		//Gets input	
		getInput();

		//Draw mode specific ui and related input is handled
		if (hideUI && hideOptions) //Text view shown
		{
			if (input & KEY_RIGHT && nextAvailable) { page++; }
			else if (input & KEY_LEFT && page != 0) { page--; }
			else if (input & KEY_UP && greyScale) { greyScale = false; }
			else if (input & KEY_DOWN && !greyScale) { greyScale = true; }
			else if ((posX > 0 && posX < 320) && (posY > 0 && posY < 240)) { hideUI = false; quickui_animation = 0; }
		}
		else
		{		
			drawQuickUi();
			if ((posX > 221 && posX < 320) && (posY > 240 - 24 && posY < 240)) hideOptions = !hideOptions; //Hide/show menu
			if (hideOptions) //Only Quick UI shown
			{
				if ((posX > 0 && posX < 320) && (posY > 0 && posY < 240 - 24)) hideUI = true;
				if ((posX > 0 && posX < 100) && (posY > 240 - 24 && posY < 240)) break;
				if ((posX > 100 && posX < 221) && (posY > 240 - 24 && posY < 240)) SaveBookmark(path, page);
			}
			else //Quick ui + options menu shown
			{
				drawOptionsUi(greyScale);
				if ((posX > 55 && posX < 146) && (posY > 81 && posY < 171)) greyScale = true;
				if ((posX > 173 && posX < 264) && (posY > 81 && posY < 171)) greyScale = false;
			}
		}

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
		gspWaitForVBlank();
	}
}

void SaveBookmark(char* path, int page)
{
	if (page != GetBookmark(path))
	{
		FILE *fp;
		char str[4];
		sprintf(str, "%04d", page);
		fp = fopen(path, "r+");
		fwrite(str, 1, sizeof(str), fp);
		fclose(fp);
	}
}

int GetBookmark(char* path)
{
	char buffer[4];

	FILE* fp;
	fp = fopen(path, "r");
	fseek(fp, SEEK_SET, 0);
	fread(buffer, 4, 1, fp); //Reads the file
	fclose(fp);

	return atoi(buffer);
}