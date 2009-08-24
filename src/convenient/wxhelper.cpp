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

#include "convenient/wxhelper.hpp"

#include <wx/defs.h>

namespace wxhelper
{
int FromWxStyleToSelectionIndex( unsigned int style )
{
	switch ( style )
	{
		case wxSOLID:
			return 0;
		case wxTRANSPARENT:
			return 1;
		case wxDOT:
			return 2;
		case wxLONG_DASH:
			return 3;
		case wxSHORT_DASH:
			return 4;
		case wxDOT_DASH:
			return 5;
		case wxBDIAGONAL_HATCH:
			return 2;
		case wxFDIAGONAL_HATCH:
			return 3;
		case wxCROSSDIAG_HATCH:
			return 4;
		case wxHORIZONTAL_HATCH:
			return 5;
		case wxVERTICAL_HATCH:
			return 6;
		case wxCROSS_HATCH:
			return 7;
		default:
			return 0;
	}
}

int FromPolygonSelectionIndexToWxStyle( unsigned int selection )
{
	switch ( selection )
	{
		case 0:
			return wxSOLID;
		case 1:
			return wxTRANSPARENT;
		case 2:
			return wxDOT;
		case 3:
			return wxLONG_DASH;
		case 4:
			return wxSHORT_DASH;
		case 5:
			return wxDOT_DASH;
		default:
			return wxSOLID;
	}
}

int FromLineSelectionIndexToWxStyle( unsigned int selection )
{
	switch ( selection )
	{
		case 0:
			return wxSOLID;
		case 1:
			return wxTRANSPARENT;
		case 2:
			return wxBDIAGONAL_HATCH;
		case 3:
			return wxFDIAGONAL_HATCH;
		case 4:
			return wxCROSSDIAG_HATCH;
		case 5:
			return wxHORIZONTAL_HATCH;
		case 6:
			return wxVERTICAL_HATCH;
		case 7:
			return wxCROSS_HATCH;
		default:
			return wxSOLID;
	}
}
}
