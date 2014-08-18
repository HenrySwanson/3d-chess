#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include "piece.h"

enum MoveType {
    QUIET, DOUBLE_PAWN_PUSH, CAPTURE, EN_PASSANT, CASTLE, PROMOTE,
    PROMO_CAPTURE
};

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
