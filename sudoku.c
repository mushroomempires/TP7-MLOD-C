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

#define GAMEMODE_EASY 1
#define GAMEMODE_MEDIUM 2
#define GAMEMODE_HARD 3
#define GAMEMODE_VERY_HARD 4

//----------------------------------------------------------------------------------
// Structures
//----------------------------------------------------------------------------------
typedef struct tile_st {
   bool isFilled;
   bool isFixed;
   int digit;
   bool isSolved;
} sudokuTile;

//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------

bool solver(sudokuTile sudokuGrid[HEIGHT][WIDTH], int i, int j);

int generateIntInARange(int lower, int upper){
    return (rand() % (upper - lower + 1) + lower);
}

void shuffle(int *t, int l) {
  for (int j = 0; j < l; j++) {
      int tmp = t[j];
      int p = rand()%l;
      t[j] = t[p];
      t[p] = tmp;
  }
}

void initializeEmptySudokuGrid(sudokuTile sudokuGrid[HEIGHT][WIDTH]){
       for (unsigned int i = 0; i < HEIGHT; i++){
        for (unsigned int j = 0; j < WIDTH; j++){             
            sudokuGrid[i][j].isFixed = false;
            sudokuGrid[i][j].isFilled = false;
            sudokuGrid[i][j].digit = 0;
			sudokuGrid[i][j].isSolved = false;
        }
    } 
}

void initializeCompleteSudokuGrid(sudokuTile sudokuGrid[HEIGHT][WIDTH]){
    for (unsigned int i = 0; i < HEIGHT; i++){
        for (unsigned int j = 0; j < WIDTH; j++){             
            sudokuGrid[i][j].isFixed = false;
            sudokuGrid[i][j].isFilled = false;
            sudokuGrid[i][j].digit = 0;
			sudokuGrid[i][j].isSolved = false;
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

void createSudokuGridFromCompleteGrid(sudokuTile sudokuGrid[HEIGHT][WIDTH], int difficultyLevel){
    int numberOfTilePairsToRemove;
     switch(difficultyLevel) {
      case 1 :
        numberOfTilePairsToRemove = generateIntInARange(21,23);
        break;
      case 2 :
        numberOfTilePairsToRemove = generateIntInARange(23,25);
        break;
      case 3 :
         numberOfTilePairsToRemove = generateIntInARange(25,27);
         break;
      case 4 :
         numberOfTilePairsToRemove = generateIntInARange(27,29);
         break;
      default : 
         break;
    };
    for (unsigned int counter = 0; counter < numberOfTilePairsToRemove; counter++){
        int rand_i = generateIntInARange(0, HEIGHT - 1), rand_j = generateIntInARange(0, WIDTH - 1);
        if (sudokuGrid[rand_i][rand_j].isFilled == false){
            counter--;
        } else {
            sudokuGrid[rand_i][rand_j].digit = 0;
            sudokuGrid[rand_i][rand_j].isFilled = false;
            sudokuGrid[rand_i][rand_j].isFixed = false;
            sudokuGrid[(HEIGHT - 1) - rand_i][(WIDTH - 1) - rand_j].digit = 0;
            sudokuGrid[(HEIGHT - 1) - rand_i][(WIDTH - 1) - rand_j].isFilled = false;
            sudokuGrid[(HEIGHT - 1) - rand_i][(WIDTH - 1) - rand_j].isFixed = false;
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
	if(sudokuGrid[0][0].isSolved == false) {
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
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Sudoku");
    sudokuTile sudokuGrid[HEIGHT][WIDTH];
    initializeEmptySudokuGrid(sudokuGrid);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyPressed('S')){  solver(sudokuGrid, 0, 0); sudokuGrid[0][0].isSolved = true;}
        if (IsKeyPressed('1')){  initializeCompleteSudokuGrid(sudokuGrid); createSudokuGridFromCompleteGrid(sudokuGrid, GAMEMODE_EASY);}  
        if (IsKeyPressed('2')){  initializeCompleteSudokuGrid(sudokuGrid); createSudokuGridFromCompleteGrid(sudokuGrid, GAMEMODE_MEDIUM);}  
        if (IsKeyPressed('3')){  initializeCompleteSudokuGrid(sudokuGrid); createSudokuGridFromCompleteGrid(sudokuGrid, GAMEMODE_HARD);}     
        if (IsKeyPressed('4')){  initializeCompleteSudokuGrid(sudokuGrid); createSudokuGridFromCompleteGrid(sudokuGrid, GAMEMODE_VERY_HARD);}  
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);
            for(unsigned int i = 0; i < HEIGHT; i++){
                for(unsigned int j = 0; j < WIDTH; j++){
                    Color linesColor;
                    if (j % 3 == 0) {linesColor = BLACK;}
                    else {linesColor = LIGHTGRAY;}
                    DrawText("Press 1 for easy mode.", 20, 20, 20, LIGHTGRAY);                   
                    DrawText("Press 2 for medium mode.", 270, 20, 20, LIGHTGRAY);
                    DrawText("Press 3 for hard mode.", 20, 50, 20, LIGHTGRAY);
                    DrawText("Press 4 for very hard mode.", 270, 50, 20, LIGHTGRAY);
                    DrawText("Press S for the solution.", 20, 560, 20, LIGHTGRAY);
                    DrawLine(INITIAL_X_OFFSET, INITIAL_Y_OFFSET + j * SQUARE_SIZE, INITIAL_X_OFFSET + WIDTH * SQUARE_SIZE, INITIAL_Y_OFFSET + j * SQUARE_SIZE, linesColor);
                    DrawLine(INITIAL_X_OFFSET + j * SQUARE_SIZE, INITIAL_Y_OFFSET, INITIAL_X_OFFSET + j * SQUARE_SIZE, INITIAL_Y_OFFSET + HEIGHT * SQUARE_SIZE, linesColor);
                    if((sudokuGrid[i][j].digit != 0) && (i != WIDTH) && (j != HEIGHT)){
                        Color digitColor;
                        if (sudokuGrid[i][j].isFixed == true) {digitColor = BLACK;}
                        else {digitColor = BLUE;}
                        DrawText(TextFormat("%d", sudokuGrid[i][j].digit), (INITIAL_X_OFFSET + 15) + j * SQUARE_SIZE, (INITIAL_Y_OFFSET + 10) + i * SQUARE_SIZE, SQUARE_SIZE/2, digitColor);
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