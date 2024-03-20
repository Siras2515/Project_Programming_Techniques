/*
    ???
    Reference: https://github.com/Louis2602/Pikachu-Game/blob/master/Pikachu/Menu.h
*/
#pragma once
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include "Controller.h"
#include "Game.h"

#define _EASY 4
#define _MEDIUM 6

using namespace std;

struct Player {
	string playerName;
	string playerID;
	string className;
	int score;
	string mode;
};

struct Menu {
	static bool exitGame;			  //Exit game flag
	static int current_option;		  //Current option
	const static string options[8];	  //Name of options
	static void mainScreen();		  //Main screen or Introduction screen
	static void mainMenu();			  //Menu
	static void playMenu();			  //Menu play
	static void leaderBoard();		  //LeaderBoard screen
	static void helpScreen();		  //Tutorial Screen
	static void exitScreen();		  //Exit Screen
	static void playEasy();			  //Difficulty: Easy
	static void playMedium();		  //Difficulty: Medium
	static void printLogo();		  //Print Logo of game
	static void goBack() {}			  //Go back to menu screen
	static void printOptionsBoard();  //Print options board
	static void printAnimation();	  //Print the introduction animation
	static void printRectangle(int, int, int, int);	 //Draw rectangle
	static void changeOption(bool, bool);			 //Change option
};