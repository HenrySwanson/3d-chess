#include "../src/piece.h"

#include "unit_test.h"

void constructor_test(PieceType pt, bool color, bool moved)
{
    Piece p (pt, color, moved);
    EXPECT_TRUE(pt == p.type());
    EXPECT_TRUE(color == p.color());
    EXPECT_TRUE(moved == p.moved());
}

TEST(Piece, Constructor)
{
    constructor_test(W_PAWN, WHITE, true);
    constructor_test(B_PAWN, BLACK, false);
    constructor_test(KNIGHT, WHITE, false);
    constructor_test(ROOK, BLACK, true);
}
