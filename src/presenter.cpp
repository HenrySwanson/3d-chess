#include "presenter.h"

using std::list;

Presenter::Presenter(ViewInterface* view)
{
    model_ = new Board();
    view_ = view;

    selected_cell_ = 0;

    model_->setup();
}

Presenter::~Presenter()
{
    delete model_;
}

Piece Presenter::getPiece(int i, int j, int k) const
{
    int index = mailbox(i, j, k);
    return model_->getPiece(index);
}

list<Cell> Presenter::getMoveIndicators() const
{
    list<Cell> cells;

    list<Move>::const_iterator it;
    for(it = selected_moves_.begin(); it != selected_moves_.end(); it++)
    {
        int target = it->target();
        Cell c;
        c.x = unmailboxX(target);
        c.y = unmailboxY(target);
        c.z = unmailboxZ(target);
        cells.push_back(c);
    }

    return cells;
}

void Presenter::click(int i, int j, int k)
{
    int index = mailbox(i, j, k);
    selected_cell_ = index;
    selected_moves_ = model_->generateMoves(index);
    view_->refresh();
}
