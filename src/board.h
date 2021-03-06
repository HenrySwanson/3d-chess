#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <list>
#include <stack>

#include "common.h"
#include "move.h"
#include "piece.h"

/**
 * Represents the possible end states of the board, or that the game has not
 * yet ended (IN_PROGRESS).
 */
enum GameState {
    IN_PROGRESS, CHECKMATE_WHITE, CHECKMATE_BLACK, STALEMATE_WHITE,
    STALEMATE_BLACK
};

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


    /** Puts pieces on the board in their initial position. */
    void setup();


    /** Retrieves the piece at i. */
    Piece getPiece(int i) const;

    /** Puts the specifed piece at i and returns the previous occupant. */
    Piece putPiece(const Piece& p, int i);

    /** Returns true if the king of the specified color is in check. */
    bool isInCheck(bool color) const;

    /** Returns the current state of the game (checkmate, stalemate, etc). */
    GameState getGameState() const;

    /** Returns a const reference to the history of this board. */
    const std::stack<Move>& getHistory() const;


    /** Generates all pseudo-legal moves that the given color can make. */
    std::list<Move> generatePseudoLegalMoves(int color) const;

    /**
     * Generates all pseudo-legal moves that the piece on this square can make.
     * It is safe to call this method on a non-piece (NIL or BORDER); it will
     * just return an empty list.
     */
    std::list<Move> generateMoves(int origin) const;

    /** Generates all pseudo-legal castling moves for the given team. */
    std::list<Move> generateCastlingMoves(bool color) const;


    /** Returns true if the given move is legal for this configuration. */
    bool isLegalMove(const Move& m) const;


    /** Performs the given move. Does not check for (pseudo-)legality. */
    void makeMove(const Move& m);

    /** Undoes the most recent move. Does nothing if there was none. */
    void undoMove();

  private:
    /** Generates all pseudo-legal moves for a pawn. */ 
    std::list<Move> generatePawnMoves(int origin) const;

    /** Generates all pseudo-legal moves for a non-pawn piece. */ 
    std::list<Move> generateNonPawnMoves(int origin) const;

    /** When a move is made, updates the castling rights accordingly. */
    void updateCastlingRights(bool color, int origin);

    /** Returns true if all moves for the given side put the king in check. */
    bool isInSomemate(bool color) const;

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

    /** Contains all the moves that have been done on this board. */
    std::stack<Move> history_;

    /**
     * For each turn, contains the location of the en passant square, or 0 if
     * there is none.
     */
    std::stack<int> ep_locations_;

    /**
     * For each turn, the nth bit is set if White has castling rights along
     * the nth axis, and the n+6th bit is set if Black does. (6 axes each)
     */
    std::stack<unsigned short> castling_rights_;

    /**
     * Contains the pieces captured during this game in order. Note that this
     * is _not_ a per-turn structure.
     */
    std::stack<Piece> captured_;
};

#endif
