#include "../src/board.h"

#include "unit_test.h"

#include <string>
#include <sstream>

using std::list;
using std::string;
using std::stringstream;

// TODO maybe move these into the appropriate cpp files, wrapped in an #ifdef?
// perhaps as << overloads

/**
 * Converts the piece into a string representation.
 * Not that useful, but unlike pieces, strings can be sorted.
 */
string toString(Piece p)
{
    stringstream ss;

    if(p.type() == NIL)
        return "Nil";

    if(p.type() == BORDER)
        return "Border";

    string names [16] = {
        "", "", "Pawn", "Pawn", "Knight", "Griffin", "Dragon", "Unicorn",
        "Rook", "Bishop", "Mace", "Wizard", "Archer", "Cannon", "Queen", "King"
    };
    
    string color = (p.color() == WHITE ? "White" : "Black");

    string moved = (p.moved() ? "(M)" : "(S)");

    ss << color << " " << names[p.type()] << " " << moved;

    return ss.str();
}

/**
 * Converts the move into a string representation.
 * Not that useful, but unlike moves, strings can be sorted.
 */
string toString(Move m)
{
    stringstream ss;

    int ox = unmailboxX(m.origin());
    int oy = unmailboxY(m.origin());
    int oz = unmailboxZ(m.origin());

    int tx = unmailboxX(m.target());
    int ty = unmailboxY(m.target());
    int tz = unmailboxZ(m.target());

    switch(m.type())
    {
      case QUIET:
        ss << "Quiet: (" << ox << ", " << oy << ", " << oz << ") -> (" << tx <<
                ", " << ty << ", " << tz << ")";
        break;
      case DOUBLE_PAWN_PUSH:
        ss << "DPP: (" << ox << ", " << oy << ", " << oz << ") -> (" << tx <<
                ", " << ty << ", " << tz << ")";
        break;
      case CAPTURE:
        ss << "Capture: (" << ox << ", " << oy << ", " << oz << ") -> (" <<
                tx << ", " << ty << ", " << tz << ") - " <<
                toString(m.captured());
        break;
      case EN_PASSANT:
        ss << "EP: (" << ox << ", " << oy << ", " << oz << ") -> (" << tx <<
                ", " << ty << ", " << tz << ")";
        break;
      case CASTLE:
        ss << "Castle: (" << ox << ", " << oy << ", " << oz << ") -> (" <<
                tx << ", " << ty << ", " << tz << ")";
        break;
      case PROMOTE:
        ss << "Promote: (" << ox << ", " << oy << ", " << oz << ") -> (" <<
                tx << ", " << ty << ", " << tz << ") = " <<
                toString(m.promoted());
        break;
      case PROMO_CAPTURE:
        ss << "Promo-capture: (" << ox << ", " << oy << ", " << oz <<
                ") -> (" << tx << ", " << ty << ", " << tz << ") = " <<
                toString(m.promoted()) << " - " << toString(m.captured());
        break;
    }

    return ss.str();
}

PieceType getPieceTypeFromChar(char name, bool color)
{
    switch(name)
    {
      case 'P': return (color == WHITE ? W_PAWN : B_PAWN);
      case 'N': return KNIGHT;
      case 'G': return GRIFFIN;
      case 'D': return DRAGON;
      case 'U': return UNICORN;
      case 'R': return ROOK;
      case 'B': return BISHOP;
      case 'M': return MACE;
      case 'W': return WIZARD;
      case 'A': return ARCHER;
      case 'C': return CANNON;
      case 'Q': return QUEEN;
      case 'K': return KING;
      default:  return NIL;
    }
}

/**
 * Puts pieces on the board, and then generates all the moves that the first
 * piece can make. Then it converts the moves to strings, sorts them, and
 * concatenates them. If this new string matches the given string, returns
 * true, otherwise false.
 * 
 * Parameters:
 *     name - the name of the test
 *     pieces, indices - pieces and their locations
 *     expected - the string of concatenated sorted moves we expect to get
 */
void testConfiguration(string name, list<Piece> pieces, list<int> indices,
        string expected)
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
        concatenated += (*it + "\n");

    EXPECT_TRUE(concatenated == expected);
}

/**
 * Reads a bizarrely formatted text file containing lists of pieces and moves.
 * It parses them, and feeds the pieces and moves to testConfiguration. This is
 * pretty janky, but it does the job. (And it's not production code, at least?)
 *
 * TODO perhaps give the file loading code to MissionControl?
 */
TEST(Board, MoveGeneration)
{
    // TODO what's convention? run from root dir, or from bin dir?
    std::ifstream file ("test/test_board_aux.txt");

    ASSERT_TRUE(file.is_open());

    string line;
    bool loadingPieces = true; // if not, loading strings

    string test_name;
    list<Piece> pieces;
    list<int> indices;
    string move_strs = "";

    while(getline(file, line))
    {
        if(line[0] == '#' || line == "")
            continue;

        if(line[0] == '+')
        {
            loadingPieces = true;
            test_name = line.substr(1);
        }
        else if(line[0] == '-')
        {
            loadingPieces = false;
        }
        else if(line[0] == '=')
        {
            testConfiguration(test_name, pieces, indices, move_strs);
            pieces.clear();
            indices.clear();
            move_strs = "";
        }
        else if(loadingPieces)
        {
            bool moved;
            int x, y, z;
            char name, color;
            stringstream ss;
            ss << line;
            ss >> color >> name >> x >> y >> z >> moved;

            bool colorBool = color == 'W' ? WHITE : BLACK;
            PieceType pt = getPieceTypeFromChar(name, colorBool);

            Piece p = Piece(pt, colorBool, moved);
            int i = mailbox(x,y,z);

            pieces.push_back(p);
            indices.push_back(i);
        }
        else
        {
            move_strs += (line + "\n");
        }
    }
}
