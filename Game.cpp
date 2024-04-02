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
	board = new BoardView(min(_mode, 6), min(_mode + 1, 8), LEFT, TOP);

	// Set the game state to playing
	isPlaying = true;
	isPaused = false;
	ratio = 1;
	time_streak = 0;

	// Initialize other variables
	_lockedBlock = 0;
	_lockedBlockPair.clear();
	_remainBlocks = min(_mode, 6) * min(_mode + 1, 8);
	score = 0;
}

Game::~Game() {
	delete board;
	board = nullptr;
}

void Game::startGame() {
	// Reset the console screen
	Controller::clearConsole();

	// Create a mutex for thread safety
	mutex mu;

	// The game continues as long as 'isPlaying' is true
	while (isPlaying) {
		// At the start of each game, initialize the number of remaining blocks and the score
		_remainBlocks = min(_mode, 6) * min(_mode + 1, 8);
		score = 0;

		// Set a timer based on the game mode
		time = _mode * 20;
		time_streak = 0;
		ratio = 1;

		// Flag to check if wait to another thread
		isPaused = false;

		// Create the game board background
		board->createBackground();

		// Show the game board with animations
		board->showBoard();

		// Build the game board data until there is an available block
		do {
			board->buildBoardData();
		} while (!isAvailableBlock(true));

		// Render the game board
		board->renderBoard();

		// Print the UI of the game screen
		printInterface();

		// Set the coordinates of the game board on the screen
		_x = board->getXAt(0, 0);
		_y = board->getYAt(0, 0);

		// Show the block that the current point is on
		Controller::gotoXY(_x, _y);
		board->selectedBlock(_x, _y, GREEN);
		putchar(board->getPokemons(_x, _y));
		Controller::gotoXY(_x, _y);

		// Define a timer function to be executed in a separate thread
		auto timer = [&]() {
			Sleep(1000);
			while (time >= -0.1F) {
				// End the game if there are no remaining blocks or 'isPlaying' is false
				if (_remainBlocks == 0 || !isPlaying)
					break;
				// Skip the current iteration if the game is paused
				if (isPaused)
					continue;
				// Lock the mutex before updating shared data
				mu.lock();
				Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
				printf("\033[%d;%dHTime remain: %.1fs  ", 10, 82, abs(time));
				Controller::setConsoleColor(BRIGHT_WHITE, RED);
				printf("\033[%d;%dH %.1f ", 10, 100, time_streak);
				// Unlock the mutex after updating shared data
				mu.unlock();
				Sleep(100);
				time -= 0.1;
				if (1 <= time_streak && time_streak < 5) {
					ratio = 2;
					time_streak += 0.1;
				} else if (time_streak >= 5) {
					time_streak = 0;
					ratio = 1;
				}
			}
			// Check if the game is over due to time running out
			if (time <= 0.0F) {
				Controller::setConsoleColor(BRIGHT_WHITE, RED);
				Controller::gotoXY(85, 18);
				cout << "Game Announcement";
				Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
				Controller::gotoXY(83, 19);
				cout << "You have lose the game.";
				Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
				Controller::gotoXY(85, 20);
				cout << "Your score: " << score;
				Controller::playSound(EFFECT_SOUND);
				board->unselectedBlock(_x, _y);
				_x = board->getXAt(0, 0);
				_y = board->getYAt(0, 0);
				Controller::gotoXY(_x, _y);
				board->selectedBlock(_x, _y, BRIGHT_WHITE);
				Sleep(4000);
			}
		};	// End of timer function definition

		// Start the timer function in a separate thread
		thread f(timer);

		// Continue the game as long as there are remaining blocks and time left
		while (_remainBlocks && time > 0.0F) {
			// If the game is paused, reset the game board and interface
			if (isPaused) {
				Controller::clearConsole();
				board->showBoard();
				board->renderBoard(false);
				printInterface();
				isPaused = false;
			}

			// If there are no more available pairs, pause the game and reshuffle the board
			if (_remainBlocks && !isAvailableBlock(true)) {
				isPaused = true;
				mu.lock();
				Controller::setConsoleColor(BRIGHT_WHITE, RED);
				Controller::gotoXY(85, 18);
				cout << "Game Announcement";
				Controller::gotoXY(80, 19);
				cout << "There are no more ways left!";
				Sleep(800);
				Controller::gotoXY(76, 21);
				cout << "Auto shuffle the board. Have fun!";
				Sleep(2000);

				// Shuffle the board again
				Controller::clearConsole();
				do {
					// Build the data for the game board
					board->buildBoardData(false);
				} while (!isAvailableBlock(true));
				board->showBoard();
				board->renderBoard(false);
				printInterface();
				mu.unlock();
				isPaused = false;
			}

			// Handle user input
			switch (Controller::getConsoleInput()) {
				case 0:	 // Some random key
					Controller::playSound(ERROR_SOUND);
					break;
				case 1:	 // ESC to back home
					isPaused = true;
					if (Menu::backHome()) {
						isPlaying = false;
						f.join();
						return;
					}
					break;
				case 2:	 // W-key or Up-arrow key to move up
					mu.lock();
					moveUp();
					mu.unlock();
					break;
				case 3:	 // D-key or Left-arrow key to move left
					mu.lock();
					moveLeft();
					mu.unlock();
					break;
				case 4:	 // A-key or Right-arrow key to move right
					mu.lock();
					moveRight();
					mu.unlock();
					break;
				case 5:	 // S-key or Down-arrow key to move down
					mu.lock();
					moveDown();
					mu.unlock();
					break;
				case 6:	 // Enter key to choose and lock block
					mu.lock();
					lockBlock();
					board->showBoard(false);
					mu.unlock();
					break;
				case 7:	 // H-key to enter help screen
					isPaused = true;
					Menu::helpScreen();
					break;
				case 8:	 // M-key to get move suggestion
					mu.lock();
					moveSuggestion();
					mu.unlock();
					break;
			}
		}

		// If there are no more blocks, display win notification
		if (_remainBlocks == 0) {
			mu.lock();
			Controller::setConsoleColor(BRIGHT_WHITE, RED);
			Controller::gotoXY(85, 18);
			cout << "Game Announcement";
			Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
			Controller::gotoXY(83, 19);
			cout << "You have won the game.";
			Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
			Controller::gotoXY(85, 20);
			cout << "CONGRATULATIONS!";
			Controller::gotoXY(86, 21);
			cout << "Your score: " << score;
			Controller::playSound(WIN_SOUND);
			board->unselectedBlock(_x, _y);
			_x = board->getXAt(0, 0);
			_y = board->getYAt(0, 0);
			Controller::gotoXY(_x, _y);
			board->selectedBlock(_x, _y, BRIGHT_WHITE);
			mu.unlock();

			// Save data of player on another file
			saveData();
			Sleep(7000);
		}

		// Join the timer thread to the main thread
		f.join();

		// Ask the player if they want to play again
		askContinue();
	}
}

