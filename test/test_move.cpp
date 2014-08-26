#include "../src/move.h"

#include "unit_test.h"

TEST(Move, Quiet)
{
    int i = mailbox(3,4,1);
    int j = mailbox(3,4,6);
    Move m = Move::Quiet(i, j);
    EXPECT_EQ(m.origin(), i);
    EXPECT_EQ(m.target(), j);
}

TEST(Move, DPP)
{
    int i = mailbox(3,4,1);
    int j = mailbox(3,4,3);
    Move m = Move::DPP(i, j);
    EXPECT_EQ(m.origin(), i);
    EXPECT_EQ(m.target(), j);
}

TEST(Move, Capture)
{
    int i = mailbox(3,4,1);
    int j = mailbox(3,4,3);
    Piece p (BISHOP, BLACK, true);
    Move m = Move::Capture(i, j, p);
    EXPECT_EQ(m.origin(), i);
    EXPECT_EQ(m.target(), j);
    EXPECT_TRUE(m.captured() == p);
}

TEST(Move, EP)
{
    int i = mailbox(5,2,4);
    int j = mailbox(4,3,5);
    Move m = Move::EP(i, j);
    EXPECT_EQ(m.origin(), i);
    EXPECT_EQ(m.target(), j);
}

TEST(Move, Castle)
{
    int i = mailbox(4,4,0);
    int j = mailbox(4,7,0);
    Move m = Move::Castle(i, j);
    EXPECT_EQ(m.origin(), i);
    EXPECT_EQ(m.target(), j);
}

TEST(Move, Promote)
{
    int i = mailbox(3,1,6);
    int j = mailbox(3,1,7);
    Piece p (WIZARD, WHITE, false);
    Move m = Move::Promote(i, j, p);
    EXPECT_EQ(m.origin(), i);
    EXPECT_EQ(m.target(), j);
    EXPECT_TRUE(m.promoted() == p);
}

TEST(Move, PromoCapture)
{
    int i = mailbox(3,1,6);
    int j = mailbox(3,1,7);
    Piece p (QUEEN, WHITE, true);
    Piece q (DRAGON, BLACK, false);
    Move m = Move::PromoCapture(i, j, p, q);
    EXPECT_TRUE(m.origin() == i);
    EXPECT_TRUE(m.target() == j);
    EXPECT_TRUE(m.promoted() == p);
    EXPECT_TRUE(m.captured() == q);
}
