#include "../src/piece.h"

#include "unit_test.h"

void constructor_test(PieceType pt, bool color, bool moved)
{
    Piece p (pt, color, moved);
    EXPECT_TRUE(pt == p.getType());
    EXPECT_TRUE(color == p.getColor());
    EXPECT_TRUE(moved == p.getMoved());
}

TEST(Piece, Constructor)
{
    constructor_test(W_PAWN, WHITE, true);
    constructor_test(B_PAWN, BLACK, false);
    constructor_test(KNIGHT, WHITE, false);
    constructor_test(ROOK, BLACK, true);
}
