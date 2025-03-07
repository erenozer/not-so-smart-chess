#include <iostream>
using namespace std;

#include "Board.h"

int main() {
    Board chess;
    string input;

    cout << "***           Welcome to Chess!           ***\n"
    << "- There are some options available to perform:\n"
    << "- Enter your move in standard form (ex: e2e4)\n"
    << "- Type 'suggest' to receive a move suggestion\n"
    << "- Type 'save' to save the current state of the board into a file\n"
    << "- Type 'load' to load a previously saved board file to this game\n"
    << "- Type 'exit' to end the game and exit the program.\n\n";

    chess.printBoard();

    int old_row=0, old_col=0, new_row=0, new_col=0;
    int attempts = 1;
    int turnColor = 0; // Holds 0 for white's turn, 1 for black's turn
    int kingSafe = 1; // King starts as safe

    do {
        cout << (turnColor == 0 ? "[White's turn]\n" : "[Black's turn]\n");

        // Warn the player if their King is in check
        if(kingSafe == 0)
            cout << "Check! The King is in danger, you have to protect it.\n";

        // King was not found in the board, game error
        if(kingSafe == -2) {
            cout << "An error occurred. You can start the game again.\n\n";
            exit(1);
        }

        cout << "Enter your move: ";
        getline(cin, input);
        int inputResult = chess.inputMove(input, old_row, old_col, new_row, new_col, turnColor);

        // Call specified functions according to the inputMove's return value (you can read more about that function's declaration)
        if(inputResult == 2) {
            chess.suggestMove(turnColor);
        } else if(inputResult == 3) {
            chess.saveToFile(turnColor);
        } else if(inputResult == 4) {
            int loadResult = chess.loadFromFile();
            if(loadResult != -1) {
                // Change the current turn to the one saved on the saved board
                turnColor = loadResult; 
            }
        }
        else if(inputResult == -1) {
            cout << "Exiting the game...\n\n";
            exit(0);
        }

        // Input is not valid, ask for it again until 5 attempts are reached
        while (inputResult == 0) {
            if(attempts<4) {
                cout << "Invalid move, please try again.\n";
                ++attempts;

                // Warn the player if their King is in check
                if(kingSafe == 0)
                    cout << "Check! The King is in danger, you have to protect it.\n";

                // King was not found in the board, game error
                if(kingSafe == -2) {
                    cout << "An error occurred. You can start the game again.\n\n";
                    exit(1);
                }

                cout << "Enter your move: ";
                getline(cin, input);
                inputResult = chess.inputMove(input, old_row, old_col, new_row, new_col, turnColor);

                // Call specified functions according to the inputMove's return value (you can read more about that function's declaration)
                if(inputResult == 2) {
                    chess.suggestMove(turnColor);
                } else if(inputResult == 3) {
                    chess.saveToFile(turnColor);
                } else if(inputResult == 4) {
                    int loadResult = chess.loadFromFile();
                    if(loadResult != -1) {
                        turnColor = loadResult; 
                    }
                }
                else if(inputResult == -1) {
                    cout << "Exiting the game...\n\n";
                    exit(0);
                }
            } else {
                cout << "Too many invalid attempts, exiting the game...\n";
                exit(1);
            }
        }
        // Reset attempt counter if the input is valid
        attempts = 1;


        // Input is valid, try to make the move to see if it's a legal chess move
        if (inputResult == 1 && chess.movePiece(old_row, old_col, new_row, new_col)) {
            if (chess.isKingSafe(turnColor) == 0) {
                // This move puts the King in danger, don't allow it
                cout << "Move not allowed! Check your King's surroundings.\n";
                chess.revertMove(); // Only revert the move, the turn will not change so don't revert the turn.
            } else {
                // Change the turn from 0 to 1 or from 1 to 0, if the entered move was legal.
                turnColor = (turnColor == 0 ? 1 : 0);

                // Check if the King of the opponent is safe
                kingSafe = chess.isKingSafe(turnColor);

                // Check for checkmate (only if the King is not safe)
                if (kingSafe == 0) {
                    kingSafe = chess.isCheckmate(turnColor);
                }
            }
        } else if(inputResult != 2 && inputResult != 3 && inputResult != 4) {
            // Not a legal chess move and the other functions were not called either.
            cout << "Invalid move, please try again.\n\n";
        }

        // Print the board after each move
        chess.printBoard();

    } while(kingSafe != -1); // -1 is returned when it's checkmate
    cout << "\nCheckmate! " << ((turnColor==0) ? "Black" : "White") << " player won the game.\n";
    return 0;
}