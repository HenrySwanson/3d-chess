#include "move.h"

Move Move::Quiet(int origin, int target)
{
    Move m;
    m._type = QUIET;
    m._origin = origin;
    m._target = target;
    return m;
}

Move Move::DPP(int origin, int target)
{
    Move m;
    m._type = DOUBLE_PAWN_PUSH;
    m._origin = origin;
    m._target = target;
    return m;
}

Move Move::Capture(int origin, int target, Piece capt)
{
    Move m;
    m._type = CAPTURE;
    m._origin = origin;
    m._target = target;
    m._captured = capt;
    return m;
}

Move Move::EP(int origin, int target)
{
    Move m;
    m._type = EN_PASSANT;
    m._origin = origin;
    m._target = target;
    return m;
}

Move Move::Castle(int origin, int target)
{
    Move m;
    m._type = CASTLE;
    m._origin = origin;
    m._target = target;
    return m;
}

Move Move::Promote(int origin, int target, Piece promo)
{
    Move m;
    m._type = PROMOTE;
    m._origin = origin;
    m._target = target;
    m._promoted = promo;
    return m;
}

Move Move::PromoCapture(int origin, int target, Piece promo, Piece capt)
{
    Move m;
    m._type = PROMO_CAPTURE;
    m._origin = origin;
    m._target = target;
    m._promoted = promo;
    m._captured = capt;
    return m;
}

MoveType Move::getType() const
{
    return _type;
}

int Move::getOrigin() const
{
    return _origin;
}

int Move::getTarget() const
{
    return _target;
}

Piece Move::getCaptured() const
{
    return _captured;
}

Piece Move::getPromoted() const
{
    return _promoted;
}

int mailbox(int x, int y, int z)
{
    return (z + 2) * 144 + (y + 2) * 12 + (x + 2);
    //return x + 12 * y + 144 * z + 314;
}

int unmailboxX(int i)
{
    return (i % 12) - 2;
}

int unmailboxY(int i)
{
    return (i / 12) % 12 - 2;
}

int unmailboxZ(int i)
{
    return (i / 144) % 12 - 2;
}
