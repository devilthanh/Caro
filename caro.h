#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <conio.h>

#define CONSOLE_WIDTH 90
#define CONSOLE_HEIGHT 40
#define GAMEMODE_PVP 0
#define GAMEMODE_PVC 1
#define SIDE_O 1
#define SIDE_X 2
#define BOARD_MAX_HEIGHT 30
#define BOARD_MAX_WIDTH 30
#define BOARD_MIN_HEIGHT 15
#define BOARD_MIN_WIDTH 15

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_ESC 27

#define COLOR_BLUE 9
#define COLOR_RED 12
#define	COLOR_WHITE 7
#define COLOR_GRAY 8
#define COLOR_GREEN 10
#define COLOR_YELLOW 14

using namespace std;

void	setConsoleSize(int width, int height);
void 	setTextColor(int color);
void 	printxy(string str, int x, int y);
void 	printXO(int x, int y, int side, int widthOffset, int heightOffset);
void 	printCursor(int x, int y, int widthOffset, int heightOffset);
void 	loadSplashScreen(string *splashData, int * lines);
void 	showSplashScreen(string *splashData, int * lines);
void 	loadMainMenu(string *mainMenu, int * mainMenuLines);
void  	showMainMenu(string *splashData, int * lines, string * mainMenu, int * mainMenuLines, int * menuCurrent);
void 	showOptions(string *splashData, int * lines, int * rows, int * cols, int * side, int * goFirst, bool * caroRule);
void 	loadAboutUs(string *aboutUs, int * aboutUsLines);
void 	showAboutUs(string *aboutUs, int * aboutUsLines);
void	createGame(int rows, int cols, int mode, int playerSide, int goFirst, bool caroRule);
void 	aiMove(int board[BOARD_MAX_HEIGHT][BOARD_MAX_WIDTH], int rows, int cols, int aiSide, bool caroRule, int * aiX, int * aiY);
void 	updateLabels(int side);
int 	updateStatus(int board[BOARD_MAX_HEIGHT][BOARD_MAX_WIDTH], int rows, int cols, int side, int x, int y, bool caroRule);
int 	getInput();
void 	saveGame(int turns[BOARD_MAX_HEIGHT*BOARD_MAX_WIDTH][2], int turnCount, int rows, int cols, int mode, int playerSide, int goFirst, bool caroRule);
void 	loadGame(int turns[BOARD_MAX_HEIGHT*BOARD_MAX_WIDTH][2], int *turnCount, int *rows, int *cols, int *mode, int *playerSide, int *goFirst, bool *caroRule);
void	updateStatistics(bool winner);
void	loadStatistics(int *winCount, int *gameCount);
void	showStatistics(string *splashData, int *lines);

