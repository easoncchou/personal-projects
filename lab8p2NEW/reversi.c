/**
 * @file reversi.c
 * @author Chen Yi Chou
 * @brief This file is used for APS105 Lab 8. 2023W version
 * @date 2023-03-14
 * 
 */

// DO NOT REMOVE THE FOLLOWING LINE
#if !defined(TESTER_P1) && !defined(TESTER_P2)
// DO NOT REMOVE THE ABOVE LINE
#include "reversi.h"
// DO NOT REMOVE THE FOLLOWING LINE
#endif
// DO NOT REMOVE THE ABOVE LINE

void printBoard(char board[][26], int n) {
    char axis[26];
    for (char i = 0; i < 26; i++) {
        axis[i] = 'a' + i;
    }

    printf("  ");
    for (int i = 0; i < n; i++) {
        printf("%c", axis[i]);
    }
    printf("\n");

    for (int row = 0; row < n; row++) {
        printf("%c ", axis[row]);
        for (int col = 0; col < n; col++) {
            printf("%c", board[row][col]);
        }
        printf("\n");
    } 
}

bool positionInBounds(int n, int row, int col) {
    if (row < 0 || row >= n || col < 0 || col  >= n) {
        return false;
    } else {
        return true;
    }
}

bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol) {
    bool canContinue = true;
    bool immediateColour = true; // for checking if the very next piece from the starting location is the same colour

    do {
        row += deltaRow;
        col += deltaCol;
        if (positionInBounds(n, row, col)) {
            // within this layer assume that the coords are within domain
            // check if the next first-immediate piece is the same colour or if the next coord is empty. If it is, then it isnt legal!
            if ( (immediateColour && board[row][col] == colour) || board[row][col] == 'U') {
                return false;
                canContinue = false;
            } else if (board[row][col] != colour &&  board[row][col] != 'U') {
                // here there is potential for a legal move, so do nothing and
            } else if (immediateColour == false && board[row][col] == colour) {
                // here this means we found a same-colour after some streak of the opposite-colour
                return true;
                canContinue = false; 
            }
        } else {
            return false; // this means the next immediate coordinate is out of bounds
            canContinue = false;
        }
        immediateColour = false;

    } while (canContinue == true);
}

void burgerFlip(char board[][26], int n, int row, int col, char colour) {
    // now check to flip by calling checkLegalInDirection. If checkLegalInDirection returns true for a given deltaRow deltaCol,
    // save the direction. We will need to check this direction for a sandwich 
    int rowReset = row;
    int colReset = col;

    for (int deltaRow = -1; deltaRow < 2; deltaRow++) {
        for (int deltaCol = -1; deltaCol < 2; deltaCol++) {
            if (deltaRow == 0 && deltaCol == 0) {
                // dont check this direction lol
            } else {
                if (checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)) {
                    // if this statement activates then we need to burger flip until we reach a the first match-colour piece
                    do {
                        if (board[row][col] != colour) {
                            board[row][col] = colour;
                        }
                        row += deltaRow;
                        col += deltaCol;
                    } while (board[row][col] != colour);
                    row = rowReset;
                    col = colReset;
                }
            }
        }
    }
    
}

int countScore(char board[][26], int n, int row, int col, char colour) { // count the immediate score of a potential move
    int score = 0;
    int rowReset = row;
    int colReset = col;

    for (int deltaRow = -1; deltaRow < 2; deltaRow++) {
        for (int deltaCol = -1; deltaCol < 2; deltaCol++) {
            if (deltaRow == 0 && deltaCol == 0) {
                // dont check this direction lol
            } else {
                if (checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)) {
                    // if this statement activates then we need to burger flip until we reach a the first match-colour piece
                    do {
                        if (board[row][col] != colour) {
                            score++;
                        }
                        row += deltaRow;
                        col += deltaCol;
                    } while (board[row][col] != colour);
                    row = rowReset;
                    col = colReset;
                }
            }
        }
    }
    return score;
}

