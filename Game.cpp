/*	
	This file defines all function that declared in "Game.h".
    Reference: https://github.com/Louis2602/Pikachu-Game/blob/master/Pikachu/Game.cpp
*/
#include "Game.h"

Game::Game(int mode) {
	// Initialize the difficulty mode
	_mode = mode;

	// Initialize the coordinate of the top-left corner
	_x = LEFT;
	_y = TOP;

	// Allocate a gameboard
	board = new BoardView(_mode, LEFT, TOP);

	// Set the game state to playing
	isPlaying = true;

	// Initialize other variables
	_lockedBlock = 0;
	_lockedBlockPair.clear();
	_remainBlocks = _mode * _mode;
	score = 0;
}

Game::~Game() {
	delete board;
	board = nullptr;
}

void Game::startGame() {
	// Reset the console screen
	Controller::clearConsole();
	while (isPlaying) {
		// At the beginning, the game has size * size blocks on the game board
		// And the score is 0
		_remainBlocks = _mode * _mode;
		score = 0;

		// Flag to check if not playing again
		bool isPause = false;

		// Print UI for entering username, ID, and class
		printInterface();

		// Set the coordinates of the game board on the screen
		_x = board->getXAt(0, 0);
		_y = board->getYAt(0, 0);

		// Show the block that the current point is on
		Controller::gotoXY(_x, _y);
		board->selectedBlock(_x, _y, GREEN);
		putchar(board->getPokemons(_x, _y));
		Controller::gotoXY(_x, _y);

		// Check if there are no more available pairs to choose
		// TODO: Add shuffle mode when there are no pairs left :)
		if (!isAvailableBlock(true)) {
			Controller::setConsoleColor(BRIGHT_WHITE, RED);
			Controller::gotoXY(69, 18);
			cout << "Game Announcement";
			Controller::gotoXY(64, 19);
			cout << "There are no more ways left!";
			Sleep(800);
			Controller::gotoXY(62, 21);
			cout << "Auto reset the board. Have fun!";
			Sleep(1000);
			startGame();
		}

		// If there is at least one pair, continue interaction with the game
		while (_remainBlocks && !isPause) {
			board->showBoard(true);
			switch (Controller::getConsoleInput()) {
				case 0:	 // Some random key
					Controller::playSound(ERROR_SOUND);
					break;
				case 1:	 // ESC to exit
					saveData();
					Menu::exitScreen();
					return;
				case 2:	 // W-key or Up-arrow key to move up
					moveUp();
					break;
				case 3:	 // D-key or Left-arrow key to move left
					moveLeft();
					break;
				case 4:	 // A-key or Right-arrow key to move right
					moveRight();
					break;
				case 5:	 // S-key or Down-arrow key to move down
					moveDown();
					break;
				case 6:	 // Enter key to choose and lock block
					lockBlock();
					break;
				case 7:	 // H-key to enter help screen
					isPause = true;
					Menu::helpScreen();
					break;
				case 8:	 // M-key to get move suggestion
					moveSuggestion();
					break;
			}
		}
		if (isPause)
			continue;

		// Ask for play again
		askContinue();
	}

	// Save data of player on another file
	saveData();
}

void Game::setupGame() {
	// Set console color and clear the console
	Controller::setConsoleColor(BRIGHT_WHITE, YELLOW);
	Controller::clearConsole();
	Controller::gotoXY(0, 0);

	// Print logo
	Menu::printLogo();

	// Show cursor
	Controller::showCursor(true);

	// Prompt user for their name
	Controller::setConsoleColor(BRIGHT_WHITE, RED);
	Controller::gotoXY(25, 15);
	cout << "Please enter your name shortly, under 10 characters!";

	// Prompt user for name, ID, and class
	Controller::setConsoleColor(BRIGHT_WHITE, LIGHT_BLUE);
	Controller::gotoXY(35, 18);
	cout << "Enter your name:  ";
	cin.getline(playerName, 15);
	Controller::gotoXY(35, 20);
	cout << "Enter your ID:  ";
	cin.getline(playerID, 9);
	Controller::gotoXY(35, 22);
	cout << "Enter your class's name:  ";
	cin.getline(className, 8);

	// Set mode based on difficulty
	if (_mode == 4)
		strcpy(mode, "EASY");
	else
		strcpy(mode, "MEDIUM");

	// Hide cursor
	Controller::showCursor(false);
}

