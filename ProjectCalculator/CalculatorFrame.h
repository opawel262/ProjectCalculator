#pragma once
#include <wx/wx.h>
#include <map>
#include <vector>
#include "CalculatorMemory.h"
#include "MyCalculator.h"

class CalculatorFrame : public wxFrame
{
public:
	CalculatorFrame(const wxString& title);
	virtual ~CalculatorFrame();

private:
	void ToggleDarkMode(wxCommandEvent& evt);

	void ToggleScientificCalculatorMode(wxCommandEvent& evt);

	void bindEventHandlers();

	void onButtonsClicked(wxCommandEvent& evt);

	void OnMouseEnter(wxMouseEvent& event);

	void OnMouseLeave(wxMouseEvent& event);

	void OnCharHook(wxKeyEvent& event);

	void ToggleHistoryPanel(wxCommandEvent& evt);

	bool isDarkMode;
	bool isScientificOn;
	bool isHistoryVisible;
	MyCalculator* myCalculator;
	std::vector<wxString> buttonLabels;
	std::vector<wxButton*> buttons;
	wxTextCtrl* resultText;
	wxButton* modeButton;
	wxButton* button;
	std::vector<wxButton*> calculatorButtons;
	wxButton* darkModeToggle;
	wxBoxSizer* mainSizer;
	wxBoxSizer* memorySizer;
	wxGridSizer* buttonSizer;
	wxMenu* burgerMenu;
	wxMenuItem* darkModeItem;
	wxMenuItem* historyItem;
	wxMenuItem* calculatorMode;
	wxMenuBar* menuBar;
	wxPanel* historyPanel;
	wxPanel* calculatorPanel;
	std::vector<wxTextCtrl*> textCtrlList;

};

