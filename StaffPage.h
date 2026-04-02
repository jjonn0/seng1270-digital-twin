#pragma once
#include<wx/wx.h>
#include<vector>

#include<wx/simplebook.h>
#include"profiles.h"
#include "simulation.h"

class MainFrame;

class StaffPage : public wxPanel {
public:

	StaffPage(wxWindow* parent, wxSimplebook* page, MainFrame* frame2) : wxPanel(parent), m_page(page), m_frame2(frame2) {
		InitComps();
		Styles();
	};

	void InitComps() {
		m_staffSizer = new wxBoxSizer(wxVERTICAL);
		m_scrollSizer = new wxBoxSizer(wxVERTICAL);

		m_scrollWindow2 = new wxScrolledWindow(this, wxID_ANY);
		m_scrollWindow2->SetScrollRate(0, 10);

		homeBtn = new wxButton(this, wxID_ANY, "Home");
		admitBtn = new wxButton(this, wxID_ANY, "Admit Staff");

		m_addFirst = new wxTextCtrl(this, wxID_ANY, "");
		m_addLast = new wxTextCtrl(this, wxID_ANY, "");
		m_addJob = new wxTextCtrl(this, wxID_ANY, "");
		m_addAge = new wxTextCtrl(this, wxID_ANY, "");
		m_stafftext = new wxStaticText(m_scrollWindow2, wxID_ANY, "");

		m_addFirst->SetHint("First Name");
		m_addLast->SetHint("Last Name");
		m_addJob->SetHint("Job Title");
		m_addAge->SetHint("Age");

		wxArrayString unitOptions;
		unitOptions.Add("GENERAL");  unitOptions.Add("ER"); unitOptions.Add("ICU"); unitOptions.Add("CARDIAC"); unitOptions.Add("NEURO"); unitOptions.Add("POST_OP");
		unitOptions.Add("GERIATRIC"); unitOptions.Add("TRAUMA"); unitOptions.Add("NEPHRO"); unitOptions.Add("PSHYC"); unitOptions.Add("ONCOLOGY"); unitOptions.Add("PALLIATIVE");
		//m_unitChoice = new wxChoice(m_patPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, unitOptions);
		this->SetSizer(m_staffSizer);
	}
	void Styles() {
		this->SetBackgroundColour(*wxLIGHT_GREY);

		homeBtn->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { (m_page->SetSelection(0)); });
		admitBtn->Bind(wxEVT_BUTTON, &StaffPage::OnPress, this);

		m_scrollSizer->Add(m_stafftext, 1, wxEXPAND | wxALL, 10);
		m_scrollWindow2->SetSizer(m_scrollSizer);

		m_staffSizer->Add(homeBtn, 0, wxALL | wxALIGN_CENTER, 0);
		m_staffSizer->Add(m_scrollWindow2, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);

		m_staffSizer->Add(admitBtn, 0, wxALL | wxALIGN_CENTER, 0);
		m_staffSizer->Add(m_addFirst, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
		m_staffSizer->Add(m_addLast, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
		m_staffSizer->Add(m_addJob, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
		m_staffSizer->Add(m_addAge, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
		this->Layout();
	}

	void UpdateDisplay(const std::vector<StaffProfile>& stafflist, wxDateTime currenttime) {
		m_staffTot = "";
		for (const auto& staff : stafflist) {

			//m_staffTot += staff.getFirstName() + " " + staff.getLastName() + "\n";
			wxDateTime dob(staff.getDOB());
			wxDateTime now(currenttime);
			int age = now.GetYear() - dob.GetYear();
			m_fullName = staff.getFirstName() + " " + staff.getLastName();
			m_staffTot += wxString::Format("Name: %s\nAge: %d\nOccupation: %s\nID: %s\n\n", m_fullName, age, staff.getOccupation(), staff.getProfileNumber());
		}
		m_stafftext->SetLabel(m_staffTot);
		m_stafftext->Wrap(m_scrollWindow2->GetClientSize().GetWidth());
		m_scrollWindow2->FitInside();
		m_scrollWindow2->Scroll(0, m_scrollWindow2->GetScrollLines(wxVERTICAL));
		m_scrollSizer->Layout();
		this->Layout();
	}
	void OnPress(wxCommandEvent& evt);
private:
	wxBoxSizer* m_staffSizer;
	wxBoxSizer* m_scrollSizer;

	wxSimplebook* m_page;

	wxScrolledWindow* m_scrollWindow2;

	wxTextCtrl* m_addFirst;
	wxTextCtrl* m_addLast;
	wxTextCtrl* m_addJob;
	wxTextCtrl* m_addAge;

	wxStaticText* m_stafftext;

	wxString m_staffTot;
	wxString m_fullName;


	wxButton* homeBtn;
	wxButton* admitBtn;
	MainFrame* m_frame2;


};
