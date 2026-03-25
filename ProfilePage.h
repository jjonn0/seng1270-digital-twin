#pragma once
#include<wx/wx.h>
#include<vector>
#include <wx/notebook.h>
#include <wx/simplebook.h>
#include "simulation.h"
#include "profiles.h"


class ProfilePage : public wxPanel
{
public:


	ProfilePage(wxWindow* Parent) : wxPanel(Parent) {
		InitComps();
		Styles();

	};
	void InitComps() {

		m_patSizer = new wxBoxSizer(wxVERTICAL);
		m_profSizer = new wxBoxSizer(wxVERTICAL);
		m_trSizer = new wxBoxSizer(wxHORIZONTAL);

		m_notebook1 = new wxNotebook(this, wxID_ANY, wxPoint(200, 50), wxSize(400, 300));

		m_subPanel1 = new wxPanel(m_notebook1);
		m_subPanel2 = new wxPanel(m_notebook1);

		m_staffShifts = new wxStaticText(m_subPanel2, wxID_ANY, "Loading...");
		m_patientLabel = new wxStaticText(m_subPanel1, wxID_ANY, "Loading...");

		wxButton* homeBtn = new wxButton(m_subPanel1, wxID_ANY, "Home");

		homeBtn->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { static_cast<wxSimplebook*>(this->GetParent())->SetSelection(0); });

		m_trSizer->Add(m_patientLabel, 0, wxALL, 10);
		m_trSizer->AddStretchSpacer(2.1);
		m_trSizer->Add(homeBtn, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
		m_trSizer->AddStretchSpacer(4);


		m_patSizer->Add(m_trSizer, 0, wxEXPAND);
		m_profSizer->Add(m_notebook1, 1, wxEXPAND | wxALL, 0);
		m_subPanel1->SetSizer(m_patSizer);
		this->SetSizer(m_profSizer);
	}
	void Styles() {
		this->SetBackgroundColour(*wxLIGHT_GREY);
		m_subPanel1->SetBackgroundColour(*wxLIGHT_GREY);
		m_notebook1->AddPage(m_subPanel1, "Patient Profiles");

		m_subPanel2->SetBackgroundColour(*wxLIGHT_GREY);
		m_notebook1->AddPage(m_subPanel2, "Staff Profiles");

	}
	void UpdateDisplay(const std::vector<PatientProfile>& patient, const StaffProfile& staff, time_t currenttime) {
		m_patientTot = "";
		for (const auto& pat : patient){
			wxString fullName = wxString::FromUTF8(pat.getFirstName() + " " + pat.getLastName());
			wxString reason = wxString::FromUTF8(pat.getReasonOfAdmission());
			m_patientTot += "Patient: " + fullName + "\nReason: " + reason + "\n";

			//m_patientLabel->SetLabel(wxString::Format("Patient: %s\nReason: %s", fullName, reason));
	}
		m_patientLabel->SetLabel(m_patientTot);
		m_staffShifts->SetLabel("Avtive Staff: " + wxString::FromUTF8(staff.getFirstName()));
		this->Layout();
	}
	
private:
	wxBoxSizer* m_profSizer;
	wxBoxSizer* m_patSizer;
	wxBoxSizer* m_trSizer;

	wxNotebook* m_notebook1;

	wxPanel* m_subPanel1;
	wxPanel* m_subPanel2;


	wxString shiftDisplay = "Upcoming Shifts:\n";
	wxString m_patientTot;

	wxStaticText* m_staffShifts;
	wxStaticText* m_patientLabel;



};
