#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

int file_list(const char *path, char ***ls);
void FileExplorerShow();
void FileExplorerNextSelection();
void FileExplorerPrevSelection();
void FileExplorerSelect();
void FileExplorerMain();

#endif