bool legalMove(char board[][26], int n, int row, int col, char colour) {
    bool answer = false;
    if (board[row][col] == 'U') {
        for (int deltaRow = -1; deltaRow < 2; deltaRow++) {
            for (int deltaCol = -1; deltaCol < 2; deltaCol++) {
                if (deltaRow == 0 && deltaCol == 0) {
                    // dont check this direction lol
                } else {
                    if (checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)) {
                        answer = true;
                    }
                }
            }
        }
    }
    return answer;
}

bool moveAvailable(char board[][26], int n, char colour) {
    bool answer = false;

    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (legalMove(board, n, row, col, colour)) {
                answer = true;
            }       
        }
    }

    return answer;
}

int minmax(char board[26][26], int n, char turn, int *row, int *col, char colourToWin, int depth) {
    // first save our colour and the opponents colour
    char enemyColour = 'W';
    if (colourToWin == 'W') {
        enemyColour = 'B';
    }
    // initialize values for counting score in the base case
    int ourScore = 0;
    int enemyScore = 0;
    // base case: if we are at depth 5, we need to calculate the difference in number of our pieces vs the enemy pieces. This value is the score we assign to a move.
    if (depth == 0 || (!moveAvailable(board, n, colourToWin) && !moveAvailable(board, n, enemyColour))) {
        for (int iterativeRow = 0; iterativeRow < n; iterativeRow++) {
            for (int iterativeCol = 0; iterativeCol < n; iterativeCol++) {
                if (board[iterativeRow][iterativeCol] == colourToWin) {
                    ourScore++;
                } else if (board[iterativeRow][iterativeCol] == enemyColour) {
                    enemyColour++;
                }
            }
        }
        int heuristicScore = ourScore - enemyScore;
        return heuristicScore;
    } else {
        // recursion here for minmax algorithm
        int bestScore;
        int worstScore;
        int bestRow;
        int bestCol;
        int currentScore;
        bool moveMadeInSearch; // this bool will be false and raise a flag if there is no move available in the depth-search
        char nextTurn = 'W'; // use this variable to call the recursive fcn 
        if (turn == 'W') {
            nextTurn = 'B';
        }
        
        // alternate between making a move for our colour (maximize score) and a move for enemy colour (minimize score)
        if (turn == colourToWin) { // maximize
            bestScore = -99999;
            // go through every move and call recursively
            for (int iterativeRow = 0; iterativeRow < n; iterativeRow++) {
                for (int iterativeCol = 0; iterativeCol < n; iterativeCol++) {
                    // make a copy of the board before you make each move.
                    char copyBoard[26][26]; // this will be the board first passed into minmax before anything is done.
                    for (int iterativeRow2 = 0; iterativeRow2 < n; iterativeRow2++) {
                        for (int iterativeCol2 = 0; iterativeCol2 < n; iterativeCol2++) {
                            copyBoard[iterativeRow2][iterativeCol2] = board[iterativeRow2][iterativeCol2];
                        }
                    }
                    if (legalMove(copyBoard, n, iterativeRow, iterativeCol, turn)) {
                        copyBoard[iterativeRow][iterativeCol] = turn;
                        moveMadeInSearch = true;
                        burgerFlip(copyBoard, n, iterativeRow, iterativeCol, turn);
                        currentScore = minmax(copyBoard, n, nextTurn, row, col, colourToWin, depth - 1);
                        if (currentScore > bestScore) {
                            bestScore = currentScore;
                            bestRow = iterativeRow;
                            bestCol = iterativeCol;
                        }
                    } 
                }
            }
            if (moveMadeInSearch == false) {
                currentScore = minmax(board, n, nextTurn, row, col, colourToWin, depth - 1);
                bestScore = currentScore;
            }
            *row = bestRow;
            *col = bestCol;
            return bestScore;
        } else { // minimize
            worstScore = 99999;
            // go through every move and call recursively
            for (int iterativeRow = 0; iterativeRow < n; iterativeRow++) {
                for (int iterativeCol = 0; iterativeCol < n; iterativeCol++) {
                    // make a copy of the board before you make each move.
                    char copyBoard[26][26]; // this will be the board first passed into minmax before anything is done.
                    for (int iterativeRow2 = 0; iterativeRow2 < n; iterativeRow2++) {
                        for (int iterativeCol2 = 0; iterativeCol2 < n; iterativeCol2++) {
                            copyBoard[iterativeRow2][iterativeCol2] = board[iterativeRow2][iterativeCol2];
                        }
                    }
                    if (legalMove(copyBoard, n, iterativeRow, iterativeCol, turn)) {
                        copyBoard[iterativeRow][iterativeCol] = turn;
                        moveMadeInSearch = true;
                        burgerFlip(copyBoard, n, iterativeRow, iterativeCol, turn);
                        currentScore = minmax(copyBoard, n, nextTurn, row, col, colourToWin, depth - 1);
                        if (currentScore < worstScore) {
                            worstScore = currentScore;
                        }
                        // undo the move by assigning the boardBeforeMove back to the board
                    }
                }
            }
            if (moveMadeInSearch == false) {
                currentScore = minmax(board, n, nextTurn, row, col, colourToWin, depth - 1);
                worstScore = currentScore;
            }
            return worstScore;
        }
    }
}

