#include "../src/board.h"

#include "unit_test.h"

#include <algorithm>
#include <string>
#include <sstream>

using std::list;
using std::string;
using std::stringstream;

list<string> stringifyTargets(list<Move> moves)
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

void matchQuietMoves(PieceType pt, int origin, int targets [], int num_targets)
{
    // Sets up the board
    Board b;
    b.putPiece(Piece(pt, WHITE, false), origin);

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

void testCompoundPiece(PieceType pt, PieceType parts [], int num_parts,
        int origin)
{
    Board b;
    list<Move> compoundMoves;
    list<Move> componentMoves;

    // Generates moves from the compound piece
    b.putPiece(Piece(pt, WHITE, false), origin);
    compoundMoves = b.generateMoves(origin);

    // Puts down one piece, generates moves, then replaces it with another
    // piece, and appends those moves, ...
    for(int i = 0; i < num_parts; i++)
    {
        b.putPiece(Piece(parts[i], WHITE, false), origin);
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

    ASSERT_TRUE(compoundStrs == componentStrs);
}

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

// TODO Pawn testing. This'll be a pain: capturing, en passant, double pawn pushes, promotions, and promo-captures

// TODO Capture testing. Only really need to test a sliding piece and a jumping piece for this.

// TODO Castle testing. This'll suck too.
