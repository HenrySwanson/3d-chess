#include "../src/board.h"

#include <iostream>
#include <sstream>
#include <string>

using std::cout;
using std::endl;

using std::list;

using std::string;
using std::stringstream;

// TODO maybe move these into the appropriate cpp files, wrapped in an #ifdef?

/**
 * Converts the piece into a string representation.
 * Not that useful, but unlike pieces, strings can be sorted.
 */
string toString(Piece p)
{
    stringstream ss;

    if(p.type == NIL)
        return "Nil";

    if(p.type == BORDER)
        return "Border";

    string names [16] = {
        "", "", "Pawn", "Pawn", "Knight", "Griffin", "Dragon", "Unicorn",
        "Rook", "Bishop", "Mace", "Wizard", "Archer", "Cannon", "Queen", "King"
    };
    
    string color = (p.color == WHITE ? "White" : "Black");

    string moved = (p.moved ? "(M)" : "(S)");

    ss << color << " " << names[p.type] << " " << moved;

    return ss.str();
}

/**
 * Converts the move into a string representation.
 * Not that useful, but unlike move, strings can be sorted.
 */
string toString(Move m)
{
    stringstream ss;

    int ox = Board::unmailboxX(m.from);
    int oy = Board::unmailboxY(m.from);
    int oz = Board::unmailboxZ(m.from);

    int tx = Board::unmailboxX(m.to);
    int ty = Board::unmailboxY(m.to);
    int tz = Board::unmailboxZ(m.to);

    switch(m.type)
    {
      case QUIET:
        ss << "Quiet: (" << ox << ", " << oy << ", " << oz << ") -> (" <<
                tx << ", " << ty << ", " << tz << ")";
        break;
      case DOUBLE_PAWN_PUSH:
        ss << "DPP: (" << ox << ", " << oy << ", " << oz << ") -> (" <<
                tx << ", " << ty << ", " << tz << ")";
        break;
      case CAPTURE:
        ss << "Capture: (" << ox << ", " << oy << ", " << oz << ") -> (" <<
                tx << ", " << ty << ", " << tz << ") - " << toString(m.captured);
        break;
      case EN_PASSANT:
        ss << "EP: (" << ox << ", " << oy << ", " << oz << ") -> (" <<
                tx << ", " << ty << ", " << tz << ")";
        break;
      case CASTLE:
        ss << "Castle: (" << ox << ", " << oy << ", " << oz << ") -> (" <<
                tx << ", " << ty << ", " << tz << ")";
        break;
      case PROMOTE:
        ss << "Promote: (" << ox << ", " << oy << ", " << oz << ") -> (" <<
                tx << ", " << ty << ", " << tz << ") = " << toString(m.promoted);
        break;
      case PROMO_CAPTURE:
        ss << "Promo-capture: (" << ox << ", " << oy << ", " << oz << ") -> (" <<
                tx << ", " << ty << ", " << tz << ") = " << toString(m.promoted) <<
                " - " << toString(m.captured);
        break;
    }

    return ss.str();
}

/**
 * Puts pieces on the board, and then generates all the moves that the first
 * piece can make. Then it converts the moves to strings, sorts them, and
 * concatenates them. If this new string matches the given string, returns
 * true, otherwise false.
 * 
 * Parameters:
 *     pieces, indices - pieces and their locations
 *     expected - the string of concatenated sorted moves we expect to get
 */
bool testConfiguration(list<Piece> pieces, list<int> indices, string expected)
{
    Board b;

    list<Piece>::iterator p_it = pieces.begin();
    list<int>::iterator i_it = indices.begin();
    while(p_it != pieces.end() && i_it != indices.end())
        b.putPiece(*p_it++, *i_it++);

    list<Move> moves = b.generateMoves(indices.front());

    list<string> strs;
    for(list<Move>::iterator it = moves.begin(); it != moves.end(); it++)
        strs.push_back(toString(*it));
    strs.sort();

    string concatenated = "";
    for(list<string>::iterator it = strs.begin(); it != strs.end(); it++)
        concatenated += *it;

    return (concatenated == expected);
}

int main(void)
{
    cout << "----Begin test----" << endl;

    // TODO load data from text files and run them through testConfiguration

    cout << "----End test----" << endl;

    return 0;
}
