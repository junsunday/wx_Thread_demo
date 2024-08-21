#pragma once
///////////////////////////////////////////////////////////////////////////
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/tglbtn.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/statbox.h>
#include <wx/scrolwin.h>
#include <wx/frame.h>
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame1
///////////////////////////////////////////////////////////////////////////////
class MyFrame1 : public wxFrame
{
private:

protected:
	wxScrolledWindow* m_scrolledWindow1;
	wxPanel* m_panel_main;
	wxToggleButton* m_toggleBtn;
	wxToggleButton* m_toggleBtn1;
	wxPanel* m_panelOrigin;
	wxPanel* m_panelGray;
	wxPanel* m_panelContours;
	wxPanel* m_panelHVS;

	// Virtual event handlers, overide them in your derived class
	virtual void OnButton(wxCommandEvent& event) { event.Skip(); }

public:

	MyFrame1(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ThreadOpenCVDemo"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(640,480), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);

	~MyFrame1();

};