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

#ifndef PANELMANAGER_H_
#define PANELMANAGER_H_
#include <vector>

#include "../tools/pattern_singleton.hpp"
#include "../tools/pattern_factory.hpp"
#include "../gui/panel_viewer.hpp"


class panel_manager_model : public  PatternFactory<panel_viewer>
{
public:
    typedef std::vector<panel_viewer*> ArrayOfPanels;

    friend class PatternSingleton<panel_manager_model>;
    virtual ~panel_manager_model();


    // La méthode  qui va nous permettre de récupérer
    // la liste des panels
    const ArrayOfPanels &panels_list();

    panel_viewer* create_object(const std::string& id);


private:
    panel_manager_model();

    // Le tableau des plugins
    ArrayOfPanels m_panels;

};

typedef PatternSingleton<panel_manager_model> panel_manager;

#endif /* PANELMANAGER_H_ */

