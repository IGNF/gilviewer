#include <boost/filesystem.hpp>
#include <boost/bind.hpp>

#include "../convenient/utils.hpp"
#include "plugin_manager.hpp"
#include "../gui/panel_manager.hpp"
#include "../gui/define_id.hpp"


#include "../config/config_plugins.hpp"


using namespace std;

plugin_base* plugin_manager::create_object(const string& id)
{
    plugin_base* plugin = PatternFactory<plugin_base>::create_object(id);
    if(plugin)
        m_plugins.push_back(plugin);
    return plugin;
}


void plugin_manager::register_plugin(const boost::filesystem::path& path)
{
    Register(path.string(), boost::bind(load_plugin, path));
}


void plugin_manager::register_plugins(const std::string &path, wxMenuBar* menus)
{
    vector<string> all_so_files = gilviewer_utils::all_files_from_path(path, "." + plugins_ext);
    wxMenu* plugins_menu = NULL;

    unsigned int nb_of_successfully_loaded_plugins = 0;
    for(unsigned int i=0;i<all_so_files.size();++i)
    {
        boost::filesystem::path file_path(boost::filesystem::system_complete(all_so_files[i]));
        string plugin_name(BOOST_FILESYSTEM_STRING(file_path.filename()));
        const string libgilviewer_name = plugins_pre + "GilViewer." + plugins_ext;
        const string libtinyxml_name = plugins_pre + "tinyxml." + plugins_ext;
        if( file_path.filename()!= libgilviewer_name && file_path.filename()!= libtinyxml_name )
        {
            ostringstream mes;
            mes << "Plugin : " << plugin_name;
            register_plugin(file_path);
            plugin_base *p = create_object(file_path.string());
            if(p)
            {
                mes << " OK";
                if(wx_plugin_base *wxp = dynamic_cast<wx_plugin_base *>(p))
                {
                    int index_plugins_menu = menus->FindMenu(wxString("Plugins", *wxConvCurrent));
                    if(index_plugins_menu == wxNOT_FOUND)
                    {
                        plugins_menu = new wxMenu;
                        menus->Insert(menus->GetMenuCount(), plugins_menu, _("Plugins"));
                    }
                    else
                    {
                        plugins_menu = menus->GetMenu(index_plugins_menu);
                    }
                    if( plugins_menu )
                    {
                        plugins_menu->Append(FIRST_GILVIEWER_PLUGIN+nb_of_successfully_loaded_plugins, wxString(wxp->menuentry_name().c_str(), *wxConvCurrent));
                        menus->GetParent()->Connect(
                                FIRST_GILVIEWER_PLUGIN+nb_of_successfully_loaded_plugins,
                                wxEVT_COMMAND_MENU_SELECTED,
                                (wxObjectEventFunction)&wx_plugin_base::gui,
                                NULL, wxp );
                        menus->Connect(
                                FIRST_GILVIEWER_PLUGIN+nb_of_successfully_loaded_plugins,
                                wxEVT_COMMAND_MENU_SELECTED,
                                (wxObjectEventFunction)&wx_plugin_base::gui,
                                NULL, wxp );
                        ++nb_of_successfully_loaded_plugins;
                    }
                }
            }
            else
            {
                mes << " ERROR!";
            }
            GILVIEWER_LOG_MESSAGE(mes.str());
        }
    }
}
