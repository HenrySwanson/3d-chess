#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include "piece.h"

/**
 * A list of all types of moves. Quiet moves are normal moves that don't alter
 * any material. Double pawn pushes are when a pawn moves forward two squares.
 * Capture, en passant, castle, and promote are self-explanatory. The last
 * case is promo-capture, which is rare, but possible, and has to be coded
 * differently from promotion and capture.
 */
enum MoveType {
    QUIET, DOUBLE_PAWN_PUSH, CAPTURE, EN_PASSANT, CASTLE, PROMOTE,
    PROMO_CAPTURE
};

/**
 * As it stands, I have three ways of encoding this into a bitfield.
 *
 * Way 1:
 *   If bit 1 is set, this is a promo-capture.
 *     Bit 2 - Color of pawn
 *     Bits 3-13 - Origin square
 *     Bits 14-17 - Displacement from target to origin, as encoded in PIECE_DIRECTIONS
 *     Bits 18-23 - Promoted piece
 *     Bits 24-29 - Captured piece
 *   Else, it is a different move.
 *     Bits 2-4 Move type
 *     Bits 5-15 Origin square
 *     Bits 16-26 Target square
 *     Bits 27-32 Captured/promoted piece
 *
 * Way 2: Same as Way 2: but instead of origin and displacement, we have
 *  origin_x_and_y and target_x_and_y, which can range from 0 to 143 (8 bits!).
 *  Then find some way of encoding which rank (maybe just a single bit!).
 *
 * Way 3:
 *   We unmailbox our squares, so they only take up 9 bits now.
 *   Bits 1-9 - Origin square
 *   Bits 10-18 - Target square
 *   Bits 19-24 - Captured piece
 *   Bits 25-28 - Promoted piece type
 *   Bits 29-31 - Move type
 *
 * TODO decide which to use!
 */

// XXX how am i supposed to correctly undo a move following an en passant? I
// can't just restore the ep square, because I wouldn't know what it is...
// Possible solution: have makeMove(Move), and undo(). I'd have to have a
// stack containing all the moves made so far, but that would actually be just
// fine. Plus, I could remove "captured" from Move, and just throw those in a
// stack as well.

/**
 * Contains all the data necessary to make and unmake a chess move.
 */
class Move
{
  public:
    /** Constructs a quiet move from origin to target. */
    static Move Quiet(int origin, int target);

    /** Constructs a double pawn push from origin to target. */
    static Move DPP(int origin, int target);

    /** Constructs a capturing move from origin to target. */
    static Move Capture(int origin, int target);

    /** Constructs an en passant move from origin to target. */
    static Move EP(int origin, int target);

    /** Constructs a castling move from origin to target. */
    static Move Castle(int origin, int target);

    /**
     * Constructs a promotion move from origin to target, with the pawn
     * becoming the specified piece.
     */
    static Move Promote(int origin, int target, Piece promo);

    /**
     * Constructs a promo-capture from origin to target, where the promoted-to
     * piece is specified.
     */
    static Move PromoCapture(int origin, int target, Piece promo);


    /** Returns the type of this move. */
    MoveType type() const;

    /** Returns the origin square of this move. */
    int origin() const;

    /** Returns the target square of this move. */
    int target() const;

    /** Returns the piece promoted to during this move. */
    Piece promoted() const;

    bool operator==(const Move& m) const;
    bool operator!=(const Move& m) const;

  private:
    /** The type of this move. */
    MoveType type_;

    /** The origin square of this move. */
    int origin_;

    /** The target square of this move. */
    int target_;

    /** The piece promoted to during this move. */
    Piece promoted_;
};

/**
 * Converts from an 8 x 8 x 8 cube to a 1728 array. Like the Board.pieces_
 * array, x-coordinates vary first, then y, then z.
 */
int mailbox(int x, int y, int z);

/*
 * Note that it's just as cheap to have the unmailboxing functions separate
 * as to have them together.
 */

/** Converts from a 1728 array to the x-coordinate of an 8 x 8 x 8 cube. */
int unmailboxX(int i);

/** Converts from a 1728 array to the y-coordinate of an 8 x 8 x 8 cube. */
int unmailboxY(int i);

/** Converts from a 1728 array to the z-coordinate of an 8 x 8 x 8 cube. */
int unmailboxZ(int i);

#endif
