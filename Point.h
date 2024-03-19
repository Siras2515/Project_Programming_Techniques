/*
    This file contains the declaration of structure Point.
    Through the structure, we control every cell on the board.
    Reference: https://github.com/Louis2602/Pikachu-Game/blob/master/Pikachu/Point.h
*/
#pragma once

#define _NORMAL 0
#define _LOCK 1
#define _DELETE -1

struct Point {
	int _x = 0;		 //Coordinate x
	int _y = 0;		 //Coordinate y
	int _check = 0;	 //1: LOCK, 0: NORMAL, -1: DELETE
	char _pokemon = ' ';

	Point();					 //Constructor
	Point(int, int, int, char);	 //Constructor

	int getX();			 //Get coordinate x
	int getY();			 //Get coordinate y
	int getCheck();		 //Get the cell's status
	void setX(int);		 //Set coordinate x
	void setY(int);		 //Set coordinate y
	void setCheck(int);	 //Set the cell's status

	char getPokemons();
	void setPokemons(char);
};