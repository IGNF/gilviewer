#include "plugin_base.hpp"
#include "plugin_manager.hpp"

#include <boost/bind.hpp>

#include <string>
#include <iostream>

#include <wx/dynlib.h>

#include "../convenient/macros_gilviewer.hpp"
#include "../io/gilviewer_io_factory.hpp"

using namespace std;
using namespace boost;

wx_plugin_base::wx_plugin_base() : wxFrame(NULL, wxID_ANY, wxString("", *wxConvCurrent)) {}
wx_plugin_base::wx_plugin_base(const wxString &title) : wxFrame(NULL, wxID_ANY, title) {}

plugin_base* load_plugin(const boost::filesystem::path &path)
{
    //Load the plugin
    wxDynamicLibrary dll;
    dll.Load(wxString(path.string().c_str(), *wxConvCurrent));
    if(dll.IsLoaded())
    {
        //Create a valid function pointer using the function pointer type in plugin.h
        wxDYNLIB_FUNCTION(create_plugin_base_function,create_plugin_base,dll);
        //check if the function is found
        if(pfncreate_plugin_base)
        {
            //Important: Use Detach(), otherwise the DLL will be unloaded once the wxDynamibLibrary object
            //goes out of scope
            dll.Detach();
            //Create the plugin
            plugin_base* plugin = pfncreate_plugin_base();
            plugin->Register(PatternSingleton<gilviewer_io_factory>::instance());
            return plugin;
        }
    }
    else
    {
        GILVIEWER_LOG_ERROR("Could not load plugin " << path)
    }
    return NULL;
}

void register_plugin(const boost::filesystem::path& path)
{
    plugin_manager::instance()->Register(path.string(), bind(load_plugin, path));
}

void wx_plugin_base::parent(wxWindow* parent)
{
    m_parent = parent;
}
