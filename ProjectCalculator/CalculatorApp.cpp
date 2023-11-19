#include <wx/wx.h>
#include "CalculatorApp.h"
#include "CalculatorFrame.h"

wxIMPLEMENT_APP(CalculatorApp);

bool CalculatorApp::OnInit() {

	wxInitAllImageHandlers();

	CalculatorFrame* calculatorFrame = new CalculatorFrame("Kalkulator");
	calculatorFrame->SetClientSize(400, 600);
	calculatorFrame->Center();
	calculatorFrame->Show();

	return true;
}
