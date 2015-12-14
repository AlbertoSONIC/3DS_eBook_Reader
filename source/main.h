#ifndef MAIN_H
#define MAIN_H

typedef enum 
{
	MENU,
	TEXT,
	BOOK_INTRO,
	EXIT
} appMode_t;

int main();

extern appMode_t mode;

#endif
