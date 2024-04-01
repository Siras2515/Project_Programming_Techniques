/*
    This file contains the declaration of structure Controller.
    Through this structure, we can control the console window.
    Reference: https://github.com/Louis2602/Pikachu-Game/blob/master/Pikachu/Controller.h
*/
#pragma once

#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// COLOR CODE
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define AQUA 3
#define RED 4
#define PURPLE 5
#define YELLOW 6
#define WHITE 7
#define GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_AQUA 11
#define LIGHT_RED 12
#define LIGHT_PURPLE 13
#define LIGHT_YELLOW 14
#define BRIGHT_WHITE 15

// MOVEMENT
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ESC 27

// SOUNDTRACK
#define MOVE_SOUND 0
#define ENTER_SOUND 1
#define ERROR_SOUND 2
#define WIN_SOUND 4
#define BACKGROUND_SOUND 5
#define EFFECT_SOUND 6

// SPECIAL CHARACTER
#define HORIZONTAL_LINE_BOUND char(205)
#define VERTICAL_LINE_BOUND char(186)
#define TOP_LEFT_CORNER_BOUND char(201)
#define TOP_RIGHT_CORNER_BOUND char(187)
#define BOTTOM_LEFT_CORNER_BOUND char(200)
#define BOTTOM_RIGHT_CORNER_BOUND char(188)
#define VERTICAL_LINE char(179)
#define HORIZONTAL_LINE char(196)
#define TOP_LEFT_CORNER char(218)
#define TOP_RIGHT_CORNER char(191)
#define BOTTOM_LEFT_CORNER char(192)
#define BOTTOM_RIGHT_CORNER char(217)

using namespace std;

struct Controller {
	Controller(){};
	~Controller(){};
	static HWND consoleWindow;
	static HANDLE consoleOutput;

	static void setUpConsole();				// Set up the console window
	static void gotoXY(int, int);			// Move the cursor to coordinate (x, y)
	static void setSizeWindow();			// Set the console screen resolution
	static void setConsoleColor(int, int);	// Set the font and background color
	static void setConsoleTitle();			// Set the console title
	static void disableMaximize();			// Disable the maximize mode of console window
	static void disableResize();			// Disable the resize mode of console window
	static void showCursor(bool);			// Hide or show the cursor
	static void setFontInfo();				// Set the font
	static void clearConsole();				// Clear screen
	static void disableMouseInput();		// Disable mouse
	static int getConsoleInput();			// Get the keyboard input
	static void playSound(int);				// Play sound of game
};

int getRandomInt(int begin, int end);
void printCharAtXY(int = 0, int = 0, char = ' ');