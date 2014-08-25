#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include "piece.h"

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

class Move { // TODO write docs
  public:
    static Move Quiet(int origin, int target);
    static Move DPP(int origin, int target);
    static Move Capture(int origin, int target, Piece capt);
    static Move EP(int origin, int target);
    static Move Castle(int origin, int target);
    static Move Promote(int origin, int target, Piece promo);
    static Move PromoCapture(int origin, int target, Piece promo, Piece capt);

    MoveType type() const;
    int origin() const;
    int target() const;
    Piece captured() const;
    Piece promoted() const;
  private:
    MoveType type_;
    int origin_;
    int target_;
    Piece captured_;
    Piece promoted_;
};

/**
 * Converts from an 8 x 8 x 8 cube to a 1728 array. Like the Board.pieces_
 * array, x-coordinates vary first, then y, then z.
 */
int mailbox(int x, int y, int z);

/**
 * Converts from a 1728 array to an 8 x 8 x 8 cube. Like the Board.pieces_
 * array, x-coordinates vary first, then y, then z.
 * Note that it's just as cheap to have these separate as together.
 */
int unmailboxX(int i);
int unmailboxY(int i);
int unmailboxZ(int i);

#endif
