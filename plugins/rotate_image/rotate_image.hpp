#ifndef __SAMPLE_PLUGIN_HPP__
#define __SAMPLE_PLUGIN_HPP__

#include "GilViewer/plugins/plugin_base.hpp"

class rotate_image_plugin : public plugin_base
{
public:
    /// It is mandatory to provide an empty constructor (used to register the plugin)
    rotate_image_plugin() : plugin_base() {}
    rotate_image_plugin(const wxString &title);
    virtual void process();
    virtual wxWindow* gui();

    virtual std::string submenu_name() { return "Image processing"; }
    virtual std::string menuentry_name() { return "Sample"; }

    void on_button_90cw(wxCommandEvent& e);
};

#endif // __SAMPLE_PLUGIN_HPP__
