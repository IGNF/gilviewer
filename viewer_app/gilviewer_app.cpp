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
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/cmdline.h>
#include <wx/msgdlg.h>
#include <wx/log.h>

#include "GilViewer/io/gilviewer_io_factory.hpp"
#include "gilviewer_frame.hpp"
#include "gilviewer_app.hpp"

#ifdef __LINUX__
#	include <locale.h>
#endif

#include "GilViewer/config/config.hpp"
#if GILVIEWER_USE_GDAL
#   include <gdal/ogrsf_frmts.h>
#endif // GILVIEWER_USE_GDAL

#include <sstream>
using namespace std;

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
{   wxCMD_LINE_PARAM,
    NULL,
    NULL,
#ifdef __WINDOWS__
    _("Input files"),
#else
    wxT("Input files"),
#endif
    wxCMD_LINE_VAL_STRING,
    wxCMD_LINE_PARAM_OPTIONAL },
{   wxCMD_LINE_NONE } };


IMPLEMENT_APP(gilviewer_app);

bool gilviewer_app::OnInit()
{
#ifdef __LINUX__
    setlocale(LC_ALL, "POSIX");
#endif

    register_all_file_formats();
#if GILVIEWER_USE_GDAL
    OGRRegisterAll();
#endif // GILVIEWER_USE_GDAL

    // Langage
    set_langage(wxLANGUAGE_FRENCH);

    // Parsing command line: it is possible to pass files, e.g. for a "Open with" contextual menu command in file explorer
    wxArrayString cmdFiles;
    wxString cmdFilename;
    wxCmdLineParser cmdLineParser(g_cmdLineDesc, argc, argv);

    cmdLineParser.Parse(false);

    if (cmdLineParser.GetParamCount() > 0)
    {
        for (unsigned int i=0;i<cmdLineParser.GetParamCount();++i)
        {
            cmdFilename = cmdLineParser.GetParam(i);
            wxFileName fileName(cmdFilename);
            fileName.Normalize(wxPATH_NORM_LONG | wxPATH_NORM_DOTS | wxPATH_NORM_TILDE | wxPATH_NORM_ABSOLUTE);
            cmdFilename = fileName.GetFullPath();
            cmdFiles.Add( cmdFilename );
        }
    }

    try
    {
        m_frame = new gilviewer_frame((wxFrame *)NULL, wxID_ANY, _("GilViewer"), wxPoint(50,50), wxSize(800,600));
	    m_frame->AddLayersFromFiles( cmdFiles );
	    m_frame->Show();
    }
    catch( std::exception &e )
    {
        GILVIEWER_LOG_EXCEPTION(e.what())
        wxString message(e.what(), *wxConvCurrent);
	wxMessageBox( message );
    }

    return true;
}

void gilviewer_app::set_langage(unsigned int language_id)
{
    wxLocale* locale;
    long language;

    language =  language_id;

    // Load language if possible, fall back to english otherwise
    if(wxLocale::IsAvailable(language))
    {
        locale = new wxLocale( language, wxLOCALE_CONV_ENCODING );

#		ifdef __WXGTK__
        // add locale search paths
        locale->AddCatalogLookupPathPrefix(wxT("/usr"));
        locale->AddCatalogLookupPathPrefix(wxT("/usr/local"));
        wxStandardPaths* paths = (wxStandardPaths*) &wxStandardPaths::Get();
        wxString prefix = paths->GetInstallPrefix();
        locale->AddCatalogLookupPathPrefix( prefix );
#		endif // __WXGTK__

        locale->AddCatalog(wxT("libGilViewer"));
        locale->AddCatalog(wxT("GilViewerApp"));

        if(! locale->IsOk() )
        {
            GILVIEWER_LOG_MESSAGE("Selected language is wrong!")
            delete locale;
            locale = new wxLocale( wxLANGUAGE_ENGLISH );
            language = wxLANGUAGE_ENGLISH;
        }
    }
    else
    {
        GILVIEWER_LOG_MESSAGE("Selected language is wrong!")
        locale = new wxLocale( wxLANGUAGE_ENGLISH );
        language = wxLANGUAGE_ENGLISH;
    }
}

