#include "piece.h"

Piece::Piece() : type_(NIL), color_(WHITE), moved_(false)
{}

Piece::Piece(PieceType pt, bool color, bool moved) : type_(pt), color_(color),
         moved_(moved)
{}

PieceType Piece::type() const
{
    return type_;
}

bool Piece::color() const
{
    return color_;
}

bool Piece::moved() const
{
    return moved_;
}

bool Piece::isOn(bool color) const
{
    return (type_ != NIL) && (type_ != BORDER) && (color_ == color);
}

bool Piece::isFriend(Piece p) const
{
    return (p.type_ != NIL) && (p.type_ != BORDER) && (p.color_ == color_);
}

bool Piece::isEnemy(Piece p) const
{
    return (p.type_ != NIL) && (p.type_ != BORDER) && (p.color_ != color_);
}

bool Piece::isEmpty() const
{
    return (type_ == NIL);
}

bool Piece::operator==(const Piece& p) const
{
    return (type_ == p.type_) && (color_ == p.color_) && (moved_ == p.moved_);
}

bool Piece::operator!=(const Piece& p) const
{
    return !(*this == p);
}
