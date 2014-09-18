#include "board.h"

using std::list;
using std::stack;

/*
 * The following lookup tables are all indexed by PieceType. If the order of
 * them changes, make sure to update this as well.
 */

/**
 * This array just tells you how long the relevant row of
 * PIECE_DIRECTIONS is (the rest is garbage).
 */
static const int NUM_DIRECTIONS [16] = {
    0, 0, 8, 8,
    24, 24, 24, 72,
    6, 12, 8,
    18, 20, 14,
    26, 26
};

/** Whether the indexing piece is a sliding piece. */
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

    /* W_PAWN - Captures only */
    {157, 156, 155, 145, 143, 133, 132, 131},
    /* B_PAWN - Captures only */
    {-157,-156,-155,-145,-143,-133,-132,-131},


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

/** Represents the lack of an en passant square for the turn. */
const int NO_EP_SQUARE = 0;

/** Describes the initial setup of the back rank. */
static const PieceType INITIAL_SETUP [8][8] = {
   {  WIZARD,  DRAGON, GRIFFIN,    ROOK,    ROOK, GRIFFIN,  DRAGON,  WIZARD },
   {  DRAGON,  CANNON,    MACE,  KNIGHT,  KNIGHT,    MACE,  CANNON,  DRAGON },
   { GRIFFIN,    MACE,  ARCHER,  BISHOP,  BISHOP,  ARCHER,    MACE, GRIFFIN },
   {    ROOK,  KNIGHT,  BISHOP, UNICORN,   QUEEN,  BISHOP,  KNIGHT,    ROOK },
   {    ROOK,  KNIGHT,  BISHOP,   QUEEN,    KING,  BISHOP,  KNIGHT,    ROOK },
   { GRIFFIN,    MACE,  ARCHER,  BISHOP,  BISHOP,  ARCHER,    MACE, GRIFFIN },
   {  DRAGON,  CANNON,    MACE,  KNIGHT,  KNIGHT,    MACE,  CANNON,  DRAGON },
   {  WIZARD,  DRAGON, GRIFFIN,    ROOK,    ROOK, GRIFFIN,  DRAGON,  WIZARD }
};

//----HELPER METHODS----

/** Returns the square that the king of the given team starts on. */
static int kingSquare(bool color)
{
    int z = (color == WHITE) ? 0 : 7;
    return mailbox(4,4,z);
}

/** Returns the direction along which the king castles. */
static int castleDir(int axis)
{
    int array [6] = {1, 12, 13, -1, -12, -13};
    return array[axis];
}

/** Returns the distance from the king to the rook. */
static int castleDist(int axis)
{
   return (axis < 3) ? 3 : 4;
}

/**
 * If color is WHITE, returns 1 << axis.
 * If it's BLACK, returns 1 << (axis + 6).
 */
static int castleMask(bool color, int axis)
{
   int offset = (color == WHITE) ? 0 : 6;
   return (1 << (offset + axis)); 
}

/**
 * If color is WHITE, returns 0b111111.
 * If it's BLACK, returns 0b111111 << 6.
 */
static int castleMaskAll(bool color)
{
   return (color == WHITE) ? 0x3F : 0xFC;
}

//----CLASS METHODS----

Board::Board()
{
    for(int i = 0; i < 1728; i++)
        pieces_[i] = Piece(BORDER, WHITE);

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            for(int k = 0; k < 8; k++)
                pieces_[mailbox(i, j, k)] = Piece(NIL, WHITE);

    ep_locations_.push(NO_EP_SQUARE);
    castling_rights_.push(0x0FFF);
}

void Board::setup()
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            PieceType pt = INITIAL_SETUP[i][j];
            pieces_[mailbox(i,j,0)] = Piece(pt, WHITE);
            pieces_[mailbox(i,j,1)] = Piece(W_PAWN, WHITE);
            pieces_[mailbox(i,j,2)] = Piece(NIL, WHITE);
            pieces_[mailbox(i,j,3)] = Piece(NIL, WHITE);
            pieces_[mailbox(i,j,4)] = Piece(NIL, WHITE);
            pieces_[mailbox(i,j,5)] = Piece(NIL, WHITE);
            pieces_[mailbox(i,j,6)] = Piece(B_PAWN, BLACK);
            pieces_[mailbox(i,j,7)] = Piece(pt, BLACK);
        }
    }

    history_ = stack<Move>();
    ep_locations_ = stack<int>();
    castling_rights_ = stack<unsigned short>();
    captured_ = stack<Piece>();

    ep_locations_.push(NO_EP_SQUARE);
    castling_rights_.push(0x0FFF);
}

