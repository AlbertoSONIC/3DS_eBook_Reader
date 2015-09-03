#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

int file_list(const char *path, char ***ls);
void FileExplorer_NextSelection();
void FileExplorer_PrevSelection();
void FileExplorer_Select();
void FileExplorer_Main();

extern int pointer;
extern char **files;
extern int count;
extern int beginning;
extern int itemShown;
extern char title[100];
extern char path[100];

#endif
