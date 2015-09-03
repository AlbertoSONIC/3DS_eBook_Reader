#ifndef GUI_H
#define GUI_H

void drawStatusBar(char* title);
void drawOptionsUi(bool greyScale);
void drawQuickUi();
void drawMenuView();
void drawTextView(char* path, int page, bool greyScale);
void guiClock();
void guiPopup(char* title, char* line1, char* line2, char* line3, char* button1, char* button2, bool closeonly);

extern bool nextAvailable;
extern int quickui_animation;
extern int bookmark_animation;

#endif
