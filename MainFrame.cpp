#include "MainFrame.h"
#include "PatientPage.h"
#include "StatusPage.h"
#include "simulation.h"
#include "profiles.h"
#include "filehandler.h"
#include <wx/notebook.h>
#include <wx/simplebook.h>
#include <format>
#include <string>
#include <sstream>
#include<wx/wx.h>

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_TIMER(wxID_ANY, MainFrame::OnTimer)
wxEND_EVENT_TABLE();



MainFrame::MainFrame(const wxString& title, const std::string& currentPassword) : wxFrame(nullptr, wxID_ANY, title), m_currentPassword(currentPassword) {
	try {
		m_patient = FileHandler::loadPatientData("patients.csv", currentPassword);
		m_staff = FileHandler::loadStaffData("staff.csv", currentPassword);

	}
	catch (const std::exception& e) {
		wxLogDebug("Error loading data: %s", e.what());
	}
	m_timer.Start(1000);

	wxSimplebook* pageContainer = new wxSimplebook(this, wxID_ANY);
	page1 = new wxPanel(pageContainer);
	page2 = new PatientPage(pageContainer, this);
	page3 = new StatusPage(pageContainer);
	//Initialize the pages and their layouts

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* page1Sizer = new wxBoxSizer(wxVERTICAL);
	//Creates sizers which control the layout of the pages, in this case page 1 is vertical and centered, and the main sizer is vertical and fills the frame

	page1->SetBackgroundColour(*wxLIGHT_GREY);
	wxStaticText* HomeT = new wxStaticText(page1, wxID_ANY, "Vitals Vitals");
	HomeT->SetFont(wxFont(22, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	wxButton* Prof = new wxButton(page1, wxID_ANY, "Profiles");
	wxButton* Stat = new wxButton(page1, wxID_ANY, "Status");
	wxButton* timeBtn = new wxButton(page1, wxID_ANY, "Submit Time");
	wxTextCtrl* timeInp = new wxTextCtrl(page1, wxID_ANY);
	//Creates the buttons and text input for page 1, which is the home page, and sets the background color to light grey for aesthetics
	timeInp->SetHint("YYYY,MM,D");




	page1Sizer->Add(HomeT, 0, (wxALIGN_CENTER | wxTOP), 10);
	page1Sizer->AddSpacer(40);
	page1Sizer->Add(Prof, 0, (wxALIGN_CENTER | wxALL), 10);
	page1Sizer->Add(Stat, 0, (wxALIGN_CENTER | wxALL), 10);
	page1Sizer->AddStretchSpacer(1);
	page1Sizer->Add(timeBtn, 0, (wxALIGN_CENTER | wxALL), 10);
	page1Sizer->Add(timeInp, 0, (wxALIGN_CENTER | wxALL), 10);
	page1->SetSizer(page1Sizer);
	// Adds the buttons and text input to the page 1 sizer, which is vertical and centered, and sets the sizer for page 1



	pageContainer->AddPage(page1, "Home");
	pageContainer->AddPage(page2, "Profile Page");
	pageContainer->AddPage(page3, "Status");
	pageContainer->SetSelection(0);
	// Adds the three pages to the page container, which is a simplebook, and sets the initial page to page 1 (home page)
	mainSizer->Add(pageContainer, 1, wxEXPAND);
	this->SetSizer(mainSizer);
	this->Layout();

	Prof->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { pageContainer->SetSelection(1); });
	Stat->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { pageContainer->SetSelection(2); });
	// Binds the profile and status buttons to switch to their respective pages when clicked, using lambda functions for simplicity
	timeBtn->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
		std::string input = timeInp->GetValue().ToStdString();
		std::replace(input.begin(), input.end(), ',', ' ');
		std::stringstream ss(input);
		int year, month, day;
		if (ss >> year >> month >> day) {
			if (month > 12 || day > 31) {
				wxMessageBox("Please enter a valid date in the format: YYYY, MM, DD", "Invalid Date Format", wxOK | wxICON_ERROR);
				timeInp->Clear();
				return;
			}
			time_t newTime = getTimestamp(year, month, day);
			time_t actualTime = wxDateTime::Now().GetTicks();
			time_t timeCap = actualTime + (100LL * 365 * 24 * 3600);
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
				page2->UpdateDisplay(m_patient, m_staff, newTime, "");
				page3->UpdateDisplay(m_patient, newTime);
				pageContainer->SetSelection(0);
			}

		}
		else {
			wxMessageBox("Please enter a valid date in the format: YYYY, MM, DD", "Invalid Date Format", wxOK | wxICON_ERROR);
		}
		});
	// Binds the time submission button to update the simulated time based on user input, with error handling for invalid formats and unrealistic dates, and updates the displays on the profile and status pages accordingly

	m_timer.SetOwner(this);

	CreateStatusBar();
	page2->UpdateDisplay(m_patient, m_staff, getSimTime());
	page3->UpdateDisplay(m_patient, getSimTime());

	this->Layout();
	this->Refresh();
	// Initializes the timer to trigger events for updating the displays, creates the status bar, and performs an initial update of the profile and status pages with the current simulated time, then lays out and refreshes the frame to ensure everything is displayed correctly
}








