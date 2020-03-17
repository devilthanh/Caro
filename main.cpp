#include "caro.h"

int main() {

	setConsoleSize(CONSOLE_WIDTH, CONSOLE_HEIGHT);

	string 	*splashData = NULL,
		*mainMenu = NULL,
		*aboutUs = NULL;

	splashData = new string[CONSOLE_HEIGHT];
	mainMenu = new string[CONSOLE_HEIGHT];
	aboutUs = new string[CONSOLE_HEIGHT];

	int 	splashLines = 0,
		mainMenuLines = 0,
		menuCurrent = 0,
		aboutUsLines = 0,
		mainStage = 0,
		boardCols = 25,
		boardRows = 25,
		playerSide = SIDE_X,
		goFirst = SIDE_X;
	bool	caroRule = false;

	HANDLE 	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO		cursor;
	GetConsoleCursorInfo(handle, &cursor);
	cursor.bVisible = false;
	SetConsoleCursorInfo(handle, &cursor);

	loadSplashScreen(splashData, &splashLines);
	loadMainMenu(mainMenu, &mainMenuLines);
	loadAboutUs(aboutUs, &aboutUsLines);


	Sleep(500);

	showSplashScreen(splashData, &splashLines);
	while (true) {

		showMainMenu(splashData, &splashLines, mainMenu, &mainMenuLines, &menuCurrent);

		switch (menuCurrent) {
		case 0:
			createGame(boardRows, boardCols, GAMEMODE_PVP, playerSide, goFirst, caroRule);
			break;
		case 1:
			createGame(boardRows, boardCols, GAMEMODE_PVC, playerSide, goFirst, caroRule);
			break;
		case 2:
			showStatistics(splashData, &splashLines);
			break;
		case 3:
			showOptions(splashData, &splashLines, &boardRows, &boardCols, &playerSide, &goFirst, &caroRule);
			break;
		case 4:
			showAboutUs(aboutUs, &aboutUsLines);
			break;
		case 5:
			exit(0);
			break;
		}
	}

	return 0;
}
