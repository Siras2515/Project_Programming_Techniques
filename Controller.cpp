/*	
	This file defines all function that declared in "Controller.h"
	Reference: https://github.com/Louis2602/Pikachu-Game/blob/master/Pikachu/Controller.cpp
*/
#include "Controller.h"

HWND Controller::consoleWindow = GetConsoleWindow();
HANDLE Controller::consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

void Controller::setUpConsole() {
	// setFontInfo();
	setAndCenterWindow();
	disableMaximize();
	setConsoleTitle();
	hideScrollBars();
	showCursor(false);
	disableMouseInput();
}

void Controller::gotoXY(int x, int y) {
	COORD postion;
	postion.X = x;
	postion.Y = y;

	SetConsoleCursorPosition(consoleOutput, postion);
}

void Controller::setAndCenterWindow() {
	int width = 1080;
	int height = 720;

	int posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2,
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	MoveWindow(consoleWindow, posX, posY, width, height, TRUE);
}

void Controller::setConsoleColor(int background, int text) {
	int color_code = background * 16 + text;
	SetConsoleTextAttribute(consoleOutput, color_code);
}

void Controller::hideScrollBars() {
	ShowScrollBar(consoleWindow, SB_BOTH, false);
}

void Controller::setConsoleTitle() {
	SetConsoleTitleA("PIKACHU");
}

void Controller::disableMaximize() {
	HMENU consoleMenu = GetSystemMenu(consoleWindow, false);
	DeleteMenu(consoleMenu, SC_MAXIMIZE, MF_BYCOMMAND);
}

void Controller::showCursor(bool isShow) {
	CONSOLE_CURSOR_INFO cursorInfo = {1, isShow};
	SetConsoleCursorInfo(consoleOutput, &cursorInfo);
}

void Controller::setFontInfo() {}

void Controller::clearConsole() {
	// Reference: https://student.cs.uwaterloo.ca/~cs452/terminal.html
	std::cout << "\033[2J"
			  << "\033[H";
}

void Controller::disableMouseInput() {
	DWORD prev_mode;
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hInput, &prev_mode);
	SetConsoleMode(hInput, prev_mode & ~ENABLE_QUICK_EDIT_MODE);
}

int Controller::getConsoleInput() {
	int c = _getch();
	// Arrow key
	if (c == 0 || c == 224) {
		switch (_getch()) {
			case KEY_UP:
				return 2;
			case KEY_LEFT:
				return 3;
			case KEY_RIGHT:
				return 4;
			case KEY_DOWN:
				return 5;
			default:
				return 0;
		}
	} else {
		if (c == KEY_ESC)  //esc
			return 1;
		else if (c == 87 || c == 119)  //W, w
			return 2;
		else if (c == 65 || c == 97)  //A, a
			return 3;
		else if (c == 68 || c == 100)  //D, d
			return 4;
		else if (c == 83 || c == 115)  //S, s
			return 5;
		else if (c == 13)  //Enter
			return 6;
		else if (c == 72 || c == 104)  //H, h
			return 7;
		else if (c == 77 || c == 109)  // M, m
			return 8;
		else
			return 0;
	}
}

// void Controller::playSound(int i) {
// 	static vector<const char*> soundFile{
// 		"move.wav", "enter.wav",	  "error.wav", "placed.wav",
// 		"win.wav",	"background.wav", "effect.wav"};
// 	PlaySound(TEXT(soundFile[i]), NULL, SND_FILENAME | SND_ASYNC);
// }

int getRandomInt(int begin, int end) {
	static random_device rd;
	static mt19937 mt(rd());
	uniform_int_distribution<int> dist(0, end);
	return dist(mt);
}