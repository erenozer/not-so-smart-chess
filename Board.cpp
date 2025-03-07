#include "Board.h"

Board::Board() {
    createBoard();
}

void Board::createBoard() {
    // Resize the board vector to have 8x8 slots for pieces. (Defaults them to Piece constructor's default values)
    board.resize(8, vector<Piece>(8));

    // Put the pieces into their starting state
    // Pawns for white and black
    for(int x=0; x<8; ++x) {
        board[1][x] = Piece(PieceType::Pawn, 1);
        board[6][x] = Piece(PieceType::Pawn, 0);
    }

    // Rest of the white pieces (color=0)
    board[7][0] = Piece(PieceType::Rook, 0);
    board[7][7] = Piece(PieceType::Rook, 0);
    board[7][1] = Piece(PieceType::Knight, 0);
    board[7][6] = Piece(PieceType::Knight, 0);
    board[7][2] = Piece(PieceType::Bishop, 0);
    board[7][5] = Piece(PieceType::Bishop, 0);
    board[7][3] = Piece(PieceType::Queen, 0);
    board[7][4] = Piece(PieceType::King, 0);


    // Rest of the black pieces (color=1)
    board[0][0] = Piece(PieceType::Rook, 1);
    board[0][7] = Piece(PieceType::Rook, 1);
    board[0][1] = Piece(PieceType::Knight, 1);
    board[0][6] = Piece(PieceType::Knight, 1);
    board[0][2] = Piece(PieceType::Bishop, 1);
    board[0][5] = Piece(PieceType::Bishop, 1);
    board[0][3] = Piece(PieceType::Queen, 1);
    board[0][4] = Piece(PieceType::King, 1);

    // Initialize previous board as same as the default board
    previousBoard = board;
}

void Board::clearBoard() {
    // Resize the board vector to have 8x8 slots for pieces.
    // Each Piece will be defaulted to Empty via the Piece's constructor.
    board.clear();
    board.resize(8, vector<Piece>(8));
}


void Board::printBoard() const {
    // Prints the chess board's current layout with the row and column numbers
    int line = 8;
    for (int y = 0; y < 8; ++y) {
        // Print the line number once for each line
        cout << line << " | ";
        --line;
        for (int x = 0; x < 8; ++x) {
            // Print all the pieces' representations for each line
            cout << board[y][x].getSymbol() << " ";
        }
        cout << endl;
    }
    cout << "    ---------------\n    a b c d e f g h \n";
}


int Board::inputMove(string &input, int &old_row, int &old_col, int &new_row, int &new_col, const int& status) const {
    /* Returns 0 if invalid input
     * Returns 1 if valid Chess notation input
     * Returns 2 if the user wants a move suggestion (if input is "suggest")
     * Returns 3 if user wants to save the current board to file (input is "save")
     * Returns 4 if user wants to load a game from file (input is "load")
     * Returns -1 if the user want to exit the game (if input is "exit") */

    // Lowercase all the letters in the input
    for (auto &c : input) {
        c = tolower(static_cast<char>(c));
    }

    // Check if the input is not a Chess move but is another valid action, if so, return the specified values
    if(input == "suggest")
        return 2;
    else if(input == "save")
        return 3;
    else if(input == "load")
        return 4;
    else if(input == "exit")
        return -1;

    // Then the input should be a move,
    // Check if the input is valid and if so, turn the string input into integer represented input accordingly

    // Check the length of the input
    if(input.length() != 4) {
        return 0;
    }

    // Check if alphabetical
    if(!isalpha(input[0]) || !isalpha(input[2])) {
        return 0;
    }

    // Check if it is in the bounds of the board
    if(input[0] < 'a' || input[0] > 'h' || input[2] < 'a' || input[2] > 'h') {
        return 0;
    }

    // Check the digits
    if((!isdigit(input[1]) || !isdigit(input[3]))) {
        return 0;
    }

    // The input looks valid (unless the numbers are out of bounds of the board), turn the values into integer representations.

    old_col = input[0] - 'a';
    old_row = '8' - input[1];

    new_col = input[2] - 'a';
    // example result: 'e' - 'a' = 4 which is the column number for 'e' in the board vector
    new_row = '8' - input[3];
    // 8 - input usage explanation: the board starts from row 0 in the vector, however the top row's number in the chess game is 8
    // to comply, subtract the input from 8, so that the input 8 becomes 0 for the row, which is the correct row for the board vector.

    // Check if the numbers are in the bounds of the board
    if(old_row < 0 || old_col < 0 || old_row > 7 || old_col > 7 || new_row < 0 || new_col < 0 || new_row > 7 || new_col > 7) {
        return 0;
    }

    // Check if the player is trying to move opponent's piece
    int pieceToBeMovedColor = board[old_row][old_col].getColor();
    if(status != pieceToBeMovedColor) {
        return 0;
    }

    // Input is a valid Chess notation
    return 1;
}


