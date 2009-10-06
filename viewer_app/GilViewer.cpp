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

//#include "vld.h"

#include <stdexcept>

#include <boost/shared_ptr.hpp>

#include <wx/cmdline.h>
#include <wx/filename.h>
#include <wx/cmdline.h>
#include <wx/msgdlg.h>
#include <wx/image.h>




#include "FrameViewer.hpp"
#include "GilViewer/layers/ImageLayer.hpp"
#include "GilViewer/layers/VectorLayer.hpp"
#include "GilViewer/layers/VectorLayerMultiGeometries.hpp"

#include "GilViewer/tools/Orientation2D.h"
#include "GilViewer/vectorutils/CreateShapes.h"


#include "GilViewer.h"
//#include "plugins/PluginManager.h"


static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
{ wxCMD_LINE_PARAM, NULL, NULL, _("input file"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
{ wxCMD_LINE_NONE } };

#ifdef __LINUX__
#include <locale.h>
#endif


IMPLEMENT_APP(MyApp);



bool MyApp::OnInit()
{
#ifdef __LINUX__
	setlocale(LC_ALL, "POSIX");
#endif


	// Parsing de la ligne de commande : on peut passer un nom de fichier
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


	wxInitAllImageHandlers();

	frame = new FrameViewer((wxFrame *)NULL, wxID_ANY, _("GilViewer"), wxPoint(50,50), wxSize(800,600));
	frame->AddLayersFromFiles( cmdFiles );

	frame->BuildPluginsMenu();

	frame->Show();
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


