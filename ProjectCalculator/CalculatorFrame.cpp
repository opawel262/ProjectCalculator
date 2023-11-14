#include "CalculatorFrame.h"
#include "MyCalculator.h"
#include <wx/wx.h>
#include <sstream>
#include <iomanip>

CalculatorFrame::CalculatorFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title), isDarkMode(false)
{
    mainSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer = new wxGridSizer(4, 4, 4);
    SetBackgroundColour(wxColour(184, 182, 182));
    // Text control for displaying the result
    resultText = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_RIGHT | wxTE_MULTILINE | wxTE_WORDWRAP | wxTE_READONLY);
    resultText->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    resultText->SetMargins(7, 7);
    resultText->CanCopy();
    resultText->CanUndo();


    // Calculator buttons
    wxString buttonLabels[] = {
        "-(x)", "10^(x)", "C", "<=",
        "sin(x)", "cos(x)", "tan(x)", "sqrt(x)",
        "PI", "e", "1/(x)", "exp(x)",
        "(", ")", "=","^",
        "7","8", "9", "/",
        "4", "5","6", "*",
        "1", "2", "3","-",
        ".", "0", "%", "+",

    };
    const int lenButtonLabels = sizeof(buttonLabels) / sizeof(buttonLabels[0]);

    // Create an array to hold the buttons
    wxButton* buttons[lenButtonLabels];

    for (int i = 0; i < lenButtonLabels; i++) {
        buttons[i] = new wxButton(this, 1000 + i, buttonLabels[i], wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
        buttonSizer->Add(buttons[i], 1, wxEXPAND);
        buttons[i]->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        buttons[i]->SetBackgroundColour(wxColour(220, 220, 220));
        buttons[i]->SetForegroundColour(*wxBLACK);
    }

    // Create a menu for the burger menu
    burgerMenu = new wxMenu;
    darkModeItem = burgerMenu->Append(wxID_ANY, wxT("Dark Mode"));
    historyItem = burgerMenu->Append(wxID_ANY, wxT("History"));

    // Store the buttons in a class member
    for (int i = 0; i < lenButtonLabels; i++) {
        calculatorButtons.push_back(buttons[i]);
    }

    // Create a menu bar and set it for the frame
    menuBar = new wxMenuBar;
    menuBar->Append(burgerMenu, wxT("☰"));

    // Set the menu bar for the frame
    SetMenuBar(menuBar);

    mainSizer->Add(resultText, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(buttonSizer, 3, wxEXPAND | wxALL, 5);
    SetSizerAndFit(mainSizer);


    // Bind event handlers
    bindEventHandlers();
}

void CalculatorFrame::ToggleDarkMode(wxCommandEvent& evt) {
    isDarkMode = !isDarkMode;

    // Update the button's style
    for (wxButton* button : calculatorButtons) {
        if (isDarkMode) {
            button->SetBackgroundColour(wxColour(50, 50, 50));
            button->SetForegroundColour(*wxWHITE);
        }
        else {
            // Reset the button's colors to their original state
            button->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            button->SetBackgroundColour(wxColour(220, 220, 220));
            button->SetForegroundColour(*wxBLACK);
        }

        // Refresh the button
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

    // odswieza ramke
    Refresh();
}

void CalculatorFrame::bindEventHandlers()
{
    for (wxButton* button : calculatorButtons) {
        button->Bind(wxEVT_BUTTON, &CalculatorFrame::onButtonsClicked, this);
        button->Bind(wxEVT_ENTER_WINDOW, &CalculatorFrame::OnMouseEnter, this);
        button->Bind(wxEVT_LEAVE_WINDOW, &CalculatorFrame::OnMouseLeave, this);
    }
    Bind(wxEVT_MENU, &CalculatorFrame::ToggleDarkMode, this, darkModeItem->GetId());
    Bind(wxEVT_CHAR_HOOK, &CalculatorFrame::OnCharHook, this);

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
            if ((resultSize == 0) || (resultSize >= 2 || resultSize <= 1) && ((secondLastElement == '-' || secondLastElement == '*' 
                || secondLastElement == '+'|| secondLastElement == '/' || secondLastElement == '^'))) {
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
        else if (buttonLabel == "cos" || buttonLabel == "tan" || buttonLabel == "sin" 
            || buttonLabel == "sqrt") {
            
        }
        else if (buttonLabel == "<=") {
            // warunki zeby ladnie usuwalo operatory i nie psulo formatu
            if(!result.IsEmpty()){
                if (result[resultSize - 1] == ' ' && (result[resultSize - 2] == '+'
                    || result[resultSize - 2] == '-' || result[resultSize - 2] == '*'
                    || result[resultSize - 2] == '/' || result[resultSize - 2] == '^')){
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
        else if (buttonLabel == "." ) {
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
                && !result.Contains("%") && !result.Contains("^"))
            {
                resultText->AppendText("");
            }
            else {
                long double resultFromStringCalc;
                MyCalculator myCalculator;
                myCalculator.infix = result;
                result = "";
                resultFromStringCalc = myCalculator.calculateString();
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
            }
            resultText->ChangeValue(result);
        }
        else if (buttonLabel == "0" || buttonLabel == "1" || buttonLabel == "2" || buttonLabel == "3" ||
            buttonLabel == "4" || buttonLabel == "5" || buttonLabel == "6" || buttonLabel == "7" ||
            buttonLabel == "8" || buttonLabel == "9") {
            resultText->AppendText(buttonLabel);
        }
        // przypadki dla tych przyciskow co pobieraja pierwsza ostatnia liczbe z resultTextu i owijaja ją
        else if (buttonLabel == "10^(x)" || buttonLabel == "sin(x)" ||buttonLabel == "cos(x)" 
            || buttonLabel == "tan(x)" || buttonLabel == "sqrt(x)" || buttonLabel == "-(x)") {
             if (!result.IsEmpty()) {
                 bool isLastClosingBracket = false;
                 wxString NumberToWrap = "";
                 int howManyCharactersToDelete = 0;

                 if (result[result.size() - 1] == ')') {
                     isLastClosingBracket = true;
                     howManyCharactersToDelete += 1;
                 }

                 for (int i = result.size() - 1; i >= 0; i--) {
                     if (isLastClosingBracket) {
                         if (result[i] == '(') {
                             howManyCharactersToDelete--;
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
                 if (buttonLabel == "sin(x)") {
                     result.append("sin(");
                 }
                 if (buttonLabel == "cos(x)") {
                     result.append("cos(");
                 }
                 if (buttonLabel == "tan(x)") {
                     result.append("tan(");
                 }
                 if (buttonLabel == "sqrt(x)") {
                     result.append("sqrt(");
                 }
                 if (buttonLabel == "-(x)") {
                    result.append("-");
                 }
                 result.append(NumberToWrap);
                 if(result[result.size() - 1] != ')') result.append(')');
                 if (buttonLabel == "-(x)" && NumberToWrap[0] != '(' && result[result.size() - 1] == ')') {
                     result.RemoveLast();
                 }
                 resultText->ChangeValue(result);
             }

        }
    }
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

    event.Skip();
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

    event.Skip();
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
        else if(key == '0' && event.ShiftDown()) {
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
    else if (key == WXK_RETURN) {
        label = "=";
    }
    else if (key == '-') {
        label = "-";
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
    for (wxButton* button : calculatorButtons) {
        if (button->GetLabel() == label) {
            wxCommandEvent evt(wxEVT_BUTTON, button->GetId());
            evt.SetEventObject(button);
            onButtonsClicked(evt);
            break;
        }
    }

    event.Skip();
}