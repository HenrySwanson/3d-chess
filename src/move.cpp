#include "move.h"

Move createQuietMove(int origin, int target)
{
    Move m;
    m.type = QUIET;
    m.from = origin;
    m.to = target;
    return m;
}

Move createDPPMove(int origin, int target)
{
    Move m;
    m.type = DOUBLE_PAWN_PUSH;
    m.from = origin;
    m.to = target;
    return m;
}

Move createCaptureMove(int origin, int target, Piece capt)
{
    Move m;
    m.type = CAPTURE;
    m.from = origin;
    m.to = target;
    m.captured = capt;
    return m;
}

Move createEPMove(int origin, int target)
{
    Move m;
    m.type = EN_PASSANT;
    m.from = origin;
    m.to = target;
    return m;
}

Move createCastleMove(int origin, int target)
{
    Move m;
    m.type = CASTLE;
    m.from = origin;
    m.to = target;
    return m;
}

Move createPromoteMove(int origin, int target, Piece promo)
{
    Move m;
    m.type = PROMOTE;
    m.from = origin;
    m.to = target;
    m.promoted = promo;
    return m;
}

Move createPromoCaptureMove(int origin, int target, Piece promo, Piece capt)
{
    Move m;
    m.type = PROMO_CAPTURE;
    m.from = origin;
    m.to = target;
    m.promoted = promo;
    m.captured = capt;
    return m;
}