void Game::saveData() {
	fstream fs("rank/leaderboard.txt", ios::app);
	fs << playerName << ',' << playerID << ',' << className << ',' << mode << ',' << score
	   << '\n';
	fs.close();
}

void Game::moveRight() {
	// Check if the current x-coordinate is less than the maximum x-coordinate on the game board
	if (_x < board->getXAt(board->getSize() - 1, board->getSize() - 1)) {
		// Play the "move" sound
		Controller::playSound(MOVE_SOUND);

		// If the block is not locked, unselect it
		if (board->getCheck(_x, _y) != _LOCK) {
			board->unselectedBlock(_x, _y);
		}

		// If the check status at the current position is 0, move the cursor there
		if (board->getCheckAtXY(_x, _y) == 0) {
			Controller::gotoXY(_x, _y);
		}

		// Increment the x-coordinate by 8 (move right)
		_x += 8;

		// If the block is not locked, select it in green
		if (board->getCheck(_x, _y) != _LOCK) {
			board->selectedBlock(_x, _y, GREEN);
		}
	}

	// Play an "error" sound
	Controller::playSound(ERROR_SOUND);
}

void Game::moveLeft() {
	// Check if the current x-coordinate is greater than the minimum x-coordinate on the game board
	if (_x > board->getXAt(0, 0)) {
		// Play the "move" sound
		Controller::playSound(MOVE_SOUND);

		// If the block is not locked, unselect it
		if (board->getCheck(_x, _y) != _LOCK) {
			board->unselectedBlock(_x, _y);
		}

		// If the check status at the current position is 0, move the cursor there
		if (board->getCheckAtXY(_x, _y) == 0) {
			Controller::gotoXY(_x, _y);
		}

		// Decrement the x-coordinate by 8 (move left)
		_x -= 8;

		// If the block is not locked, select it in green
		if (board->getCheck(_x, _y) != _LOCK) {
			board->selectedBlock(_x, _y, GREEN);
		}
	}

	// Play an "error" sound
	Controller::playSound(ERROR_SOUND);
}

void Game::moveDown() {
	// Check if the current y-coordinate is less than the maximum y-coordinate on the game board
	if (_y < board->getYAt(board->getSize() - 1, board->getSize() - 1)) {
		// Play the "move" sound
		Controller::playSound(MOVE_SOUND);

		// If the block is not locked, unselect it
		if (board->getCheck(_x, _y) != _LOCK) {
			board->unselectedBlock(_x, _y);
		}

		// If the check status at the current position is 0, move the cursor there
		if (board->getCheckAtXY(_x, _y) == 0) {
			Controller::gotoXY(_x, _y);
		}

		// Increment the y-coordinate by 4 (move down)
		_y += 4;

		// If the block is not locked, select it in green
		if (board->getCheck(_x, _y) != _LOCK) {
			board->selectedBlock(_x, _y, GREEN);
		}
	}

	// Play an "error" sound
	Controller::playSound(ERROR_SOUND);
}

void Game::moveUp() {
	// Check if the current y-coordinate is greater than the minimum y-coordinate on the game board
	if (_y > board->getYAt(0, 0)) {
		// Play the "move" sound
		Controller::playSound(MOVE_SOUND);

		// If the block is not locked, unselect it
		if (board->getCheck(_x, _y) != _LOCK) {
			board->unselectedBlock(_x, _y);
		}

		// If the check status at the current position is 0, move the cursor there
		if (board->getCheckAtXY(_x, _y) == 0) {
			Controller::gotoXY(_x, _y);
		}

		// Decrement the y-coordinate by 4 (move up)
		_y -= 4;

		// If the block is not locked, select it in green
		if (board->getCheck(_x, _y) != _LOCK) {
			board->selectedBlock(_x, _y, GREEN);
		}
	}

	// Play an "error" sound
	Controller::playSound(ERROR_SOUND);
}

