#include "../src/piece.h"

#include "unit_test.h"

void constructor_test(PieceType pt, bool color)
{
    Piece p (pt, color);
    EXPECT_EQ(pt, p.type());
    EXPECT_EQ(color, p.color());
}

TEST(Piece, Constructor)
{
    constructor_test(W_PAWN, WHITE);
    constructor_test(B_PAWN, BLACK);
    constructor_test(KNIGHT, WHITE);
    constructor_test(ROOK, BLACK);
}
