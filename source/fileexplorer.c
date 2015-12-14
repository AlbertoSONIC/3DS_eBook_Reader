#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> 
#include <string.h>
#include "main.h"
#include "FileExplorer.h"
#include "TextView.h"
#include "draw.h"
#include "input.h"
#include "rendering.h"
#include "gui.h"

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
char title[100];
char path[100];

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

void FileExplorer_NextSelection(){
	if (pointer != count - 1) pointer++;
}

void FileExplorer_PrevSelection(){
	if (pointer != 0) pointer--;
}

void FileExplorer_Select(){
	//Enter text view, before, it gets the title and the file path
	int i;
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
	strcpy(path, strcat(dir, files[pointer]));
	mode = BOOK_INTRO; //Switch to textview

	//Prepare eBook
	page = 0;
	while (page != TextView_GetBookmark(path))
	{
		GUI_TextView(path, page, greyScale);
		page++;
	}
}


void FileExplorer_Main() {

	dir[6] = '\0'; //Reset the file path
	count = file_list(dir, &files);

	//------------------------------------ INPUT ----------------------------------------
	//Both touch and buttons input supported
	if ((posX > 0 && posX < 158) && (posY > 240 - 24 && posY < 240)) mode = 2; //Close
	//Buttons item selection
	if (input & KEY_UP) FileExplorer_PrevSelection();
	else if (input & KEY_DOWN) FileExplorer_NextSelection();
	else if (input & KEY_A) FileExplorer_Select();
	//Touch item selection
	if ((posX > 32 && posX < 266) && (posY > 10 && posY < 240 - 24 - 40 - (10 - itemShown)*fontBlack.height))
	{
		//If tap on file manager item
		int selectedItem = (posY - 19) / fontBlack.height + beginning;
		if (pointer == selectedItem)FileExplorer_Select();
		else pointer = selectedItem;
	}
	if (((posX > 11 && posX < 30) && (posY > 62 && posY < 129)) && beginning != 0) pointer = beginning - 1;//Prev FM page
	if (((posX > 289 && posX < 312) && (posY > 62 && posY < 129)) && count > beginning + 10) pointer = beginning + 10;//Next FM page

	if (pointer - beginning == 10)beginning += 10;
	if (pointer < beginning)beginning -= 10;
}

