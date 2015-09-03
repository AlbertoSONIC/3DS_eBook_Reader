#ifndef APP_H
#define APP_H
#include <3ds.h>

void MenuView();
void TextView(char* title, char* path);
void SaveBookmark(char* path, int page);
int GetBookmark(char* path);

#endif