void Game::printInterface() {
	// Create the game board background
	board->createBackground();

	// Show the game board without any animations
	board->showBoard(false);

	// Build the data for the game board
	board->buildBoardData();

	// Render the game board
	board->renderBoard();

	// Print rectangles for player and game information
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	Menu::printRectangle(59, 1, 33, 10);
	Menu::printRectangle(59, 12, 33, 10);

	// Player information section
	Menu::printRectangle(60, 2, 31, 2);
	Controller::setConsoleColor(BRIGHT_WHITE, RED);
	Controller::gotoXY(67, 3);
	cout << "PLAYER'S INFORMATION";

	// Display player name, ID, and class
	Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
	Controller::gotoXY(65, 5);
	if (strlen(playerName) != 0)
		cout << "Player's name: " << playerName;
	else {
		strcpy(playerName, "unknown");
		cout << "Player's name: " << playerName;
	}
	Controller::gotoXY(65, 7);
	if (strlen(playerID) != 0)
		cout << "Student's ID: " << playerID;
	else {
		strcpy(playerID, "unknown");
		cout << "Student's ID: " << playerID;
	}
	Controller::gotoXY(65, 9);
	if (strlen(className) != 0)
		cout << "Class: " << className;
	else {
		strcpy(className, "unknown");
		cout << "Class: " << className;
	}

	// Game information section
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	Menu::printRectangle(60, 13, 31, 2);
	Controller::setConsoleColor(BRIGHT_WHITE, RED);
	Controller::gotoXY(69, 14);
	cout << "GAME INFORMATION";
	Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
	Controller::gotoXY(65, 16);
	cout << "Moves:";
	Controller::gotoXY(65, 17);
	cout << "Current score:";
	Controller::gotoXY(80, 17);
	cout << score;

	// Additional options section
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	Menu::printRectangle(59, 24, 33, 2);
	Menu::printRectangle(59, 27, 14, 2);
	Menu::printRectangle(78, 27, 14, 2);
	Controller::setConsoleColor(BRIGHT_WHITE, PURPLE);
	Controller::gotoXY(67, 25);
	cout << "M : Move suggestion";
	Controller::setConsoleColor(BRIGHT_WHITE, GREEN);
	Controller::gotoXY(63, 28);
	cout << "H : Help";
	Controller::setConsoleColor(BRIGHT_WHITE, YELLOW);
	Controller::gotoXY(81, 28);
	cout << "Esc : Exit";
}

char Game::getPokemons(int x, int y) {
	return board->getPokemons(x, y);
}

void Game::lockBlock() {
	// Play the "enter" sound
	Controller::playSound(ENTER_SOUND);

	// If the block is marked as deleted, return (do nothing)
	if (board->getCheck(_x, _y) == _DELETE) {
		return;
	}

	// If the block is already locked, unselect it, clear the locked block pair,
	// reset the locked block count, and then select it again in green
	if (board->getCheck(_x, _y) == _LOCK) {
		board->unselectedBlock(_x, _y);
		_lockedBlockPair.clear();
		_lockedBlock = 0;
		board->selectedBlock(_x, _y, GREEN);
		return;
	}

	// Lock the block at the current position
	board->lockBlock(_x, _y);

	// Add the current position to the locked block pair
	_lockedBlockPair.push_back(pii(_x, _y));
	_lockedBlock++;

	// If there are now 2 locked blocks, call the deleteBlock function
	if (_lockedBlock == 2) {
		deleteBlock();
	}
}

bool Game::checkMatchedPokemons(pii firstBlock, pii secondBlock) {
	return (board->getPokemons(firstBlock.first, firstBlock.second) ==
			board->getPokemons(secondBlock.first, secondBlock.second));
}

