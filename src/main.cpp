#include "gui-3d.h"

class MyApp : public wxApp
{
  public:
    virtual bool OnInit();
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    Gui3D *gui = new Gui3D();
    gui->Show(true);

    return true;
}
