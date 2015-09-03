#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gui.h"
#include "app.h"
#include "draw.h"
#include "rendering.h"

//For Background
#include "TextBKG_bin.h"
#include "MenuBKG_bin.h"
#include "FMBKG_bin.h"
#include "NextPG_bin.h"
#include "PrevPG_bin.h"
#include "QuickUI_bin.h"
#include "Exit_bin.h"
#include "Options_bin.h"
#include "Wifi_bin.h"
#include "BatteryLowest_bin.h"
#include "BatteryLowest_bin.h"
#include "BatteryLow_bin.h"
#include "BatteryMid_bin.h"
#include "BatteryHigh_bin.h"
#include "BatteryFull_bin.h"

//For time
#define SECONDS_IN_DAY 86400
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60
int time = 0;

char buffer[100];
bool nextAvailable = true;
int offsets[1000];
int charging_animation = 1;
int quickui_animation = 0;
int bookmark_animation = 0;

u8* batteryLevels[] = {
	(u8*)BatteryLowest_bin,
	(u8*)BatteryLowest_bin,
	(u8*)BatteryLow_bin,
	(u8*)BatteryMid_bin,
	(u8*)BatteryHigh_bin,
	(u8*)BatteryFull_bin,
};

//Needed for the status bar
u32 wifiStatus = 0;
u8 batteryLevel = 5;
u8 charging = 0;


void guiTop(int player, int p1score, int p2score)
{
	////Background
	//if (player==1) gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)blueround_bin, 240, 400, 0, 0);
	//else gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)redround_bin, 240, 400, 0, 0);

	////Prints a blue rectangle!
	//drawFillRect(0, 0, 399, 17, 0, 128, 255, screenTopLeft);

 //   //Prints the statusBar
	//gfxDrawText(GFX_TOP, GFX_LEFT, &fontBlack, "3DS Tic Tac Toe", 5, 238 - &fontBlack.height * 1);
	//guiClock();

	////Prints the score
	//sprintf(buffer, "%d   :   %d", p1score, p2score);
	//gfxDrawText(GFX_TOP, GFX_LEFT, &fontBlack, buffer, 183, 240 - &fontBlack.height * 14);
}

void drawOptionsUi(bool greyScale)
{
	gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)Options_bin, 171, 291, 14, 46);

	if (greyScale) //Color selection
	{
		drawFillRect(54, 176, 144, 184, 192, 192, 192, screenBottom);
	}
	else
	{
		drawFillRect(172, 176, 263, 184, 192, 192, 192, screenBottom);
	}
}


void drawQuickUi()
{
	if (quickui_animation != 24)quickui_animation+=4; //Animation
	gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)QuickUI_bin, 24, 320, 0,-24+quickui_animation);
}

void drawMenuView()
{
	gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)MenuBKG_bin, 240, 400, 0, 0); //Top Background
	gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)FMBKG_bin, 216, 320, 0, 24);
	gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)Exit_bin, 24, 320, 0, 0); //Exit button
}

void drawStatusBar(char* title)
{
	ACU_GetWifiStatus(NULL, &wifiStatus);
	PTMU_GetBatteryLevel(NULL, &batteryLevel);
	PTMU_GetBatteryChargeState(NULL, &charging);

	//Wifi icon
	if (wifiStatus) gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)Wifi_bin, 20, 40, 0, 220);

	//Battery icon
	if (charging)
	{
		gfxDrawSprite(GFX_TOP, GFX_LEFT, batteryLevels[charging_animation/9], 19, 17, 0, 221);
		if(charging_animation!=49)charging_animation++;
		else charging_animation = 1;
	}
	else gfxDrawSprite(GFX_TOP, GFX_LEFT, batteryLevels[batteryLevel], 19, 17, 0, 221);

	//Clock
    guiClock();

	//Title
    gfxDrawText(GFX_TOP, GFX_LEFT, &fontBlack, title, 80, 238 - fontBlack.height * 1);
}


