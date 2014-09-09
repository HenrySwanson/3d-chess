#ifndef CHESS_PRESENTATION_H
#define CHESS_PRESENTATION_H

#include <list>
#include <stack>
#include <string>

#include "view-interface.h"
#include "board.h"
#include "move.h"
#include "piece.h"

struct Cell
{
    int x, y, z;
};

inline bool operator==(Cell a, Cell b)
{
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

class Presenter
{
  public:
    Presenter(ViewInterface* view);
    ~Presenter();

    Piece getPiece(int i, int j, int k) const;
    std::list<Cell> getMoveIndicators() const;
    std::list<std::string> getMoveHistory() const;

    void click(int i, int j, int k);
    void newGame();
    void undoMove();
    void redoMove();

  private:
    Board* model_;
    ViewInterface* view_;

    bool turn_;
    int selected_cell_;
    std::list<Move> selected_moves_;
    std::stack<Move> undid_moves_;
};

#endif