int Game::checkIMatching(pii firstBlock, pii secondBlock, bool isChecking) {
	/*	
		Idea or algorithm: Check if two block on the same line and we can go forward to each other
		without any block on the way
		Pseudo-code:
		If firsBlock.x != secondBlock.x and firsBlock.y != secondBlock.y then return false
		If firstBlock.y = secondBlock.y then:
			Swap if firstBlock.x > secondBlock.x
			For i: firstBlock.x -> secondBlock.x:
				Skip if i == firstBlock.x or secondBlock.x
				Check if block at coordinate (i, firstBlock.y) didn't deleted then return false
		Else if firstBlock.x = secondBlock.x then:
			Swap if firstBlock.y > secondBlock.y
			For i: firstBlock.y -> secondBlock.y, i += 4
				Skip if i == firstBlock.y or secondBlock.y
				Check if block at coordinate (i, firstBlock.y) didn't deleted then return false
		Check if firstBlock or secondBlock or both deleted return true.
		If firstBlock and secondBlock have the same pokemon then:
			Draw I-shape line
			Delete I-shape line
			Return true
	*/
	// If both coordinate x and y of these block not equal each other
	// These block is not on the same line
	if (firstBlock.first != secondBlock.first && firstBlock.second != secondBlock.second)
		return 0;
	// If firsBlock and secondBlock are on the same row or the same coordinate y
	if (firstBlock.second == secondBlock.second) {
		if (firstBlock.first > secondBlock.first)
			swap(firstBlock, secondBlock);
		// Go straight from coordinate x of firstBlock to coordinate x of secondBlock
		for (int i = firstBlock.first; i <= secondBlock.first; i += 8) {
			if (i == firstBlock.first || i == secondBlock.first)
				continue;
			// Check if at coordinate (i, firstBlock.y) have a block that didn't delete
			// If that block didn't delete, we couldn't get a way between two block
			// Return false
			if (board->getCheck(i, firstBlock.second) != _DELETE) {
				return 0;
			}
		}
	}
	// If firsBlock and secondBlock are on the same col or the same coordinate x
	else if (firstBlock.first == secondBlock.first) {
		if (firstBlock.second > secondBlock.second)
			swap(firstBlock, secondBlock);
		// Go straight from coordinate y of firstBlock to coordinate x of secondBlock
		for (int i = firstBlock.second; i <= secondBlock.second; i += 4) {
			if (i == firstBlock.second || i == secondBlock.second)
				continue;
			// Check if at coordinate (i, firstBlock.y) have a block that didn't delete
			// If that block didn't delete, we couldn't get a way between two block
			// Return false
			if (board->getCheck(firstBlock.first, i) != _DELETE) {
				return 0;
			}
		}
	}
	// We don't need check anymore if one of two block was deleted before
	// Or if we just want to check that can draw a I line from firstBlock and secondBlock
	if (board->getCheck(firstBlock.first, firstBlock.second) == _DELETE ||
		board->getCheck(secondBlock.first, secondBlock.second) == _DELETE) {
		return 1;
	}
	// Check if two blocks have the same pokemon
	if (checkMatchedPokemons(firstBlock, secondBlock)) {
		if (isChecking == false) {
			board->drawLineI(firstBlock, secondBlock);
			Sleep(1000);  // Sleep(200);
			board->drawLineI(firstBlock, secondBlock, false);
		}
		return 1;
	}

	return 0;
}
bool Game::checkLMatching(pii firstBlock, pii secondBlock, bool isChecking) {
	/*	
		Idea or algorithm: Find the corner of L-shape line which have the same x with one
		and the same y with another one. And check if we can have I-shape lines from it to
		each block.
		Pseudo-code: 
			Swap if firstBlock.x > secondBlock.y
			Lcorner.x := firstBlock.x
			Lcorner.y := secondBlock.y
			If block at coordinate of Lcorner deleted then:
				If have I-shape line from Lcorner to each block then:
					Draw the L-shape line
					Delete I-shape line
					Return true
			Lcorner.x := secondBlock.x
			Lcorner.y := firstBlock.y
			If block at coordinate of Lcorner deleted then:
				If have I-shape line from Lcorner to each block then:
					Draw the L-shape line
					Delete I-shape line
					Return true

			Return false if nothing happened
	*/
	pii Lcorner;

	auto isAvailable = [&]() {
		// Check if at Lcorner is a deleted block
		if (board->getCheck(Lcorner.first, Lcorner.second) == _DELETE) {
			// If true, then check if we can have a I line from Lcorner to each blocks
			if (checkIMatching(Lcorner, firstBlock, isChecking) &&
				checkIMatching(Lcorner, secondBlock, isChecking)) {
				if (isChecking == false) {
					board->drawLineL(firstBlock, secondBlock, Lcorner);
					Sleep(1000);  // Sleep(200);
					board->drawLineL(firstBlock, secondBlock, Lcorner, false);
				}
				return 1;
			}
		}
	};
	// Swap them if firstBlock is at the right-side of secondBlock
	if (firstBlock.first > secondBlock.first)
		swap(firstBlock, secondBlock);

	// Initialize the corner of the L line
	// First case: Lcorner have the same x with firstBlock and the same y with secondBlock
	// Bottom-left or top-left corner
	Lcorner.first = firstBlock.first;
	Lcorner.second = secondBlock.second;

	if (isAvailable())
		return 1;

	// Second case: Lcorner have the same x with secondBlock and the same y with firstBlock
	// Bottom-right or top-right corner
	Lcorner.first = secondBlock.first;
	Lcorner.second = firstBlock.second;

	if (isAvailable())
		return 1;

	return 0;
}
bool Game::checkZMatching(pii firstBlock, pii secondBlock, bool isChecking) {
	/*
		Idea or algorithm: Find two corner of Z-shape line which have the same x or y
		with two block. And through two corner, we can have three I-shape line to connect
		two corner and two block.
		Pseudo code:
			Swap if firstBlock.x > secondBlock.x
			For i: firstBlock.x -> secondBlock.x:
				Initialize coordinate of Zcorner1 to (i, firstBlock,y)
				Initialize coordinate of Zcorner2 to (i, secondBlock,y)
				If block at Zcorner1 and at Zcorner2 deleted then:
					If have I-shape lines from firstBlock to Zcorner1, from Zcorner1 and Zcorner2
					and from Zcorner2 to secondBlock then:
						Draw Z-shape line
						Delete Z-shape line
						Return true
			Swap if firstBlock.y > secondBlock.y
			For i: firstBlock.y -> secondBlock.y:
				Initialize coordinate of Zcorner1 to (firstBlock.x, i)
				Initialize coordinate of Zcorner2 to (secondBlock.x, i)
				If block at Zcorner1 and at Zcorner2 deleted then:
					If have I-shape lines from firstBlock to Zcorner1, from Zcorner1 and Zcorner2
					and from Zcorner2 to secondBlock then:
						Draw Z-shape line
						Delete Z-shape line
						Return true
			
			Return false if nothing happened
	*/
	pii Zcorner1;
	pii Zcorner2;

	auto isAvailable = [&]() {
		// Check if these two coordinate are deleted block
		if (board->getCheck(Zcorner1.first, Zcorner1.second) == _DELETE &&
			board->getCheck(Zcorner2.first, Zcorner2.second) == _DELETE) {
			// Check if we can have a I line from firstBlock to Zcorner1
			// and from secondBlock to Zcorner2 and between two Zcorner
			if (checkIMatching(firstBlock, Zcorner1, isChecking) &&
				checkIMatching(Zcorner1, Zcorner2, isChecking) &&
				checkIMatching(Zcorner2, secondBlock, isChecking)) {
				if (isChecking == false) {
					board->drawLineZ(firstBlock, secondBlock, Zcorner1, Zcorner2);
					Sleep(1000);  // Sleep(200);
					board->drawLineZ(firstBlock, secondBlock, Zcorner1, Zcorner2, false);
				}
				return 1;
			}
		}
	};
	// Swap them if firstBlock is at the right-side of secondBlock
	if (firstBlock.first > secondBlock.first)
		swap(firstBlock, secondBlock);

	// Between coordinate x of firstBlock and coordinate x of secondBlock
	// Find two Zcorner
	for (int i = firstBlock.first + 8; i < secondBlock.first; i += 8) {
		// Initialize these two corner
		Zcorner1.first = i, Zcorner1.second = firstBlock.second;
		Zcorner2.first = i, Zcorner2.second = secondBlock.second;

		if (isAvailable())
			return 1;
	}

	// Swap them if firstBlock is at the bottom-side of secondBlock
	if (firstBlock.second > secondBlock.second)
		swap(firstBlock, secondBlock);

	// Between coordinate y of firstBlock and coordinate y of secondBlock
	// Find two Zcorner
	for (int i = firstBlock.second + 4; i < secondBlock.second; i += 4) {
		// Initialize these two corner
		Zcorner1.first = firstBlock.first, Zcorner1.second = i;
		Zcorner2.first = secondBlock.first, Zcorner2.second = i;

		if (isAvailable())
			return 1;
	}
	return 0;
}
bool Game::checkUMatching(pii firstBlock, pii secondBlock, bool isChecking) {
	/*
		Idea or algorithm: Find two corner of U-shape line which have the same x or y
		with two block. And through two corner, we can have three I-shape line to connect
		two corner and two block.
		Pseudo-code:
			size := size of game board
			x := coordinate x of block (0, 0) on screen
			y := coordinate y of block (0, 0) on screen

			left_x := coordinate x of left most block
			right_x := coordinate x of right most block
			top_y := coordinate x of higher block
			bottom_y := coordinate x of lower block

			For i: x - 8 -> left_x - 8:
				Initialize coordinate of Ucorner1 to (i, firstBlock.y)
				Initialize coordinate of Ucorner2 to (i, secondBlock.y)
				If block at Ucorner1 and at Ucorner2 deleted then:
					If have I-shape lines from firstBlock to Ucorner1, from Ucorner1 and Ucorner2
					and from Ucorner2 to secondBlock then:
						Draw Z-shape line
						Delete Z-shape line
						Return true
			For i: right_x + 8 -> x + size * 8:
				Initialize coordinate of Ucorner1 to (i, firstBlock.y)
				Initialize coordinate of Ucorner2 to (i, secondBlock.y)
				If block at Ucorner1 and at Ucorner2 deleted then:
					If have I-shape lines from firstBlock to Ucorner1, from Ucorner1 and Ucorner2
					and from Ucorner2 to secondBlock then:
						Draw Z-shape line
						Delete Z-shape line
						Return true
			For i: y - 4 -> top_y - 4:
				Initialize coordinate of Ucorner1 to (firstBlock.x, i)
				Initialize coordinate of Ucorner2 to (secondBlock.x, i)
				If block at Ucorner1 and at Ucorner2 deleted then:
					If have I-shape lines from firstBlock to Ucorner1, from Ucorner1 and Ucorner2
					and from Ucorner2 to secondBlock then:
						Draw Z-shape line
						Delete Z-shape line
						Return true
			For i: bottom_y + 4 -> y + size * 4:
				Initialize coordinate of Ucorner1 to (firstBlock.x, i)
				Initialize coordinate of Ucorner2 to (secondBlock.x, i)
				If block at Ucorner1 and at Ucorner2 deleted then:
					If have I-shape lines from firstBlock to Ucorner1, from Ucorner1 and Ucorner2
					and from Ucorner2 to secondBlock then:
						Draw Z-shape line
						Delete Z-shape line
						Return true
			
			Return false if nothing happened

	*/
	pii Ucorner1;
	pii Ucorner2;
	const int size = board->getSize();
	const int x = board->getXAt(0, 0);
	const int y = board->getYAt(0, 0);

	int left_x = min(firstBlock.first, secondBlock.first);
	int right_x = max(firstBlock.first, secondBlock.first);
	int top_y = min(firstBlock.second, secondBlock.second);
	int bottom_y = max(firstBlock.second, secondBlock.second);

	auto isAvailable = [&]() {
		// Check if these corner are deleted or not
		if (board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE &&
			board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE) {
			// If deleted, check if we can draw an U-shape
			if (checkIMatching(firstBlock, Ucorner1, isChecking) &&
				checkIMatching(Ucorner1, Ucorner2, isChecking) &&
				checkIMatching(Ucorner2, secondBlock, isChecking)) {
				if (isChecking == false) {
					board->drawLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
					Sleep(1000);  // Sleep(200);
					board->drawLineU(firstBlock, secondBlock, Ucorner1, Ucorner2, false);
				}
				return 1;
			}
		}
	};
	// Left vertical U
	// Between coordinate x of left most block and out of left bound away one block
	for (int i = left_x - 8; i >= x - 8; i -= 8) {
		// Initialize these corner
		Ucorner1.first = i, Ucorner1.second = firstBlock.second;
		Ucorner2.first = i, Ucorner2.second = secondBlock.second;

		if (isAvailable())
			return 1;
	}
	// Right vertical U
	// Between coordinate x of right most block and out of right bound away one block
	for (int i = right_x + 8; i <= x + size * 8; i += 8) {
		// Initialize these corner
		Ucorner1.first = i, Ucorner1.second = firstBlock.second;
		Ucorner2.first = i, Ucorner2.second = secondBlock.second;

		if (isAvailable())
			return 1;
	}
	// Top horizontal U
	// Between coordinate x of highest block and out of top bound away one block
	for (int i = top_y - 4; i >= y - 4; i -= 4) {
		// Initialize these corner
		Ucorner1.first = firstBlock.first, Ucorner1.second = i;
		Ucorner2.first = secondBlock.first, Ucorner2.second = i;

		if (isAvailable())
			return 1;
	}
	// Bottom horizontal U
	for (int i = bottom_y + 4; i <= y + size * 4; i += 4) {
		// Initialize these corner
		Ucorner1.first = firstBlock.first, Ucorner1.second = i;
		Ucorner2.first = secondBlock.first, Ucorner2.second = i;

		if (isAvailable())
			return 1;
	}
	return 0;
}

