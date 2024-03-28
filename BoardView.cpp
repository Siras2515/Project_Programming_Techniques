/*
    This file defines all function that declared in "BoardView.h"
    Reference: https://github.com/Louis2602/Pikachu-Game/blob/master/Pikachu/BoardView.cpp
*/
#include "BoardView.h"

BoardView::BoardView(int _height, int _width, int pX, int pY)
	: height(_height), width(_width), left(pX), top(pY) {
	// Allocate memory for the 'pokemons' 2D array
	pokemons = new int*[height];
	for (int i = 0; i < height; i++)
		pokemons[i] = new int[width];

	// Allocate memory for the 'pBoard' 2D array
	pBoard = new Point*[height];
	for (int i = 0; i < height; i++)
		pBoard[i] = new Point[width];

	// Allocate memory for the 'background' array
	background = new string[height * 4 + 1];
}

BoardView::~BoardView() {
	// Free memory for 'pBoard'
	for (int i = 0; i < height; i++)
		delete[] pBoard[i];
	delete[] pBoard;
	pBoard = nullptr;

	// Free memory for 'background'
	delete[] background;
	background = nullptr;
}
// Get height of the board
int BoardView::getHeight() {
	return height;
}
// Get width of the board
int BoardView::getWidth() {
	return width;
}
// Get the coordinate of the left side of the board
int BoardView::getLeft() {
	return left;
}
// Get the coordinate of the top side of the board
int BoardView::getTop() {
	return top;
}
// Get the x-coordinate of a cell
int BoardView::getXAt(int i, int j) {
	return pBoard[i][j].getX();
}
// Get the y-coordinate of a cell
int BoardView::getYAt(int i, int j) {
	return pBoard[i][j].getY();
}
// Get the row at coordinate (x, y)
int BoardView::getRAt(int x, int y) {
	return (y - 2 - top) / 4;
}
// Get the column at coordinate (x, y)
int BoardView::getCAt(int x, int y) {
	return (x - 5 - left) / 8;
}
// Get the kind of Pokémon at coordinate (x, y)
char BoardView::getPokemons(int x, int y) {
	return pBoard[getRAt(x, y)][getCAt(x, y)].getPokemons();
}
// Get the status of a cell at coordinate (x, y)
// And check coordinate (x, y) is not out of bound
int BoardView::getCheckAtXY(int pX, int pY) {
	// Iterate through the entire board
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// Check if the current Point's coordinates match the provided (pX, pY)
			if (pBoard[i][j].getX() == pX && pBoard[i][j].getY() == pY)
				// Return the 'check' value for this Point
				return pBoard[i][j].getCheck();
		}
	}
	// If no matching Point is found, throw an exception
	throw "Problem with cursor position";
}
// Get the status of a cell at coordinate (x, y)
int BoardView::getCheck(int x, int y) {
	// Calculate row and column indices based on (x, y) coordinates
	int r = getRAt(x, y);
	int c = getCAt(x, y);

	// Check if the indices are within valid bounds
	if (r < 0 || c < 0 || r >= height || c >= width)
		return _DELETE;	 // Return a special value indicating deletion

	// Otherwise, return the 'check' value for the specified Point
	return pBoard[r][c].getCheck();
}

