#include "../src/board.h"

#include "unit_test.h"

#include <algorithm>
#include <string>
#include <sstream>

using std::list;
using std::string;
using std::stringstream;

/**
 * Given a list of moves, this takes all their targets and converts them to
 * strings of the form "(x, y, z)".
 */
list<string> stringifyTargets(const list<Move>& moves)
{
    list<string> strs;

    list<Move>::const_iterator it;
    for(it = moves.begin(); it != moves.end(); it++)
    {
        stringstream ss;
        int i = it->target();
        ss << "(" << unmailboxX(i) << ", " << unmailboxY(i) << ", " <<
                unmailboxZ(i) << ")";
        strs.push_back(ss.str());
    }
    
    return strs;   
}

/**
 * A very kludgey way to compare moves with integer arrays (because those are
 * easy to type into a source file, and to read). It makes a blank board, puts
 * a white piece of the given type at the given location, then computes all the
 * moves it can make. It returns true if the targets of these moves are exactly
 * the targets given in the integer array (regardless of order).
 */
void matchQuietMoves(PieceType pt, int origin, int targets [], int num_targets)
{
    // Sets up the board
    Board b;
    b.putPiece(Piece(pt, WHITE), origin);

    // These hold the target squares as strings
    list<string> from_array;
    list<string> from_list;

    // Convert integer array to string representing coordinates
    for(int i = 0; i < num_targets; i++)
    {
        stringstream ss;
        ss << "(" << targets[3 * i] << ", " << targets[3 * i + 1] << ", " <<
                targets[3 * i + 2] << ")";
        from_array.push_back(ss.str());
    }

    // Convert move array to string representing target coordinates
    from_list = stringifyTargets(b.generateMoves(origin));

    // Sorts both arrays so that we just need to check equality now
    from_array.sort();
    from_list.sort();

    // Strings to store the concatenated results
    string array_str, list_str;
    list<string>::const_iterator s_it;

    // Concatenates the results
    for(s_it = from_array.begin(); s_it != from_array.end(); s_it++)
        array_str += *s_it + " ";
    for(s_it = from_list.begin(); s_it != from_list.end(); s_it++)
        list_str += *s_it + " ";

    // The actual assertion
    EXPECT_STR_EQ(array_str.c_str(), list_str.c_str());
}

/**
 * Puts a piece of type "pt" at the given location and generates all the moves
 * it can make. It then repeats this for the types given in "parts", and
 * concatenates those together. It returns true if the two lists are the same.
 */
void testCompoundPiece(PieceType pt, PieceType parts [], int num_parts,
        int origin)
{
    Board b;
    list<Move> compoundMoves;
    list<Move> componentMoves;

    // Generates moves from the compound piece
    b.putPiece(Piece(pt, WHITE), origin);
    compoundMoves = b.generateMoves(origin);

    // Puts down one piece, generates moves, then replaces it with another
    // piece, and appends those moves, ...
    for(int i = 0; i < num_parts; i++)
    {
        b.putPiece(Piece(parts[i], WHITE), origin);
        list<Move> temp = b.generateMoves(origin);
        componentMoves.splice(componentMoves.end(), temp);
    }

    list<string> compoundStrs = stringifyTargets(compoundMoves);
    list<string> componentStrs = stringifyTargets(componentMoves);

    // Sorts them and removes duplicates
    compoundStrs.sort();
    componentStrs.sort();
    compoundStrs.unique();
    componentStrs.unique();

    EXPECT_TRUE(compoundStrs == componentStrs);
}

/**
 * Returns true if the given list contains the given move.
 */
bool containsMove(const list<Move>& li, Move m)
{
    list<Move>::const_iterator it = find(li.begin(), li.end(), m);
    return (it != li.end());
}

/*
 * The following tests are to guarantee that non-pawn pieces have the correct
 * move directions. They only differ in what the array is set to, or if it is
 * a compound piece.
 */

TEST(Board, Knight)
{
    int array [] = {
    //  0 1 2  0 2 1  1 0 2  1 2 0  2 0 1  2 1 0
        2,2,2, 2,1,3, 1,3,2, 1,1,4, 0,3,3, 0,2,4, // - -
        2,2,6, 2,1,5, 1,3,6, 1,5,4, 0,3,5, 0,4,4, // - +
        2,4,2, 2,5,3, 3,3,2, 3,1,4, 4,3,3, 4,2,4, // + -
        2,4,6, 2,5,5, 3,3,6, 3,5,4, 4,3,5, 4,4,4  // + +
    };

    matchQuietMoves(KNIGHT, mailbox(2,3,4), array, 24);
}

