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
		// Initializes the components of the staff page, which includes a home button that takes the user back to the home page, an admit button that will allow the user to add new staff members, and text controls for entering the first name, last name, job title, and age of the new staff member. A scrolled window is also created to display the list of staff members, and a static text is initialized to show the staff information.
		

		m_addFirst->SetHint("First Name");
		m_addLast->SetHint("Last Name");
		m_addJob->SetHint("Job Title");
		m_addAge->SetHint("Enter DOB ex: 2026, 12, 24");
		// Sets the hint text for the text controls, which provides guidance to the user on what information to enter in each field. The first name, last name, job title, and date of birth fields are all given appropriate hints to help the user understand what information is required for admitting a new staff member.

		wxArrayString unitOptions;
		unitOptions.Add("GENERAL");  unitOptions.Add("ER"); unitOptions.Add("ICU"); unitOptions.Add("CARDIAC"); unitOptions.Add("NEURO"); unitOptions.Add("POST_OP");
		unitOptions.Add("GERIATRIC"); unitOptions.Add("TRAUMA"); unitOptions.Add("NEPHRO"); unitOptions.Add("PSHYC"); unitOptions.Add("ONCOLOGY"); unitOptions.Add("PALLIATIVE");
		this->SetSizer(m_staffSizer);
		// Creates an array of strings representing different hospital units, which can be used for categorizing staff members based on their job roles. The options include various departments such as General, ER, ICU, Cardiac, Neuro, Post-Op, Geriatric, Trauma, Nephrology, Psychiatry, Oncology, and Palliative care. The staff page's sizer is set to the vertical box sizer initialized earlier to manage the layout of the components on the page.
		
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
		//Adds all the components to the sizers and sets up the layout of the staff page.		
	}

	void UpdateDisplay(const std::vector<StaffProfile>& stafflist, wxDateTime currenttime) {
		m_staffTot = "";
		for (const auto& staff : stafflist) {

			wxDateTime dob(staff.getDOB());
			wxDateTime now(currenttime);
			int age = now.GetYear() - dob.GetYear();
			m_fullName = staff.getFirstName() + " " + staff.getLastName();
			m_staffTot += wxString::Format("Name: %s\nAge: %d\nOccupation: %s\nID: %llu\n\n", m_fullName, age, staff.getOccupation(), staff.getProfileNumber());
			
			
		}
		m_stafftext->SetLabel(m_staffTot);
		m_stafftext->Wrap(m_scrollWindow2->GetClientSize().GetWidth());
		m_scrollWindow2->FitInside();
		m_scrollWindow2->Scroll(0, m_scrollWindow2->GetScrollLines(wxVERTICAL));
		m_scrollSizer->Layout();
		this->Layout();
		//Puts all new informations of the staff members into a string and then sets that onto the scroll window.
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
