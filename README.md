# TP7-MLOD-C

# Informations sur le projet

Le sudoku est un jeu de puzzle très populaire, où le but est de remplir toute les cases d'une grille de taille 9*9 avec une série de chiffres (de 1 à 9), de sorte à ce que chaque ligne, colonne et "région" (secteur de 3 cases par 3 cases) ne soit composé que d'un seul exemplaire de chaque chiffre. Quelques symboles sont déjà disposés sur la grille au départ, permettant une résolution progressive du problème dans son ensemble.  

Ce dépôt contient ainsi un générateur de grille de sudoku selon quatre niveaux de difficulté (facile, moyen, difficile, très difficile), ainsi qu'un résolveur permettant de donner une des solutions pour la grille proposée. Ce générateur utilise la librairie RayLib pour gérer l'affichage.

# Informations de compilation et de test
La compilation s'effectue avec la commande suivante :
gcc -o sudoku.exe sudoku.c -s -static -Os -std=c11 -Wall -Iexternal -DPLATFORM_DESKTOP -lraylib -lopengl32 -lgdi32 -lwinmm -L lib/ -I include/

puis en lançant l'application "sudoku.exe" ainsi créée.

#Vidéo de présentation du projet
https://youtu.be/VKEjRWHTPa0
