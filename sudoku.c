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
   bool isModifiable;
   int correctDigit;
   int digit;
   Vector2 position;
} sudokuTile;

//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Sudoku");
    
        int grille[9][9] =
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
            for(unsigned int i = 0; i < 10; i++){
                for(unsigned int j = 0; j < 10; j++){
                    Color linesColor;
                    if (j % 3 == 0) {linesColor = BLACK;}
                        else {linesColor = LIGHTGRAY;}
                    DrawLine(INITIAL_X_OFFSET, INITIAL_Y_OFFSET + j * SQUARE_SIZE, INITIAL_X_OFFSET + WIDTH * SQUARE_SIZE, INITIAL_Y_OFFSET + j * SQUARE_SIZE, linesColor);
                    DrawLine(INITIAL_X_OFFSET + j * SQUARE_SIZE, INITIAL_Y_OFFSET, INITIAL_X_OFFSET + j * SQUARE_SIZE, INITIAL_Y_OFFSET + HEIGHT * SQUARE_SIZE, linesColor);
                    // DrawRectangleLines(INITIAL_X_OFFSET + j * SQUARE_SIZE, INITIAL_Y_OFFSET + i * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, GREY);
                    char buffer[2];
                    if(grille[i][j] != 0 && i < 9 && j < 9){
                        DrawText(itoa(grille[i][j], buffer, 10), (INITIAL_X_OFFSET + 15) + j * SQUARE_SIZE, (INITIAL_Y_OFFSET + 10) + i * SQUARE_SIZE, 20, BLACK);
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