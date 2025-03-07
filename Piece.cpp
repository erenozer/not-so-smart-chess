#include "Piece.h"

Piece::Piece(PieceType typeVal, int colorVal, int hasMovedVal) : type(typeVal), color(colorVal), hasMoved(hasMovedVal) {
    // Intentionally left blank
}

PieceType Piece::getType() const {
    return type;
}

int Piece::getColor() const {
    return color;
}

int Piece::gethasMoved() const {
    return hasMoved;
}

char Piece::getSymbol() const {
    // Returns the appropriate symbol representation of the piece to print the board
    switch(getType()) {
        // break statements are not used in switch intentionally because the function will end with the return statements in all the cases
        case PieceType::Pawn: {
            if(getColor() == 0) return 'P';
            else return 'p';
        }
        case PieceType::Rook: {
            if(getColor() == 0) return 'R';
            else return 'r';
        }
        case PieceType::Knight: {
            if(getColor() == 0) return 'N';
            else return 'n';
        }
        case PieceType::Bishop: {
            if(getColor() == 0) return 'B';
            else return 'b';
        }
        case PieceType::Queen: {
            if(getColor() == 0) return 'Q';
            else return 'q';
        }
        case PieceType::King: {
            if(getColor() == 0) return 'K';
            else return 'k';
        }
        case PieceType::Empty: {
            return '.';
        }
        default:
            return '.';
    }
}

void Piece::setType(PieceType& newType) {
    type = newType;
}

void Piece::setType(int newType) {
    // Sets the piece type from an int value
    switch (newType) {
        case 0:
            type = PieceType::Pawn;
            break;
        case 1:
            type = PieceType::Rook;
            break;
        case 2:
            type = PieceType::Knight;
            break;
        case 3:
            type = PieceType::Bishop;
            break;
        case 4:
            type = PieceType::Queen;
            break;
        case 5:
            type = PieceType::King;
            break;
        case 6:
            type = PieceType::Empty;
            break;
        default:
            type = PieceType::Empty;
    }
}

void Piece::setColor(int newColor) {
    color = newColor;
}

void Piece::setMoved(int newStatus) {
    hasMoved = newStatus;
}

void Piece::makeEmpty() {
    hasMoved = 0;
    color = 0;
    type = PieceType::Empty;
}
