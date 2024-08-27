#include "game_structs.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


// void init_player(Player* player, char *player_name, Color player_color){
//     /**
//      * this function initializes a player strucutre.
//      * receives a player pointer, a player name and a color (stuff that the user chooses)
//      */
//     int len = sizeof(player_name);
//     if (len <= MAX_NAME_LENGTH){
//         strcpy(player->player_name, player_name);
//         player->player_color = player_color;
//         player->points = 0;
//     }
// }

static void carve_maze(bool **horizontal_walls, bool **vertical_walls, Coords size){
    /*
        carvs the maze so you can get to every point.
        using prim's algorithm:
            selects a random starting square.
            marks all of the boundary squares.
            selects a random boundary square and removes the wall between the 2 squares.
            adds the new square the the discovered square list.
            * selects a new square from the neighboring square list.
            marks all of the boundary squares
            removes the wall between the selcted square and one of the discovered squares (random)
            adds to the discovered list.
            repeats from * until all squares are discovered.
    */
    // set random with current time.
    srand((unsigned int)time(NULL));

    // get random starting square
    Coords current_square = (Coords){rand() % size.x, rand() % size.y};
    char* temp_square;
    int i, j;
    short int selected_boundary_square;
    short int squares_discovered = 1;
    short int total_squares = size.x * size.y;
    char **squares = (char**)malloc(size.x * sizeof(char*));
    for (i=0; i < size.x; i++){
        squares[i] = (char*)calloc(size.y, sizeof(char));  // set each square with 0. 0 = not discovered, 1 = boundary, 2 = discovered.
    }

    // starting size will be 25% of squares
    Coords* boundary_squares = (Coords*)malloc((total_squares >> 2) * sizeof(Coords));
    short int boundary_squares_sizing = 1;
    int boundary_count = 0;
    char neighboring_discovered_squares_count;
    Coords neighboring_discovered_squares[4];
    Coords discovered_square;


    do{
        
        squares[current_square.x][current_square.y] = 2;

        for(i = -1 ; i <= 1 ; i+=2){
            
            if(boundary_count >= (boundary_squares_sizing * (total_squares >> 2)) - 2){
                // increase array size by 0.25 of the total number of squares.
                boundary_squares_sizing++;
                boundary_squares = (Coords*)realloc(boundary_squares, boundary_squares_sizing * (total_squares >> 2) * sizeof(Coords));
            }

            if(current_square.x + i < size.x && current_square.x + i >= 0){
                temp_square = squares[current_square.x + i] + current_square.y;  // get pointer to slot.
                if (*temp_square == 0){
                    *temp_square = 1;
                    boundary_squares[boundary_count] = (Coords){current_square.x+i, current_square.y};
                    boundary_count++;
                }
            }

            if(current_square.y + i < size.y && current_square.y + i >= 0){
                temp_square = squares[current_square.x] + current_square.y + i;  // get pointer to slot.
                if (*temp_square == 0){
                    *temp_square = 1;
                    boundary_squares[boundary_count] = (Coords){current_square.x, current_square.y + i};
                    boundary_count++;
                }
            }
        }

        printf("%d", boundary_count);
        // pick a random square from the boundary squares.
        selected_boundary_square = rand() % boundary_count;
        current_square = boundary_squares[selected_boundary_square];
        neighboring_discovered_squares_count = 0;
        
        // get all of the squares that are already discovered and near the current bordering square.
        for(i = -1 ; i <= 1 ; i+=2){
            if(current_square.x + i < size.x && current_square.x + i >= 0 && squares[current_square.x + i][current_square.y] == 2){
                neighboring_discovered_squares[neighboring_discovered_squares_count++] = (Coords){current_square.x + i, current_square.y};
            }
            if(current_square.y + i < size.y && current_square.y + i >= 0 && squares[current_square.x][current_square.y + i] == 2){
                neighboring_discovered_squares[neighboring_discovered_squares_count++] = (Coords){current_square.x, current_square.y + i};
            }
        }
        // choose a random square from the ones already discovered (choose which random wall to erase)
        discovered_square = neighboring_discovered_squares[rand() % neighboring_discovered_squares_count];

        if (discovered_square.x == current_square.x){
            // if there is a horizontal wall between the two (same x value but different y value, set the horizontal wall that is one above or at the same index to be 0)
            vertical_walls[current_square.x][current_square.y < discovered_square.y ? current_square.y : discovered_square.y] = 0;
        }else{
            // if there is a vertical wall between the two (different x value but same y value, set the vertical wall that is one behind or at the same index to be 0)
            horizontal_walls[current_square.x < discovered_square.x ? current_square.x : discovered_square.x][current_square.y] = 0;
        }

        // remove the current square from boundary squares.
        boundary_count--;
        // move the last square to the slot where the current square is.
        boundary_squares[selected_boundary_square] = boundary_squares[boundary_count];

        squares_discovered++;

    } while (total_squares > squares_discovered);

    for (i = 0 ; i < size.x ; i++)
        free(squares[i]);
    free(squares);
    free(boundary_squares);


}

static void print_board(Board board){
    printf("\n\n");

     for(int i = 0 ; i < board.size.x ; i++){
            for(int j=0 ; j < board.size.y ; j++){
                if (i < board.size.x-1 && board.horizontal_walls[i][j]){
                    printf("___");
                } else{
                    printf("   ");
                }
                if (j < board.size.y - 1){
                    printf("%s", board.vertical_walls[i][j]? "|": " ");
                }
            }
            printf("\n");
        }
}
void init_board(Board* board, short int rows, short int cols){
    if (rows > 0 && cols > 0){
        board->size = (Coords){rows, cols};  // set size of board

        int i;

        // allocate memory for horizontal walls.
        board->horizontal_walls = (bool**)malloc((rows-1) * sizeof(bool*));
        for (i=0; i < rows; i++){
            board->horizontal_walls[i] = (bool*)malloc(cols * sizeof(bool));
            // set all values to 1 (wall)
            memset(board->horizontal_walls[i], 1, cols * sizeof(bool));
        }

        // allocate memory for vertical walls.
        board->vertical_walls = (bool**)malloc(rows * sizeof(bool*));
        for (i=0; i < rows; i++){
            board->vertical_walls[i] = (bool*)malloc((cols-1) * sizeof(bool));
            // set all values to 1 (wall)
            memset(board->vertical_walls[i], 1, (cols-1) * sizeof(bool));
         }

        print_board(*board);

        carve_maze(board->horizontal_walls, board->vertical_walls, board->size);
        
        print_board(*board);
       
    }
}

int main(){
    printf("\n\n");
    Board b;
    init_board(&b, 10, 10);
    printf("\n\n");
}

// void init_game(Game* game);

// char add_player(Game* game, Player * player);  // adds a player to the game
// char next_player(Game* game);  // finishes the turn of the current player and moves on to the next.
// char move(Game* game, Coords direction);  // moves the current player 