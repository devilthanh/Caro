#include "caro.h"

void setConsoleSize(int width, int height) {
	string cmd = "MODE CON COLS=" + to_string(width) + " LINES=" + to_string(height);
	system(cmd.c_str());
}

void setTextColor(int color) {
	static HANDLE  handle = NULL;
	if (!handle)
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, color);
}

int getInput() {
	int nextInput = 0;
	do nextInput = _getch(); while (nextInput == 0);
	return nextInput;
}

void printXO(int x, int y, int side, int widthOffset, int heightOffset) {
	if (side == SIDE_X) {
		setTextColor(COLOR_RED);
		printxy("X", widthOffset + x * 2, heightOffset + y);
		setTextColor(COLOR_WHITE);
	}
	else {
		setTextColor(COLOR_BLUE);
		printxy("O", widthOffset + x * 2, heightOffset + y);
		setTextColor(COLOR_WHITE);
	}
}

void printCursor(int x, int y, int widthOffset, int heightOffset) {
	static int 	currentX = -1,
		currentY = -1;

	if (currentX > -1) {
		printxy(" ", widthOffset + currentX * 2 - 1, heightOffset + currentY);
		printxy(" ", widthOffset + currentX * 2 + 1, heightOffset + currentY);
	}
	setTextColor(COLOR_GREEN);
	printxy("[", widthOffset + x * 2 - 1, heightOffset + y);
	printxy("]", widthOffset + x * 2 + 1, heightOffset + y);
	setTextColor(COLOR_WHITE);

	currentX = x;
	currentY = y;
}

void printxy(string str, int x, int y) {
	static HANDLE  handle = NULL;
	if (!handle)
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { (short)x, (short)y };
	SetConsoleCursorPosition(handle, c);
	cout << str;
}

void loadSplashScreen(string *splashData, int * lines) {
	string line;
	*lines = 0;
	ifstream file("splash.cfg");
	if (file.is_open()) {
		while (getline(file, line)) {
			splashData[*lines] = line;
			*lines = *lines + 1;
		}
		file.close();
	}
}

void showSplashScreen(string *splashData, int * lines) {
	system("cls");
	int 	maxLength = 0;

	for (int i = 0; i < *lines; i++)
		if (splashData[i].size() > maxLength) maxLength = splashData[i].size();

	int		widthOffset = (CONSOLE_WIDTH - maxLength + 1) / 2,
		heightOffset = (CONSOLE_HEIGHT - *lines) / 2,
		offset = 0;

	for (int i = 0; i < *lines; i++) {
		system("cls");
		printxy(splashData[i], widthOffset, heightOffset + i);
		Sleep(100);
	}

	system("cls");
	for (int i = 0; i < *lines; i++) {
		printxy(splashData[i], widthOffset, heightOffset + i);
		Sleep(100);
	}

	while (heightOffset - offset > 5) {
		system("cls");
		for (int i = 0; i < *lines; i++)
			printxy(splashData[i], widthOffset, heightOffset - offset + i);

		Sleep(100);
		offset++;
	}
}

void loadMainMenu(string *mainMenu, int * mainMenuLines) {
	string line;
	*mainMenuLines = 0;
	ifstream file("menu.cfg");
	if (file.is_open()) {
		while (getline(file, line)) {
			mainMenu[*mainMenuLines] = line;
			*mainMenuLines = *mainMenuLines + 1;
		}
		file.close();
	}
}

