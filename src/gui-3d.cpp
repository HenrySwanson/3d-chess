#include "gui-3d.h"

Gui3D::Gui3D() : wxFrame(NULL, wxID_ANY, wxT("3D Chess"), wxDefaultPosition, wxDefaultSize)
{
    presenter_ = new Presenter();
    game_over_ = false;

    // Note: Don't have to destruct any child widgets, wxWidgets does that.

    // Construct all sizers
    wxBoxSizer* h_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* v_sizer = new wxBoxSizer(wxVERTICAL);
    wxGridSizer* b_sizer = new wxGridSizer(2, 0, 0);

    // Construct children
    display_canvas_ = new DisplayCanvas(this, presenter_);
    move_history_ = new wxListBox(this, wxID_ANY);

    wxButton* button_new = new wxButton(this, wxID_ANY, wxT("New Game"));
    button_undo = new wxButton(this, wxID_ANY, wxT("Undo Move"));
    button_redo = new wxButton(this, wxID_ANY, wxT("Redo Move"));

    // Put things in the right places
    h_sizer->Add(display_canvas_, 3, wxEXPAND);
    h_sizer->Add(v_sizer, 0, wxEXPAND);

    v_sizer->Add(move_history_, 1, wxEXPAND);
    v_sizer->Add(b_sizer, 0, wxEXPAND);

    b_sizer->Add(button_new, 1, wxEXPAND);
    b_sizer->Add(button_undo, 1, wxEXPAND);
    b_sizer->Add((wxButton*) NULL, 1, wxEXPAND);
    b_sizer->Add(button_redo, 1, wxEXPAND);

    // Set sizer
    h_sizer->SetSizeHints(this);
    SetSizer(h_sizer);

    // Connect events from the buttons
    Connect(button_new->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
      wxCommandEventHandler(Gui3D::newGame));
    Connect(button_undo->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
      wxCommandEventHandler(Gui3D::undoMove));
    Connect(button_redo->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
      wxCommandEventHandler(Gui3D::redoMove));

    presenter_->addObserver(this);
}

Gui3D::~Gui3D()
{
    delete presenter_;
}

void Gui3D::onNotify()
{
    Refresh();
}

#if 0
    std::list<std::string> moves = presenter_->getMoveHistory();

    move_history_->Clear();
    std::list<std::string>::const_iterator it;
    for(it = moves.begin(); it != moves.end(); it++)
        move_history_->Append(wxString::FromAscii(it->c_str()));

    /*button_undo->Enable(presenter_->canUndo() && game_over_);
    button_redo->Enable(presenter_->canRedo() && game_over_);

    GameState state = presenter_->getGameState();
    // Triggers on the "rising edge" of game_over_
    if(state != IN_PROGRESS && !game_over_)
        reactGameOver(state);*/

    Refresh();
}
#endif

void Gui3D::newGame(wxCommandEvent& evt)
{
    //game_over_ = false;
    //presenter_->newGame();
}

void Gui3D::undoMove(wxCommandEvent& evt)
{
    //presenter_->undoMove();
}

void Gui3D::redoMove(wxCommandEvent& evt)
{
    //presenter_->redoMove();
}

void Gui3D::reactGameOver(GameState state)
{
    /*game_over_ = true;

    const char* message;
    const char* title;

    switch(state)
    {
      case IN_PROGRESS:
        message = "You shouldn't ever see this...";
        title = "No one wins yet";
        break;
      case CHECKMATE_WHITE:
        message = "White's king is in check and can no longer move!";
        title = "Black wins!";
        break;
      case CHECKMATE_BLACK:
        message = "Black's king is in check and can no longer move!";
        title = "White wins!";
        break;
      case STALEMATE_WHITE:
        message = "White's king is safe, but can no longer move!";
        title = "Draw!";
        break;
      case STALEMATE_BLACK:
        message = "Black's king is safe, but can no longer move!";
        title = "Draw!";
        break;
    }

    wxMessageDialog* dialog = new wxMessageDialog (NULL, 
            wxString::FromAscii(message), wxString::FromAscii(title));
    dialog->ShowModal();*/
}
