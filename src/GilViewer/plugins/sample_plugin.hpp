#ifndef __SAMPLE_PLUGIN_HPP__
#define __SAMPLE_PLUGIN_HPP__

#include "plugin_base.hpp"

class sample_plugin : public plugin_base
{
public:
    /// It is mandatory to provide an empty constructor (used to register the plugin)
    sample_plugin() : plugin_base() {}
    sample_plugin(const wxString &title);
    virtual void process();
    virtual wxWindow* gui();

    virtual std::string submenu_name() { return "Image processing"; }
    virtual std::string menuentry_name() { return "Sample"; }

    void OnButton(wxCommandEvent& e);

};

#endif // __SAMPLE_PLUGIN_HPP__
