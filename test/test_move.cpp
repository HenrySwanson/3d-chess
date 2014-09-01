#include "../src/move.h"

#include "unit_test.h"

TEST(Move, StandardConstructor)
{
    bool color = BLACK;
    MoveType type = CAPTURE;
    int i = mailbox(3,4,1);
    int j = mailbox(3,4,6);
    Move m = Move(color, type, i, j);
    EXPECT_EQ(m.type(), type);
    EXPECT_EQ(m.color(), color);
    EXPECT_EQ(m.origin(), i);
    EXPECT_EQ(m.target(), j);
}

TEST(Move, ExtraConstructor)
{
    bool color = WHITE;
    MoveType type = PROMOTE;
    int i = mailbox(3,4,6);
    int j = mailbox(3,4,7);
    PieceType promo = QUEEN;
    Move m = Move(color, type, i, j, QUEEN);
    EXPECT_EQ(m.type(), type);
    EXPECT_EQ(m.color(), color);
    EXPECT_EQ(m.origin(), i);
    EXPECT_EQ(m.target(), j);
    EXPECT_EQ(m.promoted(), promo);
}
