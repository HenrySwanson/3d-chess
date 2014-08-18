#include "board.h"

using std::list;

namespace { //TODO comment the shit out of these
    static const int NUM_DIRECTIONS [16] = {
        0, 0, 8, 8,
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
        /* NIL, BORDER */
        {}, {},

        /* W_PAWN - First entry is a move, the rest are captures */
        { 144, 157, 156, 155, 145, 143, 133, 132, 131},
        /* B_PAWN - First entry is a move, the rest are captures */
        {-144,-157,-156,-155,-145,-143,-133,-132,-131},


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
        pieces[i].type = BORDER;

    for(int i = 0; i < 512; i++)
        pieces[mailbox(i)].type = NIL;
}

list<Move> Board::generateMoves(int origin)
{
    PieceType pt = pieces[origin].type;

    if(pt == NIL || pt == BORDER)
        return list<Move>(0);
    else if(pt == W_PAWN || pt == B_PAWN)
        return generatePawnMoves(origin);
    else
        return generateNonPawnMoves(origin);
}

list<Move> Board::generatePawnMoves(int origin)
{
    list<Move> moves;
    Piece oPiece = pieces[origin];

    int forward = PIECE_DIRECTIONS[oPiece.type][0];
    int ahead = origin + forward;
    int twoAhead = ahead + forward;

    int rank = (origin / 144) - 2;
    // TODO make less (?:)-y
    int startingRank = (oPiece.type == W_PAWN ? 1 : 6);
    int promoRank = (oPiece.type == W_PAWN ? 6 : 1);

    if(pieces[ahead].type == NIL)
    {
        // Are we currently on the second-to-last rank?
        if(rank == promoRank)
        {
            // TODO generate all promotion moves
        }
        else
            moves.push_back(createQuietMove(origin, ahead));

        if(rank == startingRank && pieces[twoAhead].type == NIL)
            moves.push_back(createDPPMove(origin, twoAhead));
    }

    // We start at 1, because the first entry is a forward move
    for(int i = 1; i < NUM_DIRECTIONS[oPiece.type]; i++)
    {
        int target = origin + PIECE_DIRECTIONS[oPiece.type][i];
        Piece tPiece = pieces[target];

        // Is this an enemy piece?
        if(tPiece.type != NIL && tPiece.type != BORDER && tPiece.color != oPiece.color)
        {
            // Are we currently on the second-to-last rank?
            if(rank == promoRank)
            {
                // TODO generate all promo-capture moves
            }
            else
                moves.push_back(createCaptureMove(origin, target, tPiece));
        }

        if(target == en_passant_location)
            moves.push_back(createEPMove(origin, en_passant_location));

    }

    return moves;
}

list<Move> Board::generateNonPawnMoves(int origin)
{
    list<Move> moves;
    Piece oPiece = pieces[origin];
    int num_dirs = NUM_DIRECTIONS[oPiece.type];

    // Iterate through every permissible move direction
    for(int i = 0; i < num_dirs; i++)
    {
        int target = origin;
        while(true) // this is so sliding pieces continue moving
        {
            target += PIECE_DIRECTIONS[oPiece.type][i];
            Piece tPiece = pieces[target];

            // Did we hit something?
            if(tPiece.type != NIL)
            {
                // Was it an enemy piece?
                if(tPiece.type != BORDER && tPiece.color != oPiece.color)
                    moves.push_back(createCaptureMove(origin, target, tPiece));
                break;
            }
            moves.push_back(createQuietMove(origin, target));

            // Is this a non-sliding piece?
            if(!SLIDING[oPiece.type])
                break;
        }
    }

    return moves;
}

list<Move> Board::generateCastlingMoves(bool color)
{
    list<Move> moves;

    // TODO actually implement castling

    return moves;
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
