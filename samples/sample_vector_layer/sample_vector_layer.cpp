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
#include "GilViewer/layers/simple_vector_layer.hpp"
#include "sample_vector_layer_viewer.hpp"
#include "sample_vector_layer.h"

#include "GilViewer/config/config.hpp"
#if GILVIEWER_USE_GDAL
#   include <gdal/ogrsf_frmts.h>
#endif // GILVIEWER_USE_GDAL

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
#if GILVIEWER_USE_GDAL
    OGRRegisterAll();
#endif // GILVIEWER_USE_GDAL

    try
    {
        m_frame = new sample_vector_layer_viewer((wxFrame *)NULL, wxID_ANY, _("Sample vector layer viewer"), wxPoint(50,50), wxSize(800,600));
	m_frame->Show();

#if GILVIEWER_USE_GDAL
        // Here, we first create an ogr_vector_layer
        boost::shared_ptr<layer> vector_layer = boost::shared_ptr<layer>(new ogr_vector_layer("Hand made layer"));
        m_frame->AddLayer(vector_layer);
        vector_layer->AddCircle(50.,123.,65.);
        vector_layer->AddPoint(50.,123.);
        vector_layer->AddPoint(5.,12.);

        vector_layer->set_point_width(6);
        vector_layer->set_point_color(*wxBLUE);

        vector_layer->set_line_color(wxColor(246,123,123));
        vector_layer->set_line_width(10);
        vector_layer->set_line_style(wxSHORT_DASH);

        vector_layer->AddText(52.3, 65.8, "text in ogr_vector_layer!!");
#endif // GILVIEWER_USE_GDAL

        // We now test the simple_vector_layer_interface
        boost::shared_ptr<layer> my_vector_layer = boost::shared_ptr<layer>(new simple_vector_layer("Simple layer"));
        m_frame->AddLayer(my_vector_layer);
        my_vector_layer->AddCircle(100.,100.,10.);
        my_vector_layer->set_polygon_border_color(*wxRED);
        my_vector_layer->set_polygon_border_width(5);
        my_vector_layer->set_polygon_inner_color(*wxBLUE);
        my_vector_layer->AddCircle(123.,198.,2.3654654);
        my_vector_layer->AddPoint(63.,45.654);
        my_vector_layer->AddPoint(23.2,5.98);
        my_vector_layer->set_point_width(8);
        my_vector_layer->AddLine(0.,0.,10.,10.);
        my_vector_layer->AddEllipse(-50.,1.,10.,25.,0.987456);
        my_vector_layer->AddEllipse(10.,-23.,10.,25.);
        my_vector_layer->set_polygon_inner_color(*wxGREEN);
        my_vector_layer->set_polygon_inner_style(wxCROSSDIAG_HATCH);
        std::vector<double> xp, yp;
        xp.push_back(12.3); xp.push_back(11.8); xp.push_back(-2.9);
        yp.push_back( 2.3); yp.push_back(-1.8); yp.push_back(16.9);
        my_vector_layer->AddPolyline(xp,yp);
        my_vector_layer->set_line_style(wxSHORT_DASH);
        my_vector_layer->set_line_width(1);
        my_vector_layer->AddText(12., 32., "Youhou!!!");
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

