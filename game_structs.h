#include "color.h"
#include <stdbool.h>

#ifndef GAME_STRUCTS_H
#define GAME_STRUCTS_H

#define MAX_NAME_LENGTH 20
#define RIGHT (Coords){1, 0}
#define LEFT (Coords){-1, 0}
#define UP (Coords){0, 1}
#define DOWN (Coords){0, -1}


typedef struct{
    /*
        this structure represents a location or size (x/y coordinates)
    */
    short int x;
    short int y;
} Coords;


typedef struct{
    /*
        this structure represents a player.
    */
    char player_name[MAX_NAME_LENGTH];  // player name
    unsigned short int player_number;  // assigned player number
    unsigned short int  points;  // number of victory points
    Coords current_location;  // the current location on the board
    Coords spawn_location;  // the location player spawns in (usually one of the corners)
    Color player_color;  // player's color
} Player;


typedef struct{
    /*
        this structure represents a bord.
    */
    Coords size;  // size of board
    Coords token_location;  // current target token location
    bool **horizontal_walls;  // all of the horizontal walls. 0 = no wall, 1 = wall
    bool **vertical_walls;  // all of th vertial walls. 0 = no wall, 1 = wall
    short int **squares; // all of za squares. 0 = empty, abs value = token number, - value sign = occupied by a player, + value sign = empty square
    short int number_of_tokens;  // total number of squares that contain a token

} Board;



typedef struct{
    /*
        this strucutre represents each game taking place.
    */
    Board board;  // board of current game.
    unsigned short int player_count;  // number of players in game.
    Player *players;  // dynamic player array

    unsigned short int current_player;  // whose turn is it
    unsigned char moves_left;  // how many moves the player has
} Game;


// function delcerations
void init_player(Player* player, const char* player_name, Color player_color);
void init_board(Board* board, short int width, short int height);
void init_game(Game* game);

char add_player(Game* game, Player * player);  // adds a player to the game
char next_player(Game* game);  // finishes the turn of the current player and moves on to the next.
char move(Game* game, Coords direction);  // moves the current player 

#endif // GAME_STRUCTS_H
