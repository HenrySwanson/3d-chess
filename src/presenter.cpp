#include "presenter.h"

#include <sstream>

using std::list;
using std::string;

static const int NO_SELECTION = 0;

Presenter::Presenter(ViewInterface* view)
{
    player_ = new HumanPlayer();

    game_ = new Game(player_, player_);
    game_->begin();
    view_ = view;

    selected_cell_ = NO_SELECTION;
}

Presenter::~Presenter()
{
    delete game_;
    delete player_;
}

Piece Presenter::getPiece(int i, int j, int k) const
{
    int index = mailbox(i, j, k);
    return game_->getBoard().getPiece(index);
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
    std::stack<Move> history (game_->getBoard().getHistory());
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
    const Board& board = game_->getBoard();
    bool turn = game_->getTurn();

    list<Move>::const_iterator it;
    for(it = selected_moves_.begin(); it != selected_moves_.end(); it++)
    {
        // If we clicked a move indicator
        if(clicked == it->target())
        {
            player_->setMove(*it);
            clearSelection();
            view_->refresh(); // TODO this is called before the game finishes making the move!
            return;
        }
    }

    // If we clicked the already-selected cell
    if(clicked == selected_cell_)
    {
        clearSelection();
        view_->refresh();
    }
    // If we clicked on a piece on the current team
    else if(board.getPiece(clicked).isOn(turn))
    {
        selected_cell_ = clicked;
        list<Move> moves = board.generateMoves(clicked);
        if(board.getPiece(clicked).type() == KING)
        {
            list<Move> castles = board.generateCastlingMoves(turn);
            moves.splice(moves.end(), castles);
        }
        
        // Restrict to legal moves
        selected_moves_.clear();
        list<Move>::const_iterator it;
        for(it = moves.begin(); it != moves.end(); it++)
            if(board.isLegalMove(*it))
                selected_moves_.push_back(*it);

        view_->refresh();
    }
}

//----PRIVATE----

void Presenter::clearSelection()
{
    selected_cell_ = NO_SELECTION;
    selected_moves_.clear();
}