Piece Board::getPiece(int i) const
{
    return pieces_[i];
}

Piece Board::putPiece(const Piece& p, int i)
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

    // TODO should I wrap this into generateMoves for a king?
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

    int king_sq = kingSquare(color);

    for(int axis = 0; axis < 6; axis++)
    {
        // Get the direction of travel
        int dir = castleDir(axis);

        // Yes, that is a goto. Yes, I know it's frowned upon. But for this
        // particular circumstance, it's the cleanest tool for the job.
        if(castling_rights_.top() & castleMask(color, axis))
        {
            // Check if the path is clear
            for(int dist = 1; dist < castleDist(axis); dist++)
            {
                if(pieces_[king_sq + dist * dir].type() != NIL)
                    goto failed;
            }

            moves.push_back(Move(color, CASTLE, king_sq, king_sq + 2 * dir));
        }

        failed:
            ; // empty statement
    }

    return moves;
}

bool Board::isLegalMove(const Move& m) const // TODO make non-const version that uses undo
{
    Board copy (*this);

    if(m.type() == CASTLE)
    {
        // Note: we don't need to move the rook/wizard to determine
        // intermediate check. Only the king.
        int dir = (m.target() - m.origin()) / 2;
        int middle = m.origin() + dir;

        // If any king is in check, then we cannot castle
        Piece king = copy.pieces_[m.origin()];
        copy.pieces_[m.target()] = king;
        copy.pieces_[middle] = king;
        return !copy.isInCheck(m.color());
    }
    else
    {
        copy.makeMove(m);
        return !copy.isInCheck(m.color());
    }
}

void Board::makeMove(const Move& m)
{
    MoveType type = m.type();
    int forward = (m.color() == WHITE) ? 144 : -144;

    int next_ep = NO_EP_SQUARE;  // Modified in DPP only
    int dir, dist, rook_sq; // Used for castling only

    switch(type)
    {
      case QUIET:
        break;
      case DOUBLE_PAWN_PUSH:
        next_ep = m.origin() + forward;
        break;
      case CAPTURE:
        captured_.push(pieces_[m.target()]);
        break;
      case EN_PASSANT:
        pieces_[m.target() - forward] = Piece(NIL, WHITE);
        break;
      case CASTLE:
        // The king moves two squares, so we can find the direction by halving,
        dir = (m.target() - m.origin()) / 2;
        // the distance, by the sign of the direction,
        dist = (dir > 0) ? 3 : 4;
        // and the rook square by combining those.
        rook_sq = m.origin() + dir * dist;
        // Move the rook
        pieces_[m.origin() + dir] = pieces_[rook_sq];
        pieces_[rook_sq] = Piece(NIL, WHITE);
        break;
      case PROMOTE:
        pieces_[m.origin()] = Piece(m.promoted(), m.color());
        break;
      case PROMO_CAPTURE:
        pieces_[m.origin()] = Piece(m.promoted(), m.color());
        captured_.push(pieces_[m.target()]);
        break;
    }

    // Moves piece from origin to target, and clears the origin
    pieces_[m.target()] = pieces_[m.origin()];
    pieces_[m.origin()] = Piece(NIL, WHITE);

    // Push the next en passant location
    ep_locations_.push(next_ep);

    // Maybe we moved the king or rooks/wizards?
    updateCastlingRights(m.color(), m.origin());

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
    int forward = (m.color() == WHITE) ? 144 : -144;

    int dir, dist, rook_sq; // Used for castling only

    switch(type)
    {
      case QUIET:
      case DOUBLE_PAWN_PUSH:
        break;
      case CAPTURE:
        pieces_[m.target()] = captured_.top();
        captured_.pop();
        break;
      case EN_PASSANT:
        pieces_[m.target() - forward] = Piece::Pawn(!m.color());
        break;
      case CASTLE:
        // The king moves two squares, so we can find the direction by halving,
        dir = (m.target() - m.origin()) / 2;
        // the distance, by the sign of the direction,
        dist = (dir > 0) ? 3 : 4;
        // and the rook square by combining those.
        rook_sq = m.origin() + dir * dist;
        // Move the rook back
        pieces_[rook_sq] = pieces_[m.origin() + dir];
        pieces_[m.origin() + dir] = Piece(NIL, WHITE);
        break;
      case PROMOTE:
        pieces_[m.origin()] = Piece::Pawn(m.color());
        break;
      case PROMO_CAPTURE:
        pieces_[m.origin()] = Piece::Pawn(m.color());
        pieces_[m.target()] = captured_.top();
        captured_.pop();
        break;
    }
}

