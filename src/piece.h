#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

/**
 * A list of all piece types (and lack thereof). The value BORDER is special;
 * see the comments for "Board.pieces_" for more details. White and black pawns
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

/** The value corresponding to a white piece. */
const bool WHITE = false;

/** The value corresponding to a black piece. */
const bool BLACK = true;

/* TODO I'll make this a bitfield later.
 *
 * Bits 1-4 - type
 * Bit 5 - color
 * Bit 6 - moved
 */

/**
 * Represents a chess piece, including its color and whether it has moved.
 * Can also represent the lack of a piece, with the values NIL and BORDER.
 */
class Piece
{
  public:
    /** Constructs a piece representing an empty space. */
    Piece();

    /** Constructs a piece with the given data. */
    Piece(PieceType pt, bool color, bool moved);


    /** Returns this piece's type. */
    PieceType type() const;

    /** Returns this piece's color. */
    bool color() const;

    /** Returns true if this piece has moved. */
    bool moved() const;

    /**
     * Returns true if this piece is neither NIL nor BORDER, and it has the
     * specified color.
     */
    bool isOn(bool color) const;

    /**
     * Returns true if this piece is neither NIL nor BORDER, and it has the
     * same color as the specified piece.
     */
    bool isFriend(Piece p) const;

    /**
     * Returns true if this piece is neither NIL nor BORDER, and it has the
     * opposite color as the specified piece.
     */
    bool isEnemy(Piece p) const;


    bool operator==(const Piece& p) const;
    bool operator!=(const Piece& p) const;

  private:
    /** The type of this piece. */
    PieceType type_;

    /** The color of this piece. */
    bool color_;

    /** Whether this piece has moved. */
    bool moved_;
};

#endif
