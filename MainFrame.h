#pragma once
#include<wx/wx.h>
#include <wx/Timer.h>
#include <wx/simplebook.h>
#include <vector>
#include "ProfilePage.h"
#include "StatusPage.h"
#include "profiles.h"


class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
private:
	wxTimer m_timer;
	wxSimplebook* pageContainer;
	wxPanel* page1;
	ProfilePage* page2;
	StatusPage* page3;
	wxStaticText* m_timeText;
	PatientProfile* m_currentPatient;
	StaffProfile* m_currentStaff;
	void OnTimer(wxTimerEvent& evt);
	void OnKeyEvent(wxKeyEvent& evt);
	std::vector<PatientProfile> m_patient;
	int m_Index = 0;

	wxDECLARE_EVENT_TABLE();
};