void Game::setupGame() {
	// Declare a boolean variable to check if the input is valid
	bool isValid = false;

	// Loop until valid input is received
	while (!isValid) {
		// Set console color and clear the console
		Controller::setConsoleColor(BRIGHT_WHITE, YELLOW);
		Controller::clearConsole();
		Controller::gotoXY(0, 0);

		// Print the game logo
		Menu::printLogo();

		// Show the cursor
		Controller::showCursor(true);

		// Prompt the user for their name
		Controller::setConsoleColor(BRIGHT_WHITE, RED);
		Controller::gotoXY(25, 15);
		cout << "Please enter your name shortly, under 10 characters!";

		// Prompt the user for their name and ID
		Controller::setConsoleColor(BRIGHT_WHITE, LIGHT_BLUE);
		Controller::gotoXY(35, 18);
		cout << "Enter your name:  ";
		getline(cin, playerName);

		// Check if the name is longer than 10 characters
		if (playerName.size() > 10) {
			Controller::setConsoleColor(BRIGHT_WHITE, RED);
			Controller::gotoXY(45, 17);
			cout << "Invalid name!";
			playerName = "";
			Sleep(1000);
			continue;
		}

		// If no name is entered, set it as "Unknown"
		if (playerName == "")
			playerName = "Unknown";

		Controller::gotoXY(35, 20);
		cout << "Enter your ID:  ";
		getline(cin, playerID);

		// If no ID is entered, set it as "Unknown"
		if (playerID == "")
			playerID = "Unknown";

		// Set isValid to true to exit the loop
		isValid = true;
	}

	// Set the game mode based on the difficulty level
	if (_mode == _EASY)
		mode = "EASY";
	else if (_mode == _MEDIUM)
		mode = "MEDIUM";
	else
		mode = "HARD";

	// Hide the cursor
	Controller::showCursor(false);
}

