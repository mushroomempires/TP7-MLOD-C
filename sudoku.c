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
   int digit;
} sudokuTile;

//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------

bool isNotOnLine (int d, sudokuTile sudoTile[WIDTH][HEIGHT], int i){
    for (int j = 0; j < HEIGHT; j++){ // Fixes which row of the grid is tested, varies the column number in order to test every single tile of the row
        if (sudoTile[i][j].digit == d){
            return false;
        }
    }
    return true;
}

bool isNotOnColumn(int d, sudokuTile sudoTile[WIDTH][HEIGHT], int j){
    for (int i = 0; i < WIDTH; i++){ // Fixes which column of the grid is tested, varies the row number in order to test every single tile of the column
        if (sudoTile[i][j].digit == d){
            return false;
        }
    }
    return true;
}

bool isNotinBox(int d, sudokuTile sudoTile[WIDTH][HEIGHT], int i, int j){
    int i_box = i - (i % 3), j_box = j - (j % 3); // Identifies in which box the tested tile is, using modulos on its coordinates
    for (i = i_box; i < i_box + WIDTH/3; i++){ // Checks in the box (which is a 3x3-sized square, hence the ranges for both width and height) if the value of the selected digit already exists there
        for (j = j_box; j < j_box + HEIGHT/3; j++){
            if(sudoTile[i][j].digit == d){
                return false;
            }
        }
    }
    return true;
}



int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Sudoku");
    
        sudokuTile sudokuGrid[WIDTH][HEIGHT];
        int grille[WIDTH][HEIGHT] = 
    {
        {9,0,0,1,0,0,0,0,5},
        {0,0,5,0,9,0,2,0,1},
        {8,0,0,0,4,0,0,0,0},
        {0,0,0,0,8,0,0,0,0},
        {0,0,0,7,0,0,0,0,0},
        {0,0,0,0,2,6,0,0,9},
        {2,0,0,3,0,0,0,0,6},
        {0,0,0,2,0,0,9,0,0},
        {0,0,1,9,0,4,5,7,0}
    };
    for (unsigned int i = 0; i < WIDTH; i++){
        for (unsigned int j = 0; j < HEIGHT; j++){
            sudokuGrid[i][j].digit = grille[i][j];
        }
    }
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

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
                    char buffer[2];
                    if(sudokuGrid[i][j].digit != 0 && i < WIDTH && j < HEIGHT){
                        DrawText(itoa(sudokuGrid[i][j].digit, buffer, 10), (INITIAL_X_OFFSET + 15) + j * SQUARE_SIZE, (INITIAL_Y_OFFSET + 10) + i * SQUARE_SIZE, 20, BLACK);
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