void showMainMenu(string *splashData, int * lines, string * mainMenu, int * mainMenuLines, int * menuCurrent) {
	int 	button = 0,
		maxLength = 0;

	for (int i = 0; i < *lines; i++)
		if (splashData[i].size() > maxLength) maxLength = splashData[i].size();

	system("cls");
	for (int i = 0; i < *lines; i++)
		printxy(splashData[i], (CONSOLE_WIDTH - maxLength + 1) / 2, 6 + i);

	while (true) {

		for (int i = 0; i < *mainMenuLines; i++) {
			if (i == *menuCurrent) {
				setTextColor(COLOR_YELLOW);
				printxy(">>>  " + mainMenu[i] + "   ", (CONSOLE_WIDTH + 1) / 2 - 15, 10 + *lines + i * 2);
				setTextColor(COLOR_WHITE);
			}
			else printxy("        " + mainMenu[i], (CONSOLE_WIDTH + 1) / 2 - 15, 10 + *lines + i * 2);
		}


		while (!_kbhit()) {}
		button = getInput();

		if (button == 224) {
			switch ((button = getInput())) {
			case KEY_UP:
				if (*menuCurrent > 0) *menuCurrent = *menuCurrent - 1;
				else *menuCurrent = *mainMenuLines - 1;
				break;
			case KEY_DOWN:
				if (*menuCurrent < *mainMenuLines - 1) *menuCurrent = *menuCurrent + 1;
				else *menuCurrent = 0;
				break;
			}
		}
		else {
			if (button == KEY_ENTER)
				return;
		}
	}
}

void showOptions(string *splashData, int * lines, int * rows, int * cols, int * side, int * goFirst, bool * caroRule) {
	static 	string	options[5] = { "Rows",
									"Columns",
									"P1 Side",
									"Go first",
									"Caro Rule" },
		sides[2] = { "O", "X" };

	string 	str = "OPTIONS";

	int 	button = 0,
		maxLength = 0,
		currentOption = 0;

	for (int i = 0; i < *lines; i++)
		if (splashData[i].size() > maxLength) maxLength = splashData[i].size();

	system("cls");
	for (int i = 0; i < *lines; i++)
		printxy(splashData[i], (CONSOLE_WIDTH - maxLength + 1) / 2, 6 + i);

	printxy(str, (CONSOLE_WIDTH - str.size()) / 2, 18);
	str = "Press ESC to return Main Menu";
	printxy(str, (CONSOLE_WIDTH - str.size()) / 2, CONSOLE_HEIGHT - 2);

	while (true) {

		for (int i = 0; i < 5; i++) {
			switch (i) {
			case 0:
				str = to_string(*rows);
				break;
			case 1:
				str = to_string(*cols);
				break;
			case 2:
				str = " " + sides[*side - 1];
				break;
			case 3:
				str = " " + sides[*goFirst - 1];
				break;
			case 4:
				if (*caroRule) str = "Yes";
				else str = "No ";
			}

			if (i == currentOption) {
				setTextColor(COLOR_YELLOW);
				printxy(options[i], (CONSOLE_WIDTH + 1) / 2 - 10, 13 + *lines + i * 2);
				printxy(":  <<  " + str + "  >>", (CONSOLE_WIDTH + 1) / 2, 13 + *lines + i * 2);
				setTextColor(COLOR_WHITE);
			}
			else {
				printxy(options[i], (CONSOLE_WIDTH + 1) / 2 - 10, 13 + *lines + i * 2);
				printxy(":      " + str + "    ", (CONSOLE_WIDTH + 1) / 2, 13 + *lines + i * 2);
			}
		}

		while (!_kbhit()) {}
		button = getInput();

		if (button == 224) {
			switch ((button = getInput())) {
			case KEY_UP:
				if (currentOption > 0) currentOption = currentOption - 1;
				else currentOption = 5 - 1;
				break;
			case KEY_DOWN:
				if (currentOption < 5 - 1) currentOption = currentOption + 1;
				else currentOption = 0;
				break;
			case KEY_LEFT:
				switch (currentOption) {
				case 0:
					if (*rows > BOARD_MIN_WIDTH) *rows = *rows - 1;
					break;
				case 1:
					if (*cols > BOARD_MIN_HEIGHT) *cols = *cols - 1;
					break;
				case 2:
					if (*side == SIDE_O)
						*side = SIDE_X;
					else *side = SIDE_O;
					break;
				case 3:
					if (*goFirst == SIDE_O)
						*goFirst = SIDE_X;
					else *goFirst = SIDE_O;
					break;
				case 4:
					*caroRule = !*caroRule;
				}
				break;
			case KEY_RIGHT:
				switch (currentOption) {
				case 0:
					if (*rows < BOARD_MAX_WIDTH) *rows = *rows + 1;
					break;
				case 1:
					if (*cols < BOARD_MAX_HEIGHT) *cols = *cols + 1;
					break;
				case 2:
					if (*side == SIDE_O)
						*side = SIDE_X;
					else *side = SIDE_O;
					break;
				case 3:
					if (*goFirst == SIDE_O)
						*goFirst = SIDE_X;
					else *goFirst = SIDE_O;
					break;
				case 4:
					*caroRule = !*caroRule;
				}
			}
		}
		else {
			if (button == KEY_ESC)
				break;
		}
	}
}

