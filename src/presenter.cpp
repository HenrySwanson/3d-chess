#include "presenter.h"

#include <sstream>

using std::list;
using std::string;

static const int NO_SELECTION = 0;

Presenter::Presenter(ViewInterface* view)
{
    model_ = new Board();
    model_->setup();
    view_ = view;

    turn_ = WHITE;
    selected_cell_ = NO_SELECTION;
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
        // If we clicked a move indicator
        if(clicked == it->target())
        {
            model_->makeMove(*it);
            turn_ = !turn_;
            selected_cell_ = NO_SELECTION;
            selected_moves_.clear();
            undid_moves_ = std::stack<Move>();
            view_->refresh();
            return;
        }
    }

    // If we clicked the already-selected cell
    if(clicked == selected_cell_)
    {
        selected_cell_ = NO_SELECTION;
        selected_moves_.clear();
        view_->refresh();
    }
    // If we clicked on a piece on the current team
    else if(model_->getPiece(clicked).isOn(turn_))
    {
        selected_cell_ = clicked;
        selected_moves_ = model_->generateMoves(clicked);
        view_->refresh();
    }
}

void Presenter::newGame()
{
    turn_ = WHITE;
    selected_cell_ = NO_SELECTION;
    selected_moves_ = std::list<Move>(0);
    undid_moves_ = std::stack<Move>();

    model_->setup();
    view_->refresh();
}

void Presenter::undoMove()
{
    const std::stack<Move>& history = model_->getHistory();
    if(history.empty())
        return;

    undid_moves_.push(history.top());
    model_->undoMove();
    view_->refresh();
}

void Presenter::redoMove()
{
    if(undid_moves_.empty())
        return;

    model_->makeMove(undid_moves_.top());
    undid_moves_.pop();
    view_->refresh();
}
