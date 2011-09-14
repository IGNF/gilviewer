#ifndef __PLUGIN_BASE_HPP__
#define __PLUGIN_BASE_HPP__

#include <wx/wx.h>
#ifdef _WINDOWS
#   define IMPLEMENT_PLUGIN(name) extern "C" __declspec(dllexport) plugin_base* create_plugin_base() { return new name();};
#else
#   define IMPLEMENT_PLUGIN(name) extern "C" plugin_base* create_plugin_base() { return new name();};
#endif // _WINDOWS

//the plugin interface (a.k.a. abstract class)
//our plugin will contain GUI in itself - therefore we need to make it extend wxEvtHandler (or wxDialog for that matter)
class plugin_base : public wxFrame
{
public:
    plugin_base();
    plugin_base(const wxString &title);
    virtual void process() = 0;
    virtual void on_mouse_move(wxMouseEvent &event){};
    virtual wxWindow* gui() = 0;
    void parent(wxWindow* parent);

    // id: plugin name
    // path: directory containing the plugin library
    // ex to load rotate: id -> librotate.so; path -> /home/user/dev/gilviewer-unstable/lib
    static void Register(const std::string &id, const std::string &path);
    friend plugin_base* load_plugin(const std::string &id, const std::string &path);

    virtual std::string submenu_name() = 0;
    virtual std::string menuentry_name() = 0;

protected:
    wxWindow* m_parent;
};

//define a function pointer type for convenience
#ifndef __PLUGIN_BASE_FUNCTION__
#define __PLUGIN_BASE_FUNCTION__
    typedef plugin_base* ( *create_plugin_base_function)();
#endif //__PLUGIN_BASE_FUNCTION__

#endif // __PLUGIN_BASE_HPP__
