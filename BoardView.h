/*
    This file contains the declaration of struture BoardView.
    Through the struture, we show and interact with the board on the screen
    Reference: https://github.com/Louis2602/Pikachu-Game/blob/master/Pikachu/BoardView.h
*/
#pragma once

#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Controller.h"
#include "Point.h"

using namespace std;

#define LEFT 5
#define TOP 2
#define pii pair<int, int>

struct BoardView {
	int height, width;	 // Size of the game board
	int left, top;		 // Coordinates of the game board's top-left
	Point** pBoard;		 // 2D-pointer array representing the board 
	string* background;	 // A dynamic array representing the background

	BoardView(int, int, int, int);	// Constructor of struct
	~BoardView();					// Destructor of struct

	int getHeight();	   // Get height of the board
	int getWidth();		   // Get width of the board
	int getLeft();		   // Get the coordinate of the left side of the board
	int getTop();		   // Get the coordinate of the top side of the board
	int getXAt(int, int);  // Get the x-coordinate of a cell
	int getYAt(int, int);  // Get the y-coordinate of a cell
	// Check the a cell at coordinate (x, y) is out of bound or not
	int getCheckAtXY(int, int);
	int getRAt(int, int);		 // Get the row at coordinate (x, y)
	int getCAt(int, int);		 // Get the column at coordinate (x, y)
	char getPokemons(int, int);	 // Get the kind of Pokémon at coordinate (x, y)
	int getCheck(int, int);		 // Get the status of a cell at coordinate (x, y)

	// Show the board game on the screen and reset the bound liner
	void showBoard(bool = true);
	void renderBoard(bool = true);	   // Show the Pokémon character on the board
	void buildBoardData(bool = true);  // Construct a game board that can play

	void selectedBlock(int, int, int);	// Select a block on the board with a color
	void unselectedBlock(int, int);		// Unselect a block on the board

	void lockBlock(int, int);	 // Lock a block that has chosen on the board
	void deleteBlock(int, int);	 // Delete a block on the board

	// Print a char at coordinate (x, y) on screen
	void printChar(int, int, char, bool);
	// Draw or erase the I-line from firstBlock to secondBlock on the board
	void drawLineI(pii, pii, bool = true);
	// Draw or erase the L-line from firstBlock to secondBlock on the board
	void drawLineL(pii, pii, pii, bool = true);
	// Draw or erase the Z-line from firstBlock to secondBlock on the board
	void drawLineZ(pii, pii, pii, pii, bool = true);
	// Draw or erase the U-line from firstBlock to secondBlock on the board
	void drawLineU(pii, pii, pii, pii, bool = true);
	// Read file and create background for the board
	void createBackground();
};