void Game::saveData() {
	fstream fs("rank/leaderboard.txt", ios::app);
	fs << playerName << ',' << playerID << ',' << mode << ',' << time << ',' << score
	   << '\n';
	fs.close();
}

void Game::moveRight() {
	// Check if the current x-coordinate is less than the maximum x-coordinate on the game board
	if (_x < board->getXAt(board->getHeight() - 1, board->getWidth() - 1)) {
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
}

void Game::moveDown() {
	// Check if the current y-coordinate is less than the maximum y-coordinate on the game board
	if (_y < board->getYAt(board->getHeight() - 1, board->getWidth() - 1)) {
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
}

void Game::printInterface() {
	// Print rectangles for player and game information
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	Menu::printRectangle(75, 1, 33, 10);
	Menu::printRectangle(75, 12, 33, 10);

	// Player information section
	Menu::printRectangle(76, 2, 31, 2);
	Controller::setConsoleColor(BRIGHT_WHITE, RED);
	Controller::gotoXY(83, 3);
	cout << "PLAYER'S INFORMATION";

	// Display player name, ID, and class
	Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
	Controller::gotoXY(81, 5);
	cout << "Player's name : " << playerName;
	Controller::gotoXY(81, 7);
	cout << "Student's ID: " << playerID;

	// Game information section
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	Menu::printRectangle(76, 13, 31, 2);
	Controller::setConsoleColor(BRIGHT_WHITE, RED);
	Controller::gotoXY(85, 14);
	cout << "GAME INFORMATION";
	Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
	Controller::gotoXY(81, 16);
	cout << "Moves:";
	Controller::gotoXY(81, 17);
	cout << "Current score:";
	Controller::gotoXY(96, 17);
	cout << score;

	// Additional options section
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	Menu::printRectangle(75, 24, 33, 2);
	Menu::printRectangle(75, 27, 14, 2);
	Menu::printRectangle(94, 27, 14, 2);
	Controller::setConsoleColor(BRIGHT_WHITE, PURPLE);
	Controller::gotoXY(83, 25);
	cout << "M : Move suggestion";
	Controller::setConsoleColor(BRIGHT_WHITE, GREEN);
	Controller::gotoXY(79, 28);
	cout << "H : Help";
	Controller::setConsoleColor(BRIGHT_WHITE, YELLOW);
	Controller::gotoXY(97, 28);
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

bool Game::checkIMatching(pii firstBlock, pii secondBlock, bool isChecking) {
	// If both coordinate x and y of these blocks are not equal to each other,
	// these blocks are not on the same line
	if (firstBlock.first != secondBlock.first && firstBlock.second != secondBlock.second)
		return false;

	// If firstBlock and secondBlock are on the same row or have the same y-coordinate
	if (firstBlock.second == secondBlock.second) {
		// Swap if firstBlock's x-coordinate is greater than secondBlock's
		if (firstBlock.first > secondBlock.first)
			swap(firstBlock, secondBlock);

		// Go straight from the x-coordinate of firstBlock to the x-coordinate of secondBlock
		for (int i = firstBlock.first; i <= secondBlock.first; i += 8) {
			// Skip if i is equal to the x-coordinate of firstBlock or secondBlock
			if (i == firstBlock.first || i == secondBlock.first)
				continue;

			// Check if the block at coordinate (i, firstBlock.y) has been deleted
			// If that block hasn't been deleted, we can't find a path between the two blocks
			// Return false
			if (board->getCheck(i, firstBlock.second) != _DELETE) {
				return false;
			}
		}
	}
	// If firstBlock and secondBlock are on the same column or have the same x-coordinate
	else if (firstBlock.first == secondBlock.first) {
		// Swap if firstBlock's y-coordinate is greater than secondBlock's
		if (firstBlock.second > secondBlock.second)
			swap(firstBlock, secondBlock);

		// Go straight from the y-coordinate of firstBlock to the y-coordinate of secondBlock
		for (int i = firstBlock.second; i <= secondBlock.second; i += 4) {
			// Skip if i is equal to the y-coordinate of firstBlock or secondBlock
			if (i == firstBlock.second || i == secondBlock.second)
				continue;

			// Check if the block at coordinate (firstBlock.x, i) has been deleted
			// If that block hasn't been deleted, we can't find a path between the two blocks
			// Return false
			if (board->getCheck(firstBlock.first, i) != _DELETE) {
				return false;
			}
		}
	}

	// We don't need to check anymore if one or both of the blocks were deleted before
	// Or if we just want to check that we can draw a line from firstBlock to secondBlock
	if (board->getCheck(firstBlock.first, firstBlock.second) == _DELETE ||
		board->getCheck(secondBlock.first, secondBlock.second) == _DELETE) {
		return true;
	}

	// Check if the two blocks have the same pokemon
	if (checkMatchedPokemons(firstBlock, secondBlock)) {
		// If we're not just checking, draw a line between the blocks, pause,
		// and then erase the line
		if (isChecking == false) {
			isPaused = true;
			board->drawLineI(firstBlock, secondBlock);
			Sleep(500);
			board->drawLineI(firstBlock, secondBlock, false);
			isPaused = false;
		}
		return true;
	}

	// If none of the above conditions are met, return false
	return false;
}
bool Game::checkLMatching(pii firstBlock, pii secondBlock, bool isChecking) {
	// This function checks if there is an L-shaped line connecting two blocks in a game.
	// The blocks are represented as pairs of integers (pii), and the function also takes
	// a boolean flag 'isChecking' to control whether it should draw and delete the line.

	// If the two blocks do not contain the same Pokémon, the function returns false.
	if (!checkMatchedPokemons(firstBlock, secondBlock))
		return false;

	pii Lcorner;  // This represents the corner of the L-shaped line.

	// This lambda function checks if the block at the Lcorner is deleted and if there are
	// I-shaped lines from the Lcorner to each block. If both conditions are met, it draws
	// and deletes the L-shaped line (if 'isChecking' is false), and returns 1.
	// Otherwise, it returns 0.
	auto isAvailable = [&]() {
		if (board->getCheck(Lcorner.first, Lcorner.second) == _DELETE) {
			if (checkIMatching(Lcorner, firstBlock, isChecking) &&
				checkIMatching(Lcorner, secondBlock, isChecking)) {
				if (isChecking == false) {
					isPaused = true;
					board->drawLineL(firstBlock, secondBlock, Lcorner);
					Sleep(500);
					board->drawLineL(firstBlock, secondBlock, Lcorner, false);
					isPaused = false;
				}
				return 1;
			}
		}
		return 0;
	};

	// If the x-coordinate of the first block is greater than that of the second block,
	// they are swapped.
	if (firstBlock.first > secondBlock.first)
		swap(firstBlock, secondBlock);

	// The function then checks for the availability of an L-shaped line in two cases:
	// Case 1: The Lcorner has the same x-coordinate as the first block and the same
	// y-coordinate as the second block.
	Lcorner.first = firstBlock.first;
	Lcorner.second = secondBlock.second;
	if (isAvailable())
		return 1;

	// Case 2: The Lcorner has the same x-coordinate as the second block and the same
	// y-coordinate as the first block.
	Lcorner.first = secondBlock.first;
	Lcorner.second = firstBlock.second;
	if (isAvailable())
		return 1;

	// If neither case is available, the function returns 0.
	return 0;
}

bool Game::checkZMatching(pii firstBlock, pii secondBlock, bool isChecking) {
	// This function checks if there is a Z-shaped line connecting two blocks in a game.
	// The blocks are represented as pairs of integers (pii), and the function also takes
	// a boolean flag 'isChecking' to control whether it should draw and delete the line.

	// If the two blocks do not contain the same Pokémon, the function returns false.
	if (!checkMatchedPokemons(firstBlock, secondBlock))
		return false;

	pii Zcorner1, Zcorner2;	 // These represent the corners of the Z-shaped line.

	// This lambda function checks if the blocks at the Zcorners are deleted and
	// if there are I-shaped lines from the first block to Zcorner1, from Zcorner1
	// to Zcorner2, and from Zcorner2 to the second block.
	// If all conditions are met, it draws and deletes the Z-shaped line (if 'isChecking'
	// is false), and returns 1. Otherwise, it returns 0.
	auto isAvailable = & {
		if (board->getCheck(Zcorner1.first, Zcorner1.second) == _DELETE &&
			board->getCheck(Zcorner2.first, Zcorner2.second) == _DELETE) {
			if (checkIMatching(firstBlock, Zcorner1, isChecking) &&
				checkIMatching(Zcorner1, Zcorner2, isChecking) &&
				checkIMatching(Zcorner2, secondBlock, isChecking)) {
				if (isChecking == false) {
					isPaused = true;
					board->drawLineZ(firstBlock, secondBlock, Zcorner1, Zcorner2);
					Sleep(500);
					board->drawLineZ(firstBlock, secondBlock, Zcorner1, Zcorner2, false);
					isPaused = false;
				}
				return 1;
			}
		}
		return 0;
	};

	// If the x-coordinate of the first block is greater than that of the second block,
	// they are swapped.
	if (firstBlock.first > secondBlock.first)
		swap(firstBlock, secondBlock);

	// The function then checks for the availability of a Z-shaped line in two cases:
	// Case 1: The Zcorners have the same y-coordinate as the first block and the second
	// block, and their x-coordinates are between those of the first block and the second block.
	for (int i = firstBlock.first + 8; i < secondBlock.first; i += 8) {
		Zcorner1.first = i, Zcorner1.second = firstBlock.second;
		Zcorner2.first = i, Zcorner2.second = secondBlock.second;
		if (isAvailable())
			return 1;
	}

	// If the y-coordinate of the first block is greater than that of the second block,
	// they are swapped.
	if (firstBlock.second > secondBlock.second)
		swap(firstBlock, secondBlock);

	// Case 2: The Zcorners have the same x-coordinate as the first block and the second
	// block, and their y-coordinates are between those of the first block and the second block.
	for (int i = firstBlock.second + 4; i < secondBlock.second; i += 4) {
		Zcorner1.first = firstBlock.first, Zcorner1.second = i;
		Zcorner2.first = secondBlock.first, Zcorner2.second = i;
		if (isAvailable())
			return 1;
	}

	// If neither case is available, the function returns 0.
	return 0;
}

bool Game::checkUMatching(pii firstBlock, pii secondBlock, bool isChecking) {
	// This function checks if there is a U-shaped line connecting two blocks in a game.
	// The blocks are represented as pairs of integers (pii), and the function also takes
	// a boolean flag 'isChecking' to control whether it should draw and delete the line.

	// If the two blocks do not contain the same Pokémon, the function returns false.
	if (!checkMatchedPokemons(firstBlock, secondBlock))
		return false;

	pii Ucorner1, Ucorner2;	 // These represent the corners of the U-shaped line.
	const int height = board->getHeight();
	const int width = board->getWidth();
	const int x = board->getXAt(0, 0);
	const int y = board->getYAt(0, 0);

	int left_x = min(firstBlock.first, secondBlock.first);
	int right_x = max(firstBlock.first, secondBlock.first);
	int top_y = min(firstBlock.second, secondBlock.second);
	int bottom_y = max(firstBlock.second, secondBlock.second);

	// This lambda function checks if the blocks at the Ucorners are deleted and if
	// there are I-shaped lines from the first block to Ucorner1, from Ucorner1 to Ucorner2,
	// and from Ucorner2 to the second block. If all conditions are met, it draws and deletes
	// the U-shaped line (if 'isChecking' is false), and returns 1. Otherwise, it returns 0.
	auto isAvailable = & {
		if (board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE &&
			board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE) {
			if (checkIMatching(firstBlock, Ucorner1, isChecking) &&
				checkIMatching(Ucorner1, Ucorner2, isChecking) &&
				checkIMatching(Ucorner2, secondBlock, isChecking)) {
				if (isChecking == false) {
					isPaused = true;
					board->drawLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
					Sleep(500);
					board->drawLineU(firstBlock, secondBlock, Ucorner1, Ucorner2, false);
					isPaused = false;
				}
				return 1;
			}
		}
		return 0;
	};

	// If the y-coordinate of the first block is greater than that of the second block,
	// they are swapped.
	if (firstBlock.second > secondBlock.second)
		swap(firstBlock, secondBlock);

	// The function then checks for the availability of a U-shaped line in four cases:
	// Case 1: The Ucorners have the same y-coordinate as the blocks, and their x-coordinates
	// are to the left of the blocks.
	for (int i = left_x - 8; i >= x - 8; i -= 8) {
		Ucorner1.first = i, Ucorner1.second = firstBlock.second;
		Ucorner2.first = i, Ucorner2.second = secondBlock.second;
		if (isAvailable())
			return 1;
	}

	// Case 2: The Ucorners have the same y-coordinate as the blocks, and their x-coordinates
	// are to the right of the blocks.
	for (int i = right_x + 8; i <= x + width * 8; i += 8) {
		Ucorner1.first = i, Ucorner1.second = firstBlock.second;
		Ucorner2.first = i, Ucorner2.second = secondBlock.second;
		if (isAvailable())
			return 1;
	}

	// If the x-coordinate of the first block is greater than that of the second block,
	// they are swapped.
	if (firstBlock.first > secondBlock.first)
		swap(firstBlock, secondBlock);

	// Case 3: The Ucorners have the same x-coordinate as the blocks, and their y-coordinates
	// are above the blocks.
	for (int i = top_y - 4; i >= y - 4; i -= 4) {
		Ucorner1.first = firstBlock.first, Ucorner1.second = i;
		Ucorner2.first = secondBlock.first, Ucorner2.second = i;
		if (isAvailable())
			return 1;
	}

	// Case 4: The Ucorners have the same x-coordinate as the blocks, and their y-coordinates
	// are below the blocks.
	for (int i = bottom_y + 4; i <= y + height * 4; i += 4) {
		Ucorner1.first = firstBlock.first, Ucorner1.second = i;
		Ucorner2.first = secondBlock.first, Ucorner2.second = i;
		if (isAvailable())
			return 1;
	}

	// If none of the cases are available, the function returns 0.
	return 0;
}

bool Game::checkMatching(pii firstBlock, pii secondBlock, bool isChecking) {
	// This function checks if there is a match between two blocks in a game.
	// The blocks are represented as pairs of integers (pii), and the function
	// also takes a boolean flag 'isChecking' to control whether it should draw
	// and delete the line.

	// If the two blocks are the same, the function returns false.
	if ((firstBlock.first == secondBlock.first) &&
		(firstBlock.second == secondBlock.second))
		return 0;

	// This lambda function prints the current score in a specific color.
	auto printScore = & {
		Controller::setConsoleColor(BRIGHT_WHITE, color);
		printf("\033[%d;%dH%d BTC", 18, 97, score);
		Controller::gotoXY(_x, _y);
	};

	// The function then checks for a match in four different shapes: I, L, Z, and U.
	// If a match is found, it draws and deletes the line (if 'isChecking' is false), updates
	// the score, and returns true. If no match is found, it shows a "Not Matched" notification,
	// decreases the player's points, resets the time streak and ratio, and returns false.
	if (checkIMatching(firstBlock, secondBlock, isChecking)) {
		if (isChecking == false) {
			Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
			printf("\033[%d;%dH I Matching. x%d", 17, 89, ratio);
			score += 1 * ratio;
			printScore(GREEN);
		}
		return 1;
	}
	if (checkLMatching(firstBlock, secondBlock, isChecking)) {
		if (isChecking == false) {
			Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
			printf("\033[%d;%dH L Matching. x%d", 17, 89, ratio);
			score += 2 * ratio;
			printScore(GREEN);
		}
		return 1;
	}
	if (checkZMatching(firstBlock, secondBlock, isChecking)) {
		if (isChecking == false) {
			Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
			printf("\033[%d;%dH Z Matching. x%d", 17, 89, ratio);
			score += 3 * ratio;
			printScore(GREEN);
		}
		return 1;
	}
	if (checkUMatching(firstBlock, secondBlock, isChecking)) {
		if (isChecking == false) {
			Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
			printf("\033[%d;%dH U Matching. x%d", 17, 89, ratio);
			score += 4 * ratio;
			printScore(GREEN);
		}
		return 1;
	}
	if (isChecking == false) {
		Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
		Controller::gotoXY(88, 16);
		cout << "Not Matched";
		score -= 2;
		printScore(RED);
		time_streak = 0;
		ratio = 1;
	}

	return 0;
}

void Game::deleteBlock() {
	// This function deletes a pair of matching blocks in a game.

	_lockedBlock = 0;  // Reset the locked block counter.

	bool isChecking = false;  // Initialize the checking flag.

	// If the locked blocks do not match, unselect them and return.
	if (!checkMatching(_lockedBlockPair[0], _lockedBlockPair[1], isChecking)) {
		for (auto block : _lockedBlockPair)
			board->unselectedBlock(block.first, block.second);	// Unselect the blocks.
		_lockedBlockPair.clear();			  // Clear the locked block pair.
		board->selectedBlock(_x, _y, GREEN);  // Select the current block.
		return;
	}

	// If the blocks match, mark them as deleted and remove them.
	for (auto block : _lockedBlockPair)
		board->deleteBlock(block.first, block.second);	// Delete the blocks.
	_lockedBlockPair.clear();							// Clear the locked block pair.
	board->selectedBlock(_x, _y, GREEN);				// Select the current block.

	// Decrease the total remaining blocks.
	_remainBlocks -= 2;

	// If there are two pairs in 5 seconds, double the score.
	if (time_streak == 0) {
		time_streak = 1;
	} else if (1 <= time_streak && time_streak < 5) {
		time_streak = 1;
		ratio = 2;
	} else {
		time_streak = 0;
		ratio = 1;
	}
}

bool Game::isAvailableBlock(bool isChecking) {
	// This function checks if there are any available matches left on the game board.
	// It takes a boolean flag 'isChecking' to control whether it should only check for
	// a match or also draw and delete the line.

	int height = board->getHeight();  // Get the height of the game board.
	int width = board->getWidth();	  // Get the width of the game board.
	pii firstBlock;	  // This represents the first block of a potential match.
	pii secondBlock;  // This represents the second block of a potential match.

	// The function uses a brute-force algorithm to check all pairs of blocks on the board.
	// If there are no available pairs, it returns false; otherwise, it returns true.
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			firstBlock.first = board->getXAt(i, j);
			firstBlock.second = board->getYAt(i, j);

			// Skip blocks marked as deleted.
			if (board->getCheck(firstBlock.first, firstBlock.second) == _DELETE) {
				continue;
			}

			for (int m = i; m < height; m++) {
				for (int n = 0; n < width; n++) {
					secondBlock.first = board->getXAt(m, n);
					secondBlock.second = board->getYAt(m, n);

					// Skip blocks marked as deleted.
					if (board->getCheck(secondBlock.first, secondBlock.second) == _DELETE)
						continue;

					// Check if the pair of blocks match.
					if (checkMatching(firstBlock, secondBlock, isChecking)) {
						return true;
					}
				}
			}
		}
	}

	// If no matches are found, the function returns false.
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
	// This flag is used to indicate whether the function is being
	// used to provide help or not
	bool isHelp = true;

	// Get the height and width of the board
	int height = board->getHeight();
	int width = board->getWidth();

	// Declare two pairs of integers to store the coordinates of the first
	// and second blocks
	pii firstBlock;
	pii secondBlock;

	// The function uses a brute-force algorithm to find any available pair of blocks
	// It iterates over the entire board, selects a pair of blocks, and checks
	// if they can be matched. If a match is found, it selects them to show a hint to
	// the player. Then it exits the function, showing only one pair to the player
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// Get the coordinates of the first block
			firstBlock.first = board->getXAt(i, j);
			firstBlock.second = board->getYAt(i, j);

			// Skip blocks marked as deleted
			if (board->getCheck(firstBlock.first, firstBlock.second) == _DELETE) {
				continue;
			}

			// Iterate over the rest of the board to find the second block
			for (int m = i; m < height; m++) {
				for (int n = 0; n < width; n++) {
					// Skip redundant pairs (same row and column)
					if (i == m && n <= j)
						continue;

					// Get the coordinates of the second block
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

							// Wait for 200 milliseconds
							Sleep(200);

							// Unselect the blocks
							board->unselectedBlock(firstBlock.first, firstBlock.second);
							board->unselectedBlock(secondBlock.first, secondBlock.second);

							// Deduct 2 points from the score
							score -= 2;

							// Set the console color to bright white text on a red background
							Controller::setConsoleColor(BRIGHT_WHITE, RED);

							// Update the score display
							if (score >= 0) {
								Controller::gotoXY(96, 17);
								cout << score << " BTC ";
							} else {
								Controller::gotoXY(96, 17);
								cout << score << " BTC";
							}

							// Exit the function
							return;
						}
					}
				}
			}
		}
	}
}