bool Game::checkMatching(pii firstBlock, pii secondBlock, bool isChecking) {
	// Parameter isChecking here mean if we JUST want to check these blocks are
	// matching or not and don't want draw line or do anything else
	if ((firstBlock.first == secondBlock.first) &&
		(firstBlock.second == secondBlock.second))
		return 0;

	auto printScore = [&](int color) {
		Controller::setConsoleColor(BRIGHT_WHITE, color);

		if (score >= 0) {
			Controller::gotoXY(80, 17);
			cout << score << " BTC ";
		} else {
			Controller::gotoXY(80, 17);
			cout << score << " BTC";
		}
	};

	// Check if these blocks don't have the same pokemon
	if (!checkMatchedPokemons(firstBlock, secondBlock)) {
		// Show the notification that not matched and decrease player point
		if (isChecking == false) {
			Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
			Controller::gotoXY(72, 16);
			cout << "Not Matched";
			score -= 2;
			printScore(RED);
		}
		return 0;
	}
	// If these block have the same pokemon
	// Check if these block match with I-shape
	if (checkIMatching(firstBlock, secondBlock, isChecking)) {
		if (isChecking == false) {
			Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
			Controller::gotoXY(72, 16);
			cout << "I Matching.";
			score += 1;
			printScore(GREEN);
		}
		return 1;
	}
	// Check if these block match with L-shape
	if (checkLMatching(firstBlock, secondBlock, isChecking)) {
		if (isChecking == false) {
			Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
			Controller::gotoXY(72, 16);
			cout << "L Matching.";
			score += 2;
			printScore(GREEN);
		}
		return 1;
	}
	// Check if these block match with Z-shape
	if (checkZMatching(firstBlock, secondBlock, isChecking)) {
		if (isChecking == false) {
			Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
			Controller::gotoXY(72, 16);
			cout << "Z Matching.";
			score += 3;
			printScore(GREEN);
		}
		return 1;
	}
	// Check if these block match with U-shape
	if (checkUMatching(firstBlock, secondBlock, isChecking)) {
		if (isChecking == false) {
			Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
			Controller::gotoXY(72, 16);
			cout << "U Matching.";
			score += 4;
			printScore(GREEN);
		}
		return 1;
	}
	return 0;
}

