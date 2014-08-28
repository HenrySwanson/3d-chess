#include "piece.h"

Piece::Piece() : type_(NIL), color_(WHITE)
{}

Piece::Piece(PieceType pt, bool color) : type_(pt), color_(color)
{}

PieceType Piece::type() const
{
    return type_;
}

bool Piece::color() const
{
    return color_;
}

bool Piece::isOn(bool color) const
{
    return (type_ != NIL) && (type_ != BORDER) && (color_ == color);
}

bool Piece::isFriend(Piece p) const
{
    return p.isOn(color_);
}

bool Piece::isEnemy(Piece p) const
{
    return p.isOn(!color_);
}

bool Piece::operator==(const Piece& p) const
{
    return (type_ == p.type_) && (color_ == p.color_);
}

bool Piece::operator!=(const Piece& p) const
{
    return !(*this == p);
}
