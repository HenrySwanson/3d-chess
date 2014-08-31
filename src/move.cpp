#include "move.h"

Move Move::Quiet(int origin, int target)
{
    Move m;
    m.data_ = QUIET | (origin << 3) | (target << 14);
    return m;
}

Move Move::DPP(int origin, int target)
{
    Move m;
    m.data_ = DOUBLE_PAWN_PUSH | (origin << 3) | (target << 14);
    return m;
}

Move Move::Capture(int origin, int target)
{
    Move m;
    m.data_ = CAPTURE | (origin << 3) | (target << 14);
    return m;
}

Move Move::EP(int origin, int target)
{
    Move m;
    m.data_ = EN_PASSANT | (origin << 3) | (target << 14);
    return m;
}

Move Move::Castle(int origin, int target)
{
    Move m;
    m.data_ = CASTLE | (origin << 3) | (target << 14);
    return m;
}

Move Move::Promote(int origin, int target, PieceType promo)
{
    Move m;
    m.data_ = PROMOTE | (origin << 3) | (target << 14) | (promo << 25);
    return m;
}

Move Move::PromoCapture(int origin, int target, PieceType promo)
{
    Move m;
    m.data_ = PROMO_CAPTURE | (origin << 3) | (target << 14) | (promo << 25);
    return m;
}

MoveType Move::type() const
{
    return static_cast<MoveType>(data_ & 0x7);
}

int Move::origin() const
{
    return (data_ >> 3) & 0x7FF;
}

int Move::target() const
{
    return (data_ >> 14) & 0x7FF;
}

PieceType Move::promoted() const
{
    return static_cast<PieceType>((data_ >> 25) & 0xF);
}

bool Move::operator==(const Move& m) const
{
    return (data_ == m.data_);
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