void Game::deleteBlock() {
	_lockedBlock = 0;
	bool isChecking = false;

	// Check if the locked blocks don't match; unselect them and return
	if (!checkMatching(_lockedBlockPair[0], _lockedBlockPair[1], isChecking)) {
		for (auto block : _lockedBlockPair)
			board->unselectedBlock(block.first, block.second);
		_lockedBlockPair.clear();
		board->selectedBlock(_x, _y, GREEN);
		return;
	}

	// If the blocks match, mark them as deleted and remove them
	for (auto block : _lockedBlockPair)
		board->deleteBlock(block.first, block.second);
	_lockedBlockPair.clear();
	board->selectedBlock(_x, _y, GREEN);

	// Decrease the total remaining blocks
	_remainBlocks -= 2;

	// Check if there are no more blocks; display win notification
	if (_remainBlocks == 0) {
		Controller::setConsoleColor(BRIGHT_WHITE, RED);
		Controller::gotoXY(69, 18);
		cout << "Game Announcement";
		Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
		Controller::gotoXY(67, 19);
		cout << "You have won the game.";
		Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
		Controller::gotoXY(69, 20);
		cout << "CONGRATULATIONS!";
		Controller::gotoXY(70, 21);
		cout << "Your score: " << score;
		Controller::playSound(WIN_SOUND);
		board->unselectedBlock(_x, _y);
		_x = board->getXAt(0, 0);
		_y = board->getYAt(0, 0);
		Controller::gotoXY(_x, _y);
		board->selectedBlock(_x, _y, BRIGHT_WHITE);
		Sleep(7000);
		return;
	}

	// Otherwise, continue checking if there are more available pairs of blocks
	// If not, reset the game board
	isChecking = true;
	if (!isAvailableBlock(isChecking)) {
		Controller::setConsoleColor(BRIGHT_WHITE, RED);
		Controller::gotoXY(69, 18);
		cout << "Game Announcement";
		Controller::gotoXY(64, 19);
		cout << "There are no more ways left!";
		Controller::playSound(EFFECT_SOUND);
		Sleep(1000);
		Controller::gotoXY(62, 21);
		cout << "Auto reset the board. Have fun!";
		Sleep(4000);
		startGame();
	}
}

