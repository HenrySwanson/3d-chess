#include "../src/board.h"

#include "unit_test.h"

TEST(MoveLegality, Pin)
{
    // All castling partners will be in their starting places
    Piece wk (KING, WHITE);
    Piece wr (ROOK, WHITE);
    Piece bw (WIZARD, BLACK);

    Board b;
    b.putPiece(wk, mailbox(4,4,0));
    b.putPiece(wr, mailbox(4,4,1));
    b.putPiece(bw, mailbox(4,4,3));

    Move m;
    m = Move(WHITE, QUIET, mailbox(4,4,1), mailbox(4,0,1));
    EXPECT_FALSE(b.isLegalMove(m));

    m = Move(WHITE, QUIET, mailbox(4,4,1), mailbox(4,4,2));
    EXPECT_TRUE(b.isLegalMove(m));

    m = Move(WHITE, QUIET, mailbox(4,4,0), mailbox(4,3,0));
    EXPECT_TRUE(b.isLegalMove(m));
}

TEST(MoveLegality, Block)
{
    // All castling partners will be in their starting places
    Piece bk (KING, BLACK);
    Piece bb (BISHOP, BLACK);
    Piece wr (ROOK, WHITE);

    Board b;
    b.putPiece(bk, mailbox(4,4,7));
    b.putPiece(bb, mailbox(4,5,6));
    b.putPiece(wr, mailbox(4,4,3));

    Move m;
    m = Move(BLACK, QUIET, mailbox(4,5,6), mailbox(4,4,5));
    EXPECT_TRUE(b.isLegalMove(m));

    m = Move(BLACK, QUIET, mailbox(4,5,6), mailbox(4,6,7));
    EXPECT_FALSE(b.isLegalMove(m));

    m = Move(BLACK, QUIET, mailbox(4,4,7), mailbox(4,3,7));
    EXPECT_TRUE(b.isLegalMove(m));

    m = Move(BLACK, QUIET, mailbox(4,4,7), mailbox(4,4,6));
    EXPECT_FALSE(b.isLegalMove(m));
}

// TODO castle tests
