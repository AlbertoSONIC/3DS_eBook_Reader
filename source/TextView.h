#ifndef TEXTVIEW_H
#define TEXTVIEW_H
#include <3ds.h>

void TextView_Main();
void TextView_SaveBookmark();
int TextView_GetBookmark();

extern int page;
extern bool greyScale;
extern bool hideUI;
extern bool hideOptions;

#endif
