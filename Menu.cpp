/*
	In the Menu.h and Menu.cpp files, there are two structures: Player, which holds user
	data, and Menu, responsible for managing the game’s user interface and the interaction
	between different menus.
	The idea and function in this file are obtained from Menu.cpp of Louis2602. 
	We have re-coded, fixed bugs, and added more comments for ease of reading.
	Reference: https://github.com/Louis2602/Pikachu-Game/blob/master/Pikachu/Menu.cpp
*/
#include "Menu.h"

int Menu::current_option;
const string Menu::options[8] = {"Play", "LeaderBoard", "Help", "Exit",
								 "Easy", "  Medium   ", "Hard", "Back"};

void Menu::mainScreen() {
	// Create a map to associate menu options with corresponding functions
	unordered_map<string, void (*)()> function_map = {
		{options[0], playMenu},	  {options[1], leaderBoard}, {options[2], helpScreen},
		{options[3], exitScreen}, {options[4], playEasy},	 {options[5], playMedium},
		{options[6], playHard},	  {options[7], goBack}};

	// Play background sound and display animation
	// Controller::playSound(BACKGROUND_SOUND);
	printAnimation();

	bool loadMenu = true;
	while (true) {
		if (loadMenu)
			mainMenu();

		switch (Controller::getConsoleInput()) {
			case 2:	 // Up
				changeOption(0, 1);
				loadMenu = false;
				break;
			case 5:	 // Down
				changeOption(1, 1);
				loadMenu = false;
				break;
			case 6:	 // Enter
				if (current_option == 0)
					loadMenu = false;
				else
					loadMenu = true;
				function_map[options[current_option]]();
				break;
			default:
				Controller::playSound(ERROR_SOUND);
				loadMenu = false;
		}
	}

	Controller::clearConsole();
}

