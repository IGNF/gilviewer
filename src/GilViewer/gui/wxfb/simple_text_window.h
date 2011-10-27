///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __simple_text_window__
#define __simple_text_window__

#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class simple_text_window
///////////////////////////////////////////////////////////////////////////////
class simple_text_window : public wxFrame 
{
	private:
	
	protected:
		wxTextCtrl* m_text_control;
		wxStdDialogButtonSizer* m_std_buttons_sizer;
		wxButton* m_std_buttons_sizerOK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnOKButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		simple_text_window( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 371,337 ), long style = wxDEFAULT_FRAME_STYLE|wxFRAME_NO_TASKBAR|wxMAXIMIZE_BOX|wxTAB_TRAVERSAL );
		~simple_text_window();
	
};

#endif //__simple_text_window__
