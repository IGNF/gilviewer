#include "plugin_base.hpp"
#include "plugin_manager.hpp"

#include <boost/bind.hpp>

#include <string>
#include <iostream>

#include <wx/dynlib.h>

#include "../convenient/macros_gilviewer.hpp"

using namespace std;
using namespace boost;

plugin_base::plugin_base() : wxFrame(NULL, wxID_ANY, wxString(_("Plugion frame"))) {}

plugin_base* load_plugin(const string &id, const string &path)
{
    //Load the plugin
    wxDynamicLibrary dll;
    //dll.Load(wxString(path.c_str(), *wxConvCurrent) + wxDynamicLibrary::CanonicalizeName(wxString(id.c_str(), *wxConvCurrent)));
    dll.Load(wxString(path.c_str(), *wxConvCurrent) + wxString("/", *wxConvCurrent) + wxString(id.c_str(), *wxConvCurrent));
    if(dll.IsLoaded())
    {
        //Create a valid function pointer using the function pointer type in plugin.h
        wxDYNLIB_FUNCTION(create_plugin_base_function,create_plugin_base,dll);
        //check if the function is found
        if(pfncreate_plugin_base)
        {
			GILVIEWER_LOG_ERROR("Loaded plugin " << path << " slash " << id)
            //Important: Use Detach(), otherwise the DLL will be unloaded once the wxDynamibLibrary object
            //goes out of scope
            dll.Detach();
            //Create the plugin
            plugin_base* plugin = pfncreate_plugin_base();
            return plugin;
        }
	} else
	{
		GILVIEWER_LOG_ERROR("Could not load plugin " << path << " slash " << id)
	}
    return NULL;
}

void plugin_base::Register(const string &id, const string &path)
{
    plugin_manager::instance()->Register(id, bind(load_plugin, id, path));
}

void plugin_base::parent(wxWindow* parent)
{
    m_parent = parent;
}
