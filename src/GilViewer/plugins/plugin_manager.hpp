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

#include "../tools/pattern_singleton.hpp"
#include "../tools/pattern_factory.hpp"
#include "plugin_base.hpp"
class wxMenuBar;
class wxMenu;
class wxAuiManager;

class plugin_manager : public PatternFactory<plugin_base>
{
public:
    virtual ~plugin_manager() {}
    plugin_base* create_object(const std::string& id);

    bool register_plugin(const boost::filesystem::path& path);
    void register_plugins(const std::string &path, wxMenuBar* menus, wxAuiManager *manager, wxWindow *parent);
    unsigned int size()const{ return (unsigned int)m_plugins.size();}
    plugin_base* at(unsigned int i) const {return m_plugins.at(i);}

private:
    // Le tableau des plugins
    std::vector<plugin_base*> m_plugins;
    wxMenu* m_plugins_menu;
};

#endif // __PLUGIN_MANAGER_HPP__