void loadAboutUs(string *aboutUs, int * aboutUsLines) {
	string line;
	*aboutUsLines = 0;
	ifstream file("aboutus.cfg");
	if (file.is_open()) {
		while (getline(file, line)) {
			aboutUs[*aboutUsLines] = line;
			*aboutUsLines = *aboutUsLines + 1;
		}
		file.close();
	}
}

void showAboutUs(string *aboutUs, int * aboutUsLines) {
	system("cls");
	string 	str = "Press any key to return...";

	for (int i = 0; i < *aboutUsLines; i++) {
		if (i < 13) {
			printxy("", (CONSOLE_WIDTH - aboutUs[i].size()) / 2 + 1, 4 + i);
			for (int j = 0; j < aboutUs[i].size(); j++) {
				cout << aboutUs[i].at(j);
				Sleep(20);
			}
		}
		else {
			printxy(aboutUs[i], (CONSOLE_WIDTH - aboutUs[i].size()) / 2 + 1, 4 + i);
			Sleep(30);
		}
	}
	printxy(str, (CONSOLE_WIDTH - str.size() + 1) / 2, 7 + *aboutUsLines);
	system("pause > nul");
}

void createGame(int rows, int cols, int mode, int playerSide, int goFirst, bool caroRule) {

	int 	board[BOARD_MAX_HEIGHT][BOARD_MAX_WIDTH],
		turns[BOARD_MAX_HEIGHT*BOARD_MAX_WIDTH][2],
		turnCount = 0,
		button = 0,
		cursorX = cols / 2,
		cursorY = rows / 2,
		heightOffset = (CONSOLE_HEIGHT - rows + 1) / 2,
		widthOffset = 33 - cols,
		status = 0,
		offset = 70,
		lastX = -1,
		lastY = -1;

	bool ended = false;

	static 	string 	sides[2] = { "O", "X" },
		modes[2] = { "PVP", "PVC" },
		statuses[4] = { "Turn O", "Turn X", "O Win!", "X Win!" },
		yesno[2] = { "No", "Yes" };

	system("cls");

	setTextColor(COLOR_GRAY);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {
			board[i][j] = 0;
			printxy(".", widthOffset + j * 2, heightOffset + i);
		}

	setTextColor(COLOR_WHITE);

	printxy("Rows:\t " + to_string(rows), offset, 5);
	printxy("Columns:\t " + to_string(cols), offset, 7);
	printxy("Mode:\t " + modes[mode], offset, 9);
	printxy("Caro Rule: " + yesno[caroRule], offset, 11);
	printxy("Player 1:\t " + sides[playerSide - 1], offset, 13);
	printxy("Go first:\t " + sides[goFirst - 1], offset, 15);
	printxy("Turns:\t " + to_string(turnCount), offset, 17);

	for (int i = 0; i < CONSOLE_HEIGHT; i++)
		printxy("||", 64, i);

	status = goFirst;

	loadGame(turns, &turnCount, &rows, &cols, &mode, &playerSide, &goFirst, &caroRule);
	for (int i = 0; i < turnCount; i++) {
		board[turns[i][0]][turns[i][1]] = status;
		printXO(turns[i][1], turns[i][0], status, widthOffset, heightOffset);
		lastX = turns[i][1];
		lastY = turns[i][0];
		status = updateStatus(board, rows, cols, status, turns[i][1], turns[i][0], caroRule);
	}

	while (true) {

		printCursor(cursorX, cursorY, widthOffset, heightOffset);
		printxy("Turns:\t " + to_string(turnCount), offset, 17);
		printxy("Status:\t " + statuses[status - 1], offset, 19);

		updateLabels(status);
		if (status >= 3 && !ended && mode==GAMEMODE_PVC) {
			updateStatistics((status-2)==playerSide);
			ended = true;
		}

		if (mode == GAMEMODE_PVC && status < 3 && status != playerSide) {

			int aiX, aiY;
			aiMove(board, rows, cols, status, caroRule, &aiX, &aiY);

			board[aiY][aiX] = status;
			printXO(aiX, aiY, status, widthOffset, heightOffset);
			turns[turnCount][0] = aiY;
			turns[turnCount][1] = aiX;
			turnCount++;
			status = updateStatus(board, rows, cols, status, aiX, aiY, caroRule);

			continue;
		}

		while (!_kbhit()) {}
		button = getInput();

		if (button == 224) {
			switch ((button = getInput())) {
			case KEY_UP:
				if (cursorY > 0) cursorY = cursorY - 1;
				else cursorY = rows - 1;
				break;
			case KEY_DOWN:
				if (cursorY < rows - 1) cursorY = cursorY + 1;
				else cursorY = 0;
				break;
			case KEY_LEFT:
				if (cursorX > 0) cursorX = cursorX - 1;
				else cursorX = cols - 1;
				break;
			case KEY_RIGHT:
				if (cursorX < cols - 1) cursorX = cursorX + 1;
				else cursorX = 0;
				break;
			}
		}
		else {
			switch (button) {
			case KEY_ENTER:
				if (mode == GAMEMODE_PVP || status == playerSide) {
					if (board[cursorY][cursorX] == 0 && status < 3) {
						board[cursorY][cursorX] = status;
						printXO(cursorX, cursorY, status, widthOffset, heightOffset);
						turns[turnCount][0] = cursorY;
						turns[turnCount][1] = cursorX;
						lastX = cursorX;
						lastY = cursorY;
						turnCount++;
						status = updateStatus(board, rows, cols, status, cursorX, cursorY, caroRule);
					}
				}
				break;
			case KEY_ESC:
				if (status < 3)
					saveGame(turns, turnCount, rows, cols, mode, playerSide, goFirst, caroRule);
				else {
					saveGame(turns, 0, rows, cols, mode, mode, goFirst, caroRule);
				}
				return;
				break;
			}
		}
	}
}

