#include "MainFrame.h"
#include "ProfilePage.h"
#include "StatusPage.h"
#include "simulation.h"
#include "profiles.h"
#include <wx/notebook.h>
#include <wx/simplebook.h>
#include <format>
#include<wx/wx.h>

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_TIMER(wxID_ANY, MainFrame::OnTimer)
wxEND_EVENT_TABLE();


MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxSimplebook* pageContainer = new wxSimplebook(this, wxID_ANY);
	page1 = new wxPanel(pageContainer);
	page2 = new ProfilePage(pageContainer);
	page3 = new StatusPage(pageContainer);

	m_patient.push_back(PatientProfile(101, "Jim", "Davis", 100, "Ate Too Much Lasanga"));
	m_patient.push_back(PatientProfile(102, "Jerry", "Slime", 100, "Died Probably idk"));
	m_patient.push_back(PatientProfile(103, "Laurel", "Yanny", 100, "Every word sounds the same to them."));
	m_currentStaff = new StaffProfile(501, "Shaun", "Admin", 0, "Head of Medicine", 5000);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* page1Sizer = new wxBoxSizer(wxVERTICAL);








	page1->SetBackgroundColour(*wxLIGHT_GREY);
	wxStaticText* HomeT = new wxStaticText(page1, wxID_ANY, "Vitals Vitals");
	HomeT->SetFont(wxFont(22, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	wxButton* Prof = new wxButton(page1, wxID_ANY, "Profiles");
	wxButton* Stat = new wxButton(page1, wxID_ANY, "Status");







	wxStaticText* StatT1 = new wxStaticText(page3, wxID_ANY, "Third Page");


	page1Sizer->Add(HomeT, 0, wxALIGN_CENTER | wxTOP, 10);
	page1Sizer->AddSpacer(40);
	page1Sizer->Add(Prof, 0, wxALIGN_CENTER | wxALL, 10);
	page1Sizer->Add(Stat, 0, wxALIGN_CENTER | wxALL, 10);
	page1->SetSizer(page1Sizer);



	pageContainer->AddPage(page1, "Home");
	pageContainer->AddPage(page2, "Profile Page");
	pageContainer->AddPage(page3, "Status");

	mainSizer->Add(pageContainer, 1, wxEXPAND);
	this->SetSizer(mainSizer);

	this->Layout();

	Prof->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { pageContainer->SetSelection(1); });
	Stat->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { pageContainer->SetSelection(2); });

	m_timer.SetOwner(this);
	m_timer.Start(1000);


	CreateStatusBar();

}

void MainFrame::OnTimer(wxTimerEvent& evt) {
	if (m_Index >= m_patient.size()) {
		m_Index = 0;
	}
	time_t currentTime = wxDateTime::Now().GetTicks();
	page2->UpdateDisplay(m_patient, *m_currentStaff, currentTime);
	wxString timeStr = (wxDateTime::Now().Format("%Y, %B %d, %A - %H:%M:%S"));
	SetStatusText(timeStr);
	m_Index++;
}
