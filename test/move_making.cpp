#include "../src/board.h"

#include "unit_test.h"

using std::list;

TEST(MoveMaking, Quiet)
{
    // The rook will move from i to j
    Piece wr (ROOK, WHITE);
    int i = mailbox(6,3,5);
    int j = mailbox(6,6,5);
    Move m = Move(WHITE, QUIET, i, j);

    Board b;
    b.putPiece(wr, i);

    b.makeMove(m);

    // Ideally, origin is empty, and the rook is at target.
    EXPECT_TRUE(b.getPiece(i).type() == NIL);
    EXPECT_TRUE(b.getPiece(j) == wr);

    b.undoMove();

    // When we undo, the reverse should be true.
    EXPECT_TRUE(b.getPiece(i) == wr);
    EXPECT_TRUE(b.getPiece(j).type() == NIL);
}

TEST(MoveMaking, DPP)
{
    // The pawn will move from i to j
    Piece wp (W_PAWN, WHITE);
    int i = mailbox(3,5,1);
    int j = mailbox(3,5,3);
    Move m = Move(WHITE, DOUBLE_PAWN_PUSH, i, j);

    // Put the pieces down
    Board b;
    b.putPiece(wp, i);

    b.makeMove(m);

    // Ideally, origin is empty, and the rook is at target.
    EXPECT_TRUE(b.getPiece(i).type() == NIL);
    EXPECT_TRUE(b.getPiece(j) == wp);

    b.undoMove();

    // When we undo, the reverse should be true.
    EXPECT_TRUE(b.getPiece(i) == wp);
    EXPECT_TRUE(b.getPiece(j).type() == NIL);
}

TEST(MoveMaking, Capture)
{
    // The rook will capture the knight
    Piece wr (ROOK, WHITE);
    Piece bn (KNIGHT, BLACK);
    int i = mailbox(6,3,5);
    int j = mailbox(6,6,5);
    Move m = Move(WHITE, CAPTURE, i, j);

    // Put the pieces down
    Board b;
    b.putPiece(wr, i);
    b.putPiece(bn, j);

    b.makeMove(m);

    // After capture, origin should be empty, but target should be the rook.
    EXPECT_TRUE(b.getPiece(i).type() == NIL);
    EXPECT_TRUE(b.getPiece(j) == wr);

    b.undoMove();

    // When we undo, both pieces should be where they started.
    EXPECT_TRUE(b.getPiece(i) == wr);
    EXPECT_TRUE(b.getPiece(j) == bn);
}

TEST(MoveMaking, EnPassant)
{
    Piece wp (W_PAWN, WHITE);
    Piece bp (B_PAWN, BLACK);

    /*
     * For reference, the squares are laid out like this:
     * _________
     * | . j . | 6
     * | . k . | 5
     * | . l i | 4
     * +-------+
     *   1 2 3
     */
    int i = mailbox(2,5,4);
    int j = mailbox(3,5,6);
    int k = mailbox(3,5,5);
    int l = mailbox(3,5,4);

    Move first = Move(BLACK, DOUBLE_PAWN_PUSH, j, l);
    Move second = Move(WHITE, EN_PASSANT, i, k);

    Board b;
    b.putPiece(wp, i);
    b.putPiece(bp, j);

    b.makeMove(first);
    b.makeMove(second);

    // After en passant, the only piece should be the white pawn on k.
    EXPECT_TRUE(b.getPiece(i).type() == NIL);
    EXPECT_TRUE(b.getPiece(j).type() == NIL);
    EXPECT_TRUE(b.getPiece(k) == wp);
    EXPECT_TRUE(b.getPiece(l).type() == NIL);

    b.undoMove();

    // Undoing the en passant, the white pawn should return to i, but the black
    // pawn should reappear on l.
    EXPECT_TRUE(b.getPiece(i) == wp);
    EXPECT_TRUE(b.getPiece(j).type() == NIL);
    EXPECT_TRUE(b.getPiece(k).type() == NIL);
    EXPECT_TRUE(b.getPiece(l) == bp);

    b.undoMove();

    // Undoing the double pawn push for good measure.
    EXPECT_TRUE(b.getPiece(i) == wp);
    EXPECT_TRUE(b.getPiece(j) == bp);
    EXPECT_TRUE(b.getPiece(k).type() == NIL);
    EXPECT_TRUE(b.getPiece(l).type() == NIL);
}

