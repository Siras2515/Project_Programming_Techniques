/*
    This file contains the declaration of struture BoardView.
    Through the struture, we show and control the board on the screen
    Reference: https://github.com/Louis2602/Pikachu-Game/blob/master/Pikachu/BoardView.h
*/
#pragma once

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Controller.h"
#include "Point.h"

using namespace std;

#define pii pair<int, int>

struct BoardView {

	int size;		 // Size of the game board
	int left, top;	 // Coordinate of the game board's top-left
	Point** pBoard;	 // 2D-Pointer array coordinate of the board
	int** pokemons;	 // 2D-pointer array kind of pokemon
	string* background;

	BoardView(int, int, int);
	~BoardView();

	int getSize();				 //Get size of the board
	int getLeft();				 //Get the coordinate of left-side of the board
	int getTop();				 //Get the coordinate of top-side of the board
	int getXAt(int, int);		 //Get the coordinate x of that cell
	int getYAt(int, int);		 //Get the coordinate y of that cell
	int getCheckAtXY(int, int);	 //Get the status of cell at coordinate (x, y)
	int getRAt(int, int);		 //Get the row at coordinate (x, y)
	int getCAt(int, int);		 //Get the column at coordinate (x, y)
	char getPokemons(int, int);	 //Get the kind of pokemon at coordinate (x, y)
	int getCheck(int, int);		 //Get the status of a cell at coordinate (x, y)

	void showBoard();
	void renderBoard();
	void buildBoardData();

	void selectedBlock(int, int, int);
	void unselectedBlock(int, int);

	void lockBlock(int, int);
	void deleteBlock(int, int);

	void drawLineI(pii, pii);
	void deleteLineI(pii, pii);
	void drawLineL(pii, pii, pii);
	void deleteLineL(pii, pii, pii);
	void drawLineZ(pii, pii, pii, pii);
	void deleteLineZ(pii, pii, pii, pii);
	void drawLineU(pii, pii, pii, pii);
	void deleteLineU(pii, pii, pii, pii);

	void createBackground();
};