bool Board::movePiece(int old_row, int old_col, int new_row, int new_col) {
    // Check if the move is legal to do as a Chess move
    bool legal = isLegalMove(old_row, old_col, new_row, new_col);

    if(legal) {
        // Save the non-updated version of the board to be able to revert the move in case it's needed for other functions
        previousBoard = board;

        // Move the piece to new slot

        // Assign the old slot's features to the new one
        board[new_row][new_col] = board[old_row][old_col];
        // Change the has moved value of that piece
        board[new_row][new_col].setMoved(1);
        // Set the older location to be an empty slot
        board[old_row][old_col].makeEmpty();
    } else {
        return false;
    }
    return true;
}


void Board::revertMove() {
    // Assigns the previous board to the current board which makes the last move revert
    board = previousBoard;
}


bool Board::isLegalMove(int old_row, int old_col, int new_row, int new_col) const {
    int color = board[old_row][old_col].getColor();

    // Check for the possible moves of a piece
    switch (board[old_row][old_col].getType()) {
        case PieceType::Empty: {
            // Can't move an empty slot
            return false;
        }
        case PieceType::Pawn: {
            // Pawn can move 1 forward (or 2 forward if it has not been moved before) if it's path is clear.
            // It can move diagonally 1-1 only to capture enemy piece.
            // (isPathEmpty function is not used on purpose for pawns since their movement is different)
            PieceType new_slot_type = board[new_row][new_col].getType();

            if(new_slot_type == PieceType::Empty && old_col == new_col) {
                // Can move 2 squares forward if it has not been moved before
                if(new_row == old_row + (color == 0 ? -2 : 2)) {
                    if(board[old_row][old_col].gethasMoved() == 0
                       && board[old_row + (color == 0 ? -1 : 1)][old_col].getType() == PieceType::Empty) {
                        return true;
                    }
                    // Pawn can move 1 square forward in normal cases
                } else if(new_row == old_row + (color == 0 ? -1 : 1)) {
                    return true;
                }
                // Pawn can capture diagonally
            } else if((new_slot_type != PieceType::Empty)
                      && board[new_row][new_col].getColor() == (color == 0 ? 1 : 0)
                      && (new_col == old_col + 1 || new_col == old_col - 1)
                      && (new_row == old_row + (color == 0 ? -1 : 1))) {
                return true;
            }

            return false;
        }
        case PieceType::Rook: {
            // Can move straight, only row or col value can change with a single move
            if(!isPathEmpty(old_row, old_col, new_row, new_col))
                return false;
            int rowChange = old_row - new_row;
            int colChange = old_col - new_col;
            if((rowChange == 0 && colChange != 0) || (rowChange != 0 && colChange == 0))
                return true;

            return false;
        }
        case PieceType::Bishop: {
            // Can move diagonally, difference of row and col must be equal
            if(!isPathEmpty(old_row, old_col, new_row, new_col))
                return false;
            int rowChange = abs(old_row - new_row);
            int colChange = abs(old_col - new_col);
            if(rowChange == colChange && rowChange != 0 && colChange != 0)
                return true;

            return false;
        }
        case PieceType::Queen: {
            // Queen can move like Bishop and Rook combined
            if(!isPathEmpty(old_row, old_col, new_row, new_col))
                return false;
            int rowChange = abs(old_row - new_row);
            int colChange = abs(old_col - new_col);
            if( (rowChange == colChange && rowChange != 0 && colChange != 0) // Can move like Bishop
                || ((rowChange == 0 && colChange != 0) || (rowChange != 0 && colChange == 0)))
                return true;

            return false;
        }
        case PieceType::King: {
            if(!isPathEmpty(old_row, old_col, new_row, new_col))
                return false;
            int rowChange = abs(old_row - new_row);
            int colChange = abs(old_col - new_col);

            if((rowChange == 1 && colChange == 1) || (rowChange == 1 && colChange == 0)
               || (rowChange == 0 && colChange == 1) )
                return true;

            return false;
        }
        case PieceType::Knight: {
            // Can move like an L shape
            // No need for the path to be empty for Knight, it can "jump" over other pieces
            int rowChange = abs(old_row - new_row);
            int colChange = abs(old_col - new_col);

            PieceType new_slot_type = board[new_row][new_col].getType();
            int old_slot_color = board[old_row][old_col].getColor();
            int new_slot_color = board[new_row][new_col].getColor();

            if(((rowChange == 2 && colChange == 1) || (rowChange == 1 && colChange == 2))
               && ((new_slot_type == PieceType::Empty) || (new_slot_color != old_slot_color)) )
                return true;

            return false;
        }
        default: {
            return false;
        }
    }
}

