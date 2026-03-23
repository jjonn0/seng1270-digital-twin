#pragma once
#include<wx/wx.h>

class ProfilePage : public wxPanel
{
public:
	ProfilePage(wxWindow* Parent) : wxPanel(Parent) {
		InitComps();
		Styles();

	};
	void InitComps() {
		wxStaticText* Profiles = new wxStaticText(this, wxID_ANY, "Patient Profiles", wxPoint(400, 400));
		//wxButton* Home2 = new wxButton(page3, wxID_ANY, "Home", wxPoint(50, 50));
	}
	void Styles() {
		this->SetBackgroundColour(*wxLIGHT_GREY);

	}
private:

};
