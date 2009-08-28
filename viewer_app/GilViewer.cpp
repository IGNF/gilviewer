/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets. GilViewer is distributed under the CeCILL-B licence. 
See Licence_CeCILL-B_V1-en.txt or http://www.cecill.info for more details.


Homepage: 

	http://launchpad.net/gilviewer
	
Copyright:
	
	Institut Geographique National (2009)

Authors: 

	Olivier Tournaire, Adrien Chauve

	
	

This software is governed by the CeCILL-B license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL-B
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-B license and that you accept its terms.
 
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
#include "layers/ImageLayer.hpp"
#include "layers/VectorLayer.hpp"
#include "layers/VectorLayerMultiGeometries.hpp"

#include "tools/Orientation2D.h"
#include "vectorutils/CreateShapes.h"


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


