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

#define INITIAL_X_OFFSET 370
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

bool solver(sudokuTile sudokuGrid[HEIGHT][WIDTH], int i, int j);

void shuffle(int *t, int l) {
  for (int j = 0; j < l; j++) {
      int tmp = t[j];
      int p = rand()%l;
      t[j] = t[p];
      t[p] = tmp;
  }
}

void initializeCompleteSudokuGrid(sudokuTile sudokuGrid[HEIGHT][WIDTH]){
    for (unsigned int i = 0; i < HEIGHT; i++){
        for (unsigned int j = 0; j < WIDTH; j++){             
            sudokuGrid[i][j].isFixed = false;
            sudokuGrid[i][j].isFilled = false;
            sudokuGrid[i][j].digit = 0;
        }
    }
    solver(sudokuGrid, 0, 0);
    for (unsigned int i = 0; i < HEIGHT; i++){
        for (unsigned int j = 0; j < WIDTH; j++){             
            sudokuGrid[i][j].isFixed = true;
            sudokuGrid[i][j].isFilled = true;
        }
    }
}

void initializeSudokuGrid(sudokuTile sudokuGrid[HEIGHT][WIDTH]){
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

bool isNotOnLine (int d, sudokuTile sudokuGrid[HEIGHT][WIDTH], int j){
    for (int i = 0; i < HEIGHT; i++){ // Fixes which row of the grid is tested, varies the column number in order to test every single tile of the row
        if (sudokuGrid[i][j].digit == d){
            return false;
        }
    }
    return true;
}

bool isNotOnColumn(int d, sudokuTile sudokuGrid[HEIGHT][WIDTH], int i){
    for (int j = 0; j < WIDTH; j++){ // Fixes which column of the grid is tested, varies the row number in order to test every single tile of the column
        if (sudokuGrid[i][j].digit == d){
            return false;
        }
    }
    return true;
}

bool isNotInBox(int d, sudokuTile sudokuGrid[HEIGHT][WIDTH], int i, int j){
    int i_box = i - (i % 3), j_box = j - (j % 3); // Identifies in which box the tested tile is, using modulos on its coordinates
    for (i = i_box; i < i_box + HEIGHT/3; i++){ // Checks in the box (which is a 3x3-sized square, hence the ranges for both width and height) if the value of the selected digit already exists there
        for (j = j_box; j < j_box + WIDTH/3; j++){
            if(sudokuGrid[i][j].digit == d){
                return false;
            }
        }
    }
    return true;
}

bool canNumberBePlacedHere(int d, sudokuTile sudokuGrid[HEIGHT][WIDTH], int i, int j){
    return (isNotOnLine(d, sudokuGrid, j) && isNotOnColumn(d, sudokuGrid, i) && isNotInBox(d, sudokuGrid, i, j));
}

bool solver(sudokuTile sudokuGrid[HEIGHT][WIDTH], int i, int j) {
    if(sudokuGrid[i][j].isFixed == true) { // Checks if the number in this tile has been given from the start
        if(j == WIDTH - 1) { // Checks if the solver has reached the end of the line
            if (i == HEIGHT - 1) { // Checks if the solver has reached the end of the grid (if i = j = 8, there's no tile to check afterwards)
                return true;
            } else { return solver(sudokuGrid, i + 1, 0); } // If the solver has reached the end of the grid and no inconsistency was found, the grid is considered valid
        } else { return solver(sudokuGrid, i, j + 1);} // If the solver can access the following tile, it will try to solve it 
    } else { 
        int array[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        shuffle(array, 9);
        for(int d = 0; d <= 8; d++) {
            if(canNumberBePlacedHere(array[d], sudokuGrid, i, j) == true) {
                sudokuGrid[i][j].isFilled = true;
                sudokuGrid[i][j].digit = array[d];
                if(j == WIDTH - 1) { // Once again, we have to check if the solver has reached the end of the line
                    if(i == HEIGHT - 1) { // Once again, we have to check if the solver has reached the end of the grid
                       return true;
                    } else { if (solver(sudokuGrid, i + 1, 0) == true) { return true;} } // Once again, if the solver has reached the end of the grid and no inconsistency was found, the grid is considered valid
                } else { if (solver(sudokuGrid, i, j + 1) == true) {return true;} }   
            }
        }
        sudokuGrid[i][j].digit = 0; // If no digit fits in the grid, then the last value is systematically replaced with a 0 and the solver gets back to the last tile to correct any inconsistency
        sudokuGrid[i][j].isFilled = false;
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
    initializeCompleteSudokuGrid(sudokuGrid);
    solver(sudokuGrid, 0, 0);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);
            for(unsigned int i = 0; i < HEIGHT; i++){
                for(unsigned int j = 0; j < WIDTH; j++){
                    Color linesColor;
                    if (j % 3 == 0) {linesColor = BLACK;}
                    else {linesColor = LIGHTGRAY;}
                    DrawLine(INITIAL_X_OFFSET, INITIAL_Y_OFFSET + j * SQUARE_SIZE, INITIAL_X_OFFSET + WIDTH * SQUARE_SIZE, INITIAL_Y_OFFSET + j * SQUARE_SIZE, linesColor);
                    DrawLine(INITIAL_X_OFFSET + j * SQUARE_SIZE, INITIAL_Y_OFFSET, INITIAL_X_OFFSET + j * SQUARE_SIZE, INITIAL_Y_OFFSET + HEIGHT * SQUARE_SIZE, linesColor);
                    if((sudokuGrid[i][j].digit != 0) && (i != WIDTH) && (j != HEIGHT)){
                        Color digitColor;
                        if (sudokuGrid[i][j].isFixed == true) {digitColor = BLACK;}
                        else {digitColor = BLUE;}
                        DrawText(TextFormat("%d", sudokuGrid[i][j].digit), (INITIAL_X_OFFSET + 15) + j * SQUARE_SIZE, (INITIAL_Y_OFFSET + 10) + i * SQUARE_SIZE, 20, digitColor);
                    }
                }
                DrawLine(INITIAL_X_OFFSET, INITIAL_Y_OFFSET + 9 * SQUARE_SIZE, INITIAL_X_OFFSET + WIDTH * SQUARE_SIZE, INITIAL_Y_OFFSET + 9 * SQUARE_SIZE, BLACK);
                DrawLine(INITIAL_X_OFFSET + 9 * SQUARE_SIZE, INITIAL_Y_OFFSET, INITIAL_X_OFFSET + 9 * SQUARE_SIZE, INITIAL_Y_OFFSET + HEIGHT * SQUARE_SIZE, BLACK);
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