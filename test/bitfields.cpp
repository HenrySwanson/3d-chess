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

TEST(Piece, Helpers)
{
    Piece nil (NIL, WHITE);
    Piece wp (W_PAWN, WHITE);
    Piece br (ROOK, BLACK);

    // Nil shouldn't be on either side
    EXPECT_FALSE(nil.isOn(WHITE));
    EXPECT_FALSE(nil.isOn(BLACK));

    // The pieces should be on the right sides
    EXPECT_TRUE(wp.isOn(WHITE));
    EXPECT_TRUE(br.isOn(BLACK));
    EXPECT_FALSE(wp.isOn(BLACK));
    EXPECT_FALSE(br.isOn(WHITE));

    // Nil is neither friend nor enemy
    EXPECT_FALSE(wp.isFriend(nil));
    EXPECT_FALSE(br.isFriend(nil));
    EXPECT_FALSE(wp.isEnemy(nil));
    EXPECT_FALSE(br.isEnemy(nil));

    // But the two pieces are enemies
    EXPECT_TRUE(wp.isEnemy(br));
    EXPECT_TRUE(br.isEnemy(wp));
    EXPECT_FALSE(wp.isFriend(br));
    EXPECT_FALSE(br.isFriend(wp));

    // And friends with themselves
    EXPECT_TRUE(wp.isFriend(wp));
    EXPECT_TRUE(br.isFriend(br));
    EXPECT_FALSE(wp.isEnemy(wp));
    EXPECT_FALSE(br.isEnemy(br));
}

TEST(Move, Constructor)
{
    constructor_test(WHITE, QUIET, mailbox(4,4,5), mailbox(5,3,7));
    constructor_test(WHITE, CAPTURE, mailbox(6,1,5), mailbox(4,3,3));
    constructor_test(BLACK, EN_PASSANT, mailbox(2,3,3), mailbox(2,2,2));
    constructor_test(BLACK, DOUBLE_PAWN_PUSH, mailbox(1,5,6), mailbox(1,5,4));

    constructor_test(WHITE, PROMOTE, mailbox(1,4,6), mailbox(1,4,7), UNICORN);
    constructor_test(BLACK, PROMO_CAPTURE, mailbox(4,2,1), mailbox(4,1,0), QUEEN);
}
