/*
    This file contains the declarations of structure Game, which controls
	the game mechanic.
    Reference: https://github.com/Louis2602/Pikachu-Game/blob/master/Pikachu/Game.h
*/
#pragma once

#include <mutex>
#include <thread>
#include "BoardView.h"
#include "Menu.h"
#include "Point.h"

using namespace std;

struct Game {
	BoardView* board;					// The game board
	string playerName, playerID, mode;	// Player information
	int _mode;							// Difficulty mode
	int _x, _y;							// Coordinate of top-left corner of the game board
	bool isPlaying, isPaused;			// Status of game
	int _lockedBlock;					// Number of locked block
	int _remainBlocks;					// Number of remain block
	int score;							// Score of player
	float time, time_streak;			// Time limit and time streak
	int ratio;							// Ratio of score which greater than or equal 1
	// List of block that chose and lock
	vector<pii> _lockedBlockPair;  //First: row - Second: column

	// Check if two block have the same kind of pokemon
	bool checkMatchedPokemons(pii, pii);
	// Check if have I matching pattern
	bool checkIMatching(pii, pii, bool);
	// Check if have L matching pattern
	bool checkLMatching(pii, pii, bool);
	// Check if have Z matching pattern
	bool checkZMatching(pii, pii, bool);
	// Check if have U matching pattern
	bool checkUMatching(pii, pii, bool);
	// Check if have valid matching pattern
	bool checkMatching(pii, pii, bool);

	Game(int);	// Constructor of struct to initialize the entire the game
	~Game();	// Deconstructor of struct

	// Set up player information for game and difficulty mode
	void setupGame();
	// Game loop during the whole game will be controlled
	void startGame();
	// Print the information board at the right side of screen
	void printInterface();
	// Save data of player when finished the game
	void saveData();

	void moveRight();  // Move right on the board
	void moveLeft();   // Move left on the board
	void moveUp();	   // Move up on the board
	void moveDown();   // Move down on the board

	// Get the pokemon at coordinate (x, y) on the board
	char getPokemons(int x, int y);

	void lockBlock();			  // Lock the block that chose
	void deleteBlock();			  // Delete the valid pair of block that chose
	bool isAvailableBlock(bool);  // Check if exist a valid pair to continue the game

	void askContinue();		// Ask play again after finished or lose the game
	void moveSuggestion();	// Get the move suggestion on the board
};