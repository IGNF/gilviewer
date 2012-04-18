/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets.


Homepage:

        http://code.google.com/p/gilviewer

Copyright:

        Institut Geographique National (2009)

Authors:

        Olivier Tournaire, Adrien Chauve




    GilViewer is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GilViewer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with GilViewer.  If not, see <http://www.gnu.org/licenses/>.

***********************************************************************/

/// @TODO: move from error_logger --> gilviewer_logger

#include "error_logger.hpp"

#include <wx/log.h>
#include <wx/frame.h>
#include <wx/textctrl.h>

void gilviewer_wx_error_logger::log_error(const std::string& message)
{
    gilviewer_wx_error_logger::log_common(message, new wxColour(255, 140, 0));
}

void gilviewer_wx_error_logger::log_exception(const std::string& message)
{
    gilviewer_wx_error_logger::log_common(message, const_cast<wxColour*>(wxRED) );
}

void gilviewer_wx_error_logger::log_warning(const std::string& message)
{
    gilviewer_wx_error_logger::log_common(message, new wxColour(148, 0, 211));
}

void gilviewer_wx_error_logger::log_message(const std::string& message)
{
    gilviewer_wx_error_logger::log_common(message, const_cast<wxColour*>(wxBLUE) );
}

void gilviewer_wx_error_logger::log_common(const std::string& message, wxColour* color)
{
    wxLog* current_logger = wxLog::GetActiveTarget();
    wxLogWindow* log_window = static_cast<wxLogWindow*>(current_logger);
    wxWindowList& children = log_window->GetFrame()->GetChildren();
    for(wxWindowList::compatibility_iterator node=children.GetFirst();node;node=node->GetNext())
    {
        wxWindow* current_window = (wxWindow*)node->GetData();
        wxTextCtrl* txtctrl = wxDynamicCast(current_window, wxTextCtrl);
        if(txtctrl)
        {
            txtctrl->SetDefaultStyle(wxTextAttr(*color));
        }
    }
    #if wxMINOR_VERSION < 9
        ::wxLogMessage( wxString(message.c_str(), *wxConvCurrent) );
    #else
        wxLogMessage( wxString(message.c_str(), *wxConvCurrent) );
    #endif
}