void aiMove(int board[BOARD_MAX_HEIGHT][BOARD_MAX_WIDTH], int rows, int cols, int aiSide, bool caroRule, int * aiX, int * aiY) {

	int	playerCountMax = 0, aiCountMax = 0,
		playerBlockedMax = 0, aiBlockedMax = 0,
		playerMoveX = 0, aiMoveX = 0,
		playerMoveY = 0, aiMoveY = 0,
		playerDirX = 0, aiDirX = 0,
		playerDirY = 0, aiDirY = 0,
		maxToWin = 5;

	bool goFirst = true;

	if (caroRule) maxToWin = 6;

	for (int y = 0; y < rows; y++)
		for (int x = 0; x < cols; x++) {

			if (board[y][x] == 0) continue;

			goFirst = false;

			for (int i = -1; i <= 1; i++)
				for (int j = -1; j <= 1; j++) {

					int blocked = 0,
						count = 0,
						xx = 0,
						yy = 0;

					if (i == 0 && j == 0) continue;

					if (x - j < 0 || x - j >= cols || y - i < 0 || y - i >= rows)
						blocked++;
					else {
						if (board[y - i][x - j] == board[y][x])
							continue;
						if (board[y - i][x - j] != board[y][x] && board[y - i][x - j] != 0)
							blocked++;
					}

					xx = x;
					yy = y;

					for (int m = 0; m < maxToWin; m++) {

						if ((xx < 0 || xx >= cols || yy < 0 || yy >= rows) || (board[yy][xx] != board[y][x] && board[yy][xx] != 0)) {
							blocked = 0;
							count = 0;
							break;
						}

						if (board[yy][xx] == board[y][x]) count++;
						xx += j;
						yy += i;
					}

					if (count == 4) blocked = 0;

					if (board[y][x] == aiSide) {
						if (count * 2 - blocked >= aiCountMax * 2 - aiBlockedMax) {
							aiCountMax = count;
							aiBlockedMax = blocked;
							aiMoveX = x;
							aiMoveY = y;
							aiDirX = j;
							aiDirY = i;
						}
					}
					else {
						if (count * 2 - blocked >= playerCountMax * 2 - playerBlockedMax) {
							playerCountMax = count;
							playerBlockedMax = blocked;
							playerMoveX = x;
							playerMoveY = y;
							playerDirX = j;
							playerDirY = i;
						}
					}
				}
		}

	if (goFirst) {
		*aiX = cols / 2;
		*aiY = rows / 2;
		return;
	}

	if (playerCountMax - playerBlockedMax > aiCountMax - aiBlockedMax) {
		while (board[playerMoveY][playerMoveX] != 0) {
			playerMoveX += playerDirX;
			playerMoveY += playerDirY;
		}

		*aiX = playerMoveX;
		*aiY = playerMoveY;

	}
	else {
		while (board[aiMoveY][aiMoveX] != 0) {
			aiMoveX += aiDirX;
			aiMoveY += aiDirY;
		}

		*aiX = aiMoveX;
		*aiY = aiMoveY;
	}
}

