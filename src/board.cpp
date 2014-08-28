#include "board.h"

using std::list;

/**
 * The following lookup tables are all indexed by PieceType. If the order of
 * them changes, make sure to update this as well.
 */
namespace {
    /**
     * This array just tells you how long the relevant row of
     * PIECE_DIRECTIONS is (the rest is garbage).
     */
    static const int NUM_DIRECTIONS [16] = {
        0, 0, 9, 9,
        24, 24, 24, 72,
        6, 12, 8,
        18, 20, 14,
        26, 26
    };

    /** Whether the indexing piece is a sliding piece */
    static const bool SLIDING [16] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        1, 1, 1,
        1, 1, 1,
        1, 0
    };

    /**
     * The directions that the indexing piece can move in. Since the pieces are
     * stored in a linear array, we can encode an offset triple (x,y,z) as a
     * single integer offset.
     */
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
        {13, 11, 145, 143, 156, 132, -13, -11, -145, -143, -156, -132,
         157, 155, 133, 131, -157, -155, -133, -131},
        /* CANNON */
        {1, 12, 144, -1, -12, -144,
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
        pieces_[i] = Piece(BORDER, WHITE);

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            for(int k = 0; k < 8; k++)
                pieces_[mailbox(i, j, k)] = Piece(NIL, WHITE);
}

Piece Board::getPiece(int i) const
{
    return pieces_[i];
}

Piece Board::putPiece(Piece p, int i)
{
    Piece q = pieces_[i];
    pieces_[i] = p;
    return q;
}

int Board::getEnPassant() const
{
    return ep_location_;
}

void Board::setEnPassant(int i)
{
    ep_location_ = i;
}

list<Move> Board::generatePseudoLegalMoves(int color) const
{
    list<Move> moves;
    list<Move> temp;

    for(int i = 0; i < 1728; i++)
    {
        if(pieces_[i].isOn(color))
        {
            temp = generateMoves(i);
            moves.splice(moves.end(), temp);
        }
    }

    temp = generateCastlingMoves(color);
    moves.splice(moves.end(), temp);

    return moves;
}

list<Move> Board::generateMoves(int origin) const
{
    PieceType pt = pieces_[origin].type();

    if(pt == NIL || pt == BORDER)
        return list<Move>(0);
    else if(pt == W_PAWN || pt == B_PAWN)
        return generatePawnMoves(origin);
    else
        return generateNonPawnMoves(origin);
}

list<Move> Board::generateCastlingMoves(bool color) const
{
    list<Move> moves;

    // TODO actually implement castling

    return moves;
}

bool Board::isInCheck(bool color) const
{
    list<Move> opponents_moves = generatePseudoLegalMoves(!color);
    list<Move>::const_iterator it;
    for(it = opponents_moves.begin(); it != opponents_moves.end(); it++)
    {
        if(it->type() == CAPTURE || it->type() == PROMO_CAPTURE)
        {
            Piece captured = it->captured();
            if(captured.type() == KING)
                return true;
        }
    }

    return false;
}

list<Move> Board::generatePawnMoves(int origin) const
{
    list<Move> moves;
    Piece oPiece = pieces_[origin];
    PieceType oPt = oPiece.type();

    int forward = PIECE_DIRECTIONS[oPt][0];
    int ahead = origin + forward;
    int twoAhead = ahead + forward;

    int rank = (origin / 144) - 2;

    // Mmm... Tasty bitwise hacks.     // WHITE    BLACK
    int color_flag = -(oPt == W_PAWN); //  -1        0
    int color_mask = color_flag & 0x7; //   7        0
    int startingRank = color_mask ^ 6; //   1        6
    int promoRank = color_mask ^ 1;    //   6        1

    // Can we move forward?
    if(pieces_[ahead].type() == NIL)
    {
        // Are we currently on the second-to-last rank?
        if(rank == promoRank)
        {
            // Iterate through all possible promotions
            for(int i = 0; i < NUM_PROMOTION_PIECES; i++)
            {
                Piece p = Piece(PROMOTION_PIECES[i], oPiece.color());
                moves.push_back(Move::Promote(origin, ahead, p));
            }
        }
        else
            moves.push_back(Move::Quiet(origin, ahead));

        // Can we perform a double pawn push?
        if(rank == startingRank && pieces_[twoAhead].type() == NIL)
            moves.push_back(Move::DPP(origin, twoAhead));
    }

    // We start at 1, because the first entry is a forward move
    for(int i = 1; i < NUM_DIRECTIONS[oPt]; i++)
    {
        int target = origin + PIECE_DIRECTIONS[oPt][i];
        Piece tPiece = pieces_[target];

        // Is this an enemy piece?
        if(oPiece.isEnemy(tPiece))
        {
            // Are we currently on the second-to-last rank?
            if(rank == promoRank)
            {
                // Iterate through all possible promotions
                for(int i = 0; i < NUM_PROMOTION_PIECES; i++)
                {
                    Piece p = Piece(PROMOTION_PIECES[i], oPiece.color());
                    moves.push_back(Move::PromoCapture(origin, target, p,
                            tPiece));
                }
            }
            else
                moves.push_back(Move::Capture(origin, target, tPiece));
        }

        // Can we perform en passant?
        if(target == ep_location_)
            moves.push_back(Move::EP(origin, ep_location_));

    }

    return moves;
}

list<Move> Board::generateNonPawnMoves(int origin) const
{
    list<Move> moves;
    Piece oPiece = pieces_[origin];
    PieceType oPt = oPiece.type();

    int num_dirs = NUM_DIRECTIONS[oPt];

    // Iterate through every permissible move direction
    for(int i = 0; i < num_dirs; i++)
    {
        int target = origin;
        while(true) // this is so sliding pieces continue moving
        {
            target += PIECE_DIRECTIONS[oPt][i];
            Piece tPiece = pieces_[target];

            // Did we hit something?
            if(!tPiece.type() == NIL)
            {
                // Was it an enemy piece?
                if(oPiece.isEnemy(tPiece))
                    moves.push_back(Move::Capture(origin, target, tPiece));
                break;
            }
            moves.push_back(Move::Quiet(origin, target));

            // Is this a non-sliding piece?
            if(!SLIDING[oPt])
                break;
        }
    }

    return moves;
}
