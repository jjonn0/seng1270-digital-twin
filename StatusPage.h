#pragma once
#include<wx/wx.h>
#include<wx/simplebook.h>
#include<vector>
#include"profiles.h"

class StatusPage : public wxPanel
{
public:
	StatusPage(wxWindow* Parent) : wxPanel(Parent) {
		Styles();
		InitComps();

	};
	void InitComps() {
		//m_statPage = new wxSimplebook(this);

		homeBtn = new wxButton(this, wxID_ANY, "Home");
		homeBtn->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { static_cast<wxSimplebook*>(this->GetParent())->SetSelection(0); });

		m_displayInfo = new wxStaticText(this, wxID_ANY, "Loading Patients");
		m_statSizer->Add(homeBtn, 0, wxALL | wxALIGN_CENTER, 5);
		m_statSizer->Add(m_displayInfo, 1, wxEXPAND | wxALL, 5);




	}
	void Styles() {
		m_statSizer = new wxBoxSizer(wxVERTICAL);
		this->SetBackgroundColour(*wxLIGHT_GREY);
		this->SetSizer(m_statSizer);


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

			m_patInfo += wxString::Format("Patient: %s %s\nReason of Admission: %s\nTime of Admission: %s\nCurrent Time Stayed: Years: %d, Months: %d, Days: %d, %dhr : %dm : %ds\n\n", pat.getFirstName(), pat.getLastName(), pat.getReasonOfAdmission(), wxDateTime(pat.getTimeOfAdmission()).Format("%Y-%m-%d"), years, months, days, hours, minutes, seconds);

		}
		m_displayInfo->SetLabel(m_patInfo);
		this->Layout();
	}
private:
	//wxSimplebook* m_statPage;
	wxBoxSizer* m_statSizer;
	wxButton* homeBtn;
	wxStaticText* m_displayInfo;


};
