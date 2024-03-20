/*
    This file contains the declarations of structure Game, which controls
	the game mechanic.
    Reference: https://github.com/Louis2602/Pikachu-Game/blob/master/Pikachu/Game.h
*/
#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "BoardView.h"
#include "Menu.h"
#include "Point.h"

#define LEFT 5
#define TOP 2

using namespace std;

struct Game {
	BoardView* board;  // Create new board
	char playerName[15] = "", playerID[9] = "", className[8] = "", mode[8] = "";
	int _mode;
	int _x, _y;
	bool isPlaying;
	int _lockedBlock;
	int _remainBlocks;
	int score;

	vector<pii> _lockedBlockPair;  //First: row - Second: column

	bool checkMatchedPokemons(pii, pii);
	int checkIMatching(pii, pii, bool);
	bool checkLMatching(pii, pii, bool);
	bool checkZMatching(pii, pii, bool);
	bool checkUMatching(pii, pii, bool);
	bool checkMatching(pii, pii, bool);

	Game(int);
	~Game();

	void setupGame();
	void startGame();
	void printInterface();
	void saveData();

	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();

	char getPokemons(int x, int y);

	void lockBlock();
	void deleteBlock();
	bool isAvailableBlock(bool);

	void askContinue();
	void moveSuggestion();
};