#include "MainFrame.h"
#include "simulation.h"
#include "profiles.h"
#include "StatusPage.h"
#include "ProfilePage.h"
#include <wx/notebook.h>
#include <wx/simplebook.h>
#include<wx/wx.h>

PatientProfile myPatient(101, "John", "Doe");
wxString fullName = wxString::FromUTF8(myPatient.getFirstName() + " " + myPatient.getLastName());


MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	//wxButton* bv1 = new wxButton(this, wxID_ANY, "A1", wxDefaultPosition, wxSize(300, 100));
	//wxButton* bv2 = new wxButton(this, wxID_ANY, "B1", wxDefaultPosition, wxSize(300, 100));
	wxBoxSizer* boxSizer = new wxBoxSizer(wxHORIZONTAL);
	//boxSizer->AddStretchSpacer();

	//boxSizer->Add(bv1);
	//boxSizer->Add(bv2);
	//boxSizer->AddStretchSpacer();



	//SetSizerAndFit(boxSizer);



	wxSimplebook* pageContainer = new wxSimplebook(this, wxID_ANY, wxPoint(200, 50));
	ProfilePage* page2 = new ProfilePage(pageContainer);
	StatusPage* page3 = new StatusPage(pageContainer);

	wxPanel* page1 = new wxPanel(pageContainer);
	page1->SetBackgroundColour(*wxLIGHT_GREY);
	wxStaticText* HomeT = new wxStaticText(page1, wxID_ANY, "Vitals Vitals", wxPoint(400, 75));
	HomeT->SetFont(wxFont(22, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	wxButton* Prof = new wxButton(page1, wxID_ANY, "Profiles", wxPoint(20, 50));
	wxButton* Stat = new wxButton(page1, wxID_ANY, "Status", wxPoint(200, 200));
	wxButton* Home1 = new wxButton(page2, wxID_ANY, "Home", wxPoint(200, 200));
	wxButton* Home2 = new wxButton(page3, wxID_ANY, "Home", wxPoint(200, 200));






	//wxPanel* page2 = new wxPanel(pageContainer);
	//page2->SetBackgroundColour(*wxLIGHT_GREY);
	//wxStaticText* Patient = new wxStaticText(page2, wxID_ANY, "Modify Profiles", wxPoint(200, 400));

	wxStaticText* StatT1 = new wxStaticText(page3, wxID_ANY, "Third Page", wxPoint(200, 400));



	pageContainer->AddPage(page1, "Home");
	pageContainer->AddPage(page2, "Profile Page");
	pageContainer->AddPage(page3, "Status");
	boxSizer->Add(pageContainer, 1, wxEXPAND);
	Home1->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { pageContainer->SetSelection(0); });
	Home2->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { pageContainer->SetSelection(0); });
	Prof->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { pageContainer->SetSelection(1); });
	Stat->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { pageContainer->SetSelection(2); });

	////pageContainer->AddPage(page3, "Third");


	////wxButton* button1 = new wxButton(page1, wxID_ANY, "a", wxPoint(300, 275), wxSize(200, 100));
	////wxButton* button2 = new wxButton(page1, wxID_ANY, "Button2", wxPoint(300, 500), wxSize(200, 100));
	////wxStaticText* staticText = new wxStaticText(page1, wxID_ANY, fullName, wxPoint(200, 400), wxSize(300, 100));
	//page1->Bind(wxEVT_CHAR_HOOK, &MainFrame::OnKeyEvent, this);



	CreateStatusBar();

}

//void MainFrame::OnKeyEvent(wxKeyEvent& evt) {
//	if (evt.GetKeyCode() == WXK_TAB) {
//		wxWindow* window = (wxWindow*)evt.GetEventObject();
//		window->Navigate();
//	}
//
//	wxChar keyChar = evt.GetUnicodeKey();
//	if (keyChar == WXK_NONE) {
//		int keyCode = evt.GetKeyCode();
//		wxLogStatus("Key Event %d", keyCode);
//	}
//	else {
//
//		wxLogStatus(keyChar);
//
//	}
//}
