#include "piece.h"

Piece createPiece(PieceType pt, bool color, bool moved)
{
    Piece p;
    p.type = pt;
    p.color = color;
    p.moved = moved;
    return p;
}
