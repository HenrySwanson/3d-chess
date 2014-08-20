#include "piece.h"

Piece::Piece() : _type(NIL), _color(WHITE), _moved(false)
{}

Piece::Piece(PieceType pt, bool color, bool moved) : _type(pt), _color(color),
         _moved(moved)
{}

PieceType Piece::getType()
{
    return _type;
}

bool Piece::getColor()
{
    return _color;
}

bool Piece::getMoved()
{
    return _moved;
}

bool Piece::isFriend(Piece p)
{
    return (p._type != NIL) && (p._type != BORDER) && (p._color == _color);
}

bool Piece::isEnemy(Piece p)
{
    return (p._type != NIL) && (p._type != BORDER) && (p._color != _color);
}

bool Piece::isEmpty()
{
    return (_type == NIL);
}