void MainFrame::OnTimer(wxTimerEvent& evt) {
	time_t currentTime = getSimTime();
	page2->UpdateDisplay(m_patient, m_staff, currentTime);
	page3->UpdateDisplay(m_patient, currentTime);
	wxDateTime displayTime(currentTime);
	wxString timeStr = (displayTime.Format("%Y, %B %d, %A - %H:%M:%S"));
	SetStatusText(timeStr);
	// This function is called every time the timer event is triggered (every second in this case) and updates the profile and status pages with the current simulated time, and also updates the status bar to display the current simulated time in a human-readable format
}

void MainFrame::AdmitPatient(const std::string& f, const std::string& l, const std::string& r, time_t dob, Unit u, Gender g) {
	size_t IDP = m_patient.size() + 1;
	PatientProfile newPatient(IDP, f, l, dob, r, wxDateTime::Now().GetTicks(), 0, u, g);
	m_patient.push_back(newPatient);
	FileHandler::savePatientData(newPatient, "patients.csv", m_currentPassword);
	SetStatusText("Admitted: " + wxString(f) + " " + wxString(l));
	// This function is called when a new patient is admitted through the profile page, and it creates a new PatientProfile with the provided information, adds it to the list of patients, saves the new patient data to the CSV file, and updates the status bar to indicate that the patient has been admitted
}

void MainFrame::AdmitStaff(const std::string& f, const std::string& l, const std::string& j, time_t dob) {
	size_t IDS = m_staff.size() + 1;
	StaffProfile newStaff(IDS, f, l, dob, j, 5000, GENERAL, Other);
	m_staff.push_back(newStaff);
	FileHandler::saveStaffData(newStaff, "staff.csv", m_currentPassword);
	SetStatusText("Admitted: " + wxString(f) + " " + wxString(l));
	// This function is called when a new staff member is admitted through the profile page, and it creates a new StaffProfile with the provided information, adds it to the list of staff, saves the new staff data to the CSV file, and updates the status bar to indicate that the staff member has been admitted

}

void PatientPage::OnAdmit(wxCommandEvent& evt) {
	wxString fName = m_addFirst->GetValue().ToStdString();
	wxString lName = m_addLast->GetValue().ToStdString();
	wxString Reason = m_addReason->GetValue().ToStdString();
	wxString Age = m_addAge->GetValue().ToStdString();
	Unit unit = static_cast<Unit>(m_unitChoice->GetSelection());
	Gender gender = static_cast<Gender>(m_genderChoice->GetSelection());
	if (fName.IsEmpty() || lName.IsEmpty() || Reason.IsEmpty() || Age.IsEmpty()) {
		wxMessageBox("Please fill out all fields.", "Missing Information", wxOK | wxICON_ERROR);
		return;
	}
	std::string iAge = Age.ToStdString();

	std::replace(iAge.begin(), iAge.end(), ',', ' ');

	std::stringstream ss(iAge);
	time_t year, month, day;
	time_t dob = 0;

	if (ss >> year >> month >> day) {
		dob = getTimestamp(year, month, day);
		wxDateTime dobDate(dob);
		if (year < 1970) {
			wxMessageBox("Age can not be lower than 1970 due to time_t", "Invalid Date", wxOK | wxICON_ERROR);
			return;
		}
		if (dobDate > m_frame1->getSimTime()) {

			wxMessageBox("Date of birth cannot be in the future.", "Invalid Date", wxOK | wxICON_ERROR);
			return;
		}
		else {
			m_frame1->AdmitPatient(fName.ToStdString(), lName.ToStdString(), Reason.ToStdString(), dob, unit, gender);

			m_addFirst->Clear();
			m_addLast->Clear();
			m_addReason->Clear();
			m_addAge->Clear();
			m_unitChoice->SetSelection(0);
			m_genderChoice->SetSelection(0);
			// After admitting the patient, clear the input fields and reset the choices to their default selections for a better user experience
		}
	}
	else {
		wxMessageBox("Please enter a valid date of birth in the format: YYYY, MM, DD", "Invalid Date Format", wxOK | wxICON_ERROR);
	}
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
	time_t year, month, day;
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
			// After admitting the staff member, clear the input fields for a better user experience
		}
	}
	else {
		wxMessageBox("Please enter a valid date of birth in the format: YYYY, MM, DD", "Invalid Date Format", wxOK | wxICON_ERROR);
	}

}
