///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __new_layer_infos_control__
#define __new_layer_infos_control__

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
/// Class new_layer_infos_control
///////////////////////////////////////////////////////////////////////////////
class new_layer_infos_control : public wxFrame 
{
	private:
	
	protected:
		wxTextCtrl* m_text_control;
		wxStdDialogButtonSizer* m_std_buttons_sizer;
		wxButton* m_std_buttons_sizerOK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnOKButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		new_layer_infos_control( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 371,337 ), long style = wxDEFAULT_FRAME_STYLE|wxFRAME_NO_TASKBAR|wxMAXIMIZE|wxMAXIMIZE_BOX|wxTAB_TRAVERSAL );
		~new_layer_infos_control();
	
};

#endif //__new_layer_infos_control__