bool Game::isAvailableBlock(bool isChecking) {
	int size = board->getSize();
	pii firstBlock;
	pii secondBlock;

	// We use a brute-force algorithm to check all pairs of blocks on the board
	// If there are no available pairs, return false; otherwise, return true
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			firstBlock.first = board->getXAt(i, j);
			firstBlock.second = board->getYAt(i, j);

			// Skip blocks marked as deleted
			if (board->getCheck(firstBlock.first, firstBlock.second) == _DELETE) {
				continue;
			}

			for (int m = i; m < size; m++) {
				for (int n = 0; n < size; n++) {
					// Skip redundant pairs (same row and column)
					if (i == m && n <= j)
						continue;

					secondBlock.first = board->getXAt(m, n);
					secondBlock.second = board->getYAt(m, n);

					// Skip blocks marked as deleted
					if (board->getCheck(secondBlock.first, secondBlock.second) == _DELETE)
						continue;

					// Check if the pair of blocks match
					if (checkMatching(firstBlock, secondBlock, isChecking)) {
						return true;
					}
				}
			}
		}
	}

	return false;
}

void Game::askContinue() {
	// Set console color and clear the console
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	Controller::clearConsole();
	Controller::gotoXY(0, 0);

	// Print logo in red
	Controller::setConsoleColor(BRIGHT_WHITE, RED);
	Menu::printLogo();

	// Print rectangles
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	Menu::printRectangle(34, 15, 35, 6);
	Menu::printRectangle(37, 18, 7, 2);
	Menu::printRectangle(60, 18, 6, 2);

	// Ask if the player wants to play another round
	Controller::gotoXY(36, 16);
	Controller::setConsoleColor(BRIGHT_WHITE, GREEN);
	cout << "Do you want to play another round?";

	// Options for Yes and No
	string str[2] = {"Yes", "No"};
	int left[] = {35, 40, 47, 58, 63, 69}, word[] = {32, 32, 175, 174};
	int color[] = {BLACK, GREEN};
	int top = 19;
	bool choice = 1;

	// Function to print options
	auto print1 = [&]() {
		int i = 0;
		while (i < 2) {
			Controller::playSound(MOVE_SOUND);
			Controller::setConsoleColor(BRIGHT_WHITE, color[i]);
			Controller::gotoXY(left[choice * 3], top);
			putchar(word[i * 2]);
			Controller::gotoXY(left[choice * 3 + 1], top);
			cout << str[choice];
			Controller::gotoXY(left[choice * 3 + 2], top);
			putchar(word[i * 2 + 1]);
			if (!i++)
				choice = !choice;
		}
	};

	// Initial print
	print1();

	// Loop to handle user input
	while (true) {
		int key = Controller::getConsoleInput();
		if ((key == 3 && choice == 1) || (key == 4 && choice == 0))
			print1();
		else if (key == 6) {
			if (!choice)
				isPlaying = true;
			else
				isPlaying = false;
			return;
		}
		Controller::playSound(ERROR_SOUND);
	}
}

