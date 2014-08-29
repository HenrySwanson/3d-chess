#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <list>
#include <stack>

#include "move.h"
#include "piece.h"

// TODO add move history stack, captured stack, and implement castling flags
// (another stack, probably. 6 bits per side)

/**
 * Represents an 8 x 8 x 8 chessboard, the pieces on it, and all necessary
 * metadata, such as move history, and if en passant is possible.
 * Can generate lists of all legal and pseudo-legal moves.
 */
class Board
{
  public:
    /** Generates an empty board. */
    Board();

    /** Retrieves the piece at i. */
    Piece getPiece(int i) const;

    /** Puts the specifed piece at i and returns the previous occupant. */
    Piece putPiece(Piece p, int i);

    /** Generates all pseudo-legal moves that the given color can make. */
    std::list<Move> generatePseudoLegalMoves(int color) const;

    // TODO generate all legal moves (and have an isLegal(Move))

    /**
     * Generates all pseudo-legal moves that the piece on this square can make.
     * It is safe to call this method on a non-piece (NIL or BORDER); it will
     * just return an empty list.
     */
    std::list<Move> generateMoves(int origin) const;

    /** Generates all possible castling moves for the given team. */
    std::list<Move> generateCastlingMoves(bool color) const;

    /** Returns true if the king of the specified color is in check. */
    bool isInCheck(bool color) const;

    // TODO checkmate and stalemate

    // TODO implement move making and unmaking

  private:
    /** Generates all pseudo-legal moves for a pawn. */ 
    std::list<Move> generatePawnMoves(int origin) const;

    /** Generates all pseudo-legal moves for a non-pawn piece. */ 
    std::list<Move> generateNonPawnMoves(int origin) const;

    /**
     * Represents the pieces on the board. This is a "mailbox" representation,
     * which is, apparently, a pretty common representation for chess engines.
     *
     * There are two parts to understanding this representation. The 8 x 8 x 8
     * chessboard is centered inside a 12 x 12 x 12 cube. The edges of this
     * cube are always set to the PieceType BORDER. This makes move generation
     * much easier, because when a piece reaches the edge of the board, instead
     * of "wrapping around", it hits the sentinel value of BORDER.
     *
     * The second part is that this 12 x 12 x 12 cube is unspooled into a
     * linear array of size 1728. The x-coordinates vary first, then y, then z.
     */
    Piece pieces_ [1728];

    std::stack<Move> history_;
    std::stack<Piece> captured_;
    bool ep_possible_;
    std::stack<int> ep_locations_;
    std::stack<unsigned char> castling_rights_white_;
    std::stack<unsigned char> castling_rights_black_;
};

#endif
