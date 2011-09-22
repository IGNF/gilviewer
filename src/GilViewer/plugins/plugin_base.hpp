#ifndef __PLUGIN_BASE_HPP__
#define __PLUGIN_BASE_HPP__

#include <wx/wx.h>

#ifdef _WINDOWS
#   define IMPLEMENT_PLUGIN(name) extern "C" __declspec(dllexport) plugin_base* create_plugin_base() { return new name();};
#else
#   define IMPLEMENT_PLUGIN(name) extern "C" plugin_base* create_plugin_base() { return new name();};
#endif // _WINDOWS

#include <boost/filesystem/path.hpp>
class gilviewer_io_factory;

//the plugin interface (a.k.a. abstract class)
class plugin_base
{
public:
    virtual bool Register(gilviewer_io_factory *) { return true; }
};

//our plugin will contain GUI in itself - therefore we need to make it extend wxEvtHandler (or wxDialog for that matter)
class wx_plugin_base : public plugin_base, public wxFrame
{
public:
    wx_plugin_base();
    wx_plugin_base(const wxString &title);
    virtual void on_mouse_move(wxMouseEvent &event) {};
    virtual wxWindow* gui() { return NULL; }
    void parent(wxWindow* parent);

    virtual std::string submenu_name() = 0;
    virtual std::string menuentry_name() = 0;

protected:
    wxWindow* m_parent;
};


void register_plugin(const boost::filesystem::path& path);


//define a function pointer type for convenience
#ifndef __PLUGIN_BASE_FUNCTION__
#define __PLUGIN_BASE_FUNCTION__
    typedef plugin_base* ( *create_plugin_base_function)();
#endif //__PLUGIN_BASE_FUNCTION__

#endif // __PLUGIN_BASE_HPP__