TEST(MoveMaking, CastleKingside)
{
    Piece wk (KING, WHITE);
    Piece wr (ROOK, WHITE);

    int i = mailbox(4,4,0);
    int j = mailbox(4,5,0);
    int k = mailbox(4,6,0);
    int l = mailbox(4,7,0);

    Board b;
    b.putPiece(wk, i);
    b.putPiece(wr, l);

    b.makeMove(Move(WHITE, CASTLE, i, k));

    // The king should be on k, and the rook on j.
    EXPECT_TRUE(b.getPiece(i).type() == NIL);
    EXPECT_TRUE(b.getPiece(j) == wr);
    EXPECT_TRUE(b.getPiece(k) == wk);
    EXPECT_TRUE(b.getPiece(l).type() == NIL);

    b.undoMove();

    // Both pieces should be back where they started
    EXPECT_TRUE(b.getPiece(i) == wk);
    EXPECT_TRUE(b.getPiece(j).type() == NIL);
    EXPECT_TRUE(b.getPiece(k).type() == NIL);
    EXPECT_TRUE(b.getPiece(l) == wr);    
}

TEST(MoveMaking, CastleQueenside)
{
    Piece bk (KING, BLACK);
    Piece br (ROOK, BLACK);

    int i = mailbox(4,4,7);
    int j = mailbox(3,3,7);
    int k = mailbox(2,2,7);
    int l = mailbox(0,0,7);

    Board b;
    b.putPiece(bk, i);
    b.putPiece(br, l);

    b.makeMove(Move(BLACK, CASTLE, i, k));

    // The king should be on k, and the rook on j.
    EXPECT_TRUE(b.getPiece(i).type() == NIL);
    EXPECT_TRUE(b.getPiece(j) == br);
    EXPECT_TRUE(b.getPiece(k) == bk);
    EXPECT_TRUE(b.getPiece(l).type() == NIL);

    b.undoMove();

    // Both pieces should be back where they started
    EXPECT_TRUE(b.getPiece(i) == bk);
    EXPECT_TRUE(b.getPiece(j).type() == NIL);
    EXPECT_TRUE(b.getPiece(k).type() == NIL);
    EXPECT_TRUE(b.getPiece(l) == br);    
}

TEST(MoveMaking, Promotion)
{
    // The pawn should move from i to j and promote to the queen.
    Piece wp (W_PAWN, WHITE);
    Piece wq (QUEEN, WHITE);
    int i = mailbox(5,1,6);
    int j = mailbox(5,1,7);
    Move m = Move(WHITE, PROMOTE, i, j, QUEEN);

    Board b;
    b.putPiece(wp, i);

    b.makeMove(m);

    // The only piece should be the queen on j.
    EXPECT_TRUE(b.getPiece(i).type() == NIL);
    EXPECT_TRUE(b.getPiece(j) == wq);

    b.undoMove();

    // After undoing, the pawn should be back where it was.
    EXPECT_TRUE(b.getPiece(i) == wp);
    EXPECT_TRUE(b.getPiece(j).type() == NIL);
}

TEST(MoveMaking, PromoCapture)
{
    // The pawn should move from i to j, capturing the knight,
    // and promote to the queen.
    Piece wp (W_PAWN, WHITE);
    Piece wq (QUEEN, WHITE);
    Piece bn (KNIGHT, BLACK);
    int i = mailbox(5,1,6);
    int j = mailbox(5,2,7);
    Move m = Move(WHITE, PROMO_CAPTURE, i, j, QUEEN);

    Board b;
    b.putPiece(wp, i);
    b.putPiece(bn, j);

    b.makeMove(m);

    // The only piece should be the queen on j.
    EXPECT_TRUE(b.getPiece(i).type() == NIL);
    EXPECT_TRUE(b.getPiece(j) == wq);

    b.undoMove();

    // After undoing, the pawn and knight should be back where they were.
    EXPECT_TRUE(b.getPiece(i) == wp);
    EXPECT_TRUE(b.getPiece(j) == bn);
}