TEST(Board, Griffin)
{
    int array [] = {
    //  - - -  - - +  - + -  - + +  + - -  + - +  + + -  + + +
        1,2,2, 1,2,6, 1,4,2, 1,4,6, 3,2,2, 3,2,6, 3,4,2, 3,4,6, // 1 1 2
        1,1,3, 1,1,5, 1,5,3, 1,5,5, 3,1,3, 3,1,5, 3,5,3, 3,5,5, // 1 2 1
        0,2,3, 0,2,5, 0,4,3, 0,4,5, 4,2,3, 4,2,5, 4,4,3, 4,4,5  // 2 2 1
    };

    matchQuietMoves(GRIFFIN, mailbox(2,3,4), array, 24);
}

TEST(Board, Dragon)
{
    int array [] = {
    //  - - -  - - +  - + -  - + +  + - -  + - +  + + -  + + +
        1,1,2, 1,1,6, 1,5,2, 1,5,6, 3,1,2, 3,1,6, 3,5,2, 3,5,6, // 1 2 2
        0,2,2, 0,2,6, 0,4,2, 0,4,6, 4,2,2, 4,2,6, 4,4,2, 4,4,6, // 2 1 2
        0,1,3, 0,1,5, 0,5,3, 0,5,5, 4,1,3, 4,1,5, 4,5,3, 4,5,5  // 2 2 1
    };

    matchQuietMoves(DRAGON, mailbox(2,3,4), array, 24);
}

TEST(Board, Unicorn)
{
    PieceType array [] = {KNIGHT, GRIFFIN, DRAGON};
    testCompoundPiece(UNICORN, array, 3, mailbox(2,3,4));
}

TEST(Board, Rook)
{
    int array [] = {
        2,3,0, 2,3,1, 2,3,2, 2,3,3,        2,3,5, 2,3,6, 2,3,7, // 0 0 +
        2,0,4, 2,1,4, 2,2,4,        2,4,4, 2,5,4, 2,6,4, 2,7,4, // 0 + 0
        0,3,4, 1,3,4,        3,3,4, 4,3,4, 5,3,4, 6,3,4, 7,3,4  // + 0 0
    };

    matchQuietMoves(ROOK, mailbox(2,3,4), array, 21);
}

TEST(Board, Bishop)
{
    int array [] = {
        0,1,4, 1,2,4,        3,4,4, 4,5,4, 5,6,4, 6,7,4,       // + + 0
        0,5,4, 1,4,4,        3,2,4, 4,1,4, 5,0,4,              // + - 0
        0,3,2, 1,3,3,        3,3,5, 4,3,6, 5,3,7,              // + 0 +
        0,3,6, 1,3,5,        3,3,3, 4,3,2, 5,3,1, 6,3,0,       // + 0 -
        2,0,1, 2,1,2, 2,2,3,        2,4,5, 2,5,6, 2,6,7,       // 0 + +
        2,0,7, 2,1,6, 2,2,5,        2,4,3, 2,5,2, 2,6,1, 2,7,0 // 0 + -
    };

    matchQuietMoves(BISHOP, mailbox(2,3,4), array, 35);
}

TEST(Board, Mace)
{
    int array [] = {
        0,1,2, 1,2,3,        3,4,5, 4,5,6, 5,6,7,        // + + +
        0,1,6, 1,2,5,        3,4,3, 4,5,2, 5,6,1, 6,7,0, // + + -
        0,5,2, 1,4,3,        3,2,5, 4,1,6, 5,0,7,        // + - +
        0,5,6, 1,4,5,        3,2,3, 4,1,2, 5,0,1         // + - -
    };

    matchQuietMoves(MACE, mailbox(2,3,4), array, 21);
}

TEST(Board, Wizard)
{
    PieceType array [] = {ROOK, BISHOP};
    testCompoundPiece(WIZARD, array, 2, mailbox(2,3,4));
}

TEST(Board, Archer)
{
    PieceType array [] = {BISHOP, MACE};
    testCompoundPiece(ARCHER, array, 2, mailbox(2,3,4));
}

