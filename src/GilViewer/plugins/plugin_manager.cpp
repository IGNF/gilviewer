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

void plugin_manager::clear()
{
    for(std::vector<plugin_base*>::iterator it=m_plugins.begin(); it!=m_plugins.end(); ++it)
        delete *it;
    m_plugins.clear();
}


bool plugin_manager::register_plugin(const boost::filesystem::path& path)
{
    return Register(BOOST_FILESYSTEM_STRING(path.filename()),
                    boost::bind(load_plugin, path));
}


void plugin_manager::register_plugins(const std::string &path, wxMenuBar* menus, wxAuiManager *manager, wxWindow *parent)
{
    vector<string> all_so_files = gilviewer_utils::all_files_from_path(path, "." + plugins_ext);

    for(unsigned int i=0;i<all_so_files.size();++i)
    {
        boost::filesystem::path file_path(boost::filesystem::system_complete(all_so_files[i]));
        string plugin_name(BOOST_FILESYSTEM_STRING(file_path.filename()));

        ostringstream mes;
        mes << "Plugin : " << plugin_name;

        if(!register_plugin(file_path))
        {
            mes << " is already loaded, skipping!";
            GILVIEWER_LOG_MESSAGE(mes.str());
            continue;
        }

        plugin_base *p = create_object(plugin_name);
        if(!p) continue;

		// Currently, wx based plugins crash when loaded in windows...
#ifndef _WINDOWS
        if(wx_plugin_base *wxp = dynamic_cast<wx_plugin_base *>(p))
        {
            mes << " (wx)";
            wxp->parent(parent);
            wxp->manager(manager);
            if(menus->FindMenu(_("Plugins"))==wxNOT_FOUND)
            {
                m_plugins_menu = new wxMenu;
                size_t menu_pos = menus->GetMenuCount();
                int about_position = menus->FindMenu(_("About ..."));
                if(about_position!=wxNOT_FOUND)
                    menu_pos = about_position;
                menus->Insert(menu_pos, m_plugins_menu, _("Plugins"));
            }
            long id = wxNewId();
            m_plugins_menu->Append(id, wxString(wxp->menuentry_name().c_str(), *wxConvCurrent));

            menus->GetParent()->Connect(
                    id, wxEVT_COMMAND_MENU_SELECTED,
                    wxCommandEventHandler(wx_plugin_base::show),
                    NULL, wxp );
            menus->Connect(
                    id, wxEVT_COMMAND_MENU_SELECTED,
                    wxCommandEventHandler(wx_plugin_base::show),
                    NULL, wxp );
        }
#endif // _WINDOWS
        mes << " OK";
        GILVIEWER_LOG_MESSAGE(mes.str());
    }
}