bool Board::isPathEmpty(int old_row, int old_col, int new_row, int new_col) const {
    int oldColor = board[old_row][old_col].getColor();
    int newColor = board[new_row][new_col].getColor();

    // Check the path till the new slot
    int rowDifference = old_row - new_row;
    int colDifference = old_col - new_col;
    int rowChangeDirection = 0;
    int colChangeDirection = 0;

    if(rowDifference > 0) rowChangeDirection = -1;
    else if(rowDifference < 0) rowChangeDirection = 1;

    if(colDifference > 0) colChangeDirection = -1;
    else if(colDifference < 0) colChangeDirection = 1;

    if(rowDifference != 0 && colDifference != 0) {
        while(old_row != new_row && old_col != new_col) {
            // If only row or col is changed, then row/col change direction value will be 0, only the one that changed will change.
            old_row += rowChangeDirection;
            old_col += colChangeDirection;
            if(board[old_row][old_col].getType() != PieceType::Empty) {
                if(old_row == new_row && old_col == new_col && oldColor != newColor)
                    return true;
                return false;
            }
        }
    } else if(rowDifference != 0 /*&& colDifference == 0*/) {
        while(old_row != new_row) {
            old_row += rowChangeDirection;
            if(board[old_row][old_col].getType() != PieceType::Empty) {
                if(old_row == new_row && oldColor != newColor)
                    return true;
                return false;
            }
        }
    } else if(/*rowDifference == 0 &&*/ colDifference != 0) {
        while(old_col != new_col) {
            old_col += colChangeDirection;
            if(board[old_row][old_col].getType() != PieceType::Empty) {
                if(old_col == new_col && oldColor != newColor)
                    return true;
                return false;
            }
        }
    }


    return true;
}



int Board::isKingSafe(int colorOfKing) {
    /* Returns -2 if something is wrong (King not found)
     * Returns  0 for CHECK - the King is not safe and there is at least one move to save it
     * Returns  1 if the King is safe */


    // Find the current location of the King with the specified color
    int king_row = -1;
    int king_col = -1;

    for(int i=0; i<8; ++i) {
        for(int j=0; j<8; ++j) {
            if(board[i][j].getType() == PieceType::King && board[i][j].getColor() == colorOfKing) {
                king_row = i;
                king_col = j;
                break;
            }
        }
    }

    // Something is wrong, king not found
    if(king_row == -1 || king_col == -1)
        return -2;

    // Loop through all the pieces to see if any one has a legal move to the King
    for(int i=0; i<8; ++i) {
        for(int j=0; j<8; ++j) {
            if(board[i][j].getColor() != colorOfKing && board[i][j].getType() != PieceType::Empty) {
                if(isLegalMove(i,j,king_row,king_col)) {
                    // There is a legal move to King, it is not safe.
                    return 0;
                }
            }
        }
    }


    // No legal moves to King, return safe.
    return 1;
}

