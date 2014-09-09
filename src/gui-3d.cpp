#include "gui-3d.h"

Gui3D::Gui3D() : wxFrame(NULL, wxID_ANY, wxT("3D Chess"), wxDefaultPosition, wxDefaultSize)
{
    presenter_ = new Presenter(this);

    // Note: Don't have to destruct any child widgets, wxWidgets does that.

    // Construct all sizers
    wxBoxSizer* h_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* v_sizer = new wxBoxSizer(wxVERTICAL);
    wxGridSizer* b_sizer = new wxGridSizer(2, 0, 0);

    // Construct children
    display_canvas_ = new DisplayCanvas(this, presenter_);
    move_history_ = new wxListBox(this, wxID_ANY);

    wxButton* button_new = new wxButton(this, wxID_ANY, wxT("New Game"));
    wxButton* button_undo = new wxButton(this, wxID_ANY, wxT("Undo Move"));
    wxButton* button_redo = new wxButton(this, wxID_ANY, wxT("Redo Move"));

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
}

Gui3D::~Gui3D()
{
    delete presenter_;
}

void Gui3D::refresh()
{
    std::list<std::string> moves = presenter_->getMoveHistory();

    move_history_->Clear();

    std::list<std::string>::const_iterator it;
    for(it = moves.begin(); it != moves.end(); it++)
    {
        move_history_->Append(wxString::FromAscii(it->c_str()));
    }

    Refresh();
}
