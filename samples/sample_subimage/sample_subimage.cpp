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

/*
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/convenience.hpp>
 */

#include <stdexcept>
#include <iostream>
#include <sstream>

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/cmdline.h>
#include <wx/msgdlg.h>
#include <wx/log.h>

#include "../src/GilViewer/io/gilviewer_io_factory.hpp"
#include "../src/GilViewer/layers/ogr_vector_layer.hpp"
#include "../src/GilViewer/layers/simple_vector_layer.hpp"
#include "../src/GilViewer/convenient/macros_gilviewer.hpp"
#include "sample_subimage_viewer.hpp"
#include "sample_subimage.hpp"

#include "GilViewer/config/config.hpp"
#if GILVIEWER_USE_GDAL
#   include <gdal/ogrsf_frmts.h>
#endif // GILVIEWER_USE_GDAL

using namespace boost;
using namespace std;

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
{ wxCMD_LINE_PARAM, NULL, NULL, wxT("Input files"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
{ wxCMD_LINE_NONE } };

#ifdef __LINUX__
#	include <locale.h>
#endif

IMPLEMENT_APP(sample_subimage_app);

bool sample_subimage_app::OnInit()
{
#ifdef __LINUX__
    setlocale(LC_ALL, "POSIX");
#endif

    register_all_file_formats();

    m_frame = new sample_subimage_viewer((wxFrame *)NULL, wxID_ANY, _("Sample subimage viewer"), wxPoint(50,50), wxSize(800,600));
    m_frame->Show();

    string filename("../data/09-Paris.JPG");
    /*
     // You should use this, but, this is not necessary here since everything is know at compile time (and it also saves compile time)
    string extension(filesystem::extension(filename));
    extension = extension.substr(1,extension.size()-1);
    to_lower(extension);
    */
    try
    {
        shared_ptr<gilviewer_file_io> file = gilviewer_io_factory::instance()->create_object("jpg");
        m_frame->add_layer( file->load(filename) );
        boost::shared_ptr<layer> sublayer = file->load(filename, 1200, 200 , 950, 550);
        m_frame->add_layer( sublayer );
        sublayer->transform().translation_x(100);
        sublayer->transform().translation_y(250);
        //m_frame->add_layer( file->load(filename, 1000, 650, 450, 250) );
    }
    catch (const std::exception &e)
    {
        GILVIEWER_LOG_EXCEPTION("")
    }

    return true;
}

