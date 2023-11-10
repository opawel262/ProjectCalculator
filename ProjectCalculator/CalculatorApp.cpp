#include <wx/wx.h>
#include "CalculatorApp.h"
#include "CalculatorFrame.h"

wxIMPLEMENT_APP(CalculatorApp);

bool CalculatorApp::OnInit() {

	wxInitAllImageHandlers();

	CalculatorFrame* frame = new CalculatorFrame("Kalkulator");
	frame->SetClientSize(400, 600);
	frame->Center();
	frame->Show();
	return true;
}
