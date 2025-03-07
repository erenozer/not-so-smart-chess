/* Board definition for the game of Chess, implementation file of class Board. */

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <fstream>
#include "Piece.h"

//using namespace std;

using std::cout;
using std::string;
using std::vector;
using std::endl;
using std::rand;
using std::time;
using std::ofstream;
using std::ifstream;

class Board {
public:
    // Constructor initializes the Chess board's starting state using createBoard function.
    Board();

    void printBoard() const;

    /* Checks if the input is another function like "save", "load", "suggest" or "exit", if so, calls the according function
    * Otherwise, checks if the input is a 'valid' chess notation like e2e4
    * Returns 0 if invalid input
    * Returns 1 if valid Chess notation input
    * Returns 2 if the user wants a move suggestion (if input is "suggest")
    * Returns 3 if user wants to save the current board to file (input is "save")
    * Returns 4 if user wants to load a game from file (input is "load")
    * Returns -1 if the user want to exit the game (if input is "exit") */
    int inputMove(string &input, int &old_row, int &old_col, int &new_row, int &new_col, const int& status) const;

    // Move piece from old row, old_col to new row, new_col, checks if the move is legal using isLegalMove function
    // if the move is legal and was successful, returns true, otherwise false.
    bool movePiece(int old_row, int old_col, int new_row, int new_col);

    // Reverts the last move done by any player
    void revertMove();

    /* Returns -2 if something is wrong (King not found)
     * Returns  0 for CHECK - the King is not safe and there is at least one move to save it
     * Returns  1 if the King is safe */
    int isKingSafe(int color);


    /* Returns -2 if something is wrong (King not found)
     * Returns -1 for CHECKMATE - the King is not safe and no move can save it
     * Returns  0 for CHECK - the King is not safe and there is at least one move to save it
     * Returns  1 if the King is safe */
    int isCheckmate(int color);

    // Suggests a move where the current color has the best score for the next move.
    void suggestMove(int color);

    // Precondition: This function assumes that there is a folder  named "saves" in the same directory of the project.
    // Saves the current layout of the Chess board into a txt file.
    void saveToFile(int turn) const;

    // Precondition: This function assumes that there is a folder named "saves" in the same directory of the project.
    // Loads a previously saved board to the current game.
    // If the load was successful, returns whose turn is it (0 for white 1 for black), otherwise returns -1.
    int loadFromFile();

private:
    // This function is only called by the constructor
    void createBoard();

    // Sets all the board pieces to PieceType::Empty
    void clearBoard();

    // Returns true if the input move is a legal chess move. This function is called by the movePiece function.
    bool isLegalMove(int old_row, int old_col, int new_row, int new_col) const;

    // Returns true if the path of a piece is empty. This function is called by the isLegalMove function.
    bool isPathEmpty(int old_row, int old_col, int new_row, int new_col) const;

    // Returns the overall score of the specified color's pieces
    double calculateScore(int color);

    /* Precondition: This function assumes the given row and col is NOT empty.
     * Returns 0 if the piece on specified row and col is under attack by any opponent piece
     * Returns 1 if the piece is safe
     * Function is used for calculateScore function. */
    bool isPieceSafe(int row, int col, int color) const;

    // Vectors to hold the current and the previous positions of the chess board
    vector <vector<Piece> > board;
    vector <vector<Piece> > previousBoard;
};

// Move struct only for the suggestMove function to use while looking for different moves
struct Move {
    int old_row;
    int old_col;
    int new_row;
    int new_col;
};


#endif //CHESS_BOARD_H
