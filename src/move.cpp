#include "move.h"

#include <cassert>

Move::Move()
{
    data_ = 0;
}

Move::Move(bool color, MoveType type, int origin, int target)
{
    assert(type != PROMOTE && type != PROMO_CAPTURE);
    data_ = (type) | (color << 3) | (origin << 4) | (target << 15);
}

Move::Move(bool color, MoveType type, int origin, int target, PieceType promo)
{
    assert(type == PROMOTE || type == PROMO_CAPTURE);
    data_ = (type) | (color << 3) | (origin << 4) | (target << 15) | (promo << 26);
}

MoveType Move::type() const
{
    return static_cast<MoveType>(data_ & 0x7);
}

int Move::color() const
{
    return (data_ >> 3) & 0x1;
}

int Move::origin() const
{
    return (data_ >> 4) & 0x7FF;
}

int Move::target() const
{
    return (data_ >> 15) & 0x7FF;
}

PieceType Move::promoted() const
{
    return static_cast<PieceType>((data_ >> 26) & 0xF);
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