TEST(Board, Cannon)
{
    PieceType array [] = {ROOK, MACE};
    testCompoundPiece(CANNON, array, 2, mailbox(2,3,4));
}

TEST(Board, Queen)
{
    PieceType array [] = {ROOK, BISHOP, MACE};
    testCompoundPiece(QUEEN, array, 3, mailbox(2,3,4));
}

TEST(Board, King)
{
    int array [] = {
    // (       0 0 -       )(       0 0 0       )(       0 0 1       )
        1,2,3, 1,3,3, 1,4,3, 1,2,4, 1,3,4, 1,4,4, 1,2,5, 1,3,5, 1,4,5, // - 0 0 
        2,2,3, 2,3,3, 2,4,3, 2,2,4,        2,4,4, 2,2,5, 2,3,5, 2,4,5, // 0 0 0
        3,2,3, 3,3,3, 3,4,3, 3,2,4, 3,3,4, 3,4,4, 3,2,5, 3,3,5, 3,4,5  // + 0 0
    //  0 - 0  0 0 0  0 + 0  0 - 0  0 0 0  0 + 0  0 - 0  0 0 0  0 + 0
    };

    matchQuietMoves(KING, mailbox(2,3,4), array, 26);
}

// Next come several pawn tests

/*
 * Tests against Type II errors (false negatives). These pawns will be put in
 * optimal conditions; that is, they will be able to make lots of moves, which
 * we expect to find in the move lists.
 */
TEST(Board, PawnII)
{
    /*
     * Two pawns, one white, one black, on their starting ranks (at locations i
     * and j). Every square that they can capture to will be filled with a pawn
     * of the opposite color. There should be 8 captures, one quiet move, and
     * one double pawn push for each pawn.
     */

    // Declare board, locations, and pawns.
    Board b;
    Piece wp (W_PAWN, WHITE);
    Piece bp (B_PAWN, BLACK);
    int i = mailbox(2,3,1);
    int j = mailbox(2,3,6);

    // Put down pawns
    b.putPiece(wp, i);
    b.putPiece(bp, j);

    // Put down capture targets (also pawns)
    b.putPiece(bp, mailbox(1,2,2));
    b.putPiece(bp, mailbox(1,3,2));
    b.putPiece(bp, mailbox(1,4,2));
    b.putPiece(bp, mailbox(2,2,2));
    b.putPiece(bp, mailbox(2,4,2));
    b.putPiece(bp, mailbox(3,2,2));
    b.putPiece(bp, mailbox(3,3,2));
    b.putPiece(bp, mailbox(3,4,2));

    b.putPiece(wp, mailbox(1,2,5));
    b.putPiece(wp, mailbox(1,3,5));
    b.putPiece(wp, mailbox(1,4,5));
    b.putPiece(wp, mailbox(2,2,5));
    b.putPiece(wp, mailbox(2,4,5));
    b.putPiece(wp, mailbox(3,2,5));
    b.putPiece(wp, mailbox(3,3,5));
    b.putPiece(wp, mailbox(3,4,5));

    // Generate movelists and check their contents
    list<Move> w_moves = b.generateMoves(i);
    list<Move> b_moves = b.generateMoves(j);

    EXPECT_TRUE(containsMove(w_moves, Move::Quiet(  i, mailbox(2,3,2)    )));
    EXPECT_TRUE(containsMove(w_moves, Move::DPP(    i, mailbox(2,3,3)    )));
    EXPECT_TRUE(containsMove(w_moves, Move::Capture(i, mailbox(1,2,2), bp)));
    EXPECT_TRUE(containsMove(w_moves, Move::Capture(i, mailbox(1,3,2), bp)));
    EXPECT_TRUE(containsMove(w_moves, Move::Capture(i, mailbox(1,4,2), bp)));
    EXPECT_TRUE(containsMove(w_moves, Move::Capture(i, mailbox(2,2,2), bp)));
    EXPECT_TRUE(containsMove(w_moves, Move::Capture(i, mailbox(2,4,2), bp)));
    EXPECT_TRUE(containsMove(w_moves, Move::Capture(i, mailbox(3,2,2), bp)));
    EXPECT_TRUE(containsMove(w_moves, Move::Capture(i, mailbox(3,3,2), bp)));
    EXPECT_TRUE(containsMove(w_moves, Move::Capture(i, mailbox(3,4,2), bp)));

    EXPECT_TRUE(containsMove(b_moves, Move::Quiet(  j, mailbox(2,3,5)    )));
    EXPECT_TRUE(containsMove(b_moves, Move::DPP(    j, mailbox(2,3,4)    )));
    EXPECT_TRUE(containsMove(b_moves, Move::Capture(j, mailbox(1,2,5), wp)));
    EXPECT_TRUE(containsMove(b_moves, Move::Capture(j, mailbox(1,3,5), wp)));
    EXPECT_TRUE(containsMove(b_moves, Move::Capture(j, mailbox(1,4,5), wp)));
    EXPECT_TRUE(containsMove(b_moves, Move::Capture(j, mailbox(2,2,5), wp)));
    EXPECT_TRUE(containsMove(b_moves, Move::Capture(j, mailbox(2,4,5), wp)));
    EXPECT_TRUE(containsMove(b_moves, Move::Capture(j, mailbox(3,2,5), wp)));
    EXPECT_TRUE(containsMove(b_moves, Move::Capture(j, mailbox(3,3,5), wp)));
    EXPECT_TRUE(containsMove(b_moves, Move::Capture(j, mailbox(3,4,5), wp)));

    // Confirms that there are no other moves in the lists
    EXPECT_EQ(w_moves.size(), 10);
    EXPECT_EQ(b_moves.size(), 10);
}