/*=======================================================================*/
// Show the board game on the screen and reset the bound liner
void BoardView::showBoard(bool isResetBound) {
	if (pBoard == nullptr)
		return;	 // If the board is not initialized, return immediately

	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	if (!isResetBound)
		Controller::clearConsole();	 // Clear the console if not resetting bounds

	// Draw top line
	Controller::gotoXY(left + 1, top);
	putchar(201);  // Top-left corner character
	for (int i = 1; i < width * 8; i++) {
		if (!isResetBound)
			Sleep(5);  // Sleep for a short delay (optional)
		putchar(205);  // Horizontal line character
	}
	putchar(187);  // Top-right corner character

	// Draw right line
	for (int i = 1; i < height * 4; i++) {
		if (!isResetBound)
			Sleep(10);	// Sleep for a short delay (optional)
		Controller::gotoXY(width * 8 + left + 1, i + top);
		putchar(186);  // Vertical line character
	}
	Controller::gotoXY(width * 8 + left + 1, height * 4 + top);
	putchar(188);  // Bottom-right corner character

	// Draw bottom line
	for (int i = 1; i < width * 8; i++) {
		Controller::gotoXY(width * 8 + left - i + 1, height * 4 + top);
		if (!isResetBound)
			Sleep(5);  // Sleep for a short delay (optional)
		putchar(205);  // Horizontal line character
	}
	Controller::gotoXY(left + 1, height * 4 + top);
	putchar(200);  // Bottom-left corner character

	// Draw left line
	for (int i = 1; i < height * 4; i++) {
		if (!isResetBound)
			Sleep(10);	// Sleep for a short delay (optional)
		Controller::gotoXY(left + 1, height * 4 + top - i);
		putchar(186);  // Vertical line character
	}

	if (!isResetBound) {
		// Draw vertical lines
		for (int i = 1; i < height * 4; i++) {
			for (int j = 8; j < width * 8; j += 8) {
				if (i % 4 != 0) {
					Controller::gotoXY(j + left + 1, i + top);
					putchar(179);  // Vertical line character
				}
			}
			Sleep(10);	// Sleep for a short delay (optional)
		}

		// Draw horizontal lines
		for (int i = 1; i < width * 8; i++) {
			for (int j = 4; j < height * 4; j += 4) {
				if (i % 8 != 0) {
					Controller::gotoXY(i + left + 1, j + top);
					putchar(196);  // Horizontal line character
				}
			}
			Sleep(5);  // Sleep for a short delay (optional)
		}
	}
}

