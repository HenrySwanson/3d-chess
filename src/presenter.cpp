#include "presenter.h"

#include <sstream>

using std::list;
using std::string;

static const int NO_SELECTION = 0;

Presenter::Presenter(ViewInterface* view)
{
    model_ = new Board();
    view_ = view;

    selected_cell_ = NO_SELECTION;

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

// TODO move to view?
list<string> Presenter::getMoveHistory() const
{
    // Make a copy so it's not destroyed!
    std::stack<Move> history (model_->getHistory());
    list<string> strings;

    while(!history.empty())
    {
        Move m = history.top();
        std::stringstream ss;
        ss << "(" << unmailboxX(m.origin()) << ", " << unmailboxY(m.origin()) <<
            ", " << unmailboxZ(m.origin()) << ") -> (" << unmailboxX(m.target()) <<
            ", " << unmailboxY(m.target()) << ", " << unmailboxZ(m.target()) << ")";
        strings.push_back(ss.str());
        history.pop();
    }

    return strings;
}

void Presenter::click(int i, int j, int k)
{
    int clicked = mailbox(i, j, k);

    list<Move>::const_iterator it;
    for(it = selected_moves_.begin(); it != selected_moves_.end(); it++)
    {
        if(clicked == it->target())
        {
            model_->makeMove(*it);
            selected_cell_ = NO_SELECTION;
            selected_moves_.clear();
            view_->refresh();
            return;
        }
    }

    if(clicked == selected_cell_)
    {
        selected_cell_ = NO_SELECTION;
        selected_moves_.clear();
        view_->refresh();
    }
    else
    {
        selected_cell_ = clicked;
        selected_moves_ = model_->generateMoves(clicked);
        view_->refresh();
    }
}