/*
 * Tests against Type I errors (false postives). These pawns will be put in
 * positions such that there are moves they cannot make. We expect that the
 * move list does _not_ contain these moves.
 */
TEST(Board, PawnI)
{
    /*
     * The only relevant section of board is y = 0, and looks like:
     * ___________________
     * | . . A . . . . . |
     * | . . . . . . . . |
     * | . . . . . . . . |
     * | . . . . . . . . |
     * | . . . C . . D . |
     * | . . . . . . . F |
     * | . . . B . . . E |
     * | . . . . . . . . |
     * ^^^^^^^^^^^^^^^^^^^
     * (all pieces are white pawns)
     */

    Board board;
    Piece wp (W_PAWN, WHITE);
    list<Move> moves;

    int a = mailbox(2,0,7);
    int b = mailbox(3,0,1);
    int c = mailbox(3,0,3);
    int d = mailbox(6,0,3);
    int e = mailbox(7,0,1);
    int f = mailbox(7,0,2);

    board.putPiece(wp, a);
    board.putPiece(wp, b);
    board.putPiece(wp, c);
    board.putPiece(wp, d);
    board.putPiece(wp, e);
    board.putPiece(wp, f);

    // Pawns cannot move off the board
    moves = board.generateMoves(a);
    EXPECT_FALSE(containsMove(moves, Move::Quiet(a, mailbox(2,0,8))));

    // Pawns cannot move forward twice if that square is non-empty
    moves = board.generateMoves(b);
    EXPECT_FALSE(containsMove(moves, Move::DPP(b, mailbox(3,0,3))));
    // but they can move forward once
    EXPECT_TRUE(containsMove(moves, Move::Quiet(b, mailbox(3,0,2))));

    // Pawns cannot move forward twice if they are off their home rank
    moves = board.generateMoves(c);
    EXPECT_FALSE(containsMove(moves, Move::DPP(c, mailbox(3,0,5))));
    // once more, for good measure (and because D doesn't do anything else)
    moves = board.generateMoves(d);
    EXPECT_FALSE(containsMove(moves, Move::DPP(d, mailbox(6,0,5))));

    // Pawns cannot move forward at all if they are immediately obstructed
    moves = board.generateMoves(e);
    EXPECT_FALSE(containsMove(moves, Move::Quiet(e, mailbox(7,0,2))));
    EXPECT_FALSE(containsMove(moves, Move::DPP(e, mailbox(7,0,3))));

    // Pawns cannot capture their teammates, nil spaces, or borders
    moves = board.generateMoves(f);
    EXPECT_FALSE(containsMove(moves, Move::Capture(f, mailbox(6,0,3),
            board.getPiece(mailbox(6,0,3)) ))); // White pawn
    EXPECT_FALSE(containsMove(moves, Move::Capture(f, mailbox(7,1,3),
            board.getPiece(mailbox(7,1,3)) ))); // Nil
    EXPECT_FALSE(containsMove(moves, Move::Capture(f, mailbox(8,0,3),
            board.getPiece(mailbox(8,0,3)) ))); // Border
}

