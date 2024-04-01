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
				return 0;
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
void BoardView::showBoard(bool isResetBoard) {
	if (pBoard == nullptr)
		return;	 // If the board is not initialized, return immediately

	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	if (isResetBoard)
		Controller::clearConsole();	 // Clear the console if resetting bounds

	// Draw top line
	printCharAtXY(left + 1, top, TOP_LEFT_CORNER_BOUND);
	for (int i = 1; i < width * 8; i++) {
		if (isResetBoard)
			Sleep(5);  // Sleep for a short delay (optional)
		printCharAtXY(left + 1 + i, top, HORIZONTAL_LINE_BOUND);
	}
	printCharAtXY(left + 1 + width * 8, top, TOP_RIGHT_CORNER_BOUND);

	// Draw right line
	for (int i = 1; i < height * 4; i++) {
		if (isResetBoard)
			Sleep(10);	// Sleep for a short delay (optional)
		printCharAtXY(width * 8 + left + 1, i + top, VERTICAL_LINE_BOUND);
	}
	printCharAtXY(width * 8 + left + 1, height * 4 + top, BOTTOM_RIGHT_CORNER_BOUND);

	// Draw bottom line
	for (int i = 1; i < width * 8; i++) {
		if (isResetBoard)
			Sleep(5);  // Sleep for a short delay (optional)
		printCharAtXY(width * 8 + left - i + 1, height * 4 + top, HORIZONTAL_LINE_BOUND);
	}
	printCharAtXY(left + 1, height * 4 + top, BOTTOM_LEFT_CORNER_BOUND);

	// Draw left line
	for (int i = 1; i < height * 4; i++) {
		if (isResetBoard)
			Sleep(10);	// Sleep for a short delay (optional)
		printCharAtXY(left + 1, height * 4 + top - i, VERTICAL_LINE_BOUND);
	}

	if (isResetBoard) {
		// Draw vertical lines
		for (int i = 1; i < height * 4; i++) {
			for (int j = 8; j < width * 8; j += 8) {
				if (i % 4 != 0)
					printCharAtXY(j + left + 1, i + top, VERTICAL_LINE);
			}
			Sleep(10);	// Sleep for a short delay (optional)
		}

		// Draw horizontal lines
		for (int i = 1; i < width * 8; i++) {
			for (int j = 4; j < height * 4; j += 4) {
				if (i % 8 != 0)
					printCharAtXY(i + left + 1, j + top, HORIZONTAL_LINE);
			}
			Sleep(5);  // Sleep for a short delay (optional)
		}
	}
}
// Show the Pokémon character on the board
void BoardView::renderBoard(bool isResetBoard) {
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
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
void BoardView::buildBoardData(bool isResetBoard) {
	// Seed the random number generator
	srand((unsigned int)time(NULL));

	// Allocate memory for arrays to track duplicates and positions
	bool* checkDuplicate = new bool[height * width]{0};
	int* pos = new int[height * width];

	// Generate random Pokémon characters
	if (isResetBoard) {
		for (int j = 0; j < width; j++) {
			for (int i = 0; i < height; i += 2) {
				char c = rand() % 26 + 'A';
				pokemons[i][j] = c;
				pokemons[i + 1][j] = c;
			}
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
			if (isResetBoard) {
				pBoard[i][j].setPokemons(pokemons[r][c]);
				// Set the x and y coordinates for the current Point
				pBoard[i][j].setX(8 * j + left + 5);
				pBoard[i][j].setY(4 * i + top + 2);

				// Initialize the 'check' value for the current Point
				pBoard[i][j].setCheck(_NORMAL);
			} else {
				swap(pBoard[i][j]._check, pBoard[r][c]._check);
				swap(pBoard[i][j]._pokemon, pBoard[r][c]._pokemon);
			}
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
			printCharAtXY(j, i);  // Print a space character (clear the block)
		}
	}

	// Check if the block is not marked for deletion
	if (getCheck(x, y) != _DELETE) {
		printCharAtXY(x, y, getPokemons(x, y));	 // Print the Pokémon character
		Controller::gotoXY(x, y);				 // Move cursor back to (x, y)
	} else {
		// Restore the original background within the 7x3 block
		for (int i = y - 1; i <= y + 1; i++) {
			for (int j = x - 3; j <= x + 3; j++) {
				// Print the background character
				printCharAtXY(j, i, background[i - top][j - left]);
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
			printCharAtXY(j, i);  // Print a space character (clear the block)
		}
	}

	// Check if the block is not marked for deletion
	if (getCheck(x, y) != _DELETE) {
		printCharAtXY(x, y, getPokemons(x, y));	 // Print the Pokémon character
		Controller::gotoXY(x, y);				 // Move cursor back to (x, y)
	} else {
		// Restore the original background within the 7x3 block
		for (int i = y - 1; i <= y + 1; i++) {
			for (int j = x - 3; j <= x + 3; j++) {
				// Print the background character
				printCharAtXY(j, i, background[i - top][j - left]);
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
			printCharAtXY(j, i);  // Print a space character (clear the block)
		}
	}

	// Print the Pokémon character at the specified (x, y) position
	printCharAtXY(x, y, getPokemons(x, y));

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
			// Restore the background character
			printCharAtXY(j, i, background[i - top][j - left]);
		}
	}

	// Delete the top border if applicable
	if (y - 4 >= getYAt(0, 0) && getCheck(x, y - 4) == _DELETE) {
		for (int i = x - 3; i <= x + 3; i++) {
			// Restore the background character
			printCharAtXY(i, y - 2, background[y - 2 - top][i - left]);
		}
	}

	// Delete the bottom border if applicable
	if (y + 4 <= getYAt(height - 1, width - 1) && getCheck(x, y + 4) == _DELETE) {
		for (int i = x - 3; i <= x + 3; i++) {
			// Restore the background character
			printCharAtXY(i, y + 2, background[y + 2 - top][i - left]);
		}
	}

	// Delete the left border if applicable
	if (x - 8 >= getXAt(0, 0) && getCheck(x - 8, y) == _DELETE) {
		for (int i = y - 1; i <= y + 1; i++) {
			// Restore the background character
			printCharAtXY(x - 4, i, background[i - top][x - 4 - left]);
		}
	}

	// Delete the right border if applicable
	if (x + 8 <= getXAt(height - 1, width - 1) && getCheck(x + 8, y) == _DELETE) {
		for (int i = y - 1; i <= y + 1; i++) {
			// Restore the background character
			printCharAtXY(x + 4, i, background[i - top][x + 4 - left]);
		}
	}
}

/*=======================================================================*/
void BoardView::printChar(int x, int y, char c, bool isPrint) {
	if (isPrint) {
		printCharAtXY(x, y, c);	 // Character
	} else {
		int r = y - top, c = x - left;
		if (r <= 0 || c <= 0 || r >= height * 4 + 1 || c >= width * 8 + 1) {
			printCharAtXY(x, y);
		} else
			printCharAtXY(x, y, background[r][c]);
	}
}
// Draw or erase the I-line from firstBlock to secondBlock on the board
void BoardView::drawLineI(pii firstBlock, pii secondBlock, bool isDraw) {
	if (isDraw) {
		// Set the color for the line
		Controller::setConsoleColor(RED, BRIGHT_WHITE);
	} else {
		// To delete the line, we use the blank or space character ' '
		// Set the color to default
		Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	}
	// Draw the vertical I line
	if (firstBlock.first == secondBlock.first) {
		if (firstBlock.second > secondBlock.second)
			swap(firstBlock, secondBlock);

		// Draw a straight line from firstBlock to secondBlock
		for (int i = firstBlock.second + 2; i <= secondBlock.second - 2; i++) {
			printChar(firstBlock.first, i, VERTICAL_LINE, isDraw);
		}

		return;
	}

	// Draw the horizontal I line
	if (firstBlock.second == secondBlock.second) {
		if (firstBlock.first > secondBlock.first)
			swap(firstBlock, secondBlock);

		// Draw a straight line from firstBlock to secondBlock
		for (int i = firstBlock.first + 2; i <= secondBlock.first - 2; i++) {
			printChar(i, firstBlock.second, HORIZONTAL_LINE, isDraw);
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
		Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	}

	// Top-left corner
	if (Lcorner.first < secondBlock.first && Lcorner.second < firstBlock.second) {
		for (int i = firstBlock.second - 2; i >= Lcorner.second; i--) {
			printChar(firstBlock.first, i, VERTICAL_LINE, isDraw);
		}
		printChar(Lcorner.first, Lcorner.second, TOP_LEFT_CORNER, isDraw);
		for (int i = Lcorner.first + 1; i <= secondBlock.first - 2; i++) {
			printChar(i, secondBlock.second, HORIZONTAL_LINE, isDraw);
		}

		return;
	}

	// Top-right corner
	if (Lcorner.second < secondBlock.second && Lcorner.first > firstBlock.first) {
		for (int i = firstBlock.first + 2; i <= Lcorner.first; i++) {
			printChar(i, firstBlock.second, HORIZONTAL_LINE, isDraw);
		}
		printChar(Lcorner.first, Lcorner.second, TOP_RIGHT_CORNER, isDraw);
		for (int i = Lcorner.second + 1; i <= secondBlock.second - 2; i++) {
			printChar(secondBlock.first, i, VERTICAL_LINE, isDraw);
		}

		return;
	}

	// Bottom-left corner
	if (Lcorner.first < secondBlock.first && Lcorner.second > firstBlock.second) {
		for (int i = firstBlock.second + 2; i <= Lcorner.second; i++) {
			printChar(firstBlock.first, i, VERTICAL_LINE, isDraw);
		}
		printChar(Lcorner.first, Lcorner.second, BOTTOM_LEFT_CORNER, isDraw);
		for (int i = Lcorner.first + 1; i <= secondBlock.first - 2; i++) {
			printChar(i, secondBlock.second, HORIZONTAL_LINE, isDraw);
		}

		return;
	}

	// Bottom-right corner
	if (Lcorner.second > secondBlock.second && Lcorner.first > firstBlock.first) {
		for (int i = firstBlock.first + 2; i <= Lcorner.first; i++) {
			printChar(i, firstBlock.second, HORIZONTAL_LINE, isDraw);
		}
		printChar(Lcorner.first, Lcorner.second, BOTTOM_RIGHT_CORNER, isDraw);
		for (int i = Lcorner.second - 1; i >= secondBlock.second + 2; i--) {
			printChar(secondBlock.first, i, VERTICAL_LINE, isDraw);
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
		Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	}

	// Vertical Z
	if (firstBlock.first < secondBlock.first && firstBlock.first < Zcorner1.first) {
		for (int i = firstBlock.first + 2; i <= Zcorner1.first - 1; i++) {
			printChar(i, firstBlock.second, HORIZONTAL_LINE, isDraw);
		}

		if (Zcorner1.second < Zcorner2.second) {
			printChar(Zcorner1.first, Zcorner1.second, TOP_RIGHT_CORNER, isDraw);
			for (int i = Zcorner1.second + 1; i <= Zcorner2.second - 1; i++) {
				printChar(Zcorner1.first, i, VERTICAL_LINE, isDraw);
			}
			printChar(Zcorner2.first, Zcorner2.second, BOTTOM_LEFT_CORNER, isDraw);
		} else {
			printChar(Zcorner1.first, Zcorner1.second, BOTTOM_RIGHT_CORNER, isDraw);
			for (int i = Zcorner2.second + 1; i <= Zcorner1.second - 1; i++) {
				printChar(Zcorner1.first, i, VERTICAL_LINE, isDraw);
			}
			printChar(Zcorner2.first, Zcorner2.second, TOP_LEFT_CORNER, isDraw);
		}

		for (int i = Zcorner2.first + 1; i <= secondBlock.first - 2; i++) {
			printChar(i, secondBlock.second, HORIZONTAL_LINE, isDraw);
		}

		return;
	}

	// Horizontal Z
	if (firstBlock.second < secondBlock.second && firstBlock.second < Zcorner1.second) {
		for (int i = firstBlock.second + 2; i <= Zcorner1.second - 1; i++) {
			printChar(firstBlock.first, i, VERTICAL_LINE, isDraw);
		}

		if (Zcorner1.first < Zcorner2.first) {
			printChar(Zcorner1.first, Zcorner1.second, BOTTOM_LEFT_CORNER, isDraw);
			for (int i = Zcorner1.first + 1; i <= Zcorner2.first - 1; i++) {
				printChar(i, Zcorner1.second, HORIZONTAL_LINE, isDraw);
			}
			printChar(Zcorner2.first, Zcorner2.second, TOP_RIGHT_CORNER, isDraw);
		} else {
			printChar(Zcorner1.first, Zcorner1.second, BOTTOM_RIGHT_CORNER, isDraw);
			for (int i = Zcorner2.first + 1; i <= Zcorner1.first - 1; i++) {
				printChar(i, Zcorner1.second, HORIZONTAL_LINE, isDraw);
			}
			printChar(Zcorner2.first, Zcorner2.second, TOP_LEFT_CORNER, isDraw);
		}

		for (int i = Zcorner2.second + 1; i <= secondBlock.second - 2; i++) {
			printChar(secondBlock.first, i, VERTICAL_LINE, isDraw);
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
		Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	}

	int left_x = min(firstBlock.first, secondBlock.first);
	int right_x = max(firstBlock.first, secondBlock.first);
	int top_y = min(firstBlock.second, secondBlock.second);
	int bottom_y = max(firstBlock.second, secondBlock.second);

	// Left vertical U
	if (Ucorner1.first < left_x) {
		for (int i = Ucorner1.first; i <= firstBlock.first - 2; i++) {
			printChar(i, firstBlock.second, HORIZONTAL_LINE, isDraw);
		}
		printChar(Ucorner1.first, Ucorner1.second, TOP_LEFT_CORNER, isDraw);
		for (int i = top_y + 1; i <= bottom_y - 1; i++) {
			printChar(Ucorner1.first, i, VERTICAL_LINE, isDraw);
		}
		printChar(Ucorner2.first, Ucorner2.second, BOTTOM_LEFT_CORNER, isDraw);
		for (int i = Ucorner2.first + 1; i <= secondBlock.first - 2; i++) {
			printChar(i, secondBlock.second, HORIZONTAL_LINE, isDraw);
		}

		return;
	}

	// Right vertical U
	if (Ucorner1.first > right_x) {
		for (int i = Ucorner1.first; i >= firstBlock.first + 2; i--) {
			printChar(i, firstBlock.second, HORIZONTAL_LINE, isDraw);
		}
		printChar(Ucorner1.first, Ucorner1.second, TOP_RIGHT_CORNER, isDraw);
		for (int i = top_y + 1; i <= bottom_y - 1; i++) {
			printChar(Ucorner1.first, i, VERTICAL_LINE, isDraw);
		}
		printChar(Ucorner2.first, Ucorner2.second, BOTTOM_RIGHT_CORNER, isDraw);
		for (int i = Ucorner2.first - 1; i >= secondBlock.first + 2; i--) {
			printChar(i, secondBlock.second, HORIZONTAL_LINE, isDraw);
		}

		return;
	}

	// Top horizontal U
	if (Ucorner1.second < top_y) {
		for (int i = Ucorner1.second + 1; i <= firstBlock.second - 2; i++) {
			printChar(firstBlock.first, i, VERTICAL_LINE, isDraw);
		}
		printChar(Ucorner1.first, Ucorner1.second, TOP_LEFT_CORNER, isDraw);
		for (int i = left_x + 1; i <= right_x - 1; i++) {
			printChar(i, Ucorner1.second, HORIZONTAL_LINE, isDraw);
		}
		printChar(Ucorner2.first, Ucorner2.second, TOP_RIGHT_CORNER, isDraw);
		for (int i = Ucorner2.second + 1; i <= secondBlock.second - 2; i++) {
			printChar(secondBlock.first, i, VERTICAL_LINE, isDraw);
		}

		return;
	}

	// Bottom horizontal U
	if (Ucorner1.second > bottom_y) {
		for (int i = Ucorner1.second - 1; i >= firstBlock.second + 2; i--) {
			printChar(firstBlock.first, i, VERTICAL_LINE, isDraw);
		}
		printChar(Ucorner1.first, Ucorner1.second, BOTTOM_LEFT_CORNER, isDraw);
		for (int i = left_x + 1; i <= right_x - 1; i++) {
			printChar(i, Ucorner1.second, HORIZONTAL_LINE, isDraw);
		}
		printChar(Ucorner2.first, Ucorner2.second, BOTTOM_RIGHT_CORNER, isDraw);
		for (int i = Ucorner2.second - 1; i >= secondBlock.second + 2; i--) {
			printChar(secondBlock.first, i, VERTICAL_LINE, isDraw);
		}

		return;
	}
}
// Read file and create background for the board
void BoardView::createBackground() {
	ifstream bg;
	if (height == 4 && width == 5)
		bg.open("images/easy.txt");
	else if (height == 6 && width == 7)
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