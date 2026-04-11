#include "App.h"
#include <wx/wx.h>
#include "MainFrame.h"


wxIMPLEMENT_APP(App);

bool App::OnInit() 
{
	wxString password = "Secret";
	bool passwordCorrect = false;
	while (!passwordCorrect) {
		wxPasswordEntryDialog login(nullptr, "Enter System Password:", "VitalVitals Login");

		if (login.ShowModal() == wxID_OK) {
			password = login.GetValue();
			if (password == "Secret") {
				passwordCorrect = true;
			}
			else {
				wxMessageBox("Incorrect password. Please try again.", "Login Failed", wxICON_ERROR);
			}
		}
		else {
			return false; // User cancelled the login dialog
		}
	}

	MainFrame* mainFrame = new MainFrame("VitalVitals GUI", password.ToStdString());
	mainFrame->SetClientSize(800, 600);
	mainFrame->Center();
	mainFrame->Show();
	return true;
}
