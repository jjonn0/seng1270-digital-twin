#pragma once
#include<wx/wx.h>

class StatusPage : public wxPanel
{
public:
	StatusPage(wxWindow* Parent) : wxPanel(Parent) {
		InitComps();
		Styles();

	};
	void InitComps() {
		wxStaticText* Status = new wxStaticText(this, wxID_ANY, "Status Profiles", wxPoint(400, 400));
		//wxButton* Home2 = new wxButton(page3, wxID_ANY, "Home", wxPoint(50, 50));
	}
	void Styles() {
		this->SetBackgroundColour(*wxLIGHT_GREY);

	}
private:

};
