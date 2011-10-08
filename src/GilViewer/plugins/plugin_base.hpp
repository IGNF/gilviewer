#ifndef __PLUGIN_BASE_HPP__
#define __PLUGIN_BASE_HPP__

#include <wx/wx.h>

#ifdef _WINDOWS
#   define IMPLEMENT_PLUGIN(name) extern "C" __declspec(dllexport) plugin_base* create_plugin() { return new name();};
#else
#   define IMPLEMENT_PLUGIN(name) extern "C" plugin_base* create_plugin() { return new name();};
#endif // _WINDOWS

#include <boost/filesystem/path.hpp>
class gilviewer_io_factory;

//the plugin interface (a.k.a. abstract class)
class plugin_base
{
public:
    virtual ~plugin_base() {}
    virtual bool Register(gilviewer_io_factory *) { return true; }
};

//our plugin will contain GUI in itself - therefore we need to make it extend wxEvtHandler (or wxDialog for that matter)
class wx_plugin_base : public plugin_base, public wxFrame
{
public:
    wx_plugin_base();
    wx_plugin_base(const wxString &title);
    virtual ~wx_plugin_base() {}

    virtual void on_mouse_move(wxMouseEvent &event) {}
    virtual void on_close(wxCloseEvent& event) { Hide(); }
    virtual void show(wxCommandEvent&) {}
    void parent(wxWindow* parent);

    virtual std::string submenu_name() = 0;
    virtual std::string menuentry_name() = 0;

protected:
    wxWindow* m_parent;

private:
    DECLARE_EVENT_TABLE();
};

plugin_base* load_plugin(const boost::filesystem::path &path);
//define a function pointer type for convenience
typedef plugin_base* ( *create_plugin_function)();

#endif // __PLUGIN_BASE_HPP__
