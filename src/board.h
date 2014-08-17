#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <list>

/**
 * A list of all pieces (and lack thereof). The value BORDER is a bit special;
 * see the comments for "squares" for more details. White and black pawns are
 * different codes for two reasons. First, it means that (standard) movements
 * are entirely determined by PieceType. Second, it gives us an even 16 types.
 */
enum PieceType {
    NIL, BORDER, W_PAWN, B_PAWN,
    KNIGHT, GRIFFIN, DRAGON, UNICORN,
    ROOK, BISHOP, MACE,
    WIZARD, ARCHER, CANNON,
    QUEEN, KING
};

// TODO pack Square and Move into bit-fields

struct Square
{
    PieceType type;
    bool color;
    bool moved;
};

struct Move {
    int from, to;
    PieceType captured;
    /** TODO flags: e.p., capture, promote, castle */
};

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

    /** Constructs a capturing move from origin to target. */
    Move createCaptureMove(int origin, int target);

    /** Constructs a quiet move from origin to target. */
    Move createQuietMove(int origin, int target);

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
    Square squares [1728];

    /**
     * Converts from a 8 x 8 x 8 cube to a 12 x 12 x 12 cube. Like the squares
     * array, x-coordinates vary first, then y, then z.
     */
    static int mailbox(int i);
    
    /**
     * Converts from a 12 x 12 x 12 cube to a 8 x 8 x 8 cube. Like the squares
     * array, x-coordinates vary first, then y, then z.
     */
    static int unmailbox(int i);
};

#endif
