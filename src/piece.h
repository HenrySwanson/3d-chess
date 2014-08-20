#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

/**
 * A list of all pieces (and lack thereof). The value BORDER is a bit special;
 * see the comments for "Board.pieces" for more details. White and black pawns
 * are different codes for two reasons. First, it means that (standard)
 * movements are entirely determined by PieceType. Second, it gives us an even
 * 16 types, which fit perfectly into a nibble.
 */
enum PieceType {
    NIL, BORDER, W_PAWN, B_PAWN,
    KNIGHT, GRIFFIN, DRAGON, UNICORN,
    ROOK, BISHOP, MACE,
    WIZARD, ARCHER, CANNON,
    QUEEN, KING
};

const bool WHITE = false;
const bool BLACK = true;

/**
 * I'll make this a bitfield later.
 *
 * Bits 1-4 - type
 * Bit 5 - color
 * Bit 6 - moved
 */

class Piece
{
public: // TODO write docs
    Piece();
    Piece(PieceType pt, bool color, bool moved);

    PieceType getType();
    bool getColor();
    bool getMoved();

    bool isFriend(Piece p);
    bool isEnemy(Piece p);
    bool isEmpty();
private:
    PieceType _type;
    bool _color;
    bool _moved;
};

#endif
