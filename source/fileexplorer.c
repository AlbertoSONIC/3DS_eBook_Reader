#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> 
#include <string.h>
#include "fileexplorer.h"
#include "app.h"
#include "draw.h"
#include "input.h"
#include "rendering.h"
#include "gui.h"
#include "NextFM_bin.h"
#include "PrevFM_bin.h"


/* run this program using the console pauser or add your own getch, system("pause") or input loop */

//---- GLOBAL VARIABLES ----
int pointer = 0;
char **files;
int count;
int i;
int beginning = 0;
char dir[1000] = "Books/";
int opened_folder = 1;
char buffer[100];
int itemShown = 0;

int file_list(const char *path, char ***ls) {
	int count = 0;
	DIR *dp = NULL;
	struct dirent *ep = NULL;

	dp = opendir(path);
	if (NULL == dp) {
		gfxDrawText(GFX_BOTTOM, GFX_LEFT, &fontBlack, "AEEE", 5, 238 - fontBlack.height);
		return 0;
	}

	*ls = NULL;
	*ls = calloc(1000, sizeof(char *));
	count = 0;
	ep = readdir(dp);

	while (NULL != ep)
	{
		(*ls)[count++] = strdup(ep->d_name);
		ep = readdir(dp);
	}

	closedir(dp);
	return count;
}

void FileExplorerShow(){
	getFB();

	
    drawMenuView();	
    drawStatusBar("");

	if (count != 0)
	{
		int n = 0;
		int divisions = count % 10 == 0 ? count / 10 : count / 10 + 1;
		int list[divisions];
		for (n = 0; n < divisions; n++)
		{
			if (n == divisions - 1 && count % 10 != 0)list[n] = count - (count / 10) * 10;
			else list[n] = 10;
		}

		int i = 0;

		itemShown = list[(pointer / 10)];

		for (i = beginning; i < beginning + list[(pointer / 10)]; i++) {
			char filename[100];
			strcpy(filename, files[i]);
			if (strlen(filename)>39)
			{
				filename[39] = '\0';
				strcat(filename, "...");
			}
			sprintf(buffer, "%s%s", i == pointer ? "> " : "", filename);
			if (pointer == i) drawFillRect(35, fontBlack.height * (i - beginning + 1), 275, fontBlack.height * (i - beginning + 1) + 15, 226, 226, 226, screenBottom);
			gfxDrawText(GFX_BOTTOM, GFX_LEFT, &fontBlack, filename, 40, 238 - fontBlack.height * (i - beginning + 1) - 15);
		}

		//Draw the next/prev page button if there's one
		//Next/Previous Page buttons:
		if (beginning != 0) gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)PrevFM_bin, 71, 23, 9, 109);       // <--
		if (count > beginning + 11) gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)NextFM_bin, 72, 22, 289, 109); // -->	

	}
	// Flush and swap framebuffers
	gfxFlushBuffers();
	gfxSwapBuffers();

	//Wait for VBlank
	gspWaitForVBlank();
}

void FileExplorerNextSelection(){
	if (pointer != count - 1) pointer++;
}

void FileExplorerPrevSelection(){
	if (pointer != 0) pointer--;
}

void FileExplorerSelect(){
	//Enter text view, before, it gets the title and the file path
	int i;
	char title[100];
	strcpy(title, files[pointer]);
	for (i = 0; i < strlen(title); i++)
	{
		if (title[i] == '.'){
			title[i] = '\0'; 
			break;
		}
	}
	if (strlen(title)>40) {   //Truncates the title if too big 
		title[40] = '\0'; strcat(title, "...");
	}
	TextView(title, strcat(dir, files[pointer]));
	dir[6] = '\0'; //Reset the file path
}


void FileExplorerMain() {

	count = file_list(dir, &files);
	while (1 == 1)
	{
		//---------------------------- DRAW THE FILE EXPLORER -------------------------------
		FileExplorerShow();

		//------------------------------------ INPUT ----------------------------------------
		//Both touch and buttons input supported
		getInput();
		if((posX > 0 && posX < 158) && (posY > 240 - 24 && posY < 240)) break;
		//Buttons item selection
		if (input & KEY_UP) FileExplorerPrevSelection(); 
		else if (input & KEY_DOWN) FileExplorerNextSelection(); 
		else if (input & KEY_A) FileExplorerSelect(); 
		//Touch item selection
		if ((posX > 32 && posX < 266) && (posY > 10 && posY < 240 - 24 - 40 - (10 - itemShown)*fontBlack.height)) 
		{
			//If tap on file manager item
			int selectedItem = (posY - 19) / fontBlack.height + beginning;
			if (pointer == selectedItem)FileExplorerSelect();
			else pointer = selectedItem;
		}
		if ((posX > 11 && posX < 30) && (posY > 62 && posY < 129)) pointer = beginning - 1;//Prev FM page
		if ((posX > 289 && posX < 312) && (posY > 62 && posY < 129)) pointer = beginning + 10;//Next FM page

		if (pointer - beginning == 10)beginning += 10;
		if (pointer < beginning)beginning -= 10;
	}
}

