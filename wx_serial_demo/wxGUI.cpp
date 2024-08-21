#include "wxGUI.h"


MyFrame1::MyFrame1(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer_Top;
	bSizer_Top = new wxBoxSizer(wxVERTICAL);

	m_scrolledWindow1 = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	m_scrolledWindow1->SetScrollRate(5, 5);
	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer(wxVERTICAL);

	m_panel_main = new wxPanel(m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* bSizer_sub;
	bSizer_sub = new wxBoxSizer(wxVERTICAL);

	bSizer_sub->SetMinSize(wxSize(2, 2));
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
	fgSizer1->SetFlexibleDirection(wxBOTH);
	fgSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_toggleBtn = new wxToggleButton(m_panel_main, wxID_ANY, _("打开摄像头"), wxPoint(240, -1), wxDefaultSize, 0);
	fgSizer1->Add(m_toggleBtn, 0, wxALL | wxTOP, 5);

	m_toggleBtn1 = new wxToggleButton(m_panel_main, wxID_ANY, _("Backup"), wxPoint(720, -1), wxDefaultSize, 0);
	m_toggleBtn1->SetValue(true);
	fgSizer1->Add(m_toggleBtn1, 0, wxALL, 5);

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer(new wxStaticBox(m_panel_main, wxID_ANY, _("Origin")), wxVERTICAL);

	sbSizer1->SetMinSize(wxSize(480, 360));
	m_panelOrigin = new wxPanel(sbSizer1->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxTAB_TRAVERSAL);
	sbSizer1->Add(m_panelOrigin, 1, wxEXPAND | wxALL, 5);


	fgSizer1->Add(sbSizer1, 1, wxEXPAND, 5);

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer(new wxStaticBox(m_panel_main, wxID_ANY, _("Gray")), wxVERTICAL);

	sbSizer2->SetMinSize(wxSize(480, 360));
	m_panelGray = new wxPanel(sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	sbSizer2->Add(m_panelGray, 1, wxEXPAND | wxALL, 5);


	fgSizer1->Add(sbSizer2, 1, wxEXPAND, 5);

	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer(new wxStaticBox(m_panel_main, wxID_ANY, _("Contours")), wxVERTICAL);

	sbSizer3->SetMinSize(wxSize(480, 360));
	m_panelContours = new wxPanel(sbSizer3->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	sbSizer3->Add(m_panelContours, 1, wxEXPAND | wxALL, 5);


	fgSizer1->Add(sbSizer3, 1, wxEXPAND, 5);

	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer(new wxStaticBox(m_panel_main, wxID_ANY, _("HVS")), wxVERTICAL);

	sbSizer4->SetMinSize(wxSize(480, 360));
	m_panelHVS = new wxPanel(sbSizer4->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	sbSizer4->Add(m_panelHVS, 1, wxEXPAND | wxALL, 5);


	fgSizer1->Add(sbSizer4, 1, wxEXPAND, 5);


	bSizer_sub->Add(fgSizer1, 1, wxEXPAND, 5);


	m_panel_main->SetSizer(bSizer_sub);
	m_panel_main->Layout();
	bSizer_sub->Fit(m_panel_main);
	bSizer->Add(m_panel_main, 1, wxEXPAND | wxALL, 5);


	m_scrolledWindow1->SetSizer(bSizer);
	m_scrolledWindow1->Layout();
	bSizer->Fit(m_scrolledWindow1);
	bSizer_Top->Add(m_scrolledWindow1, 1, wxEXPAND | wxALL, 5);


	this->SetSizer(bSizer_Top);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	m_toggleBtn->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(MyFrame1::OnButton), NULL, this);
	m_toggleBtn1->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(MyFrame1::OnButton), NULL, this);

}

MyFrame1::~MyFrame1()
{
	// Disconnect Events
	m_toggleBtn->Disconnect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(MyFrame1::OnButton), NULL, this);

}

//对话框
//Dialog_video::Dialog_video(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
//{
//	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
//
//	wxBoxSizer* bSizer_top;
//	bSizer_top = new wxBoxSizer(wxVERTICAL);
//
//	m_panel_video = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL); //
//	bSizer_top->Add(m_panel_video, 1, wxEXPAND | wxALL, 5);
//	
//	m_dc = new wxClientDC(m_panel_video);
//
//	this->SetSizer(bSizer_top);
//	this->Layout();
//
//	this->Centre(wxBOTH);
//}
//wxClientDC* Dialog_video::GetwxClientDC()
//{
//    	return m_dc;
//}
//void Dialog_video::capture()
//{
//	;
//
//}
//Dialog_video::~Dialog_video()
//{
//	m_dc->Clear();
//	delete m_dc;
//	delete m_panel_video;
//}