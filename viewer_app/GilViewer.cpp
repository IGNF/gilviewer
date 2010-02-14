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

#include "FrameViewer.hpp"
#include "GilViewer.h"

#include <gdal/ogrsf_frmts.h>

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
{ wxCMD_LINE_PARAM, NULL, NULL, wxT("Input files"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
{ wxCMD_LINE_NONE } };

#ifdef __LINUX__
#	include <locale.h>
#endif

IMPLEMENT_APP(GilViewerApp);

bool GilViewerApp::OnInit()
{
#ifdef __LINUX__
	setlocale(LC_ALL, "POSIX");
#endif

        // Register all OGR format drivers
        OGRSFDriverRegistrar* r = OGRSFDriverRegistrar::GetRegistrar();
        OGRRegisterAll();
        for(int i=0;i<r->GetDriverCount();++i)
            std::cout << r->GetDriver(i)->GetName() << std::endl;

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

	try{

	m_frame = new FrameViewer((wxFrame *)NULL, wxID_ANY, _("GilViewer"), wxPoint(50,50), wxSize(800,600));
	m_frame->AddLayersFromFiles( cmdFiles );
	m_frame->Show();
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

void GilViewerApp::set_langage(unsigned int language_id)
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
        	::wxLogMessage( _("Selected language is wrong!") );
            delete locale;
            locale = new wxLocale( wxLANGUAGE_ENGLISH );
            language = wxLANGUAGE_ENGLISH;
        }
    }
    else
    {
       	::wxLogMessage( _("The selected langage is not supported by your system. Try installing support for this language.") );
        locale = new wxLocale( wxLANGUAGE_ENGLISH );
        language = wxLANGUAGE_ENGLISH;
    }
}

