#include "board.h"

using std::list;

namespace {
    static const int NUM_DIRECTIONS [16] = {
        0, 0, 1, 1,
        24, 24, 24, 72,
        6, 12, 8,
        18, 14, 20,
        26, 26
    };

    static const bool SLIDING [16] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        1, 1, 1,
        1, 1, 1,
        1, 0
    };

    static const int PIECE_DIRECTIONS [16][72] = {
        /* NIL, BORDER, W_PAWN, B_PAWN */
        {}, {}, {}, {},

        /* KNIGHT */
        {  14,  10,  25,  23, 146, 142, 289, 287, 168, 120, 300, 276,
          -14, -10, -25, -23,-146,-142,-289,-287,-168,-120,-300,-276},
        /* GRIFFIN */
        { 158, 154, 134, 130, 169, 121, 167, 119, 301, 275, 299, 277,
         -158,-154,-134,-130,-169,-121,-167,-119,-301,-275,-299,-277},
        /* DRAGON */
        { 313, 311, 265, 263, 302, 278, 298, 274, 170, 118, 166, 122,
         -313,-311,-265,-263,-302,-278,-298,-274,-170,-118,-166,-122},
        /* UNICORN */
        {  14,  10,  25,  23, 146, 142, 289, 287, 168, 120, 300, 276,
          -14, -10, -25, -23,-146,-142,-289,-287,-168,-120,-300,-276,
          158, 154, 134, 130, 169, 121, 167, 119, 301, 275, 299, 277,
         -158,-154,-134,-130,-169,-121,-167,-119,-301,-275,-299,-277,
          313, 311, 265, 263, 302, 278, 298, 274, 170, 118, 166, 122,
         -313,-311,-265,-263,-302,-278,-298,-274,-170,-118,-166,-122},

        /* ROOK */
        {1, 12, 144, -1, -12, -144},
        /* BISHOP */
        {13, 11, 145, 143, 156, 132, -13, -11, -145, -143, -156, -132},
        /* MACE */
        {157, 155, 133, 131, -157, -155, -133, -131},

        /* WIZARD */
        {1, 12, 144, -1, -12, -144,
         13, 11, 145, 143, 156, 132, -13, -11, -145, -143, -156, -132},
        /* ARCHER */
        {1, 12, 144, -1, -12, -144,
         157, 155, 133, 131, -157, -155, -133, -131},
        /* CANNON */
        {13, 11, 145, 143, 156, 132, -13, -11, -145, -143, -156, -132,
         157, 155, 133, 131, -157, -155, -133, -131},

        /* QUEEN */
        {1, 12, 144, -1, -12, -144,
         13, 11, 145, 143, 156, 132, -13, -11, -145, -143, -156, -132,
         157, 155, 133, 131, -157, -155, -133, -131},
        /* KING */
        {1, 12, 144, -1, -12, -144,
         13, 11, 145, 143, 156, 132, -13, -11, -145, -143, -156, -132,
         157, 155, 133, 131, -157, -155, -133, -131}
    };
}

Board::Board()
{
    for(int i = 0; i < 1728; i++)
        squares[i].type = BORDER;

    for(int i = 0; i < 512; i++)
        squares[mailbox(i)].type = NIL;
}

list<Move> Board::generateMoves(int origin)
{
    list<Move> moves;
    Square oSq = squares[origin];
    int num_dirs = NUM_DIRECTIONS[oSq.type];

    // Iterate through every permissible move direction
    for(int i = 0; i < num_dirs; i++)
    {
        int target = origin;
        while(true) // this is so sliding pieces continue moving
        {
            target += PIECE_DIRECTIONS[oSq.type][i];
            Square tSq = squares[target];

            // Did we hit something?
            if(tSq.type != NIL)
            {
                // Was it an enemy piece?
                if(tSq.type != BORDER && tSq.color != oSq.color)
                    moves.push_back(createCaptureMove(origin, target));
                break;
            }
            moves.push_back(createQuietMove(origin, target));

            // Is this a non-sliding piece?
            if(!SLIDING[oSq.type])
                break;
        }
    }

    // TODO pawn moves. remember to promote correctly

    // TODO double pawn push, en passant, castle, promote

    return moves;
}

Move Board::createCaptureMove(int origin, int target)
{
    Move m;
    m.from = origin;
    m.to = target;
    m.captured = squares[target].type;
    return m;
}

Move Board::createQuietMove(int origin, int target)
{
    Move m;
    m.from = origin;
    m.to = target;
    m.captured = NIL;
    return m;
}

int Board::mailbox(int i)
{
    int x = i % 8;
    int y = (i / 8) % 8;
    int z = (i / 64) % 8;

    return (z + 2) * 144 + (y + 2) * 12 + (x + 2);
}

int Board::unmailbox(int i)
{
    int x = i % 12;
    int y = (i / 12) % 12;
    int z = (i / 144) % 12;

    if(x < 2 || x > 9 || y < 2 || y > 9 || z < 2 || z > 9)
        return -1;

    return (z - 2) * 64 + (y - 2) * 8 + (x - 2);
}
