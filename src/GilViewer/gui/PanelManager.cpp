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


#include <algorithm>

#include "PanelManager.h"

panel_manager_model::panel_manager_model()
{

}

panel_manager_model::~panel_manager_model()
{
}

const panel_manager_model::ArrayOfPanels & panel_manager_model::GetPanelsList()
{
    return m_panels;
}


panel_viewer* panel_manager_model::createObject(const std::string& id)
{
	panel_viewer* panel = PatternFactory<panel_viewer>::createObject(id);
	m_panels.push_back(panel);
	return panel;
}

