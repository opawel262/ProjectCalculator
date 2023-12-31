﻿#include "CalculatorFrame.h"
#include <wx/wx.h>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include "MyCalculator.h"
#include "CalculatorMemory.h"


CalculatorFrame::CalculatorFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title),
    isDarkMode(false), isScientificOn(false), isHistoryVisible(false)
{
    myCalculator = new MyCalculator;

    calculatorPanel = new wxPanel(this, wxID_ANY);

    historyPanel = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);

    calculatorSizer = new wxBoxSizer(wxVERTICAL);

    historySizer = new wxBoxSizer(wxVERTICAL);

    buttonSizer = new wxGridSizer(4, 4, 4);

    mainSizer = new wxBoxSizer(wxVERTICAL);

    SetBackgroundColour(wxColour(184, 182, 182));

    historyPanel->SetScrollbars(0, 10, 0, 50);
    historyPanel->Hide();

    deleteMemoryButton = new wxButton(historyPanel, wxID_ANY, "Clear history");
    deleteMemoryButton->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    deleteMemoryButton->SetBackgroundColour(wxColour(220, 220, 220));
    deleteMemoryButton->SetForegroundColour(*wxBLACK);

    historySizer->Add(deleteMemoryButton, 0, wxALIGN_CENTER || wxALL, 5);


    std::vector<std::string> vectorsMemory = myCalculator->getVectorMemory();
    std::reverse(vectorsMemory.begin(), vectorsMemory.end());
    for (std::string historyOne : vectorsMemory) {
        wxTextCtrl* textctrl = new wxTextCtrl(historyPanel, wxID_ANY, historyOne, wxDefaultPosition,
            wxDefaultSize, wxTE_MULTILINE );
        if (isDarkMode) {
            textctrl->SetBackgroundColour(wxColour(50, 50, 50));
            textctrl->SetForegroundColour(*wxWHITE);
        }
        else {
            textctrl->SetBackgroundColour(wxColour(220, 220, 220));
            textctrl->SetForegroundColour(*wxBLACK);
        }
        textctrl->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)); 

        textctrl->SetMargins(5, 5); 
        textctrl->SetWindowStyle(wxTE_MULTILINE | wxTE_READONLY | wxBORDER_NONE); 

        historySizer->Add(textctrl, 0, wxEXPAND | wxALL, 5);
        textCtrlList.push_back(textctrl);
    }

    resultText = new wxTextCtrl(calculatorPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_RIGHT | wxTE_MULTILINE | wxTE_WORDWRAP | wxTE_READONLY);
    resultText->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    resultText->SetMargins(7, 7);
    resultText->CanCopy();
    resultText->CanUndo();

    buttonLabels = {
        "1/(x)", "-(x)", "C", "<=",
        "(", ")", "%", "=",
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        ".", "0", "^", "+", };



    for (const auto& label : buttonLabels) {

        wxButton* button = new wxButton(calculatorPanel, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);

        buttonSizer->Add(button, 1, wxEXPAND);

        button->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        button->SetBackgroundColour(wxColour(220, 220, 220));
        button->SetForegroundColour(*wxBLACK);

        buttons.push_back(button);
    }

    calculatorSizer->Add(resultText, 1, wxEXPAND | wxALL, 5);
    calculatorSizer->Add(buttonSizer, 3, wxEXPAND | wxALL, 5);

    calculatorPanel->SetSizerAndFit(calculatorSizer);

    historyPanel->SetSizerAndFit(historySizer);

    this->SetSizerAndFit(mainSizer);

    mainSizer->Add(calculatorPanel, 1, wxEXPAND);


    burgerMenu = new wxMenu;

    calculatorMode = burgerMenu->Append(wxID_ANY, wxT("Turn on Scientific mode"));
    darkModeItem = burgerMenu->Append(wxID_ANY, wxT("Dark Mode"));
    historyItem = burgerMenu->Append(wxID_ANY, wxT("History"));

    menuBar = new wxMenuBar;
    menuBar->Append(burgerMenu, wxT("☰"));

    SetMenuBar(menuBar);


    bindEventHandlers();

}

