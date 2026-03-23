#include "App.h"
#include <wx/wx.h>
#include "MainFrame.h"


wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	MainFrame* mainFrame = new MainFrame("VitalVitals");
	mainFrame->SetClientSize(1000, 600);
	mainFrame->Center();
	mainFrame->Show();
	return true;
}
