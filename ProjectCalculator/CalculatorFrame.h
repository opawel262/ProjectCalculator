#pragma once
#include <wx/wx.h>
#include <map>
class CalculatorFrame : public wxFrame
{
public:
	CalculatorFrame(const wxString& title);

private:
	bool isDarkMode;

	void ToggleDarkMode(wxCommandEvent& evt);

	void bindEventHandlers();

	void onButtonsClicked(wxCommandEvent& evt);

	void OnMouseEnter(wxMouseEvent& event);

	void OnMouseLeave(wxMouseEvent& event);

	void OnCharHook(wxKeyEvent& event);

	wxTextCtrl* resultText;
	wxButton* modeButton;
	wxButton* button;
	std::vector<wxButton*> calculatorButtons;
	wxButton* darkModeToggle;
	wxBoxSizer* mainSizer;
	wxGridSizer* buttonSizer;
	wxMenu* burgerMenu;
	wxMenuItem* darkModeItem;
	wxMenuItem* historyItem;
	wxMenuBar* menuBar;

};