void CalculatorFrame::bindEventHandlers()
{
    for (wxButton* button : this->buttons) {
        button->Bind(wxEVT_BUTTON, &CalculatorFrame::onButtonsClicked, this);
        button->Bind(wxEVT_ENTER_WINDOW, &CalculatorFrame::OnMouseEnter, this);
        button->Bind(wxEVT_LEAVE_WINDOW, &CalculatorFrame::OnMouseLeave, this);
    }
    Bind(wxEVT_MENU, &CalculatorFrame::ToggleDarkMode, this, darkModeItem->GetId());
    Bind(wxEVT_CHAR_HOOK, &CalculatorFrame::OnCharHook, this);
    Bind(wxEVT_MENU, &CalculatorFrame::ToggleScientificCalculatorMode, this, calculatorMode->GetId());
    Bind(wxEVT_MENU, &CalculatorFrame::ToggleHistoryPanel, this, historyItem->GetId());
    deleteMemoryButton->Bind(wxEVT_BUTTON, &CalculatorFrame::onButtonClearHistory, this);

}

CalculatorFrame::~CalculatorFrame()
{
    delete myCalculator;
}

void CalculatorFrame::onButtonClearHistory(wxCommandEvent& evt) {
    wxButton* clickedButton = dynamic_cast<wxButton*>(evt.GetEventObject());
    if (clickedButton) {
        myCalculator->deleteMemory();
        updateHistory();
    }
    historyPanel->Refresh();
    historyPanel->Layout();
    Refresh();
    Layout();
}


void CalculatorFrame::ToggleDarkMode(wxCommandEvent& evt) {
        isDarkMode = !isDarkMode;

        for (wxButton* button : this->buttons) {
            if (isDarkMode) {
                button->SetBackgroundColour(wxColour(50, 50, 50));
                button->SetForegroundColour(*wxWHITE);
            }
            else {
                button->SetBackgroundColour(wxColour(220, 220, 220));
                button->SetForegroundColour(*wxBLACK);
            }
            button->Refresh();
        }

        if (isDarkMode) {
            SetBackgroundColour(wxColour(30, 30, 30));
            resultText->SetBackgroundColour(wxColour(50, 50, 50));
            resultText->SetForegroundColour(*wxWHITE);
        }
        else {
            SetBackgroundColour(wxColour(184, 182, 182));
            resultText->SetBackgroundColour(wxNullColour);
            resultText->SetForegroundColour(wxNullColour);
        }


        resultText->ShowPosition(resultText->GetLastPosition());
        resultText->ScrollLines(resultText->GetNumberOfLines());

        Refresh();
   
}

