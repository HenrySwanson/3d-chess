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

// TODO maybe add color and castling axis?

/**
 * Represents a chess move. Does not contain all of the necessary data to
 * unmake a move, but the Board class contains everything that Move lacks.
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
    static Move Promote(int origin, int target, PieceType promo);

    /**
     * Constructs a promo-capture from origin to target, where the promoted-to
     * piece is specified.
     */
    static Move PromoCapture(int origin, int target, PieceType promo);


    /** Returns the type of this move. */
    MoveType type() const;

    /** Returns the origin square of this move. */
    int origin() const;

    /** Returns the target square of this move. */
    int target() const;

    /** Returns the piece promoted to during this move. */
    PieceType promoted() const;

    bool operator==(const Move& m) const;
    bool operator!=(const Move& m) const;

  private:
    /**
     * A bitfield. Where LSB is Bit 0,
     *   Bits 0-2 - type
     *   Bits 3-13 - origin
     *   Bits 14-24 - target
     *   Bits 25-28 - promoted type
     */
    unsigned int data_;
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