int Board::isCheckmate(int colorOfKing) {
    /* Returns -2 if something is wrong (King not found)
     * Returns -1 for CHECKMATE - if King is NOT safe and has no moves
     * Returns  0 if the King is not safe
     * Returns  1 if the King is safe */

    // Make sure the King is not safe before checking if it's checkmate
    if(isKingSafe(colorOfKing) == 0) {
        int king_row = -1;
        int king_col = -1;

        // Find the current location of the King with the specified color
        for(int i=0; i<8; ++i) {
            for(int j=0; j<8; ++j) {
                if(board[i][j].getType() == PieceType::King && board[i][j].getColor() == colorOfKing) {
                    king_row = i;
                    king_col = j;
                    break;
                }
            }
        }

        // Something is wrong, king not found
        if(king_row == -1 || king_col == -1)
            return -2;


        // Check every pieces' every next possible move and see if any move can block/stop the attack to the King
        // (this includes checking if King can escape by itself), if anything at all is possible, it is not checkmate, only check.

        for(int i=0; i < 8; ++i) {
            for (int j=0; j < 8; ++j) {
                if(board[i][j].getType() != PieceType::Empty && board[i][j].getColor() == colorOfKing) {
                    for(int k=0; k < 8; ++k) {
                        for(int m=0; m < 8; ++m) {
                            if(movePiece(i,j,k,m)) {
                                if(isKingSafe(colorOfKing)) {
                                    // King is safe with this move! Revert the move and return not checkmate, only check.
                                    revertMove();

                                    return 0;
                                }
                                // King is still not safe with this move, revert the move and continue the loop.
                                revertMove();
                            }
                        }
                    }
                }
            }
        }
        // Checkmate, none of the pieces' moves can save the King.
        return -1;
    } else {
        // King is safe, it can not be checkmate.
        return 1;
    }
}



bool Board::isPieceSafe(int row, int col, int color) const {
    /* Precondition: This function assumes the given row and col is NOT empty.
     * Returns 0 if the piece on specified row and col is under attack by any opponent piece
     * Returns 1 if the piece is safe
     * Function is used for calculateScore function. */

    for(int i=0; i<8; ++i) {
        for(int j=0; j<8; ++j) {
            if(board[i][j].getColor() != color && board[i][j].getType() != PieceType::Empty) {
                if(isLegalMove(i,j,row,col)) {
                    // Some opponent piece has a valid move to this piece, piece not safe
                    return false;
                }
            }
        }
    }

    // Piece is safe
    return true;
}

double Board::calculateScore(int color) {
    // Calculates the overall goodness score of the specified color's pieces.
    // Add points for each piece that exists, reduce half of the piece's point if piece is not safe

    double score = 0;

    const double pawnScore = 1.0;
    const double knightScore = 3.0;
    const double bishopScore = 3.0;
    const double rookScore = 5.0;
    const double queenScore = 9.0;
    const double kingScore = 500.0; // Prioritize King's safety

    PieceType type;
    int currentPieceColor;

    // Go through all the slots on the board
    for(int k=0;k<8;++k) {
        for(int m=0;m<8;++m) {
            type = board[k][m].getType();
            currentPieceColor = board[k][m].getColor();

            // Only check the specified color's pieces
            if (currentPieceColor == color) {
                switch (type) {
                    case PieceType::Empty:
                        break;
                    case PieceType::Pawn:
                        score += pawnScore;
                        if (isPieceSafe(k, m, color) == 0)
                            score -= pawnScore / 2.0;
                        break;
                    case PieceType::Rook:
                        score += rookScore;
                        if (isPieceSafe(k, m, color) == 0)
                            score -= rookScore / 2.0;
                        break;
                    case PieceType::Knight:
                        score += knightScore;
                        if (isPieceSafe(k, m, color) == 0)
                            score -= knightScore / 2.0;
                        break;
                    case PieceType::Bishop:
                        score += bishopScore;
                        if (isPieceSafe(k, m, color) == 0)
                            score -= bishopScore / 2.0;
                        break;
                    case PieceType::Queen:
                        score += queenScore;
                        if (isPieceSafe(k, m, color) == 0)
                            score -= queenScore / 2.0;
                        break;
                    case PieceType::King:
                        //score += kingScore;
                        if(isPieceSafe(k, m, color) == 0)
                            score -= kingScore;
                        break;
                    default:
                        break;
                }
            }
        }
    }



    return score;
}

