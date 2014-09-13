#include "../src/board.h"

#include "unit_test.h"

TEST(MoveLegality, Check)
{
    // The rook and knight will check the enemy kings
    Piece wk (KING, WHITE);
    Piece br (ROOK, BLACK);
    Piece wn (KNIGHT, WHITE);
    Piece bk (KING, BLACK);

    Board b;
    b.putPiece(wk, mailbox(4,2,1));
    b.putPiece(br, mailbox(4,2,5));

    // The rook can attack the king
    EXPECT_TRUE(b.isInCheck(WHITE));

    // Put the knight in the way
    b.putPiece(wn, mailbox(4,2,4));
    b.putPiece(bk, mailbox(4,3,6));

    // So White is no longer in check, but Black is now
    EXPECT_TRUE(b.isInCheck(BLACK));
    EXPECT_FALSE(b.isInCheck(WHITE));
}

TEST(MoveLegality, Pin)
{
    // The wizard will pin the rook to the king
    Piece wk (KING, WHITE);
    Piece wr (ROOK, WHITE);
    Piece bw (WIZARD, BLACK);

    Board b;
    b.putPiece(wk, mailbox(4,4,0));
    b.putPiece(wr, mailbox(4,4,1));
    b.putPiece(bw, mailbox(4,4,3));

    Move m;

    // The rook cannot move out of the way
    m = Move(WHITE, QUIET, mailbox(4,4,1), mailbox(4,0,1));
    EXPECT_FALSE(b.isLegalMove(m));

    // The rook can move along the pin
    m = Move(WHITE, QUIET, mailbox(4,4,1), mailbox(4,4,2));
    EXPECT_TRUE(b.isLegalMove(m));

    // The king can move out of the way
    m = Move(WHITE, QUIET, mailbox(4,4,0), mailbox(4,3,0));
    EXPECT_TRUE(b.isLegalMove(m));
}

TEST(MoveLegality, Block)
{
    // The rook will attack the king
    Piece bk (KING, BLACK);
    Piece bb (BISHOP, BLACK);
    Piece wr (ROOK, WHITE);

    Board b;
    b.putPiece(bk, mailbox(4,4,7));
    b.putPiece(bb, mailbox(4,5,6));
    b.putPiece(wr, mailbox(4,4,3));

    Move m;

    // The bishop can block
    m = Move(BLACK, QUIET, mailbox(4,5,6), mailbox(4,4,5));
    EXPECT_TRUE(b.isLegalMove(m));

    // The bishop can't move anywhere else though
    m = Move(BLACK, QUIET, mailbox(4,5,6), mailbox(4,6,7));
    EXPECT_FALSE(b.isLegalMove(m));

    // The king can move out of the way
    m = Move(BLACK, QUIET, mailbox(4,4,7), mailbox(4,3,7));
    EXPECT_TRUE(b.isLegalMove(m));

    // But not along the attack
    m = Move(BLACK, QUIET, mailbox(4,4,7), mailbox(4,4,6));
    EXPECT_FALSE(b.isLegalMove(m));
}

TEST(MoveLegality, Castle)
{
    // The rook will block each square on the king's path
    Piece wk (KING, WHITE);
    Piece wr (ROOK, WHITE);
    Piece br (ROOK, BLACK);

    Board b;
    b.putPiece(wk, mailbox(4,4,0));
    b.putPiece(wr, mailbox(4,0,0));
    b.putPiece(br, mailbox(4,1,3));

    // Castling can occur even if the rook passes through check
    Move m (WHITE, CASTLE, mailbox(4,4,0), mailbox(4,2,0));
    EXPECT_TRUE(b.isLegalMove(m));

    // But not if the king moves into check
    b.makeMove(Move(BLACK, QUIET, mailbox(4,1,3), mailbox(4,2,3)));
    EXPECT_FALSE(b.isLegalMove(m));
    b.undoMove();

    // Or through check
    b.makeMove(Move(BLACK, QUIET, mailbox(4,1,3), mailbox(4,3,3)));
    EXPECT_FALSE(b.isLegalMove(m));
    b.undoMove();

    // Or out of check
    b.makeMove(Move(BLACK, QUIET, mailbox(4,1,3), mailbox(4,4,3)));
    EXPECT_FALSE(b.isLegalMove(m));
    b.undoMove();
}
