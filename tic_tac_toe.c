#include <stdio.h>
#include <stdlib.h>
//IN my childhood time i play this game very much on pen and paper

char board[3][3];  // 3x3 board for play the game 
char currentMarker;
int currentPlayer;

// Function to initialize the board
void resetBoard() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = ' ';
}

// Function to print the current board
void printBoard() {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        printf(" %c | %c | %c ", board[i][0], board[i][1], board[i][2]);
        if (i < 2)
            printf("\n---|---|---\n");
    }
    printf("\n\n");
}

// Function to place the marker on the board
int placeMarker(int slot) {
    int row = (slot - 1) / 3;
    int col = (slot - 1) % 3;
    if (slot < 1 || slot > 9 || board[row][col] != ' ')
        return 0;  // Invalid move
    board[row][col] = currentMarker;
    return 1;
}

// Function to check the winner
char checkWinner() {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }
    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];
    return ' ';
}

// Function to swap player turns
void swapTurn() {
    if (currentMarker == 'X')
        currentMarker = 'O';
    else
        currentMarker = 'X';

    currentPlayer = (currentPlayer == 1) ? 2 : 1;
}

// The main game function
void game() {
    char winner = ' ';
    int slot;

    for (int i = 0; i < 9; i++) {
        printBoard();
        printf("Player %d (%c), enter your move (1-9): ", currentPlayer, currentMarker);
        scanf("%d", &slot);

        if (!placeMarker(slot)) {
            printf("Invalid move! Try again.\n");
            i--;  // retry
            continue;
        }

        winner = checkWinner();
        if (winner != ' ') {
            printBoard();
            printf(" Player %d (%c) congratulation you win the game! \n", currentPlayer, winner);
            return;
        }
        swapTurn();
    }

    printBoard();
    printf("It's a draw!\n");
}

// Main function
int main() {
    printf("===== TIC-TAC-TOE GAME =====\n");
    printf("Player 1: X\nPlayer 2: O\n");

    resetBoard();
    currentMarker = 'X';
    currentPlayer = 1;

    game();

    return 0;
}