void Board::suggestMove(int color) {
    int best_old_row, best_old_col, best_new_row, best_new_col;

    // Start the score from the min integer since it's looking for HIGHER scores.
    double highestScore = INT_MIN;
    double currentScore;

    // If there are multiple scores with the same score values, save them here.
    vector<Move> bestMoves;

    // Create a seed for the rand() function
    std::srand(time(nullptr));

    // Loop through every piece
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // When a piece is found from the same color
            if (board[i][j].getType() != PieceType::Empty && board[i][j].getColor() == color) {
                // Check for all the legal moves of the piece and save the one with the best possible score
                for (int k = 0; k < 8; ++k) {
                    for (int m = 0; m < 8; ++m) {
                        // Make this move and calculate the score of it
                        if(movePiece(i,j,k,m)) {
                            currentScore = calculateScore(color);
                            if (currentScore >= highestScore) {
                                if (currentScore > highestScore) {
                                    // Clear the previous saved low scores, there is a lower one
                                    highestScore = currentScore;
                                    bestMoves.clear();
                                }
                                // Push the equal score producing moves into the bestMoves vector
                                bestMoves.push_back({ i, j, k, m });
                            }

                            // Revert the move before continuing for other moves
                            revertMove();
                        }
                    }
                }
            }
        }
    }

    int rand = std::rand() % bestMoves.size();
    best_old_row = bestMoves[rand].old_row;
    best_old_col = bestMoves[rand].old_col;
    best_new_row = bestMoves[rand].new_row;
    best_new_col = bestMoves[rand].new_col;


    // Print the Chess notation of the suggested move
    cout << "Suggested move: "
         << static_cast<char>(best_old_col + 'a')
         << 8 - best_old_row
         << static_cast<char>(best_new_col + 'a')
         << 8 - best_new_row << endl;
}



void Board::saveToFile(int turn) const {
    // Precondition: This function assumes that there is a folder  named "saves" in the same directory of the project.
    // Saves the current layout of the Chess board into a txt file.

    srand(time(nullptr));
    int random4digit = (std::rand() % 9000) + 1000;
    string fileID = std::to_string(random4digit);
    string fileName = "saves/" + fileID + ".txt";


    // Check if this ID is already used
    ifstream file(fileName.c_str());
    if (file.is_open()) {
        cout << "A problem occurred while saving the board. You can try \"save\" again.\n";
        return;
    }
    file.close();

    ofstream outputStream(fileName.c_str());
    if (!outputStream.is_open()) {
        cout << "A problem occurred while saving the board. You can try \"save\" again.\n";
        return;
    }

    // File is open

    // Write whose turn is it while saving the board to start of the file
    outputStream << turn << "\n";

    int typeInt;
    for(int i=0;i<8;++i) {
        for(int j=0;j<8;++j) {
            // Write row, col type, color to each line for each piece
            // Cast the type PieceType enum class into it's integer representation and save it that way
            typeInt = static_cast<int>(board[i][j].getType());
            if(typeInt != 6) // skip Empty slots, they will be defaulted when loading
                outputStream << i << "," << j << "," << typeInt << "," << board[i][j].getColor() << "\n";

        }
    }

    cout << "Game save file is created. It be loaded with the ID: " << fileID << "\n\n";

    outputStream.close();
}

int Board::loadFromFile() {
    // Precondition: This function assumes that there is a folder named "saves" in the same directory of the project.
    // Loads a previously saved board to the current game.
    // If the load was successful, returns whose turn is it (0 for white 1 for black), otherwise returns -1.
    string fileName;

    cout << "Enter a save file ID: ";
    getline(std::cin, fileName);

    fileName = "saves/" + fileName + ".txt";

    // Check if the file can be opened for reading
    ifstream inputStream(fileName.c_str());
    if (!inputStream.is_open()) {
        cout << "Can't find the save file. Make sure the ID is correct. Example input: 1516\n"
                "Please write \"load\" again if you wish to try again.\n";
        return -1;
    }


    // File is open to read

    // Clear the current board to load the new one to it
    clearBoard();

    int whoseTurn = -1;
    int rowValue, colValue, typeValue, colorValue;
    char c;

    // Read the turn which is at the start of the file
    inputStream >> whoseTurn;

    // Read the pieces locations and type, values are stored as row, col, type, color
    while(inputStream.good() && inputStream >> rowValue >> c >> colValue >> c >> typeValue >> c >> colorValue) {
        // Update the board according to the values read.
        board[rowValue][colValue].setType(typeValue);
        board[rowValue][colValue].setColor(colorValue);
    }

    // Update the previousBoard to be the same as the new created board.
    previousBoard = board;

    cout << "Board loaded from the specified save successfully.\n\n";

    return whoseTurn;
}
