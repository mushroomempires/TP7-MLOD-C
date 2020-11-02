# TP7-MLOD-C
Informations sur le projet : Réalisation d'un solveur de sudoku en utilisant la librairie RayLib.

# Informations de compilation
gcc -o sudoku.exe sudoku.c -s -static -Os -std=c16 -Wall -Iexternal -DPLATFORM_DESKTOP -lraylib -lopengl32 -lgdi32 -lwinmm -L lib/ -I include/
