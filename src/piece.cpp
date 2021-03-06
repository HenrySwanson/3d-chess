#include "piece.h"

Piece::Piece() : data_(0)
{}

Piece::Piece(PieceType pt, bool color) : data_(pt ^ (color << 4))
{}

Piece Piece::Pawn(bool color)
{
    return Piece(color == WHITE ? W_PAWN : B_PAWN, color);
}

PieceType Piece::type() const
{
    return static_cast<PieceType>(data_ & 0xF);
}

bool Piece::color() const
{
    return (bool) (data_ >> 4);
}

bool Piece::isOn(bool color) const
{
    return (type() != NIL) && (type() != BORDER) && (this->color() == color);
}

bool Piece::isFriend(const Piece& p) const
{
    return p.isOn(color());
}

bool Piece::isEnemy(const Piece& p) const
{
    return p.isOn(!color());
}

bool Piece::operator==(const Piece& p) const
{
    return (data_ == p.data_);
}

bool Piece::operator!=(const Piece& p) const
{
    return !(*this == p);
}
