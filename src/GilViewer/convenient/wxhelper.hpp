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

#ifndef __WXHELPER_HPP__
#define __WXHELPER_HPP__

#include <wx/string.h>

#include <vector>
#include <string>

namespace wxhelper
{
    /// Cette fonction permet de retourner l'indice dans la liste a partir d'un style WX
    int from_wx_style_to_selection_index( unsigned int style );
    // Meme fonction que la precedente pour les lignes, mais dans l'autre sens ...
    int from_polygon_selection_index_to_wx_style( unsigned int selection );
    // Meme fonction que la precedente pour les polygones, mais dans l'autre sens ...
    int from_line_selection_index_to_wx_style( unsigned int selection );
}

#endif // __WXHELPER_HPP__
