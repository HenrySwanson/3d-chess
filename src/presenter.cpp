#include "presenter.h"

#include <sstream>

using std::list;
using std::string;

static const int NO_SELECTION = 0;

Presenter::Presenter()
{
    player_ = new HumanPlayer();

    game_ = new Game(player_, player_);
    game_->begin();

    selected_cell_ = NO_SELECTION;

    game_->addObserver(this);
}

Presenter::~Presenter()
{
    delete game_;
    delete player_;
}

const Game* Presenter::getGame() const
{
    return game_;
}

list<Cell> Presenter::getMoveIndicators() const
{
    list<Cell> cells;

    list<Move>::const_iterator it;
    for(it = selected_moves_.begin(); it != selected_moves_.end(); it++)
        cells.push_back(unmailbox(it->target()));

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
        Cell origin = unmailbox(m.origin());
        Cell target = unmailbox(m.target());
        ss << "(" << origin.x << ", " << origin.y << ", " << origin.z <<
            ") -> (" << target.x << ", " << target.y << ", " << target.z <<
            ")";
        strings.push_back(ss.str());
        history.pop();
    }

    return strings;
}

void Presenter::click(int i, int j, int k)
{
    int clicked = mailbox(i, j, k);
    Board board = game_->getBoard();
    bool turn = game_->getTurn();

    list<Move>::const_iterator it;
    for(it = selected_moves_.begin(); it != selected_moves_.end(); it++)
    {
        // If we clicked a move indicator
        if(clicked == it->target())
        {
            player_->setMove(*it);
            clearSelection();
            return;
        }
    }

    // If we clicked the already-selected cell
    if(clicked == selected_cell_)
    {
        clearSelection();
        notifyAll();
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

        notifyAll();
    }
}

void Presenter::haltGame()
{
    game_->end();
}

void Presenter::onNotify()
{
    notifyAll();
}

//----PRIVATE----

void Presenter::clearSelection()
{
    selected_cell_ = NO_SELECTION;
    selected_moves_.clear();
}
