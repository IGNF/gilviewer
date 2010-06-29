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

#ifndef __PLUGIN_MANAGER_HPP__
#define __PLUGIN_MANAGER_HPP__
#include <vector>

#include "../tools/PatternSingleton.hpp"
#include "../tools/PatternFactory.hpp"
#include "plugin_base.hpp"

class plugin_manager_model : public PatternFactory<plugin_base>
{
public:
    friend class PatternSingleton<plugin_manager_model>;
    virtual ~plugin_manager_model() {}
    plugin_base* create_object(const std::string& id);

    static void register_plugins(const std::string &path);

private:
    plugin_manager_model() {}
    // Le tableau des plugins
    std::vector<plugin_base*> m_plugins;
};

typedef PatternSingleton<plugin_manager_model> plugin_manager;

#endif // __PLUGIN_MANAGER_HPP__

