#include "console.h"
#include <stdio.h>
#include <string.h>
#include "rit128x96x4.h"

#define CHAR_WIDTH 6
#define LINE_HEIGHT 8

char consoleImage[MAX_LINES][MAX_COLUMNS+1];

void consolePrintStr(int x, int y, char* str)
{
  int n;

  n=strlen(str);
  if(n>(MAX_COLUMNS-x)) n=MAX_COLUMNS-x;
  strncpy(consoleImage[y]+x,str,n);
}

void refreshConsole(void)
{
  int y;

  for(y=0;y<MAX_LINES;y++)							  
	  RIT128x96x4StringDraw(consoleImage[y], 0, y*LINE_HEIGHT, 15);
}

void refreshConsoleLine(int y)
{
  RIT128x96x4StringDraw(consoleImage[y], 0, y*LINE_HEIGHT, 15);
}

void initConsole(void)
{
  int i,j;

  RIT128x96x4Init(1000000);
  for(i=0;i<MAX_LINES;i++)
  {
    for(j=0;j<MAX_COLUMNS;j++) consoleImage[i][j]=' ';
	  consoleImage[i][MAX_COLUMNS]='\0';
  }
}

void eraseConsoleLine(int y)
{
  int j;

  for(j=0;j<MAX_COLUMNS;j++) consoleImage[y][j]=' ';
}
