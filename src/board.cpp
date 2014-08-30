#include "board.h"

// TODO have some functions that, given WHITE or BLACK, return things like
// promotion row, pawn home row, king squares, castling_rights_ offset, etc.
// To find these, search (color == WHITE) or such.

using std::list;

/**
 * The first 3 following lookup tables are all indexed by PieceType. If the
 * order of them changes, make sure to update this as well. The last is an
 * arbitrary order.
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

    // TODO should I overhaul this?
    static const int KING_SQUARE [2] = {mailbox(4,4,0), mailbox(4,4,7)};
    static const int CASTLE_DIRECTIONS [6] = {1, 12, 13, -1, -12, -13};
    static const int CASTLE_DISTANCES  [6] = {3,  3,  3,  4,   4,   4};
}

Board::Board()
{
    for(int i = 0; i < 1728; i++)
        pieces_[i] = Piece(BORDER, WHITE);

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            for(int k = 0; k < 8; k++)
                pieces_[mailbox(i, j, k)] = Piece(NIL, WHITE);

    ep_locations_.push(0);
    castling_rights_.push(0xFFFF);
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

// Note: This cannot be modified to only get legal castles, because that would
// require calling "isInCheck", which calls "generatePseudoLegalMoves", which
// calls this method, ...
list<Move> Board::generateCastlingMoves(bool color) const
{
    list<Move> moves;

    int mask = (color == WHITE) ? 1 : (1 << 6);
    int king_sq = KING_SQUARE[color];

    for(int axis = 0; axis < 6; axis++)
    {
        // Get the direction of travel
        int dir = CASTLE_DIRECTIONS[axis];

        // Yes, that is a goto. Yes, I know it's frowned upon. But for this
        // particular control flow, it's the right tool for the job.
        if(!(castling_rights_.top() & mask))
            goto failed;

        // Check if the path is clear
        for(int dist = 1; dist < CASTLE_DISTANCES[axis]; dist++)
        {
            if(getPiece(king_sq + dist * dir).type() != NIL)
                goto failed;
        }

        moves.push_back(Move::Castle(king_sq, king_sq + 2 * dir));

        failed:
        mask <<= 1; // moves the masking bit up one, to match axis++
    }

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
            Piece captured = getPiece(it->origin());
            if(captured.type() == KING)
                return true;
        }
    }

    return false;
}

void Board::makeMove(Move m)
{
    MoveType type = m.type();
    bool color = getPiece(m.origin()).color();
    int forward = (color == WHITE) ? 144 : -144;

    int dir, rook_sq; // Used for castling only

    switch(type)
    {
      case QUIET:
        break;
      case DOUBLE_PAWN_PUSH:
        break;
      case CAPTURE:
        captured_.push(pieces_[m.target()]);
        break;
      case EN_PASSANT:
        pieces_[m.target() - forward] = Piece(NIL, WHITE);
        break;
      case CASTLE:
        // The king moves two squares, so we can find the direction by halving
        dir = (m.target() - m.origin()) >> 1;
        // and the rook square by adding it 3 or 4 times.
        rook_sq = m.origin() + dir * ((dir > 0) ? 3 : 4);
        // Move the rook
        pieces_[m.origin() + dir] = pieces_[rook_sq];
        pieces_[rook_sq] = Piece(NIL, WHITE);
        break;
      case PROMOTE:
        pieces_[m.origin()] = Piece(m.promoted(), color);
        break;
      case PROMO_CAPTURE:
        pieces_[m.origin()] = Piece(m.promoted(), color);
        captured_.push(pieces_[m.target()]);
        break;
    }

    // Moves piece from origin to target, and clears the origin
    pieces_[m.target()] = pieces_[m.origin()];
    pieces_[m.origin()] = Piece(NIL, WHITE);

    // We can only perform en passant next turn if this turn was a DPP
    if(type == DOUBLE_PAWN_PUSH)
        ep_locations_.push(m.origin() + forward);
    else
        ep_locations_.push(0);

    // Maybe we moved the king or rooks/wizards?
    updateCastlingRights(color, m.origin());

    // Records move
    history_.push(m);
}

void Board::undoMove()
{
    if(history_.empty())
        return;

    // Recalls the most recent move
    Move m = history_.top();
    history_.pop();

    // Recalls the previous castling rights
    castling_rights_.pop();

    // Recalls the previous en passant square, if any
    ep_locations_.pop();

    // Moves piece from target to origin, and clears the target
    pieces_[m.origin()] = pieces_[m.target()];
    pieces_[m.target()] = Piece(NIL, WHITE);

    MoveType type = m.type();
    bool color = pieces_[m.origin()].color();
    int forward = (color == WHITE) ? 144 : -144;

    int dir, rook_sq; // Used for castling only

    switch(type)
    {
      case QUIET:
        break;
      case DOUBLE_PAWN_PUSH:
        break;
      case CAPTURE:
        pieces_[m.target()] = captured_.top();
        captured_.pop();
        break;
      case EN_PASSANT:
        pieces_[m.target() - forward] = Piece::Pawn(!color);
        break;
      case CASTLE:
        // The king moves two squares, so we can find the direction by halving
        dir = (m.target() - m.origin()) >> 1;
        // and the rook square by adding it 3 or 4 times.
        rook_sq = m.origin() + dir * ((dir > 0) ? 3 : 4);
        // Move the rook back
        pieces_[rook_sq] = pieces_[m.origin() + dir];
        pieces_[m.origin() + dir] = Piece(NIL, WHITE);
        break;
      case PROMOTE:
        pieces_[m.origin()] = Piece::Pawn(color);
        break;
      case PROMO_CAPTURE:
        pieces_[m.origin()] = Piece::Pawn(color);
        pieces_[m.target()] = captured_.top();
        captured_.pop();
        break;
    }
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
                PieceType pt = PROMOTION_PIECES[i];
                moves.push_back(Move::Promote(origin, ahead, pt));
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
                    PieceType pt = PROMOTION_PIECES[i];
                    moves.push_back(Move::PromoCapture(origin, target, pt));
                }
            }
            else
                moves.push_back(Move::Capture(origin, target));
        }

        // Can we perform en passant?
        if(ep_locations_.top() == target)
            moves.push_back(Move::EP(origin, target));

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
                    moves.push_back(Move::Capture(origin, target));
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

void Board::updateCastlingRights(bool color, int origin)
{
    // This means we only have to make one color lookup
    int dist_from_king = origin - KING_SQUARE[color];
    int current_rights = castling_rights_.top();

    int offset = (color == WHITE) ? 0 : 6;

    // If we moved the king, remove all castling rights for that side
    if(dist_from_king == 0)
    {
        castling_rights_.push(current_rights &~ (0x3F << offset));
        return;
    }

    for(int i = 0; i < 6; i++)
    {
        // If we moved a rook, remove the rights just for that axis
        if(dist_from_king == CASTLE_DIRECTIONS[i] * CASTLE_DISTANCES[i])
        {
            castling_rights_.push(current_rights &~ (1 << (offset + i)));
            return;
        }
    }
}