void CalculatorFrame::ToggleScientificCalculatorMode(wxCommandEvent& evt)
{
    isScientificOn = !isScientificOn;
    calculatorMode->SetItemLabel(isScientificOn ? wxT("Turn off Scientific mode") : wxT("Turn on Scientific mode"));

    // Usunięcie starych przycisków
    for (wxButton* button : this->buttons) {
        button->Destroy();
    }
    buttons.clear();


    if (isScientificOn) {
        buttonLabels = {
            "-(x)", "10^(x)", "C", "<=",
            "sin(x)", "cos(x)", "tan(x)", "sqrt(x)",
            "PI", "e", "1/(x)", "exp(x)",
            "(", ")", "=","^",
            "7","8", "9", "/",
            "4", "5","6", "*",
            "1", "2", "3","-",
            ".", "0", "%", "+",
        };
    }

    else {
        buttonLabels = {
            "1/(x)", "-(x)", "C", "<=",
            "(", ")", "%", "=",
            "7", "8", "9", "/",
            "4", "5", "6", "*",
            "1", "2", "3", "-",
            ".", "0", "^", "+",
        };
    }

    for (const auto& label : buttonLabels) {
        wxButton* button = new wxButton(calculatorPanel, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
        buttons.push_back(button);

        // Dodawanie nowych przycisków do sizer'a
        buttonSizer->Add(button, 1, wxEXPAND);
        if (isDarkMode) {
            button->SetBackgroundColour(wxColour(50, 50, 50));
            button->SetForegroundColour(*wxWHITE);
        }
        else {
            button->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            button->SetBackgroundColour(wxColour(220, 220, 220));
            button->SetForegroundColour(*wxBLACK);
        }
    }
    for (wxButton* button : this->buttons) {
        button->Bind(wxEVT_BUTTON, &CalculatorFrame::onButtonsClicked, this);
        button->Bind(wxEVT_ENTER_WINDOW, &CalculatorFrame::OnMouseEnter, this);
        button->Bind(wxEVT_LEAVE_WINDOW, &CalculatorFrame::OnMouseLeave, this);
    }
    calculatorPanel->Layout();
    calculatorPanel->Refresh();


    // Wywołanie Layout() oraz Refresh() dla zastosowania zmian
    Layout();
    Refresh();
}

void CalculatorFrame::onButtonsClicked(wxCommandEvent& evt)
{
    wxButton* clickedButton = dynamic_cast<wxButton*>(evt.GetEventObject());

    if (clickedButton) {
        
        wxString result = resultText->GetValue();
        size_t resultSize = 0;
        if (!result.IsEmpty()) {
            resultSize = result.size();
        }

        wxString buttonLabel = clickedButton->GetLabel();
        if ((buttonLabel == "+" || buttonLabel == "-" || buttonLabel == "*" || buttonLabel == "/"
            || buttonLabel == "%" || buttonLabel == "^")) {
            wxChar secondLastElement = 's';

            if (resultSize >= 2) {
                secondLastElement = result[resultSize - 2];
            }
            // warunek zeby nie wstawialo operacji arytmetycznych nieprawidlowo
            if ((resultSize == 0) || (resultSize >= 2 || resultSize <= 1) && (result[resultSize - 1] == ' ')
                && ((secondLastElement == '-' || secondLastElement == '*'
                    || secondLastElement == '+' || secondLastElement == '/' || secondLastElement == '^'))) {
                resultText->AppendText("");
            }
            else {
                resultText->AppendText(" ");
                resultText->AppendText(buttonLabel);
                resultText->AppendText(" ");
            }

        }
        else if (buttonLabel == "(" || buttonLabel == ")") {
            resultText->AppendText(buttonLabel);

        }
        else if (buttonLabel == "<=") {
            // warunki zeby ladnie usuwalo operatory i nie psulo formatu
            if (!result.IsEmpty()) {
                if (result[resultSize - 1] == ' ' && (result[resultSize - 2] == '+'
                    || result[resultSize - 2] == '-' || result[resultSize - 2] == '*'
                    || result[resultSize - 2] == '/' || result[resultSize - 2] == '^')) {
                    result = result.RemoveLast();
                    result = result.RemoveLast();
                }
                result = result.RemoveLast();
                resultText->ChangeValue(result);
            }
        }
        else if (buttonLabel == "C") {
            resultText->ChangeValue("");
        }
        else if (buttonLabel == ".") {
            if (!result.IsEmpty()) {
                // warunek zeby wpisywalo kropke tylko po liczbach
                if (result.Last() >= '0' && result.Last() <= '9') {
                    resultText->AppendText(buttonLabel);
                }
            }
        }
        else if (buttonLabel == "=") {
            // warunek zeby nic sie nie dzialo jak wyrazenie nie posiada +, -, ^ itd. do obliczenia jak zrobie potem sin() i inne rzeczy to zmienie!!!!!
            if (!result.Contains("+") && !result.Contains("-") && !result.Contains("/") && !result.Contains("*")
                && !result.Contains("%") && !result.Contains("^") && !result.Contains("sin") && !result.Contains("%")
                && !result.Contains("cos") && !result.Contains("tan") && !result.Contains("sqrt"))
            {
                resultText->AppendText("");
            }
            else {
                long double resultFromStringCalc;
                std::string addToMemory;
                myCalculator->infix = result;
                addToMemory.append(result);
                addToMemory.append(" = ");
                result = "";
                resultFromStringCalc = myCalculator->calculateString();
                if (myCalculator->isError) {
                    resultText->ChangeValue("incorrect syntax");
                    myCalculator->isError = false;
                }
                else {
                    result.Printf(wxT("%.15Lf"), resultFromStringCalc);
                    // usun zbedne zera po przecinku
                    size_t dotPosition = result.Find('.');
                    if (dotPosition != wxString::npos) {
                        size_t nonZeroPosition = result.find_last_not_of('0');
                        if (nonZeroPosition > dotPosition) {
                            result.erase(nonZeroPosition + 1);
                        }
                        else {
                            result.erase(dotPosition);
                        }
                    }

                    // jesli liczba jest całkowita, usun kropke na końcu
                    if (result.EndsWith(wxT("."))) {
                        result.RemoveLast();
                    }
                    addToMemory.append(result);
                    myCalculator->saveMemoryToFile(addToMemory);
                    resultText->ChangeValue(result);
                }
            }
 
            

        }
        else if (buttonLabel == "0" || buttonLabel == "1" || buttonLabel == "2" || buttonLabel == "3" ||
            buttonLabel == "4" || buttonLabel == "5" || buttonLabel == "6" || buttonLabel == "7" ||
            buttonLabel == "8" || buttonLabel == "9" || buttonLabel == "PI" || buttonLabel == "e"){
            resultText->AppendText(buttonLabel);
        }
        // przypadki dla tych przyciskow z kalkulatora naukowego co pobieraja 
        // pierwsza ostatnia liczbe z resultTextu i owijaja ją
        else if (isScientificOn || (!isScientificOn && (buttonLabel == "1/(x)" || buttonLabel == "-(x)"))) {
            if (buttonLabel == "10^(x)" || buttonLabel == "sin(x)" || buttonLabel == "cos(x)"
                || buttonLabel == "tan(x)" || buttonLabel == "sqrt(x)" || buttonLabel == "-(x)"
                || buttonLabel =="exp(x)" || buttonLabel == "1/(x)")
            {
                if (!result.IsEmpty()) {
                    bool isLastClosingBracket = false;
                    wxString NumberToWrap = "";
                    int howManyCharactersToDelete = 0;
                    int counter = 0;

                    if (result[result.size() - 1] == ')') {
                        isLastClosingBracket = true;
                        howManyCharactersToDelete += 3;
                    }

                    for (int i = result.size() - 1; i >= 0; i--) {
                        if (isLastClosingBracket) {
                            if (result[i] == ')') {
                                counter++;
                            }
                            if (result[i] == '(') {
                                counter--;
                                howManyCharactersToDelete--;
                            }
                            if (result[i] == '(' && counter == 0) {
                                break;
                            }
                            else {
                                NumberToWrap.Prepend(result[i]);
                            }
                        }
                        else {
                            if (result[i] == ' ') {
                                break;
                            }
                            else {
                                NumberToWrap.Prepend(result[i]);
                            }
                        }
                        howManyCharactersToDelete++;

                    }

                    for (size_t i = 0; i < howManyCharactersToDelete; i++) {
                        result.RemoveLast();
                    }

                    if (NumberToWrap.IsEmpty()) {
                        NumberToWrap = "0";
                    }
                    if (buttonLabel == "10^(x)") {
                        result.append("10 ^ (");
                    }
                    else if (buttonLabel == "sin(x)") {
                        result.append("sin(");
                    }
                    else if (buttonLabel == "cos(x)") {
                        result.append("cos(");
                    }
                    else if (buttonLabel == "tan(x)") {
                        result.append("tan(");
                    }
                    else if (buttonLabel == "sqrt(x)") {
                        result.append("sqrt(");
                    }
                    else if (buttonLabel == "-(x)") {
                        result.append("-(");
                    }
                    else if (buttonLabel == "exp(x)") {
                        result.append("exp(");
                    }
                    else if (buttonLabel == "1/(x)") {
                        result.append("1/(");
                    }
                    result.append(NumberToWrap);
                    if (result[result.size() - 1] != ')') result.append(')');

                    resultText->ChangeValue(result);
                }

            }
        }
    }
    Layout(); // Re-layout to adjust the visibility changes
    Refresh(); // Refresh to apply changes
    evt.Skip();


}

void CalculatorFrame::OnMouseEnter(wxMouseEvent& event)
{
    wxButton* button = dynamic_cast<wxButton*>(event.GetEventObject());
    if (button) {
        if (isDarkMode) {
            button->SetBackgroundColour(wxColour(40, 40, 40));
        }
        else {
            button->SetBackgroundColour(wxColour(210, 210, 210));
        }
        button->Refresh();
    }

}

void CalculatorFrame::OnMouseLeave(wxMouseEvent& event)
{
    wxButton* button = dynamic_cast<wxButton*>(event.GetEventObject());
    if (button) {
        if (isDarkMode) {
            button->SetBackgroundColour(wxColour(50, 50, 50));
        }
        else {
            button->SetBackgroundColour(wxColour(220, 220, 220));
        }
        button->Refresh();
    }

}
// funkcja sprawia ze przyciski na klawiaturze wywoluja przyciski z kalkulatora
void CalculatorFrame::OnCharHook(wxKeyEvent& event){
    int key = event.GetKeyCode();
    wxString label;

    // Check for Alt key
    bool isAltPressed = event.AltDown();
    if (key >= '0' && key <= '9') {
        if (key == '9' && event.ShiftDown()) {
            label = "(";
        }
        else if (key == '0' && event.ShiftDown()) {
            label = ")";
        }
        else if (key == '8' && event.ShiftDown()) {
            label = "*";
        }
        else if (key == '6' && event.ShiftDown()) {
            label = "^";
        }
        else if (key == '5' && event.ShiftDown()) {
            label = "%";
        }

        else {
            label = wxString::Format("%c", key);
        }
    }
    else if (key == '-') {
        label = "-";
    }
    else if (key == WXK_RETURN && !event.ShiftDown()) {
        label = "=";
    }
    else if (key == '/') {
        label = '/';
    }
    else if (key == '.') {
        label = ".";
    }
    else if (key == 'C' || key == WXK_ESCAPE) {
        label = "C";
    }
    else if (key == WXK_BACK) {
        label = "<=";
    }
    else if (key == '=' && event.ShiftDown()) {
        label = "+";
    }


    // Znajdź odpowiedni przycisk i wywołaj funkcję obsługi zdarzeń
    for (wxButton* button : this->buttons) {
        if (button->GetLabel() == label) {
            wxCommandEvent evt(wxEVT_BUTTON, button->GetId());
            evt.SetEventObject(button);
            onButtonsClicked(evt);

            break;

        }
    }
    Layout(); // Re-layout to adjust the visibility changes
    Refresh(); // Refresh to apply changes
    event.Skip();
}

void CalculatorFrame::updateHistory() {
    // Clear the existing history
    for (wxTextCtrl* textctrl : this->textCtrlList) {
        textctrl->Destroy();
    }
    textCtrlList.clear();

    // Retrieve the latest history from the calculator
    std::vector<std::string> vectorsMemory = myCalculator->getVectorMemory();
    std::reverse(vectorsMemory.begin(), vectorsMemory.end());
    // Add the latest history to the history panel
    for (std::string historyOne : vectorsMemory) {
        if (historyOne != "") {
            wxTextCtrl* textctrl = new wxTextCtrl(historyPanel, wxID_ANY, historyOne, wxDefaultPosition,
                wxDefaultSize, wxTE_MULTILINE);
            if (isDarkMode) {
                textctrl->SetBackgroundColour(wxColour(50, 50, 50));
                textctrl->SetForegroundColour(*wxWHITE);
            }
            else {
                textctrl->SetBackgroundColour(wxColour(220, 220, 220));
                textctrl->SetForegroundColour(*wxBLACK);
            }
            textctrl->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

            textctrl->SetMargins(5, 5);
            textctrl->SetWindowStyle(wxTE_MULTILINE | wxTE_READONLY | wxBORDER_NONE);

            historySizer->Add(textctrl, 0, wxEXPAND | wxALL, 5);
            textCtrlList.push_back(textctrl);
        }
    }
}

void CalculatorFrame::ToggleHistoryPanel(wxCommandEvent& evt) {
    isHistoryVisible = !isHistoryVisible;
    if (isHistoryVisible) {
        calculatorPanel->Hide();
        historyPanel->Show();
        mainSizer->Remove(0);
        mainSizer->Add(historyPanel, 1, wxEXPAND);

    }
    else {
        historyPanel->Hide();
        calculatorPanel->Show();
        mainSizer->Remove(0);
        mainSizer->Add(calculatorPanel, 1, wxEXPAND);
    }
    updateHistory();
    Layout(); // Re-layout to adjust the visibility changes
    Refresh(); // Refresh to apply changes
    evt.Skip();
}