// TODO this is very slow, and it's called in many places. Consider re-writing
// or cacheing the result
bool Board::isInCheck(bool color) const
{
    list<Move> opponents_moves = generatePseudoLegalMoves(!color);
    list<Move>::const_iterator it;
    for(it = opponents_moves.begin(); it != opponents_moves.end(); it++)
    {
        if(it->type() == CAPTURE || it->type() == PROMO_CAPTURE)
        {
            Piece captured = pieces_[it->target()];
            if(captured.type() == KING)
                return true;
        }
    }

    return false;
}

GameState Board::getGameState() const
{
    if(isInSomemate(WHITE))
        return isInCheck(WHITE) ? CHECKMATE_WHITE : STALEMATE_WHITE;

    if(isInSomemate(BLACK))
        return isInCheck(BLACK) ? CHECKMATE_BLACK : STALEMATE_BLACK;

    return IN_PROGRESS;
}

const stack<Move> Board::getHistory() const
{
    return history_;
}

//----PRIVATE----

list<Move> Board::generatePawnMoves(int origin) const
{
    list<Move> moves;
    Piece p = pieces_[origin];
    bool color = p.color();

    int forward = (color == WHITE) ? 144 : -144;
    int ahead = origin + forward;
    int twoAhead = ahead + forward;

    int rank = (origin / 144) - 2;
    int homeRank = (color == WHITE) ? 1 : 6;
    int promoRank = 7 - homeRank;

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
                moves.push_back(Move(color, PROMOTE, origin, ahead, pt));
            }
        }
        else
            moves.push_back(Move(color, QUIET, origin, ahead));

        // Can we perform a double pawn push?
        if(rank == homeRank && pieces_[twoAhead].type() == NIL)
            moves.push_back(Move(color, DOUBLE_PAWN_PUSH, origin, twoAhead));
    }

    // Can we capture things?
    for(int i = 0; i < NUM_DIRECTIONS[p.type()]; i++)
    {
        int target = origin + PIECE_DIRECTIONS[p.type()][i];
        Piece target_piece = pieces_[target];

        // Is this an enemy piece?
        if(p.isEnemy(target_piece))
        {
            // Are we currently on the second-to-last rank?
            if(rank == promoRank)
            {
                // Iterate through all possible promotions
                for(int i = 0; i < NUM_PROMOTION_PIECES; i++)
                {
                    PieceType pt = PROMOTION_PIECES[i];
                    moves.push_back(Move(color, PROMO_CAPTURE, origin, target, pt));
                }
            }
            else
                moves.push_back(Move(color, CAPTURE, origin, target));
        }

        // Can we perform en passant?
        if(ep_locations_.top() == target)
            moves.push_back(Move(color, EN_PASSANT, origin, target));

    }

    return moves;
}

list<Move> Board::generateNonPawnMoves(int origin) const
{
    list<Move> moves;
    Piece p = pieces_[origin];
    PieceType type = p.type();
    bool color = p.color();

    int num_dirs = NUM_DIRECTIONS[type];

    // Iterate through every permissible move direction
    for(int i = 0; i < num_dirs; i++)
    {
        int target = origin;
        while(true) // this is so sliding pieces continue moving
        {
            target += PIECE_DIRECTIONS[type][i];
            Piece target_piece = pieces_[target];

            // Did we hit something?
            if(target_piece.type() != NIL)
            {
                // Was it an enemy piece?
                if(p.isEnemy(target_piece))
                    moves.push_back(Move(color, CAPTURE, origin, target));
                break;
            }
            moves.push_back(Move(color, QUIET, origin, target));

            // Is this a non-sliding piece?
            if(!SLIDING[type])
                break;
        }
    }

    return moves;
}

void Board::updateCastlingRights(bool color, int origin)
{
    // This means we only have to make one color lookup
    int dist_from_king = origin - kingSquare(color);
    int current_rights = castling_rights_.top();

    // If we moved the king, remove all castling rights for that side
    if(dist_from_king == 0)
    {
        castling_rights_.push(current_rights &~ castleMaskAll(color));
        return;
    }

    for(int i = 0; i < 6; i++)
    {
        // If we moved a rook, remove the rights just for that axis
        if(dist_from_king == castleDir(i) * castleDist(i))
        {
            castling_rights_.push(current_rights &~ castleMask(color, i));
            return;
        }
    }

    // If we didn't move them at all, just keep the same rights
    castling_rights_.push(current_rights);
}

bool Board::isInSomemate(bool color) const
{
    list<Move> moves = generatePseudoLegalMoves(color);

    list<Move>::const_iterator it;
    for(it = moves.begin(); it != moves.end(); it++)
    {
        if(isLegalMove(*it))
        {
            return false;
        }
    }
    return true;
}
