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

#include <stdexcept>
#include <iostream>

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/cmdline.h>
#include <wx/msgdlg.h>
#include <wx/log.h>

#include "GilViewer/io/gilviewer_io_factory.hpp"
#include "GilViewer/layers/ogr_vector_layer.hpp"
#include "sample_vector_layer_viewer.hpp"
#include "sample_vector_layer.h"

#include <gdal/ogrsf_frmts.h>

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
{ wxCMD_LINE_PARAM, NULL, NULL, wxT("Input files"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
{ wxCMD_LINE_NONE } };

#ifdef __LINUX__
#	include <locale.h>
#endif

IMPLEMENT_APP(sample_vector_layer_app);

bool sample_vector_layer_app::OnInit()
{
#ifdef __LINUX__
    setlocale(LC_ALL, "POSIX");
#endif

    register_all_file_formats();
    OGRRegisterAll();

    try
    {

        m_frame = new sample_vector_layer_viewer((wxFrame *)NULL, wxID_ANY, _("Sample vector layer viewer"), wxPoint(50,50), wxSize(800,600));
	m_frame->Show();

        // Here, we first create an ogr_vector_layer
        boost::shared_ptr<Layer> vector_layer = boost::shared_ptr<Layer>(new ogr_vector_layer("Hand made layer"));
        m_frame->AddLayer(vector_layer);
        vector_layer->AddCircle(50.,123.,65.);
        vector_layer->AddPoint(50.,123.);
        vector_layer->AddPoint(5.,12.);

        vector_layer->set_point_width(6);
        vector_layer->set_point_color(*wxBLUE);

        vector_layer->set_line_color(wxColor(246,123,123));
        vector_layer->set_line_width(10);
        vector_layer->set_line_style(wxSHORT_DASH);
    }
    catch( std::exception &e )
    {
	wxString message;
	message << wxString(e.what(), *wxConvCurrent);
	::wxMessageBox( message );
    }
    catch( ... )
    {
	::wxMessageBox( _("Unhandled exception ...") );
    }

    return true;
}

