#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "TextView.h"
#include "draw.h"
#include "input.h"
#include "rendering.h"
#include "gui.h"
#include "FileExplorer.h"

int page;
bool greyScale = true;
bool hideUI = false;
bool hideOptions = true;

void TextView_Main() //This is the text screen
{
	//Draw mode specific ui and related input is handled
	if (hideUI && hideOptions) //Text view shown
	{
		if ((input & (KEY_RIGHT | KEY_R)) && nextAvailable) { page++; }
		else if ((input & (KEY_LEFT | KEY_L)) && page != 0) { page--; }
		else if ((posX > 0 && posX < 320) && (posY > 0 && posY < 240)) { hideUI = false; quickui_animation = 0; }
	}
	else
	{	
		if ((posX > 221 && posX < 320) && (posY > 240 - 24 && posY < 240)) hideOptions = !hideOptions; //Hide/show menu
		if (hideOptions) //Only Quick UI shown
		{
			if ((posX > 0 && posX < 320) && (posY > 0 && posY < 240 - 24)) hideUI = true;
			if ((posX > 0 && posX < 100) && (posY > 240 - 24 && posY < 240)) mode = 0; //Switch to main menu view
			if ((posX > 100 && posX < 221) && (posY > 240 - 24 && posY < 240)) TextView_SaveBookmark(path, page);
		}
		else //Quick ui + options menu shown
		{
			if ((posX > 55 && posX < 146) && (posY > 81 && posY < 171)) greyScale = true;
			if ((posX > 173 && posX < 264) && (posY > 81 && posY < 171)) greyScale = false;
		}
	}
}

void TextView_SaveBookmark()
{
	if (page != TextView_GetBookmark())
	{
		FILE *fp;
		char str[4];
		sprintf(str, "%04d", page);
		fp = fopen(path, "r+");
		fwrite(str, 1, sizeof(str), fp);
		fclose(fp);
	}
}

int TextView_GetBookmark()
{
	char buffer[4];

	FILE* fp;
	fp = fopen(path, "r");
	fseek(fp, SEEK_SET, 0);
	fread(buffer, 4, 1, fp); //Reads the file
	fclose(fp);

	return atoi(buffer);
}