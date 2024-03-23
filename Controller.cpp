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
	COORD position;
	position.X = x;	 // Set the X-coordinate
	position.Y = y;	 // Set the Y-coordinate
	// Move the cursor to the specified position
	SetConsoleCursorPosition(consoleOutput, position);
}

void Controller::setAndCenterWindow() {
	int width = 1080;  // Set the desired window width
	int height = 720;  // Set the desired window height

	// Calculate the position to center the window
	int posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	// Move the console window to the calculated position
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
	// Get the system menu of the console window
	HMENU consoleMenu = GetSystemMenu(consoleWindow, false);
	// Remove the "Maximize" option from the menu
	DeleteMenu(consoleMenu, SC_MAXIMIZE, MF_BYCOMMAND);
}

void Controller::showCursor(bool isShow) {
	CONSOLE_CURSOR_INFO cursorInfo = {1, isShow};
	SetConsoleCursorInfo(consoleOutput, &cursorInfo);
}

void Controller::setFontInfo() {}

void Controller::clearConsole() {
	system("cls");
}

void Controller::disableMouseInput() {
	DWORD prev_mode;  // Stores the previous console input mode
	// Get the console input handle
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	// Retrieve the current console input mode
	GetConsoleMode(hInput, &prev_mode);
	// Disable the quick edit mode (which includes mouse input)
	SetConsoleMode(hInput, prev_mode & ~ENABLE_QUICK_EDIT_MODE);
}

int Controller::getConsoleInput() {
	int c = _getch();  // Read a character from the console

	// Check if the character is an arrow key
	if (c == 0 || c == 224) {
		switch (_getch()) {
			case KEY_UP:
				return 2;  // Return 2 for arrow up
			case KEY_LEFT:
				return 3;  // Return 3 for arrow left
			case KEY_RIGHT:
				return 4;  // Return 4 for arrow right
			case KEY_DOWN:
				return 5;  // Return 5 for arrow down
			default:
				return 0;  // Return 0 for other arrow keys
		}
	} else {
		// Check other special keys
		if (c == KEY_ESC)
			return 1;  // Return 1 for ESC key
		else if (c == 87 || c == 119)
			return 2;  // Return 2 for 'W' or 'w'
		else if (c == 65 || c == 97)
			return 3;  // Return 3 for 'A' or 'a'
		else if (c == 68 || c == 100)
			return 4;  // Return 4 for 'D' or 'd'
		else if (c == 83 || c == 115)
			return 5;  // Return 5 for 'S' or 's'
		else if (c == 13)
			return 6;  // Return 6 for Enter key
		else if (c == 72 || c == 104)
			return 7;  // Return 7 for 'H' or 'h'
		else if (c == 77 || c == 109)
			return 8;  // Return 8 for 'M' or 'm'
		else
			return 0;  // Return 0 for other keys
	}
}

void Controller::playSound(int i) {
	static vector<const char*> soundFile{
		"move.wav", "enter.wav",	  "error.wav", "placed.wav",
		"win.wav",	"background.wav", "effect.wav"};

	// Play the sound file specified by the index 'i'
	PlaySoundA(soundFile[i], NULL, SND_FILENAME | SND_ASYNC);
}

int getRandomInt(int begin, int end) {
	static random_device rd;
	static mt19937 mt(rd());
	uniform_int_distribution<int> dist(0, end);
	return dist(mt);
}