void Menu::printLogo() {
	Controller::gotoXY(0, 0);
	unsigned char logo[] = R"(
                       ________  ___  ___  __    ________  ________  ___  ___  ___  ___
                      |\   __  \|\  \|\  \|\  \ |\   __  \|\   ____\|\  \|\  \|\  \|\  \
                      \ \  \|\  \ \  \ \  \/  /|\ \  \|\  \ \  \___|\ \  \\\  \ \  \\\  \
                       \ \   ____\ \  \ \   __\_ \ \   __  \ \  \    \ \   __  \ \  \\\  \
                        \ \  \___|\ \  \ \  \\ \  \ \  \ \  \ \  \____\ \  \ \  \ \  \\\  \
                         \ \__\    \ \__\ \__\\ \__\ \__\ \__\ \_______\ \__\ \__\ \_______\
                          \|__|     \|__|\|__| \|__|\|__|\|__|\|_______|\|__|\|__|\|_______|                                                                  
	)";
	cout << logo;
	Controller::setConsoleColor(BRIGHT_WHITE, GREEN);
	cout << R"(
           _____  _  _  ___   __  __    _  _____  ___  _  _  ___  _  _   ___    ___    _    __  __  ___ 
          |_   _|| || || __| |  \/  |  /_\|_   _|/ __|| || ||_ _|| \| | / __|  / __|  /_\  |  \/  || __|
            | |  | __ || _|  | |\/| | / _ \ | | | (__ | __ | | | | .` || (_ | | (_ | / _ \ | |\/| || _| 
            |_|  |_||_||___| |_|  |_|/_/ \_\|_|  \___||_||_||___||_|\_| \___|  \___|/_/ \_\|_|  |_||___|
	)";
}

void Menu::printOptionsBoard() {
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	int left = 49;
	int top = 16;

	// Draw the top border
	Controller::gotoXY(left, top);
	cout << TOP_LEFT_CORNER_BOUND;	// Top-left corner character
	for (int i = 1; i < 12; i++) {
		cout << HORIZONTAL_LINE_BOUND;	// Horizontal line character
	}
	cout << TOP_RIGHT_CORNER_BOUND;	 // Top-right corner character

	// Draw the middle rows
	for (int i = 1; i < 8; i++) {
		Controller::gotoXY(left, top + i);
		if (i % 2 != 0) {
			cout << VERTICAL_LINE_BOUND;  // Vertical line character
			Controller::gotoXY(left + 12, top + i);
			cout << VERTICAL_LINE_BOUND;  // Vertical line character
		} else {
			putchar(199);  // T-shaped intersection character
			for (int i = 1; i < 12; i++) {
				cout << HORIZONTAL_LINE;  // Horizontal line character
			}
			putchar(182);  // T-shaped intersection character
		}
	}

	// Draw the bottom border
	Controller::gotoXY(left, top + 8);
	cout << BOTTOM_LEFT_CORNER_BOUND;  // Bottom-left corner character
	for (int i = 1; i < 12; i++) {
		putchar(205);  // Horizontal line character
	}
	cout << BOTTOM_RIGHT_CORNER_BOUND;	// Bottom-right corner character
}

void Menu::printAnimation() {
	// Set console text color to bright white on a black background
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);

	// Clear the console screen
	Controller::clearConsole();

	// Initialize an array of color codes
	int colors[] = {LIGHT_AQUA, AQUA, LIGHT_BLUE, BLUE, LIGHT_PURPLE, PURPLE};

	// Loop 10 times
	int loop = 10;
	while (loop--) {
		// Set console text color to a random color from the array
		Controller::setConsoleColor(BRIGHT_WHITE, colors[getRandomInt(0, 5)]);

		// Call the printLogo() function
		printLogo();

		// Pause execution for 250 milliseconds
		Sleep(250);
	}
}

void Menu::changeOption(bool direction, bool flag) {
	int top = 17;

	// Check if moving up or down is allowed based on current option
	if ((direction == 0 && (current_option == 4 || current_option == 0)) ||
		(direction == 1 && (current_option == 3 || current_option == 7))) {
		Controller::playSound(ERROR_SOUND);
		return;
	}

	// Set console color and update display
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	Controller::gotoXY(55 - (int)options[current_option].length() / 2,
					   top + current_option % 4 * 2);
	cout << options[current_option];

	if (flag) {
		// Clear previous arrow indicators
		Controller::gotoXY(45, top + current_option % 4 * 2);
		putchar(32);  // Space character
		Controller::gotoXY(65, top + current_option % 4 * 2);
		putchar(32);  // Space character
	}

	// Update current option
	(direction == 1) ? current_option++ : current_option--;

	if (flag) {
		// Play sound, highlight selected option, and display arrow indicators
		Controller::playSound(MOVE_SOUND);
		Controller::setConsoleColor(BRIGHT_WHITE, RED);
		Controller::gotoXY(45, top + current_option % 4 * 2);
		putchar(175);  // Right arrow character
		Controller::gotoXY(55 - (int)options[current_option].length() / 2,
						   top + current_option % 4 * 2);
		cout << options[current_option];
		Controller::gotoXY(65, top + current_option % 4 * 2);
		putchar(174);  // Left arrow character
	}
}

void Menu::mainMenu() {
	Controller::setConsoleColor(BRIGHT_WHITE, RED);
	Controller::clearConsole();
	printLogo();

	printOptionsBoard();
	current_option = 3;
	changeOption(0, 0);
	changeOption(0, 0);
	changeOption(0, 1);
}

void Menu::playMenu() {
	current_option = 7;
	changeOption(0, 0);
	changeOption(0, 0);
	changeOption(0, 1);
}

void Menu::helpScreen() {
	Controller::showCursor(false);
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	Controller::clearConsole();
	int left = 13, top = 2, width = 85, height = 23;
	int line1 = 6, line2 = 20, line3 = 28, line4 = 15;
	printRectangle(left, top, width, height);
	Controller::gotoXY(left + 1, line1);
	for (int i = 0; i < width; i++) {
		putchar(196);
	}
	Controller::gotoXY(left + 1, line2);
	for (int i = 0; i < width; i++) {
		putchar(196);
	}
	Controller::gotoXY(left + 1, line4);
	for (int i = 0; i < width; i++) {
		putchar(196);
	}
	for (int i = 1; i < height; i++) {
		Controller::gotoXY(line3, top + i);
		putchar(179);
	}
	Controller::gotoXY(line3, line1);
	putchar(197);
	Controller::gotoXY(line3, line2);
	putchar(197);

	Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
	Controller::gotoXY(left + 3, top + 2);
	cout << "Moves:";
	Controller::gotoXY(left + 20, top + 1);
	putchar(249);
	cout << "Up:    W, up arrow";
	Controller::gotoXY(left + 52, top + 1);
	putchar(249);
	cout << "Down:  S, down arrow";
	Controller::gotoXY(left + 20, top + 3);
	putchar(249);
	cout << "Left:  A, left arrow";
	Controller::gotoXY(left + 52, top + 3);
	putchar(249);
	cout << "Right: D, right arrow";

	Controller::gotoXY(left + 3, top + 8);
	cout << "Rules:";
	Controller::gotoXY(left + 17, top + 5);
	int left1 = left + 17;
	putchar(249);
	cout << " The Matching Game (commonly known as the Pikachu Puzzle Game)";
	Controller::gotoXY(left1, top + 6);
	cout << " includes a board of multiple cells, each of which presents a "
			"figure.";
	Controller::gotoXY(left1, top + 8);
	putchar(249);
	cout << " The player finds and matches a pair of cells that contain the "
			"same";
	Controller::gotoXY(left1, top + 9);
	cout << " figure and connect each other in some particular pattern.";
	Controller::gotoXY(left1, top + 11);
	putchar(249);
	cout << " A legal match will make the two cells disappear. The game ends "
			"when";
	Controller::gotoXY(left1, top + 12);
	cout << " all matching pairs are found.";

	Controller::gotoXY(left + 3, top + 15);
	cout << "Scoring:";
	Controller::setConsoleColor(BRIGHT_WHITE, GREEN);
	Controller::gotoXY(left1 + 10, top + 14);
	putchar(249);
	cout << " I Matching: +1 BTC";
	Controller::setConsoleColor(BRIGHT_WHITE, GREEN);
	Controller::gotoXY(left1 + 40, top + 14);
	putchar(249);
	cout << " L Matching: +2 BTC";
	Controller::setConsoleColor(BRIGHT_WHITE, GREEN);
	Controller::gotoXY(left1 + 10, top + 15);
	putchar(249);
	cout << " Z Matching: +3 BTC";
	Controller::gotoXY(left1 + 40, top + 15);
	putchar(249);
	cout << " U Matching: +4 BTC";
	Controller::setConsoleColor(BRIGHT_WHITE, RED);
	Controller::gotoXY(left1 + 10, top + 16);
	putchar(249);
	cout << " Not Matched: -2 BTC";
	Controller::setConsoleColor(BRIGHT_WHITE, RED);
	Controller::gotoXY(left1 + 40, top + 16);
	putchar(249);
	cout << " Move suggestion: -2 BTC";
	Controller::setConsoleColor(BRIGHT_WHITE, GREEN);
	Controller::gotoXY(left1 + 10, top + 17);
	putchar(249);
	cout << " x2 the score if you match 2 pairs in 5 seconds.";

	Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
	Controller::gotoXY(left + 3, top + 20);
	cout << "Developers:";
	Controller::gotoXY(left + 31, top + 19);
	cout << "Dev 1: Le Hoang Lam (23127216)";
	Controller::gotoXY(left + 31, top + 21);
	cout << "Dev 2: Nguyen Le Thien Ly (23127223)";

	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	printRectangle(52, 27, 8, 2);
	Controller::setConsoleColor(BRIGHT_WHITE, RED);
	Controller::gotoXY(50, 28);
	putchar(175);  // Left arrow
	Controller::gotoXY(55, 28);
	cout << "Back";
	Controller::gotoXY(63, 28);
	putchar(174);  // Right arrow
	while (Controller::getConsoleInput() != 6) {
		Controller::playSound(ERROR_SOUND);
	}
}

void Menu::printRectangle(int left, int top, int width, int height) {
	// Print the top border
	Controller::gotoXY(left, top);
	cout << TOP_LEFT_CORNER_BOUND;	// Top-left corner
	for (int i = 0; i < width; i++)
		cout << HORIZONTAL_LINE_BOUND;	// Horizontal line
	cout << TOP_RIGHT_CORNER_BOUND;		// Top-right corner

	// Print the vertical sides
	int i = 0;
	for (; i < height; i++) {
		Controller::gotoXY(left, top + i + 1);
		cout << VERTICAL_LINE_BOUND;  // Vertical line on the left
		Controller::gotoXY(left + width + 1, top + i + 1);
		cout << VERTICAL_LINE_BOUND;  // Vertical line on the right
	}

	// Print the bottom border
	Controller::gotoXY(left, top + i);
	cout << BOTTOM_LEFT_CORNER_BOUND;  // Bottom-left corner
	for (i = 0; i < width; i++)
		cout << HORIZONTAL_LINE_BOUND;	// Horizontal line
	cout << BOTTOM_RIGHT_CORNER_BOUND;	// Bottom-right corner
}

void Menu::exitScreen() {
	// Hide the cursor
	Controller::showCursor(false);

	// Set console text color to bright white on a black background
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);

	// Clear the console screen
	Controller::clearConsole();

	// Draw two rectangles for the exit prompt
	Menu::printRectangle(38, 15, 35, 6);  // Main rectangle
	Menu::printRectangle(41, 18, 7, 2);	  // "Yes" button
	Menu::printRectangle(64, 18, 6, 2);	  // "No" button

	// Set console text color to bright white on a red background
	Controller::setConsoleColor(BRIGHT_WHITE, RED);

	// Print the logo
	printLogo();

	// Display the exit prompt message
	Controller::gotoXY(46, 16);
	cout << "Do you want to exit?";

	// Define button labels
	string str[2] = {"Yes", "No"};

	// Button positions and characters
	int left[] = {39, 44, 51, 62, 67, 72}, word[] = {32, 32, 175, 174};
	int color[] = {BLACK, RED};
	int top = 19;
	bool choice = 0;
	bool loop = 1;

	// Function to print button options
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

	// Initial button display
	print1();

	// Handle user input
	while (loop) {
		int key = Controller::getConsoleInput();
		if ((key == 3 && choice == 1) || (key == 4 && choice == 0)) {
			print1();  // Toggle button selection
		} else if (key == 6) {
			if (!choice) {
				// User chose "Yes" - exit the program
				Controller::setConsoleColor(BLACK, BRIGHT_WHITE);
				Controller::clearConsole();
				exit(0);
			}
			return;	 // User chose "No" - return without exiting
		} else {
			Controller::playSound(ERROR_SOUND);
		}
	}
}

int Menu::backHome() {
	// Hide the cursor
	Controller::showCursor(false);

	// Set console text color to bright white on a black background
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);

	// Clear the console screen
	Controller::clearConsole();

	// Draw two rectangles for the exit prompt
	Menu::printRectangle(38, 15, 35, 6);  // Main rectangle
	Menu::printRectangle(41, 18, 7, 2);	  // "Yes" button
	Menu::printRectangle(64, 18, 6, 2);	  // "No" button

	// Set console text color to bright white on a red background
	Controller::setConsoleColor(BRIGHT_WHITE, RED);

	// Print the logo
	printLogo();

	// Display the exit prompt message
	Controller::gotoXY(44, 16);
	cout << "Go back to the Home Screen?";

	// Define button labels
	string str[2] = {"Yes", "No"};

	// Button positions and characters
	int left[] = {39, 44, 51, 62, 67, 72}, word[] = {32, 32, 175, 174};
	int color[] = {BLACK, RED};
	int top = 19;
	bool choice = 0;
	bool loop = 1;

	// Function to print button options
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

	// Initial button display
	print1();

	// Handle user input
	while (loop) {
		int key = Controller::getConsoleInput();
		if ((key == 3 && choice == 1) || (key == 4 && choice == 0)) {
			print1();  // Toggle button selection
		} else if (key == 6) {
			if (!choice) {
				// User chose "Yes" - back to home screen
				return 1;
			}
			break;
		} else {
			Controller::playSound(ERROR_SOUND);
		}
	}
	// User chose "No" - return without exiting
	return 0;
}

void Menu::playEasy() {
	Game g(_EASY);
	g.setupGame();
	g.startGame();
}

void Menu::playMedium() {
	Game g(_MEDIUM);
	g.setupGame();
	g.startGame();
}

void Menu::playHard() {
	Game g(_HARD);
	g.setupGame();
	g.startGame();
}

void Menu::leaderBoard() {
	current_option = 0;
	Controller::setConsoleColor(BRIGHT_WHITE, RED);
	Controller::clearConsole();
	cout << R"(
		  _      ______          _____  ______ _____  ____   ____          _____  _____  
		 | |    |  ____|   /\   |  __ \|  ____|  __ \|  _ \ / __ \   /\   |  __ \|  __ \ 
		 | |    | |__     /  \  | |  | | |__  | |__) | |_) | |  | | /  \  | |__) | |  | |
		 | |    |  __|   / /\ \ | |  | |  __| |  _  /|  _ <| |  | |/ /\ \ |  _  /| |  | |
		 | |____| |____ / ____ \| |__| | |____| | \ \| |_) | |__| / ____ \| | \ \| |__| |
		 |______|______/_/    \_\_____/|______|_|  \_\____/ \____/_/    \_\_|  \_\_____/                                                                 
	)";
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	printRectangle(13, 8, 85, 17);

	auto printVerticalLine = [&](int pX, int pY, int size) {
		for (int i = 1; i < size; i++) {
			Controller::gotoXY(pX, pY + i);
			putchar(179);
		}
	};

	auto printHorizontalLine = [&](int pX, int pY, int size) {
		for (int i = pX; i < pX + size; i++) {
			Controller::gotoXY(i, pY);
			putchar(196);
		}
	};

	// Print column headers
	Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
	Controller::gotoXY(16, 9);
	cout << "No.";
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	printVerticalLine(20, 8, 17);
	printHorizontalLine(14, 10, 6);

	Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
	Controller::gotoXY(27, 9);
	cout << "Name";
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	printVerticalLine(38, 8, 17);
	printHorizontalLine(21, 10, 17);

	Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
	Controller::gotoXY(47, 9);
	cout << "ID";
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	printVerticalLine(58, 8, 17);
	printHorizontalLine(39, 10, 19);

	Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
	Controller::gotoXY(62, 9);
	cout << "Mode";
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	printVerticalLine(70, 8, 17);
	printHorizontalLine(59, 10, 11);

	Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
	Controller::gotoXY(72, 9);
	cout << "Time remaining";
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	printVerticalLine(88, 8, 17);
	printHorizontalLine(71, 10, 17);

	Controller::setConsoleColor(BRIGHT_WHITE, BLUE);
	Controller::gotoXY(91, 9);
	cout << "Score";
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	printHorizontalLine(89, 10, 10);

	// Initialize variables
	int y = 11;
	fstream fin;
	vector<Player> players;

	fin.open("leaderboard.txt", fstream::in);
	if (fin.is_open()) {
		// Read player data from file
		while (!fin.eof()) {
			Player player;
			getline(fin, player.playerName, ',');
			if (player.playerName == "")
				break;
			getline(fin, player.playerID, ',');
			getline(fin, player.mode, ',');
			string time;
			getline(fin, time, ',');
			player.time = stoi(time);
			fin >> player.score;
			players.push_back(player);
			fin.ignore();
		}
		fin.close();

		// Sort players by score (descending order)
		sort(players.rbegin(), players.rend());

		// Display leaderboard
		for (int i = 0; i < min((int)players.size(), 7); i++) {
			Controller::gotoXY(16, y);
			cout << i + 1;
			Controller::gotoXY(22, y);
			cout << players[i].playerName;
			Controller::gotoXY(41, y);
			cout << players[i].playerID;
			Controller::gotoXY(64 - (players[i].mode.size() / 2), y);
			cout << players[i].mode;
			Controller::gotoXY(77, y);
			cout << players[i].time;
			Controller::gotoXY(92, y);
			cout << players[i].score;
			y += 2;
		}
	}

	// Display "Back" button
	Controller::setConsoleColor(BRIGHT_WHITE, BLACK);
	printRectangle(52, 27, 8, 2);
	Controller::setConsoleColor(BRIGHT_WHITE, RED);
	Controller::gotoXY(50, 28);
	putchar(175);  // Left arrow
	Controller::gotoXY(55, 28);
	cout << "Back";
	Controller::gotoXY(63, 28);
	putchar(174);  // Right arrow

	// Wait for user input (exit when user presses Enter)
	while (Controller::getConsoleInput() != 6) {
		Controller::playSound(ERROR_SOUND);
	}
}
