#include "../src/piece.h"
#include "../src/move.h"

#include "unit_test.h"

void constructor_test(PieceType pt, bool color)
{
    Piece p (pt, color);
    EXPECT_EQ(pt, p.type());
    EXPECT_EQ(color, p.color());
}

void constructor_test(bool color, MoveType type, int origin, int target)
{
    Move m = Move(color, type, origin, target);
    EXPECT_EQ(m.type(), type);
    EXPECT_EQ(m.color(), color);
    EXPECT_EQ(m.origin(), origin);
    EXPECT_EQ(m.target(), target);
}

void constructor_test(bool color, MoveType type, int origin, int target,
        PieceType promo)
{
    Move m = Move(color, type, origin, target, promo);
    EXPECT_EQ(m.type(), type);
    EXPECT_EQ(m.color(), color);
    EXPECT_EQ(m.origin(), origin);
    EXPECT_EQ(m.target(), target);
    EXPECT_EQ(m.promoted(), promo);
}

TEST(Piece, Constructor)
{
    constructor_test(W_PAWN, WHITE);
    constructor_test(B_PAWN, BLACK);
    constructor_test(KNIGHT, WHITE);
    constructor_test(ROOK, BLACK);
}

// TODO do isOn, isFriend, etc tests


TEST(Move, Constructor)
{
    constructor_test(WHITE, QUIET, mailbox(4,4,5), mailbox(5,3,7));
    constructor_test(WHITE, CAPTURE, mailbox(6,1,5), mailbox(4,3,3));
    constructor_test(BLACK, EN_PASSANT, mailbox(2,3,3), mailbox(2,2,2));
    constructor_test(BLACK, DOUBLE_PAWN_PUSH, mailbox(1,5,6), mailbox(1,5,4));

    constructor_test(WHITE, PROMOTE, mailbox(1,4,6), mailbox(1,4,7), UNICORN);
    constructor_test(BLACK, PROMO_CAPTURE, mailbox(4,2,1), mailbox(4,1,0), QUEEN);
}
