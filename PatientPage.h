#pragma once
#include<wx/wx.h>
#include<vector>
#include<string>
#include<format>

#include <wx/notebook.h>
#include <wx/simplebook.h>
#include "simulation.h"
#include "profiles.h"
#include "StaffPage.h"


class MainFrame;
class PatientPage : public wxPanel {
public:


	PatientPage(wxWindow* Parent, MainFrame* frame) : wxPanel(Parent), m_frame1(frame) {
		InitComps();
		Styles();
	};
	void InitComps() {

		m_patSizer = new wxBoxSizer(wxVERTICAL);
		m_profSizer = new wxBoxSizer(wxVERTICAL);
		m_scrollSizer = new wxBoxSizer(wxVERTICAL);


		m_notebook1 = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

		m_patPage = new wxPanel(m_notebook1);
		m_staffPage = new StaffPage(m_notebook1, static_cast<wxSimplebook*>(this->GetParent()), m_frame1);

		m_scrolledWindow1 = new wxScrolledWindow(m_patPage, wxID_ANY);
		m_scrolledWindow1->SetScrollRate(0, 10);

		m_patientLabel = new wxStaticText(m_scrolledWindow1, wxID_ANY, "Loading...");
		m_addFirst = new wxTextCtrl(m_patPage, wxID_ANY, "");
		m_addLast = new wxTextCtrl(m_patPage, wxID_ANY, "");
		m_addReason = new wxTextCtrl(m_patPage, wxID_ANY, "");
		m_addAge = new wxTextCtrl(m_patPage, wxID_ANY, "");

		homeBtn = new wxButton(m_patPage, wxID_ANY, "Home");
		admitBtn = new wxButton(m_patPage, wxID_ANY, "Admit Patient");

		m_unitLabel = new wxStaticText(m_patPage, wxID_ANY, "Select Unit:");
		m_genderLabel = new wxStaticText(m_patPage, wxID_ANY, "Select Gender: ");


		wxArrayString unitOptions;
		unitOptions.Add("GENERAL");  unitOptions.Add("ER"); unitOptions.Add("ICU"); unitOptions.Add("CARDIAC"); unitOptions.Add("NEURO"); unitOptions.Add("POST_OP");
		unitOptions.Add("GERIATRIC"); unitOptions.Add("TRAUMA"); unitOptions.Add("NEPHRO"); unitOptions.Add("PSHYC"); unitOptions.Add("ONCOLOGY"); unitOptions.Add("PALLIATIVE");
		wxArrayString gendersOpt;
		gendersOpt.Add("Male"); gendersOpt.Add("Female"); gendersOpt.Add("Other");

		m_unitChoice = new wxChoice(m_patPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, unitOptions);
		m_genderChoice = new wxChoice(m_patPage, wxID_ANY, wxDefaultPosition, wxDefaultSize ,gendersOpt);
		m_unitChoice->SetSelection(0);
		m_genderChoice->SetSelection(0);


	}
	void Styles() {
		m_labelSizer = new wxBoxSizer(wxHORIZONTAL);
		m_choiceSizer = new wxBoxSizer(wxHORIZONTAL);

		this->SetBackgroundColour(*wxLIGHT_GREY);
		m_patPage->SetBackgroundColour(*wxLIGHT_GREY);

		m_notebook1->AddPage(m_patPage, "Patient Profiles");

		m_staffPage->SetBackgroundColour(*wxLIGHT_GREY);
		m_notebook1->AddPage(m_staffPage, "Staff Profiles");


		m_addFirst->SetHint("Enter First");
		m_addLast->SetHint("Enter Last");
		m_addReason->SetHint("Enter Reason");
		m_addAge->SetHint("Enter DOB ex: 2026, 12, 24");

		homeBtn->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { static_cast<wxSimplebook*>(this->GetParent())->SetSelection(0); });
		admitBtn->Bind(wxEVT_BUTTON, &PatientPage::OnAdmit, this);

