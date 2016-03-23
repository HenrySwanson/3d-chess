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

// TODO maybe add castling axis?

/**
 * Represents a chess move. Does not contain all of the necessary data to
 * unmake a move, but the Board class contains everything that Move lacks.
 */
class Move
{
  public:
    /** Constructs a nil move. This indicates something unusual, like end-of-game. */
    Move();

    /**
     * Constructs a move of the given type for the given color, where origin is
     * the square from which the piece departs, and target is where it arrives.
     *
     * Cannot be used with PROMOTE and PROMO_CAPTURE.
     */
    Move(bool color, MoveType type, int origin, int target);

    /**
     * Just like the Move(bool, MoveType, int, int) constructor, but also
     * specifies a piece type that is promoted to.
     *
     * Can only be used with PROMOTE and PROMO_CAPTURE.
     */
    Move(bool color, MoveType type, int origin, int target, PieceType promo);


    /** Returns the color of the team making this move. */
    int color() const;

    /** Returns the type of this move. */
    MoveType type() const;

    /** Returns the origin square of this move. */
    int origin() const;

    /** Returns the target square of this move. */
    int target() const;

    /**
     * Returns the piece promoted to during this move. Undefined if this was
     * constructed with the Move(bool, MoveType, int, int) constructor.
     */
    PieceType promoted() const;


    /** Operators */
    bool operator==(const Move& m) const;
    bool operator!=(const Move& m) const;

  private:
    /**
     * A bitfield. Where LSB is Bit 0,
     *   Bits 0-2  - type
     *   Bit  3    - color
     *   Bits 4-14 - origin
     *   Bits 15-25 - target
     *   Bits 26-28 - promoted type
     */
    unsigned int data_;
};

#endif
