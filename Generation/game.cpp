#include <iostream>
#include <cstdlib>
#include <ctime>
#include <input.h>
#include <output.h>
using namespace std;

// Maze symbol definitions
char playerSymbol = 'P';
char exitSymbol = 'E';
char wallSymbol = '-';
char borderSymbol = '|';
char doorSymbol = 'D';
char emptySymbol = ' ';

// Creates empty maze
char** initMaze(int rows, int cols){
    char** maze = new char* [rows];
    for (int i = 0; i < rows; i++)
    {
        maze[i] = new char[cols];
        for (int j = 0; j < cols; j++)
            maze[i][j] = (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) ? borderSymbol : emptySymbol;
    }
    return maze;
}

// Generation of maze filing 
void generateMaze(char** maze, int rows, int cols) {
    for (int i = 1; i < rows - 1; i++)
        for (int j = 1; j < cols - 1; j++)
            maze[i][j] = wallSymbol;

    // Create a guaranteed path to exit
    int posX = 1, posY = 1;
    maze[posY][posX] = emptySymbol;

    // Random direction
    while (posX != cols - 2 || posY != rows - 2)
    {
        int randomNum = rand() % 100;

        if (randomNum < 40 && posX < cols - 2) posX++;      // right - 40%
        else if (randomNum < 80 && posY < rows - 2) posY++; // bottom - 40%
        else if (randomNum < 90 && posX > 2) posX--;        // left - 10%
        else if (posY > 2) posY--;                          // top - 10%

        maze[posY][posX] = emptySymbol;
    }

    maze[1][1] = playerSymbol;
    maze[rows - 1][cols - 2] = exitSymbol;

    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {

            if (maze[i][j] == emptySymbol ||
                maze[i][j] == playerSymbol ||
                maze[i][j] == exitSymbol)
                continue;

            int percentage = rand() % 100;

            if (percentage < 40) maze[i][j] = wallSymbol;       // 40% wallSymbol
            else if (percentage < 43) maze[i][j] = doorSymbol;   // 3% doorSymbol
            else maze[i][j] = emptySymbol;                       // 57% emptySymbo;
        }
    }
}

int main(){
    int rows, cols;
    getInput(rows, cols);

    srand(time(0));

    char** maze = initMaze(rows, cols);
    generateMaze(maze, rows, cols);

    printMaze(maze, rows, cols);

    for (int i = 0; i < rows; i++)
        delete[] maze[i];
    delete[] maze;
}
