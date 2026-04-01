	#include "MainFrame.h"
	#include "PatientPage.h"
	#include "StatusPage.h"
	#include "simulation.h"
	#include "profiles.h"
	#include <wx/notebook.h>
	#include <wx/simplebook.h>
	#include <format>
	#include <string>
	#include <sstream>
	#include<wx/wx.h>

	wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_TIMER(wxID_ANY, MainFrame::OnTimer)
	wxEND_EVENT_TABLE();


	MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
		wxSimplebook* pageContainer = new wxSimplebook(this, wxID_ANY);
		page1 = new wxPanel(pageContainer);
		page2 = new PatientPage(pageContainer, this);
		page3 = new StatusPage(pageContainer);

		m_patient.push_back(PatientProfile("01P", "Jim", "Davis", getTimestamp(2025, 5, 13), "Ate Too Much Lasanga", getTimestamp(2025, 5, 15), 0, GENERAL));
		m_patient.push_back(PatientProfile("02P", "Jerry", "Slime", getTimestamp(1990, 8, 29), "Died Probably idk", getTimestamp(2050, 8, 29)));
		m_patient.push_back(PatientProfile("03P", "Laurel", "Yanny", getTimestamp(2020, 7, 2), "Every word sounds the same to them.", getTimestamp(2026, 2, 28)));
		m_staff.push_back(StaffProfile("01S", "Shaun", "Admin", getTimestamp(2025, 5, 13), "Head of Medicine", 5000));
		m_staff.push_back(StaffProfile("02S", "Shaun", "Admin", getTimestamp(2025, 5, 13), "Head of Medicine", 5000));


		wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* page1Sizer = new wxBoxSizer(wxVERTICAL);



		page1->SetBackgroundColour(*wxLIGHT_GREY);
		wxStaticText* HomeT = new wxStaticText(page1, wxID_ANY, "Vitals Vitals");
		HomeT->SetFont(wxFont(22, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
		wxButton* Prof = new wxButton(page1, wxID_ANY, "Profiles");
		wxButton* Stat = new wxButton(page1, wxID_ANY, "Status");

		wxButton* timeBtn = new wxButton(page1, wxID_ANY, "Submit Time");
		wxTextCtrl* timeInp = new wxTextCtrl(page1, wxID_ANY);
		timeInp->SetHint("YYYY,MM,D");





		wxStaticText* StatT1 = new wxStaticText(page3, wxID_ANY, "Third Page");


		page1Sizer->Add(HomeT, 0, wxALIGN_CENTER | wxTOP, 10);
		page1Sizer->AddSpacer(40);
		page1Sizer->Add(Prof, 0, wxALIGN_CENTER | wxALL, 10);
		page1Sizer->Add(Stat, 0, wxALIGN_CENTER | wxALL, 10);
		page1Sizer->AddStretchSpacer(1);
		page1Sizer->Add(timeBtn, 0, wxALIGN_CENTER | wxALL, 10);
		page1Sizer->Add(timeInp, 0, wxALIGN_CENTER | wxALL, 10);
		page1->SetSizer(page1Sizer);



		pageContainer->AddPage(page1, "Home");
		pageContainer->AddPage(page2, "Profile Page");
		pageContainer->AddPage(page3, "Status");

		mainSizer->Add(pageContainer, 1, wxEXPAND);
		this->SetSizer(mainSizer);

		this->Layout();

		Prof->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { pageContainer->SetSelection(1); });
		Stat->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { pageContainer->SetSelection(2); });
		timeBtn->Bind(wxEVT_BUTTON, [=	](wxCommandEvent&) {
			std::string input = timeInp->GetValue().ToStdString();
			std::replace(input.begin(), input.end(), ',', ' ');
			std::stringstream ss(input);
			int year, month, day;
			if (ss >> year >> month >> day) {
				time_t newTime = getTimestamp(year, month, day);
				time_t actualTime = wxDateTime::Now().GetTicks();
				time_t timeCap = actualTime + (100LL*365*24*3600);
				if (newTime < actualTime) {
					wxMessageBox("Time cannot be set to the past.", "Invalid Time", wxOK | wxICON_ERROR);
					timeInp->Clear();
					return;
				}
				if (newTime > timeCap) {
					wxMessageBox("Cannot input time over 100 years", "Invalid Time", wxOK);
					timeInp->Clear();
					return;
				}

				else {
					this->m_timeDif = newTime - actualTime;
					wxMessageBox("Updated");
					timeInp->Clear();
					page2->UpdateDisplay(m_patient, m_staff, newTime);
					page3->UpdateDisplay(m_patient, newTime);
					pageContainer->SetSelection(0);
				}

			}
			else {
				wxMessageBox("Please enter a valid date in the format: YYYY, MM, DD", "Invalid Date Format", wxOK | wxICON_ERROR);
			}
			});

		m_timer.SetOwner(this);
		m_timer.Start(1000);

		CreateStatusBar();

	}

	void MainFrame::OnTimer(wxTimerEvent& evt) {
		time_t currentTime = wxDateTime::Now().GetTicks() + m_timeDif;
		page2->UpdateDisplay(m_patient, m_staff, currentTime);
		page3->UpdateDisplay(m_patient,currentTime);
		wxDateTime displayTime(currentTime);
		wxString timeStr = (displayTime.Format("%Y, %B %d, %A - %H:%M:%S"));
		SetStatusText(timeStr);

	}

	void MainFrame::AdmitPatient(const std::string& f, const std::string& l, const std::string& r, time_t dob) {
		std::string IDP = std::format("{}P", m_patient.size() + 1);
		m_patient.push_back(PatientProfile(
				IDP, f, l, dob, r, wxDateTime::Now().GetTicks(), 0, GENERAL
			));
		SetStatusText("Admitted: " + wxString(f) + " " + wxString(l));
	}

	void PatientPage::OnAdmit(wxCommandEvent& evt) {
		wxDateTime now = wxDateTime::Now();
		wxString fName = m_addFirst->GetValue().ToStdString();
		wxString lName = m_addLast->GetValue().ToStdString();
		wxString Reason = m_addReason->GetValue().ToStdString();
		wxString Age = m_addAge->GetValue().ToStdString();
		if (fName.IsEmpty() || lName.IsEmpty() || Reason.IsEmpty() || Age.IsEmpty()) {
			wxMessageBox("Please fill out all fields.", "Missing Information", wxOK | wxICON_ERROR);
			return;
		}
		std::string iAge = Age.ToStdString();

		std::replace(iAge.begin(), iAge.end(), ',', ' ');

		std::stringstream ss(iAge);
		size_t year, month, day;
		time_t dob = 0;
		if (ss >> year >> month >> day) {
			dob = getTimestamp(year, month, day);
			wxDateTime dobDate(dob);
			if (dobDate > m_frame1->	getSimTime()) {
				
			wxMessageBox("Date of birth cannot be in the future.", "Invalid Date", wxOK | wxICON_ERROR);
			return;
			}
			else {
				m_frame1->AdmitPatient(fName.ToStdString(), lName.ToStdString(), Reason.ToStdString(), dob);

				m_addFirst->Clear();
				m_addLast->Clear();
				m_addReason->Clear();
				m_addAge->Clear();
			}
		}
	else {
		wxMessageBox("Please enter a valid date of birth in the format: YYYY, MM, DD", "Invalid Date Format", wxOK | wxICON_ERROR);
	}
	}

	void MainFrame::AdmitStaff(const std::string& f, const std::string& l, const std::string& j, time_t dob) {
		std::string IDS = std::format("{}S", m_staff.size() + 1);
		m_staff.push_back(StaffProfile(
			IDS, f, l, dob, j, 5000
		));
		SetStatusText("Admitted: " + wxString(f) + " " + wxString(l));

	}

	void StaffPage::OnPress(wxCommandEvent& evt) {
		wxDateTime now = wxDateTime::Now();
		wxString fName = m_addFirst->GetValue().ToStdString();
		wxString lName = m_addLast->GetValue().ToStdString();
		wxString Job = m_addJob->GetValue().ToStdString();
		wxString Age = m_addAge->GetValue().ToStdString();
		if (fName.IsEmpty() || lName.IsEmpty() || Job.IsEmpty() || Age.IsEmpty()) {
			wxMessageBox("Please fill out all fields.", "Missing Information", wxOK | wxICON_ERROR);
			return;
		}

		std::string iAge = Age.ToStdString();


		std::replace(iAge.begin(), iAge.end(), ',', ' ');

		std::stringstream ss(iAge);
		size_t year, month, day;
		time_t dob = 0;
		if (ss >> year >> month >> day) {
			dob = getTimestamp(year, month, day);
			wxDateTime dobDate(dob);
			if (dobDate > now) {
				wxMessageBox("Date of birth cannot be in the future.", "Invalid Date", wxOK | wxICON_ERROR);
			}
			else {
				m_frame2->AdmitStaff(fName.ToStdString(), lName.ToStdString(), Job.ToStdString(), dob);
				m_addFirst->Clear();
				m_addLast->Clear();
				m_addJob->Clear();
				m_addAge->Clear();
			}
		}
		else {
			wxMessageBox("Please enter a valid date of birth in the format: YYYY, MM, DD", "Invalid Date Format", wxOK | wxICON_ERROR);
		}
		
	}
