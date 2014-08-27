#include "move.h"

Move Move::Quiet(int origin, int target)
{
    Move m;
    m.type_ = QUIET;
    m.origin_ = origin;
    m.target_ = target;
    return m;
}

Move Move::DPP(int origin, int target)
{
    Move m;
    m.type_ = DOUBLE_PAWN_PUSH;
    m.origin_ = origin;
    m.target_ = target;
    return m;
}

Move Move::Capture(int origin, int target, Piece capt)
{
    Move m;
    m.type_ = CAPTURE;
    m.origin_ = origin;
    m.target_ = target;
    m.captured_ = capt;
    return m;
}

Move Move::EP(int origin, int target)
{
    Move m;
    m.type_ = EN_PASSANT;
    m.origin_ = origin;
    m.target_ = target;
    return m;
}

Move Move::Castle(int origin, int target)
{
    Move m;
    m.type_ = CASTLE;
    m.origin_ = origin;
    m.target_ = target;
    return m;
}

Move Move::Promote(int origin, int target, Piece promo)
{
    Move m;
    m.type_ = PROMOTE;
    m.origin_ = origin;
    m.target_ = target;
    m.promoted_ = promo;
    return m;
}

Move Move::PromoCapture(int origin, int target, Piece promo, Piece capt)
{
    Move m;
    m.type_ = PROMO_CAPTURE;
    m.origin_ = origin;
    m.target_ = target;
    m.promoted_ = promo;
    m.captured_ = capt;
    return m;
}

MoveType Move::type() const
{
    return type_;
}

int Move::origin() const
{
    return origin_;
}

int Move::target() const
{
    return target_;
}

Piece Move::captured() const
{
    return captured_;
}

Piece Move::promoted() const
{
    return promoted_;
}

bool Move::operator==(const Move& m) const
{
    bool match = (type_ == m.type_) && (origin_ == m.origin_) &&
            (target_ == m.target_);

    if(type_ == CAPTURE || type_ == PROMO_CAPTURE)
        match &= (captured_ != m.captured_);

    if(type_ == PROMOTE || type_ == PROMO_CAPTURE)
        match &= (promoted_ != m.promoted_);

    return match;
}

bool Move::operator!=(const Move& m) const
{
    return !(*this == m);
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