int makeMove(char board[26][26], int n, char turn, int *row, int *col) {
    // basic/general strategies
    // if the move is available to be made on a corner, always make the move (corner control OP)
    // never place a piece next to an empty corner
    if (moveAvailable(board, n, turn)) {
        if (legalMove(board, n, 0, 0, turn)) {
            *row = 0;
            *col = 0;
        } else if (legalMove(board, n, 0, n-1, turn)) {
            *row = 0;
            *col = n-1;
        } else if (legalMove(board, n, n-1, 0, turn)) {
            *row = n-1;
            *col = 0;
        } else if (legalMove(board, n, n-1, n-1, turn)) {
            *row = n-1;
            *col = n-1;
        } else {
            minmax(board, n, turn, row, col, turn, 5);
        }
    } 
    return 0;
}

// ok


//*******************************************************
// Note: Please only put your main function below
// DO NOT REMOVE THE FOLLOWING LINE
#ifndef TESTER_P2
// DO NOT REMOVE THE ABOVE LINE

int main(void) {
  // Write your own main function here
// Get dimension of the board from the user, then the colour of the computer -> player
    int dimBoard = 0;
    char compColour;
    char playerColour = 'B';
    char board[26][26];

    printf("Enter the board dimension: ");
    scanf("%d", &dimBoard);

    printf("Computer plays (B/W): ");
    scanf(" %c", &compColour);

    if (compColour == 'B') {
        playerColour = 'W';
    }

    // initialize the board
    for (int row = 0; row < dimBoard; row++) {
        for (int col = 0; col < dimBoard; col++) {
            if ( (row == (dimBoard/2 - 1) && col == (dimBoard/2 - 1)) || (row == dimBoard/2 && col == dimBoard/2) ) {
                board[row][col] = 'W';
            } else if ( (row == (dimBoard/2 - 1) && col == dimBoard/2)|| (row == dimBoard/2 && col == (dimBoard/2 - 1)) ) {
                board[row][col] = 'B';
            } else {
                board[row][col] = 'U';
            } 
        }
    }

    // Print the board
    printBoard(board, dimBoard);
    // Begin the game. If computer is B, play a move. If player is B, ask for a move first
    bool gameOver = false;
    bool manualMove = false; // when manualMove is true it is the player's turn to put down a piece
    char winner = 'N';
    bool illegalEnd = false;
    bool canPlayerMove = true;
    bool canCompMove = true;

    if (playerColour == 'B') {
        manualMove = true;
    }

    while (!gameOver) {
        if (manualMove) { // player turn
            char userRowChar;
            char userColChar;
            int userRow;
            int userCol;
            // check if the user has a legal move to make
            if (moveAvailable(board, dimBoard, playerColour)) {
                // take user input
                printf("Enter move for colour %c (RowCol): ", playerColour);
                scanf(" %c%c", &userRowChar, &userColChar);
                userRow = userRowChar - 'a';
                userCol = userColChar - 'a';
                // check if the user entered an illegal move
                if (legalMove(board, dimBoard, userRow, userCol, playerColour) == false) {
                    illegalEnd = true;
                    gameOver = true;
                    printf("Invalid move.\n");

                    if (playerColour == 'B') {
                        winner = 'W';
                    } else if (playerColour == 'W') {
                        winner = 'B';
                    }
                } else {
                    board[userRow][userCol] = playerColour;
                    burgerFlip(board, dimBoard, userRow, userCol, playerColour);
                    printBoard(board, dimBoard);
                    canCompMove = true;
                }
            } else {
                printf("%c player has no valid move. \n", playerColour);
                canPlayerMove = false;
            }
            manualMove = false;

        } else { // computer turn
            
            // check if the computer has a legal move to make
            if (moveAvailable(board, dimBoard, compColour)) {
                // calculate the score of each move. Save the coords of the move with the highest score
                int bestRow = 1337;
                int bestCol = 1337;
                makeMove(board, dimBoard, compColour, &bestRow, &bestCol);
                printf("bestRow is %d , bestCol is %d\n", bestRow, bestCol);
                board[bestRow][bestCol] = compColour;
                burgerFlip(board, dimBoard, bestRow, bestCol, compColour);
                printf("Computer places %c at %d%d.\n", compColour, bestRow, bestCol);
                printBoard(board, dimBoard);
                canPlayerMove = true;
            } else {
                printf("%c player has no valid move.\n", compColour);
                canCompMove = false;
            }

            manualMove = true;
        }

        // check if the board is full to determine if the game is lawfully over (board is full or no moves left). then print the winner.
        if (illegalEnd != true) {
            gameOver = true;    
            for (int row = 0; row < dimBoard; row++) {
                for (int col = 0; col < dimBoard; col++) {
                    if (board[row][col] == 'U') {
                        gameOver = false;
                    }
                }
            }
            // calculate the winner here too
            int Bscore = 0;
            int Wscore = 0;
            for (int row = 0; row < dimBoard; row++) {
                for (int col = 0; col < dimBoard; col++) {
                    if (board[row][col] == 'B') {
                        Bscore++;
                    } else {
                        Wscore++;
                    }
                }
            }
            if (Bscore > Wscore) {
                winner = 'B';
            } else if (Bscore < Wscore) {
                winner = 'W';
            } else {
                winner = 'N'; // its a tie...
            }
        }

        if (moveAvailable(board, dimBoard, playerColour) == false && moveAvailable(board, dimBoard, compColour) == false) {
            gameOver = true;
            // calculate the winner here too
            int Bscore = 0;
            int Wscore = 0;
            for (int row = 0; row < dimBoard; row++) {
                for (int col = 0; col < dimBoard; col++) {
                    if (board[row][col] == 'B') {
                        Bscore++;
                    } else {
                        Wscore++;
                    }
                }
            }
            if (Bscore > Wscore) {
                winner = 'B';
            } else if (Bscore < Wscore) {
                winner = 'W';
            } else {
                winner = 'N'; // its a tie...
            }
        }

        if (gameOver) {
            if (winner != 'N') {
                printf("%c player wins.", winner); 
            } else {
                printf("Draw!");
            }
        }

    }

    return 0;
}


// DO NOT REMOVE THE FOLLOWING LINE
#endif
// DO NOT REMOVE THE ABOVE LINE
//*******************************************************