		m_scrollSizer->Add(m_patientLabel, 1, wxEXPAND | wxALL, 10);
		m_scrolledWindow1->SetSizer(m_scrollSizer);

		m_patSizer->Add(homeBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 0);
		m_patSizer->Add(m_scrolledWindow1, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);

		m_labelSizer->Add(m_unitLabel , 0, wxLEFT, 10);
		m_labelSizer->AddStretchSpacer(1);
		m_labelSizer->Add(m_genderLabel, 0, wxRIGHT, 10);

		m_choiceSizer->Add(m_unitChoice, 0, wxLEFT, 10);
		m_choiceSizer->AddStretchSpacer(1);
		m_choiceSizer->Add(m_genderChoice, 0, wxRIGHT, 10);

		m_patSizer->Add(m_labelSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
		m_patSizer->Add(m_choiceSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

		m_patSizer->Add(admitBtn, 0, wxALIGN_CENTER_HORIZONTAL, 20);
		m_patSizer->Add(m_addFirst, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
		m_patSizer->Add(m_addLast, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
		m_patSizer->Add(m_addReason, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
		m_patSizer->Add(m_addAge, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

		m_patPage->SetSizer(m_patSizer);
		m_profSizer->Add(m_notebook1, 1, wxEXPAND, 0);
		this->SetSizer(m_profSizer);

	}
	void UpdateDisplay(const std::vector<PatientProfile>& patient, const std::vector<StaffProfile>& staff, wxDateTime currenttime, wxString unit = "") {
		m_patientTot = "";
		wxArrayString unitOptions;
		m_staffPage->UpdateDisplay(staff, currenttime);
		wxDateTime simNow(currenttime);
		for (const auto& pat : patient) {
			wxDateTime dob(pat.getDOB());
			int age = simNow.GetYear() - dob.GetYear();
			if (simNow.GetMonth() < dob.GetMonth() || (simNow.GetMonth() == dob.GetMonth() && simNow.GetDay() < dob.GetDay())) {
				age--;
			}
			wxString fullName = wxString::FromUTF8(pat.getFirstName() + " " + pat.getLastName());
			wxString reason = wxString::FromUTF8(pat.getReasonOfAdmission());
			Unit u = pat.getAssignedUnit();
			Gender g = pat.getGender();
			wxString unitName = m_unitChoice->GetString((int)u);
			wxString genderName = m_genderChoice->GetString((int)g);
			m_patientTot += wxString::Format("Patient: %s\nAge: %d\nReason %s\nID: %s\nUnit: %s\nGender: %s\n\n", fullName, age, reason, pat.getProfileNumber(),unitName, genderName);

		}
		m_patientLabel->SetLabel(m_patientTot);
		int width = m_scrolledWindow1->GetClientSize().GetWidth();
		if (width > 0) {
			m_patientLabel->Wrap(width);
		}
		m_scrolledWindow1->FitInside();
		m_scrolledWindow1->Scroll(0, m_scrolledWindow1->GetScrollLines(wxVERTICAL));
		m_scrollSizer->Layout();
		this->Layout();
	}
	void OnAdmit(wxCommandEvent& evt);
private:
	wxBoxSizer* m_profSizer;
	wxBoxSizer* m_patSizer;
	wxBoxSizer* m_scrollSizer;
	wxBoxSizer* m_labelSizer;
	wxBoxSizer* m_choiceSizer;

	wxNotebook* m_notebook1;
	wxScrolledWindow* m_scrolledWindow1;

	wxPanel* m_patPage;
	StaffPage* m_staffPage;

	wxTextCtrl* m_addFirst;
	wxTextCtrl* m_addLast;
	wxTextCtrl* m_addReason;
	wxTextCtrl* m_addAge;

	wxChoice* m_unitChoice;
	wxChoice* m_genderChoice;

	wxString m_patientTot;

	wxStaticText* m_patientLabel;
	wxStaticText* m_unitLabel;
	wxStaticText* m_genderLabel;

	wxButton* homeBtn;
	wxButton* admitBtn;

	MainFrame* m_frame1;

};
