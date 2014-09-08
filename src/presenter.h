#ifndef CHESS_PRESENTATION_H
#define CHESS_PRESENTATION_H

#include <list>

#include "view-interface.h"
#include "board.h"
#include "move.h"
#include "piece.h"

struct Cell
{
    int x, y, z;
};

class Presenter
{
  public:
    Presenter(ViewInterface* view);
    ~Presenter();

    Piece getPiece(int i, int j, int k) const;
    std::list<Cell> getMoveIndicators() const;

    void click(int i, int j, int k);

  private:
    Board* model_;
    ViewInterface* view_;

    int selected_cell_;
    std::list<Move> selected_moves_;
};

#endif
