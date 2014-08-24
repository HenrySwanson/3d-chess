#include "../src/piece.h"

#include "unit_test.h"

#include <iostream>
#include <cassert> //TODO change to some kind of TEST_ASSERT

using std::cout;
using std::endl;

int constructor_test(PieceType pt, bool color, bool moved)
{
    Piece p (pt, color, moved);
    assert(pt == p.getType());
    assert(color == p.getColor());
    assert(moved == p.getMoved());
}

TEST(Piece, Pawn)
{
    constructor_test(W_PAWN, WHITE, true);
    constructor_test(B_PAWN, BLACK, false);
    constructor_test(KNIGHT, WHITE, false);
    constructor_test(ROOK, BLACK, true);
}
