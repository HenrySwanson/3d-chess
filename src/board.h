#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <list>

#include "move.h"
#include "piece.h"

// TODO pack Piece and Move into bit-fields

/**
 * Represents an 8 x 8 x 8 chessboard, the pieces on it, and all necessary
 * metadata, such as which pieces have moved, and if en passant is possible.
 * Can generate lists of all legal and pseudo-legal moves,
 */
class Board
{
  public:
    /** Generates an empty board */
    Board();

//  private:
    /**
     * Generates all pseudo-legal moves that the piece on this square can make.
     * It is safe to call this method on a non-piece (NIL or BORDER); it will
     * just return an empty list.
     */
    std::list<Move> generateMoves(int origin);

    /** Generates all pseudo-legal moves for a pawn. */ 
    std::list<Move> generatePawnMoves(int origin);

    /** Generates all pseudo-legal moves for a non-pawn piece. */ 
    std::list<Move> generateNonPawnMoves(int origin);

    /** Generates all possible castling moves for the given team. */
    std::list<Move> generateCastlingMoves(bool color);

    /**
     * Represents the pieces on the board. This is a "mailbox" representation,
     * which is, apparently, a pretty common representation for chess engines.
     * Essentially, it's a 12 x 12 x 12 cube, with the 8 x 8 x 8 chessboard
     * centered inside. The borders are always set to the PieceType BORDER.
     * This makes move generation much easier, because when a piece reaches the
     * edge of the board, instead of "wrapping around", it hits the sentinel
     * value of BORDER, which is significantly easier to detect.
     *
     * The x-coordinates vary first, then y, then z.
     */
    Piece pieces [1728];

    int en_passant_location;

    /**
     * Converts from a 8 x 8 x 8 cube to a 12 x 12 x 12 cube. Like the pieces
     * array, x-coordinates vary first, then y, then z.
     */
    static int mailbox(int i);
    
    /**
     * Converts from a 12 x 12 x 12 cube to a 8 x 8 x 8 cube. Like the pieces
     * array, x-coordinates vary first, then y, then z.
     */
    static int unmailbox(int i);
};

#endif
