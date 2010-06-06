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

#include "wxhelper.hpp"

#include <wx/defs.h>

namespace wxhelper
{
int from_wx_style_to_selection_index( unsigned int style )
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

int from_polygon_selection_index_to_wx_style( unsigned int selection )
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

int from_line_selection_index_to_wx_style( unsigned int selection )
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
} // namespace wxhelper