void updateLabels(int status) {
	static 	string	*labelX = NULL,
		*labelY = NULL,
		*label = NULL,
		*labelWin = NULL;

	int	widthOffset = 71,
		heightOffset = CONSOLE_HEIGHT - 10;

	if (!label) {
		labelX = new string[7];
		labelY = new string[7];
		label = new string[5];
		labelWin = new string[5];

		ifstream file("labels.cfg");
		if (file.is_open()) {
			for (int i = 0; i < 7; i++)
				getline(file, labelX[i]);

			for (int i = 0; i < 7; i++)
				getline(file, labelY[i]);

			for (int i = 0; i < 5; i++)
				getline(file, label[i]);

			for (int i = 0; i < 5; i++)
				getline(file, labelWin[i]);

			file.close();
		}
	}



	switch (status) {
	case 1:
		for (int i = 0; i < 5; i++)
			printxy(label[i], 67, heightOffset + i - 7);
		setTextColor(COLOR_BLUE);
		for (int i = 0; i < 7; i++)
			printxy(labelY[i], widthOffset, heightOffset + i);
		setTextColor(COLOR_WHITE);
		break;
	case 2:
		for (int i = 0; i < 5; i++)
			printxy(label[i], 67, heightOffset + i - 7);
		setTextColor(COLOR_RED);
		for (int i = 0; i < 7; i++)
			printxy(labelX[i], widthOffset, heightOffset + i);
		setTextColor(COLOR_WHITE);
		break;
	case 3:
	case 4:
		for (int i = 0; i < 5; i++)
			printxy(labelWin[i], 67, heightOffset + i - 7);
	}
}