// Show the Pokémon character on the board
void BoardView::renderBoard(bool isResetBoard) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (isResetBoard) {
				// Set the x and y coordinates for the current Point
				pBoard[i][j].setX(8 * j + left + 5);
				pBoard[i][j].setY(4 * i + top + 2);

				// Initialize the 'check' value for the current Point
				pBoard[i][j].setCheck(_NORMAL);
			}
			// Get the calculated x and y values
			int r = pBoard[i][j].getX();
			int c = pBoard[i][j].getY();

			// Move the cursor to the specified position and display the Pokémon character
			Controller::gotoXY(r, c);
			if (pBoard[i][j].getCheck() == _NORMAL)
				putchar(pBoard[i][j].getPokemons());
			else if (pBoard[i][j].getCheck() == _DELETE) {
				deleteBlock(r, c);
			}
		}
	}
}
// Construct a game board that can play
void BoardView::buildBoardData() {
	// Seed the random number generator
	srand((unsigned int)time(NULL));

	// Allocate memory for arrays to track duplicates and positions
	bool* checkDuplicate = new bool[height * width]{0};
	int* pos = new int[height * width];

	// Generate random Pokémon characters
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j += 2) {
			char c = rand() % 26 + 'A';
			pokemons[i][j] = c;
			pokemons[i][j + 1] = c;
		}
	}

	// Randomize Pokémon positions
	for (int i = 0; i < height * width; i++) {
		int tmp = 0;
		do {
			tmp = rand() % (height * width);
		} while (checkDuplicate[tmp]);
		checkDuplicate[tmp] = 1;
		pos[i] = tmp;
	}

	// Construct the Pokémon matrix
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int r = pos[width * i + j] / width;
			int c = pos[width * i + j] % width;
			pBoard[i][j].setPokemons(pokemons[r][c]);
		}
	}

	// Clean up dynamically allocated memory
	delete[] pos;
	delete[] checkDuplicate;
}
// Select a block on the board with a color
void BoardView::selectedBlock(int x, int y, int color) {
	Controller::setConsoleColor(color, BLACK);	// Set the console text color

	// Clear a 7x3 block around the specified (x, y) position
	for (int i = y - 1; i <= y + 1; i++) {
		for (int j = x - 3; j <= x + 3; j++) {
			Controller::gotoXY(j, i);  // Move cursor to (j, i)
			putchar(32);			   // Print a space character (clear the block)
		}
	}

	// Check if the block is not marked for deletion
	if (getCheck(x, y) != _DELETE) {
		Controller::gotoXY(x, y);	 // Move cursor back to (x, y)
		putchar(getPokemons(x, y));	 // Print the Pokémon character
		Controller::gotoXY(x, y);	 // Move cursor back to (x, y)
	} else {
		// Restore the original background within the 7x3 block
		for (int i = y - 1; i <= y + 1; i++) {
			for (int j = x - 3; j <= x + 3; j++) {
				Controller::gotoXY(j, i);  // Move cursor to (j, i)
				// Print the background character
				putchar(background[i - top][j - left]);
			}
		}
	}
}
// Unselect a block on the board
void BoardView::unselectedBlock(int x, int y) {
	int r = getRAt(x, y);
	int c = getCAt(x, y);

	// If the block is not marked for deletion, set its check status to normal
	if (getCheck(x, y) != _DELETE)
		pBoard[r][c].setCheck(_NORMAL);

	// Set the console text color to black on bright white background
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);

	// Clear a 7x3 block around the specified (x, y) position
	for (int i = y - 1; i <= y + 1; i++) {
		for (int j = x - 3; j <= x + 3; j++) {
			Controller::gotoXY(j, i);  // Move cursor to (j, i)
			putchar(32);			   // Print a space character (clear the block)
		}
	}

	// Check if the block is not marked for deletion
	if (getCheck(x, y) != _DELETE) {
		Controller::gotoXY(x, y);	 // Move cursor back to (x, y)
		putchar(getPokemons(x, y));	 // Print the Pokémon character
		Controller::gotoXY(x, y);	 // Move cursor back to (x, y)
	} else {
		// Restore the original background within the 7x3 block
		for (int i = y - 1; i <= y + 1; i++) {
			for (int j = x - 3; j <= x + 3; j++) {
				Controller::gotoXY(j, i);  // Move cursor to (j, i)
				// Print the background character
				putchar(background[i - top][j - left]);
			}
		}
	}
}
// Lock a block that has chosen on the board
void BoardView::lockBlock(int x, int y) {
	int r = getRAt(x, y);
	int c = getCAt(x, y);

	// Set the check status of the block to _LOCK
	pBoard[r][c].setCheck(_LOCK);

	// Set the console text color to black on a red background
	Controller::setConsoleColor(RED, BLACK);

	// Clear a 7x3 block around the specified (x, y) position
	for (int i = y - 1; i <= y + 1; i++) {
		for (int j = x - 3; j <= x + 3; j++) {
			Controller::gotoXY(j, i);  // Move cursor to (j, i)
			putchar(32);			   // Print a space character (clear the block)
		}
	}

	// Print the Pokémon character at the specified (x, y) position
	Controller::gotoXY(x, y);
	putchar(getPokemons(x, y));

	// Move the cursor back to the specified (x, y) position
	Controller::gotoXY(x, y);
}
// Delete a block on the board
void BoardView::deleteBlock(int x, int y) {
	int r = getRAt(x, y);
	int c = getCAt(x, y);

	// Set the check status of the block to _DELETE
	pBoard[r][c].setCheck(_DELETE);

	// Set the console text color to black on a bright white background
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);

	// Clear a 7x3 block around the specified (x, y) position
	for (int i = y - 1; i <= y + 1; i++) {
		for (int j = x - 3; j <= x + 3; j++) {
			Controller::gotoXY(j, i);				 // Move cursor to (j, i)
			putchar(background[i - top][j - left]);	 // Restore the background character
		}
	}

	// Delete the top border if applicable
	if (y - 4 >= getYAt(0, 0) && getCheck(x, y - 4) == _DELETE) {
		for (int i = x - 3; i <= x + 3; i++) {
			Controller::gotoXY(i, y - 2);  // Move cursor to (i, y - 2)
			// Restore the background character
			putchar(background[y - 2 - top][i - left]);
		}
	}

	// Delete the bottom border if applicable
	if (y + 4 <= getYAt(height - 1, width - 1) && getCheck(x, y + 4) == _DELETE) {
		for (int i = x - 3; i <= x + 3; i++) {
			Controller::gotoXY(i, y + 2);  // Move cursor to (i, y + 2)
			// Restore the background character
			putchar(background[y + 2 - top][i - left]);
		}
	}

	// Delete the left border if applicable
	if (x - 8 >= getXAt(0, 0) && getCheck(x - 8, y) == _DELETE) {
		for (int i = y - 1; i <= y + 1; i++) {
			Controller::gotoXY(x - 4, i);  // Move cursor to (x - 4, i)
			// Restore the background character
			putchar(background[i - top][x - 4 - left]);
		}
	}

	// Delete the right border if applicable
	if (x + 8 <= getXAt(height - 1, width - 1) && getCheck(x + 8, y) == _DELETE) {
		for (int i = y - 1; i <= y + 1; i++) {
			Controller::gotoXY(x + 4, i);  // Move cursor to (x + 4, i)
			// Restore the background character
			putchar(background[i - top][x + 4 - left]);
		}
	}
}