/** Tests that en passant behaves correctly. */
TEST(Board, EnPassant)
{
    Board b;
    Piece wp (W_PAWN, WHITE);
    list<Move> moves;

    int i = mailbox(2,3,4);
    int j = mailbox(1,3,5);
    Move m = Move::EP(i, j);

    // Puts down the pawn
    b.putPiece(wp, i);
    moves = b.generateMoves(i);

    // No en passant possible
    EXPECT_FALSE(containsMove(moves, m));

    // Sets en passant square
    b.setEnPassant(j);
    moves = b.generateMoves(i);

    // En passant is possible
    EXPECT_TRUE(containsMove(moves, m));
}

/** Tests that promotions and promo-captures behave correctly. */
TEST(Board, PromoMaybeCapture)
{
    Board b;
    Piece wp (W_PAWN, WHITE);
    Piece br (ROOK, BLACK);
    int i = mailbox(1,4,6);
    int j = mailbox(1,4,7);
    int k = mailbox(1,5,7);
    list<Move> moves;

    // Nothing in the way? Promotions only.
    b.putPiece(wp, i);
    moves = b.generateMoves(i);
    EXPECT_EQ((signed) moves.size(), NUM_PROMOTION_PIECES);
    for(int t = 0; t < NUM_PROMOTION_PIECES; t++)
    {
        Piece p (PROMOTION_PIECES[t], WHITE);
        EXPECT_TRUE(containsMove(moves, Move::Promote(i, j, p)));
    }

    // Add an obstruction? Nothing to do here.
    b.putPiece(br, j);
    moves = b.generateMoves(i);
    EXPECT_EQ(moves.size(), 0);

    // Add something to capture? Promo-captures only.
    b.putPiece(br, k);
    moves = b.generateMoves(i);
    EXPECT_EQ((signed) moves.size(), NUM_PROMOTION_PIECES);
    for(int t = 0; t < NUM_PROMOTION_PIECES; t++)
    {
        Piece p (PROMOTION_PIECES[t], WHITE);
        EXPECT_TRUE(containsMove(moves, Move::PromoCapture(i, k, p, br)));
    }

    // Remove the obstruction? Both types of moves.
    b.putPiece(Piece(NIL, WHITE), j);
    moves = b.generateMoves(i);
    EXPECT_EQ((signed) moves.size(), 2 * NUM_PROMOTION_PIECES);
    for(int t = 0; t < NUM_PROMOTION_PIECES; t++)
    {
        Piece p (PROMOTION_PIECES[t], WHITE);
        EXPECT_TRUE(containsMove(moves, Move::Promote(i, j, p)));
        EXPECT_TRUE(containsMove(moves, Move::PromoCapture(i, k, p, br)));
    }
}

/**
 * Tests that capturing is possible, and blocks movement. Because of all the
 * other tests involving piece motion, it's sufficient to just test one jumping
 * and one sliding piece.
 */
TEST(Board, Capture)
{
    Board b;
    Piece wr (ROOK, WHITE);
    Piece bn (KNIGHT, BLACK);
    Piece wp (W_PAWN, WHITE);
    int i = mailbox(2,3,4);
    int j = mailbox(4,3,4);
    int k = mailbox(2,2,4);

    // Put down the rook, knight, and pawn
    b.putPiece(wr, i);
    b.putPiece(bn, j);
    b.putPiece(wp, k);

    list<Move> moves = b.generateMoves(i);

    // Rook can still move toward the knight
    EXPECT_TRUE(containsMove(moves, Move::Quiet(i, mailbox(3,3,4))));

    // Can't move onto the knight, but can capture it
    EXPECT_FALSE(containsMove(moves, Move::Quiet(i, j)));
    EXPECT_TRUE(containsMove(moves, Move::Capture(i, j, bn)));

    // Can't move past it
    EXPECT_FALSE(containsMove(moves, Move::Quiet(i, mailbox(5,3,4))));

    // It also can't capture the pawn
    EXPECT_FALSE(containsMove(moves, Move::Capture(i, k, wp)));

    // Knight can capture the pawn
    moves = b.generateMoves(j);
    EXPECT_TRUE(containsMove(moves, Move::Capture(j, k, wp)));
}

// TODO Castle testing. This'll suck too.
