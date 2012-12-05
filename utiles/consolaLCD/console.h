#ifndef CONSOLE_H
#define CONSOLE_H

#define MAX_COLUMNS 21
#define MAX_LINES 12

void initConsole(void);
void refreshConsole(void);
void consolePrintStr(int x, int y, char* str);
void refreshConsoleLine(int y);
void eraseConsoleLine(int y);

#endif
