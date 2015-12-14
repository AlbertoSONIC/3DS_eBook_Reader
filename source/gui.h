#ifndef GUI_H
#define GUI_H

void GUI_FileExplorer();
void GUI_StatusBar(char* title);
void GUI_OptionsUi(bool greyScale);
void GUI_QuickUi();
void GUI_MenuView();
void GUI_TextView(char* path, int page, bool greyScale);
void GUI_Clock();
int GUI_BookIntro();

extern bool nextAvailable;
extern int quickui_animation;
extern int bookmark_animation;

#endif
