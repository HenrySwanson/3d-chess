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

struct Piece
{
    PieceType type;
    bool color;
    bool moved;
};

#endif
