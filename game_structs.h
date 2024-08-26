#include "color.h"

#ifndef GAME_STRUCTS_H
#define GAME_STRUCTS_H


typedef struct{
    /*
        this structure represents a location or size (x/y coordinates)
    */
    unsigned int x;
    unsigned int y;
} Coords;


typedef struct{
    /*
        this structure represents a player.
    */
    unsigned short int playerNumber;  // assigned player number
    unsigned short int  points;  // number of victory points
    Coords currentLocation;  // the current location on the board
    Coords spawnLocation;  // the location player spawns in (usually one of the corners)
    Color playerColor;  // player's color
} Player;


typedef struct{
    /*
        this structure represents a bord.
    */
    Coords size;  // size of board
    Coords targetLocation;  // current target location
    unsigned char **horizontalWalls;  // all of the horizontal walls and borders
    unsigned char **verticalWalls;  // all of th vertial walls and borders
} Board;


typedef struct{
    /*
        this strucutre represents each game taking place.
    */
    Board board;  // board of current game.
    unsigned short int playerCount;  // number of players in game.
    Player *players;  // dynamic player array

    unsigned short int playerTurn;  // whose turn is it
    unsigned char movesLeft;  // how many moves the player has
} Game;


#endif // GAME_STRUCTS_H
