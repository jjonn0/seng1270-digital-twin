#pragma once
#include<wx/wx.h>
#include <wx/Timer.h>
#include <wx/simplebook.h>
#include <vector>
#include "PatientPage.h"
#include "StatusPage.h"
#include "profiles.h"


class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
	std::vector<PatientProfile> m_patient;
	std::vector<StaffProfile> m_staff;
	void AdmitPatient(const std::string& f, const std::string& l, const std::string& r, time_t dob);
	void AdmitStaff(const std::string& f, const std::string& l, const std::string& j, time_t dob);
	time_t getSimTime() {
		return wxDateTime::Now().GetTicks() + m_timeDif;
	};



private:
	wxTimer m_timer;
	time_t m_timeDif = 0;
	wxSimplebook* pageContainer;
	wxPanel* page1;
	PatientPage* page2;
	StatusPage* page3;
	wxStaticText* m_timeText;
	PatientProfile* m_currentPatient;
	StaffProfile* m_currentStaff;
	void OnTimer(wxTimerEvent& evt);


	wxDECLARE_EVENT_TABLE();
};