int updateStatus(int board[BOARD_MAX_HEIGHT][BOARD_MAX_WIDTH], int rows, int cols, int side, int x, int y, bool caroRule) {

	int	l1 = 1, n1 = 0,
		l2 = 1, n2 = 0,
		l3 = 1, n3 = 0,
		l4 = 1, n4 = 0;

	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) continue;
			int xx = x + j, yy = y + i;
			while (xx >= 0 && xx < cols && yy >= 0 && yy < rows) {
				if (board[yy][xx] == side) {
					if (i == j)
						l1++;
					else if (i == -j)
						l2++;
					else if (i == 0)
						l3++;
					else l4++;
				}
				else {
					if (board[yy][xx] != 0) {
						if (i == j)
							n1++;
						else if (i == -j)
							n2++;
						else if (i == 0)
							n3++;
						else n4++;
					}
					break;
				}
				xx += j;
				yy += i;
			}
		}

	if ((l1 >= 5 && !caroRule) || (l1 >= 5 && caroRule && n1 != 2)) 		return side + 2;
	else if ((l2 >= 5 && !caroRule) || (l2 >= 5 && caroRule && n2 != 2)) 	return side + 2;
	else if ((l3 >= 5 && !caroRule) || (l3 >= 5 && caroRule && n3 != 2)) 	return side + 2;
	else if ((l4 >= 5 && !caroRule) || (l4 >= 5 && caroRule && n4 != 2)) 	return side + 2;

	if (side == SIDE_O)
		return SIDE_X;
	else
		return SIDE_O;
}

void saveGame(int turns[BOARD_MAX_HEIGHT*BOARD_MAX_WIDTH][2], int turnCount, int rows, int cols, int mode, int playerSide, int goFirst, bool caroRule) {
	ofstream file;
	if (mode == GAMEMODE_PVP)
		file.open("gamePVP.sav");
	else
		file.open("gamePVC.sav");
	if (file.is_open()) {
		file << turnCount << " " << rows << " " << cols << " " << mode << " " << playerSide << " " << goFirst << " " << caroRule << endl;
		for (int i = 0; i < turnCount; i++) {
			file << turns[i][0] << " " << turns[i][1] << " ";
		}
		file.close();
	}
}

void loadGame(int turns[BOARD_MAX_HEIGHT*BOARD_MAX_WIDTH][2], int *turnCount, int *rows, int *cols, int *mode, int *playerSide, int *goFirst, bool *caroRule) {
	ifstream file;
	if (*mode == GAMEMODE_PVP)
		file.open("gamePVP.sav");
	else
		file.open("gamePVC.sav");

	if (file.is_open()) {
		file >> *turnCount;
		if (*turnCount > 0) {
			file >> *rows >> *cols >> *mode >> *playerSide >> *goFirst >> *caroRule;
			for (int i = 0; i < *turnCount; i++) {
				file >> turns[i][0] >> turns[i][1];
			}
		}
		file.close();
	}
}

void updateStatistics(bool winner){
	ifstream ifile("statistics.sav");

	int winCount = 0;
	int gameCount = 0;

	if (ifile.is_open()) {
		ifile >> winCount >> gameCount;
		ifile.close();
	}

	if (winner) {
		winCount++;
	}
	gameCount++;

	ofstream ofile("statistics.sav");

	if (ofile.is_open()) {
		ofile << winCount << " " << gameCount;
		ofile.close();
	}
}

void loadStatistics(int *winCount, int *gameCount) {
	ifstream file ("statistics.sav");

	*winCount = 0;
	*gameCount = 0;

	if (file.is_open()) {
		file >> *winCount >> *gameCount;
		file.close();
	}
}

void showStatistics(string *splashData, int *lines) {

	int	button = 0,
		maxLength = 0,
		winCount,
		gameCount;

	loadStatistics(&winCount, &gameCount);

	string 	str = "STATISTICS (PVC ONLY)";

	for (int i = 0; i < *lines; i++)
		if (splashData[i].size() > maxLength) maxLength = splashData[i].size();

	system("cls");
	for (int i = 0; i < *lines; i++)
		printxy(splashData[i], (CONSOLE_WIDTH - maxLength + 1) / 2, 6 + i);

	printxy(str, (CONSOLE_WIDTH - str.size()) / 2, 18);
	

	str = "Number of Win      " + to_string(winCount);
	printxy(str, CONSOLE_WIDTH/2 - 10, 22);

	str = "Total Game         " + to_string(gameCount);
	printxy(str, CONSOLE_WIDTH/2 -10, 24);

	str = "Press any key to return...";
	printxy(str, (CONSOLE_WIDTH - str.size()) / 2, CONSOLE_HEIGHT - 2);

	system("pause > nul");
}
