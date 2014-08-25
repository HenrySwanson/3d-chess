#include "piece.h"

Piece::Piece() : _type(NIL), _color(WHITE), _moved(false)
{}

Piece::Piece(PieceType pt, bool color, bool moved) : _type(pt), _color(color),
         _moved(moved)
{}

PieceType Piece::getType() const
{
    return _type;
}

bool Piece::getColor() const
{
    return _color;
}

bool Piece::getMoved() const
{
    return _moved;
}

bool Piece::isOn(bool color) const
{
    return (_type != NIL) && (_type != BORDER) && (_color == color);
}

bool Piece::isFriend(Piece p) const
{
    return (p._type != NIL) && (p._type != BORDER) && (p._color == _color);
}

bool Piece::isEnemy(Piece p) const
{
    return (p._type != NIL) && (p._type != BORDER) && (p._color != _color);
}

bool Piece::isEmpty() const
{
    return (_type == NIL);
}

bool Piece::operator==(const Piece& p) const
{
    return (_type == p._type) && (_color == p._color) && (_moved == p._moved);
}

bool Piece::operator!=(const Piece& p) const
{
    return !(*this == p);
}
