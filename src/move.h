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

struct Move {
    MoveType type;
    int from, to;
    Piece captured;
    Piece promoted;
};

/** Constructs a quiet move from origin to target. */
Move createQuietMove(int origin, int target);

/** Constructs a double pawn push from origin to target. */
Move createDPPMove(int origin, int target);

/** Constructs a capturing move from origin to target. */
Move createCaptureMove(int origin, int target, Piece capt);

/** Constructs an en-passant move from origin to target. */
Move createEPMove(int origin, int target);

/** Constructs a castling move from origin to target. */
Move createCastleMove(int origin, int target);

/**
 * Constructs a promotion move from origin to target, where the pawn becomes
 * the piece indicated by promo.
 */
Move createPromoteMove(int origin, int target, Piece promo);

/**
 * Constructs a promo-capture move from origin to target, where the pawn
 * becomes the piece indicated by promo, and the captured piece is capt.
 */
Move createPromoCaptureMove(int origin, int target, Piece promo, Piece capt);

#endif
