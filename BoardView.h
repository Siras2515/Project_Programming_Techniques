/*
    This file contains the declaration of struture BoardView.
    Through the struture, we show and interact with the board on the screen
    Reference: https://github.com/Louis2602/Pikachu-Game/blob/master/Pikachu/BoardView.h
*/
#pragma once

#include <time.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Controller.h"
#include "Point.h"

using namespace std;

#define pii pair<int, int>

struct BoardView {
	int height, width;	// Size of the game board
	int left, top;		// Coordinates of the game board's top-left
	Point** pBoard;		// 2D-pointer array representing the board coordinates
	int** pokemons;		// 2D-pointer array representing the types of Pokémon
	string* background;

	BoardView(int, int, int, int);
	~BoardView();

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

	void showBoard(bool = false);
	void renderBoard(bool = true);
	void buildBoardData();

	void selectedBlock(int, int, int);
	void unselectedBlock(int, int);

	void lockBlock(int, int);
	void deleteBlock(int, int);

	void drawLineI(pii, pii, bool = true);
	void drawLineL(pii, pii, pii, bool = true);
	void drawLineZ(pii, pii, pii, pii, bool = true);
	void drawLineU(pii, pii, pii, pii, bool = true);

	void createBackground();
};
