#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

enum class PieceType {
    Pawn, Rook, Knight, Bishop, Queen, King, Empty
};

class Piece {
public:
    // Constructor with defaulted values for all private data members
    explicit Piece(PieceType type=PieceType::Empty, int color=0, int hasMoved=0);

    // Getters for the private data members
    PieceType getType() const;
    int getColor() const;
    int gethasMoved() const;
    char getSymbol() const;

    // Setters for the private data members
    void setType(PieceType& newType);
    void setType(int newType);
    void setColor(int newColor);
    void setMoved(int newStatus);

    // Updates the private data members of a piece as a default (empty) cell all at once.
    void makeEmpty();
private:
    PieceType type;
    int color; // Holds 0 for white and 1 for black pieces
    int hasMoved; // Starts 0, turns into 1 when the piece is moved
};


#endif //CHESS_PIECE_H