void drawTextView(char* path, int page, bool greyScale)
{
	//Draw BKG
	gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)TextBKG_bin, 240, 400, 0, 0); 
	if (greyScale)
	{
		drawFillRect(40, 20, 359, 239, 255, 255, 255, screenTopLeft);
		drawFillRect(0, 0, 319, 239, 255, 255, 255, screenBottom);
	}
	else
	{
		drawFillRect(40, 20, 359, 239, 243, 231, 189, screenTopLeft);
		drawFillRect(0, 0, 319, 239, 243, 231, 189, screenBottom);
	}

	//Draw page
	sprintf(buffer, "PG: %i", page + 1);
	gfxDrawText(GFX_TOP, GFX_LEFT, &fontBlack, buffer, 5, 238 - fontBlack.height * 15);	

	//Read the page
	int c;
	FILE *file;
	nextAvailable = true;
	
	file = fopen(path, "r");
	if (file) {
		//Show bookmark (with animation)
		if (page == GetBookmark(path))
		{
			if (bookmark_animation != 24) bookmark_animation+=4;
		}
		else bookmark_animation = 0;
		if(bookmark_animation != 0) drawFillRect(368, 20, 385, 20 + bookmark_animation, 255, 0, 0, screenTopLeft);

		//Start reading the page
        fseek(file, !page ? 4 : offsets[page], SEEK_SET);
		int i = 0;
		for (i = 0; i < 29; i++)
		{
			//Loads the line
			int j = 0;
			char line[55];

			for (j = 0; j < 55; j++)
			{
				if ((c = getc(file)) != EOF && c != '\n') line[j] = c;
				else
				{
					if (c == EOF) nextAvailable = false;
					line[j] = '\0';
					break;
				}
			}

			//Prints text
			gfxDrawText(
				i < 14 ? GFX_TOP : GFX_BOTTOM, GFX_LEFT,                 //Screen on which to print the text
				greyScale ? &fontBlack : &fontBrown,
				line,                                                    //Line of text to print
				i < 14 ? 44 : 5,                                         //Horizzontal position of the text
				238 - fontBlack.height * (i < 14 ? i + 2 : i + 1 - 14)       //Vertical position of the text
				);
		}
        //Save the current offset in file, for every page
	    fflush(file);
	    offsets[page+1] = ftell(file);
		fclose(file);
	}
	else
	{
		//Error
	    
	}

	//Next/Previous Page buttons:
	if (page!=0) gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)PrevPG_bin, 71, 23, 0, 85);       // <--
	if (nextAvailable) gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)NextPG_bin, 72, 22, 378, 85); // -->	

}

void guiClock()
{
	u64 timeInSeconds = osGetTime() / 1000;
	u64 dayTime = timeInSeconds % SECONDS_IN_DAY;
	sprintf(buffer, "%02llu:%02llu", dayTime / SECONDS_IN_HOUR, (dayTime % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE);

	gfxDrawText(GFX_TOP, GFX_LEFT, &fontBlack, buffer, 365, 238 - fontBlack.height * 1);
}

void guiPopup(char* title, char* line1, char* line2, char* line3, char* button1, char* button2, bool closeonly)
{
	////Prints a dark grey rectangle!
	//drawFillRect(36, 60, 272, 85, 128, 128, 128, screenBottom);
	////Prints a light grey rectangle!
	//drawFillRect(36, 85, 272, 189, 160, 160, 160, screenBottom);
	////Prints text
	//gfxDrawText(GFX_BOTTOM, GFX_LEFT, &fontBlack, title, 124, 240 - &fontBlack.height * 5);
	//gfxDrawText(GFX_BOTTOM, GFX_LEFT, &fontBlack, line1, 50, 245 - &fontBlack.height * 7);
	//gfxDrawText(GFX_BOTTOM, GFX_LEFT, &fontBlack, line2, 50, 245 - &fontBlack.height * 8);
	//gfxDrawText(GFX_BOTTOM, GFX_LEFT, &fontBlack, line3, 50, 245 - &fontBlack.height * 9);
 //   //Prints the buttons!
	//if (closeonly)
	//{
 //       drawFillRect(107, 155, 198, 183, 192, 192, 192, screenBottom);
	//	gfxDrawText(GFX_BOTTOM, GFX_LEFT, &fontBlack, button1, 130, 240 - &fontBlack.height * 11);
	//}
	//else
	//{
	//	drawFillRect(50, 151, 141, 179, 192, 192, 192, screenBottom);
	//    drawFillRect(166, 151, 257, 179, 192, 192, 192, screenBottom);
	//	gfxDrawText(GFX_BOTTOM, GFX_LEFT, &fontBlack, button1, 60, 240 - &fontBlack.height * 11);
	//	gfxDrawText(GFX_BOTTOM, GFX_LEFT, &fontBlack, button2, 180, 240 - &fontBlack.height * 11);
	//}	
}