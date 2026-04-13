#pragma once
#include<wx/wx.h>
#include<wx/simplebook.h>
#include<vector>
#include"profiles.h"

class StatusPage : public wxPanel
{
public:
	StatusPage(wxWindow* Parent) : wxPanel(Parent) {
		InitComps();
		Styles();

	};
	void InitComps() {

		m_homeBtn = new wxButton(this, wxID_ANY, "Home");
		m_homeBtn->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { static_cast<wxSimplebook*>(this->GetParent())->SetSelection(0); });

		m_scrollWindowStat = new wxScrolledWindow(this, wxID_ANY);
		m_scrollWindowStat->SetScrollRate(0, 10);

		m_displayInfo = new wxStaticText(m_scrollWindowStat, wxID_ANY, "Loading Patients");
		// Initializes the components of the status page, which includes a home button that takes the user back to the home page, a scrolled window that will display patient information, and a static text that will show the patient information. The scroll rate is set to allow for vertical scrolling of the patient information.
	}
	void Styles() {
		m_scrollSizer = new wxBoxSizer(wxVERTICAL);
		m_statSizer = new wxBoxSizer(wxVERTICAL);

		m_scrollSizer->Add(m_displayInfo, 1, wxEXPAND | wxALL, 5);
		m_scrollWindowStat->SetSizer(m_scrollSizer);

		m_statSizer->Add(m_homeBtn, 0, wxALL | wxALIGN_CENTER, 5);
		m_statSizer->Add(m_scrollWindowStat, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
		this->SetBackgroundColour(*wxLIGHT_GREY);
		this->SetSizer(m_statSizer);
		// Sets the styles and layout of the components on the status page. The home button is added to the top of the page, and the scrolled window is added below it. The background color of the page is set to light grey, and the sizers are used to manage the layout of the components.
	}
	void UpdateDisplay(const std::vector<PatientProfile>& patient, time_t newTime) {
		wxString m_patInfo = "";
		for (const auto pat : patient) {
			time_t addedTime = pat.getTimeOfAdmission();
			time_t expectedStay = pat.getExpectedTimeOfStay();
			time_t timeStayed = newTime - addedTime;
			int years = timeStayed / 31536000;
			time_t remTime = timeStayed % 31536000;
			int months = remTime / (30 * 24 * 3600);
			remTime = remTime % (30 * 24 * 3600);
			int days = remTime / (24 * 3600);
			remTime = remTime % (24 * 3600);
			int hours = remTime / 3600;
			remTime = remTime % 3600;
			int minutes = remTime / 60;
			remTime = remTime % 60;
			int seconds = remTime;
			// Updates the display of patient information on the status page. It takes a vector of patient profiles and a new time as input, and calculates the time each patient has stayed in the hospital based on their time of admission. The patient information is formatted into a string and displayed in the static text component. The scroll window is then adjusted to fit the new content, and the layout is updated to reflect the changes.

			m_patInfo += wxString::Format("Patient: %s %s\nReason of Admission: %s\nTime of Admission: %s\nCurrent Time Stayed: Years: %d, Months: %d, Days: %d, %dhr : %dm : %ds\n\n", pat.getFirstName(), pat.getLastName(), pat.getReasonOfAdmission(), wxDateTime(pat.getTimeOfAdmission()).Format("%Y-%m-%d"), years, months, days, hours, minutes, seconds);

		}
		m_displayInfo->SetLabel(m_patInfo);
		m_scrollWindowStat->FitInside();
		this->Layout();
	}
private:
	wxBoxSizer* m_statSizer;
	wxBoxSizer* m_scrollSizer;
	wxButton* m_homeBtn;
	wxStaticText* m_displayInfo;
	wxScrolledWindow* m_scrollWindowStat;
	


};
