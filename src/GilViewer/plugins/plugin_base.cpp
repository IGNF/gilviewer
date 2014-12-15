#include "plugin_base.hpp"
#include "plugin_manager.hpp"

#include <string>
#include <iostream>

#include <wx/dynlib.h>

#include "GilViewer/tools/pattern_singleton.hpp"
#include "GilViewer/convenient/macros_gilviewer.hpp"
#include "GilViewer/io/gilviewer_io_factory.hpp"

using namespace std;
using namespace boost;

wx_plugin_base::wx_plugin_base() : wxFrame(NULL, wxID_ANY, wxString("", *wxConvCurrent)) {}
wx_plugin_base::wx_plugin_base(const wxString &title) : wxFrame(NULL, wxID_ANY, title) {}

plugin_base* load_plugin(const boost::filesystem::path &path)
{
    //Load the plugin
    wxDynamicLibrary dll;
    dll.Load(wxString(path.string().c_str(), *wxConvCurrent));
    if(!dll.IsLoaded())
    {
        GILVIEWER_LOG_ERROR(path.string() << " : Could not be loaded");
        return NULL;
    }

    //Create a valid function pointer using the function pointer type in plugin.h
    wxDYNLIB_FUNCTION(create_plugin_function,create_plugin,dll);
    //check if the function is found
    if(!pfncreate_plugin)
    {
        GILVIEWER_LOG_MESSAGE(path.string() << " : Not a plugin (no create_plugin function)");
        return NULL;
    }

    //Important: Use Detach(), otherwise the DLL will be unloaded once the wxDynamibLibrary object
    //goes out of scope
    dll.Detach();
    //Create the plugin
    plugin_base* plugin = pfncreate_plugin();
    plugin->Register(PatternSingleton<gilviewer_io_factory>::instance());
    return plugin;
}

void wx_plugin_base::parent(wxWindow* parent)
{
    m_parent = parent;
}
void wx_plugin_base::manager(wxAuiManager *manager)
{
    m_manager = manager;
}

BEGIN_EVENT_TABLE(wx_plugin_base, wxFrame)
        EVT_CLOSE(wx_plugin_base::on_close)
        END_EVENT_TABLE()