/*=======================================================================*/
void printChar(int ch, bool isPrint) {
	if (isPrint)
		putchar(ch);  // Character
	else
		putchar(32);  // Print the blank character or space character ' '
}
// Draw or erase the I-line from firstBlock to secondBlock on the board
void BoardView::drawLineI(pii firstBlock, pii secondBlock, bool isDraw) {
	if (isDraw) {
		// Set the color for the line
		Controller::setConsoleColor(RED, BRIGHT_WHITE);
	} else {
		// To delete the line, we use the blank or space character ' '
		// Set the color to default
		Controller::setConsoleColor(BRIGHT_WHITE, BRIGHT_WHITE);
	}
	// Draw the vertical I line
	if (firstBlock.first == secondBlock.first) {
		if (firstBlock.second > secondBlock.second)
			swap(firstBlock, secondBlock);

		// Draw a straight line from firstBlock to secondBlock
		for (int i = firstBlock.second + 2; i <= secondBlock.second - 2; i++) {
			Controller::gotoXY(firstBlock.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}

		return;
	}

	// Draw the horizontal I line
	if (firstBlock.second == secondBlock.second) {
		if (firstBlock.first > secondBlock.first)
			swap(firstBlock, secondBlock);

		// Draw a straight line from firstBlock to secondBlock
		for (int i = firstBlock.first + 2; i <= secondBlock.first - 2; i++) {
			Controller::gotoXY(i, firstBlock.second);
			printChar(196, isDraw);	 // Horizontal line character
		}

		return;
	}
}
// Draw or erase the L-line from firstBlock to secondBlock on the board
void BoardView::drawLineL(pii firstBlock, pii secondBlock, pii Lcorner, bool isDraw) {
	if (isDraw) {
		// Set the color for the line
		Controller::setConsoleColor(RED, BRIGHT_WHITE);
	} else {
		// To delete the line, we use the blank or space character ' '
		// Set the color to default
		Controller::setConsoleColor(BRIGHT_WHITE, BRIGHT_WHITE);
	}

	// Top-left corner
	if (Lcorner.first < secondBlock.first && Lcorner.second < firstBlock.second) {
		for (int i = firstBlock.second - 2; i >= Lcorner.second + 1; i--) {
			Controller::gotoXY(firstBlock.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}
		for (int i = Lcorner.first; i <= secondBlock.first - 2; i++) {
			Controller::gotoXY(i, secondBlock.second);
			printChar(196, isDraw);	 // Horizontal line character
		}

		return;
	}

	// Top-right corner
	if (Lcorner.second < secondBlock.second && Lcorner.first > firstBlock.first) {
		for (int i = firstBlock.first + 2; i <= Lcorner.first; i++) {
			Controller::gotoXY(i, firstBlock.second);
			printChar(196, isDraw);	 // Horizontal line character
		}
		for (int i = Lcorner.second + 1; i <= secondBlock.second - 2; i++) {
			Controller::gotoXY(secondBlock.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}

		return;
	}

	// Bottom-left corner
	if (Lcorner.first < secondBlock.first && Lcorner.second > firstBlock.second) {
		for (int i = firstBlock.second + 2; i <= Lcorner.second - 1; i++) {
			Controller::gotoXY(firstBlock.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}
		for (int i = Lcorner.first; i <= secondBlock.first - 2; i++) {
			Controller::gotoXY(i, secondBlock.second);
			printChar(196, isDraw);	 // Horizontal line character
		}

		return;
	}

	// Bottom-right corner
	if (Lcorner.second > secondBlock.second && Lcorner.first > firstBlock.first) {
		for (int i = firstBlock.first + 2; i <= Lcorner.first; i++) {
			Controller::gotoXY(i, firstBlock.second);
			printChar(196, isDraw);	 // Horizontal line character
		}
		for (int i = Lcorner.second - 1; i >= secondBlock.second + 2; i--) {
			Controller::gotoXY(secondBlock.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}

		return;
	}
}
// Draw or erase the Z-line from firstBlock to secondBlock on the board
void BoardView::drawLineZ(pii firstBlock, pii secondBlock, pii Zcorner1, pii Zcorner2,
						  bool isDraw) {
	if (isDraw) {
		// Set the color for the line
		Controller::setConsoleColor(RED, BRIGHT_WHITE);
	} else {
		// To delete the line, we use the blank or space character ' '
		// Set the color to default
		Controller::setConsoleColor(BRIGHT_WHITE, BRIGHT_WHITE);
	}

	// Top-left corner
	if (Zcorner1.first > secondBlock.first && Zcorner1.second > firstBlock.second) {
		for (int i = firstBlock.second + 2; i <= Zcorner1.second - 1; i++) {
			Controller::gotoXY(firstBlock.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}
		for (int i = Zcorner1.first; i >= Zcorner2.first; i--) {
			Controller::gotoXY(i, Zcorner1.second);
			printChar(196, isDraw);	 // Horizontal line character
		}
		for (int i = Zcorner2.second + 1; i <= secondBlock.second - 2; i++) {
			Controller::gotoXY(secondBlock.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}

		return;
	}

	// Top-right corner
	if (Zcorner1.second < secondBlock.second && Zcorner1.first > firstBlock.first) {
		for (int i = firstBlock.first + 2; i <= Zcorner1.first; i++) {
			Controller::gotoXY(i, firstBlock.second);
			printChar(196, isDraw);	 // Horizontal line character
		}
		// Move the cursor to the position of secondBlock center
		for (int i = Zcorner1.second + 1; i <= Zcorner2.second - 1; i++) {
			Controller::gotoXY(Zcorner1.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}
		for (int i = Zcorner2.first; i <= secondBlock.first - 2; i++) {
			Controller::gotoXY(i, secondBlock.second);
			printChar(196, isDraw);	 // Horizontal line character
		}

		return;
	}

	// Bottom-left corner
	if (Zcorner1.first < secondBlock.first && Zcorner1.second > firstBlock.second) {
		for (int i = firstBlock.second + 2; i <= Zcorner1.second - 1; i++) {
			Controller::gotoXY(firstBlock.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}
		// Move the cursor to the position of secondBlock center
		for (int i = Zcorner1.first; i <= Zcorner2.first; i++) {
			Controller::gotoXY(i, Zcorner1.second);
			printChar(196, isDraw);	 // Horizontal line character
		}
		for (int i = Zcorner2.second + 1; i <= secondBlock.second - 2; i++) {
			Controller::gotoXY(secondBlock.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}

		return;
	}

	// Bottom-right corner
	if (Zcorner1.second > secondBlock.second && Zcorner1.first > firstBlock.first) {
		for (int i = firstBlock.first + 2; i <= Zcorner1.first; i++) {
			Controller::gotoXY(i, firstBlock.second);
			printChar(196, isDraw);	 // Horizontal line character
		}
		// Move the cursor to the position of secondBlock center
		for (int i = Zcorner1.second - 1; i >= Zcorner2.second + 1; i--) {
			Controller::gotoXY(Zcorner1.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}
		for (int i = Zcorner2.first; i <= secondBlock.first - 2; i++) {
			Controller::gotoXY(i, secondBlock.second);
			printChar(196, isDraw);	 // Horizontal line character
		}

		return;
	}
}
// Draw or erase the U-line from firstBlock to secondBlock on the board
void BoardView::drawLineU(pii firstBlock, pii secondBlock, pii Ucorner1, pii Ucorner2,
						  bool isDraw) {
	if (isDraw) {
		// Set the color for the line
		Controller::setConsoleColor(RED, BRIGHT_WHITE);
	} else {
		// To delete the line, we use the blank or space character ' '
		// Set the color to default
		Controller::setConsoleColor(BRIGHT_WHITE, BRIGHT_WHITE);
	}

	int left_x = min(firstBlock.first, secondBlock.first);
	int right_x = max(firstBlock.first, secondBlock.first);
	int top_y = min(firstBlock.second, secondBlock.second);
	int bottom_y = max(firstBlock.second, secondBlock.second);

	// Left vertical U
	if (Ucorner1.first < left_x) {
		for (int i = Ucorner1.first; i <= firstBlock.first - 2; i++) {
			Controller::gotoXY(i, firstBlock.second);
			printChar(196, isDraw);	 // Horizontal line character
		}
		for (int i = top_y + 1; i <= bottom_y - 1; i++) {
			Controller::gotoXY(Ucorner1.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}
		for (int i = Ucorner2.first; i <= secondBlock.first - 2; i++) {
			Controller::gotoXY(i, secondBlock.second);
			printChar(196, isDraw);	 // Horizontal line character
		}

		return;
	}

	// Right vertical U
	if (Ucorner1.first > right_x) {
		for (int i = Ucorner1.first; i >= firstBlock.first + 2; i--) {
			Controller::gotoXY(i, firstBlock.second);
			printChar(196, isDraw);	 // Horizontal line character
		}
		for (int i = top_y + 1; i <= bottom_y - 1; i++) {
			Controller::gotoXY(Ucorner1.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}
		for (int i = Ucorner2.first; i >= secondBlock.first + 2; i--) {
			Controller::gotoXY(i, secondBlock.second);
			printChar(196, isDraw);	 // Horizontal line character
		}

		return;
	}

	// Top horizontal U
	if (Ucorner1.second < top_y) {
		for (int i = Ucorner1.second + 1; i <= firstBlock.second - 2; i++) {
			Controller::gotoXY(firstBlock.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}
		for (int i = left_x; i <= right_x; i++) {
			Controller::gotoXY(i, Ucorner1.second);
			printChar(196, isDraw);	 // Horizontal line character
		}
		for (int i = Ucorner2.second + 1; i <= secondBlock.second - 2; i++) {
			Controller::gotoXY(secondBlock.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}

		return;
	}

	// Bottom horizontal U
	if (Ucorner1.second > bottom_y) {
		for (int i = Ucorner1.second - 1; i >= firstBlock.second + 2; i--) {
			Controller::gotoXY(firstBlock.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}
		for (int i = left_x; i <= right_x; i++) {
			Controller::gotoXY(i, Ucorner1.second);
			printChar(196, isDraw);	 // Horizontal line character
		}
		for (int i = Ucorner2.second - 1; i >= secondBlock.second + 2; i--) {
			Controller::gotoXY(secondBlock.first, i);
			printChar(179, isDraw);	 // Vertical line character
		}

		return;
	}
}
// Read file and create background for the board
void BoardView::createBackground() {
	ifstream bg;
	if (height == 4 && width == 4)
		bg.open("images/easy.txt");
	else if (height == 6 && width == 6)
		bg.open("images/medium.txt");
	else
		bg.open("images/hard.txt");
	int i = 0;
	while (!bg.eof()) {
		getline(bg, background[i]);
		i++;
	}
	bg.close();
}