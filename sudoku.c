#include "raylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------

#define WIDTH 9
#define HEIGHT 9

#define INITIAL_X_OFFSET 220
#define INITIAL_Y_OFFSET 120

#define SQUARE_SIZE 40

//----------------------------------------------------------------------------------
// Structures
//----------------------------------------------------------------------------------
typedef struct tile_st {
   bool isFilled;
   bool isFixed;
   int digit;
} sudokuTile;

//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------

void initializeSudoGrid(sudokuTile sudokuGrid[HEIGHT][WIDTH]){
     int placeholderGrid[HEIGHT][WIDTH] = 
    {
        {0,1,6,0,0,0,7,0,9},
        {0,9,3,8,0,6,0,0,0},
        {0,0,0,0,9,0,2,8,0},
        {9,0,0,0,3,2,0,5,4},
        {0,0,5,0,0,9,0,0,1},
        {3,2,0,6,0,5,8,9,7},
        {4,0,8,9,0,7,0,6,2},
        {7,0,0,0,5,0,0,0,0},
        {0,5,2,0,6,0,0,0,0}
    };
    for (unsigned int i = 0; i < HEIGHT; i++){
        for (unsigned int j = 0; j < WIDTH; j++){
            sudokuGrid[i][j].digit = placeholderGrid[i][j];
            if(sudokuGrid[i][j].digit != 0){
                sudokuGrid[i][j].isFixed = true;
                sudokuGrid[i][j].isFilled = true;
            } else {
                sudokuGrid[i][j].isFixed = false;
                sudokuGrid[i][j].isFilled = false;
            }
        }
    }
}

bool isNotOnLine (int d, sudokuTile sudoTile[HEIGHT][WIDTH], int j){
    for (int i = 0; i < HEIGHT; i++){ // Fixes which row of the grid is tested, varies the column number in order to test every single tile of the row
        if (sudoTile[i][j].digit == d){
            return false;
        }
    }
    return true;
}

bool isNotOnColumn(int d, sudokuTile sudoTile[HEIGHT][WIDTH], int i){
    for (int j = 0; j < WIDTH; j++){ // Fixes which column of the grid is tested, varies the row number in order to test every single tile of the column
        if (sudoTile[i][j].digit == d){
            return false;
        }
    }
    return true;
}

bool isNotInBox(int d, sudokuTile sudoTile[HEIGHT][WIDTH], int i, int j){
    int i_box = i - (i % 3), j_box = j - (j % 3); // Identifies in which box the tested tile is, using modulos on its coordinates
    for (i = i_box; i < i_box + HEIGHT/3; i++){ // Checks in the box (which is a 3x3-sized square, hence the ranges for both width and height) if the value of the selected digit already exists there
        for (j = j_box; j < j_box + WIDTH/3; j++){
            if(sudoTile[i][j].digit == d){
                return false;
            }
        }
    }
    return true;
}

bool canNumberBePlacedHere(int d, sudokuTile sudoTile[HEIGHT][WIDTH], int i, int j){
    return (isNotOnLine(d, sudoTile, j) && isNotOnColumn(d, sudoTile, i) && isNotInBox(d, sudoTile, i, j));
}

bool solver(sudokuTile sudoTile[HEIGHT][WIDTH], int i, int j) {
    if(sudoTile[i][j].isFixed == true) { // Checks if the number in this tile has been given from the start
        if(j == WIDTH - 1) { // Checks if the solver has reached the end of the line
            if (i == HEIGHT - 1) { // Checks if the solver has reached the end of the grid (if i = j = 8, there's no tile to check afterwards)
                return true;
            } else { return solver(sudoTile, i + 1, 0); } // If the solver has reached the end of the grid and no inconsistency was found, the grid is considered valid
        } else { return solver(sudoTile, i, j + 1);} // If the solver can access the following tile, it will try to solve it 
    } else { 
        for(int d = 1; d <= 9; d++) {
            if(canNumberBePlacedHere(d, sudoTile, i, j) == true) {
                sudoTile[i][j].isFilled = true;
                sudoTile[i][j].digit = d;
                if(j == WIDTH - 1) { // Once again, we have to check if the solver has reached the end of the line
                    if(i == HEIGHT - 1) { // Once again, we have to check if the solver has reached the end of the grid
                       return true;
                    } else { if (solver(sudoTile, i + 1, 0) == true) { return true;} } // Once again, if the solver has reached the end of the grid and no inconsistency was found, the grid is considered valid
                } else { if (solver(sudoTile, i, j + 1) == true) {return true;} }   
            }
        }
        sudoTile[i][j].digit = 0; // If no digit fits in the grid, then the last value is systematically replaced with a 0 and the solver gets back to the last tile to correct any inconsistency
        sudoTile[i][j].isFilled = false;
        return false;
    }
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Sudoku");
    sudokuTile sudokuGrid[HEIGHT][WIDTH];
    initializeSudoGrid(sudokuGrid);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        solver(sudokuGrid, 0, 0);
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        

            ClearBackground(RAYWHITE);
            for(unsigned int i = 0; i < HEIGHT + 1; i++){
                for(unsigned int j = 0; j < WIDTH + 1; j++){

                    Color linesColor;
                    if (j % 3 == 0) {linesColor = BLACK;}
                    else {linesColor = LIGHTGRAY;}
                    DrawLine(INITIAL_X_OFFSET, INITIAL_Y_OFFSET + j * SQUARE_SIZE, INITIAL_X_OFFSET + WIDTH * SQUARE_SIZE, INITIAL_Y_OFFSET + j * SQUARE_SIZE, linesColor);
                    DrawLine(INITIAL_X_OFFSET + j * SQUARE_SIZE, INITIAL_Y_OFFSET, INITIAL_X_OFFSET + j * SQUARE_SIZE, INITIAL_Y_OFFSET + HEIGHT * SQUARE_SIZE, linesColor);
                    if((sudokuGrid[i][j].digit != 0) && (i != WIDTH) && (j != HEIGHT)){
                        char buffer[4];
                        Color digitColor;
                        if (sudokuGrid[i][j].isFixed == true) {digitColor = BLACK;}
                        else {digitColor = BLUE;}
                        DrawText(itoa(sudokuGrid[i][j].digit, buffer, 10), (INITIAL_X_OFFSET + 15) + j * SQUARE_SIZE, (INITIAL_Y_OFFSET + 10) + i * SQUARE_SIZE, 20, digitColor);
                    }
                }
            }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}