void Game::moveSuggestion() {
	bool isHelp = true;
	int size = board->getSize();
	pii firstBlock;
	pii secondBlock;

	// We use a brute-force algorithm to find any available pair
	// Search the entire board, choose a pair of blocks, and check
	// If available, select them to show a hint for the player
	// Then exit the function, showing only one pair to the player
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			firstBlock.first = board->getXAt(i, j);
			firstBlock.second = board->getYAt(i, j);

			// Skip blocks marked as deleted
			if (board->getCheck(firstBlock.first, firstBlock.second) == _DELETE) {
				continue;
			}

			for (int m = i; m < size; m++) {
				for (int n = 0; n < size; n++) {
					// Skip redundant pairs (same row and column)
					if (i == m && n <= j)
						continue;

					secondBlock.first = board->getXAt(m, n);
					secondBlock.second = board->getYAt(m, n);

					// Skip blocks marked as deleted
					if (board->getCheck(secondBlock.first, secondBlock.second) == _DELETE)
						continue;

					// Check if the pair of blocks match
					if (checkMatching(firstBlock, secondBlock, isHelp)) {
						if (isHelp) {
							// Show the hint by selecting the blocks in purple
							board->selectedBlock(firstBlock.first, firstBlock.second,
												 PURPLE);
							board->selectedBlock(secondBlock.first, secondBlock.second,
												 PURPLE);
							Sleep(200);
							board->unselectedBlock(firstBlock.first, firstBlock.second);
							board->unselectedBlock(secondBlock.first, secondBlock.second);
							score -= 2;
							Controller::setConsoleColor(BRIGHT_WHITE, RED);
							if (score >= 0) {
								Controller::gotoXY(80, 17);
								cout << score << " BTC ";
							} else {
								Controller::gotoXY(80, 17);
								cout << score << " BTC";
							}
							return;
						}
					}
				}
			}
		}
	}
}