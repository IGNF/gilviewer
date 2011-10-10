#ifndef __ROTATE_IMAGE_HPP__
#define __ROTATE_IMAGE_HPP__

#include "GilViewer/plugins/plugin_base.hpp"

class rotate_image_plugin : public wx_plugin_base
{
public:
    /// It is mandatory to provide an empty constructor (used to register the plugin)
    rotate_image_plugin() : wx_plugin_base() {}
    rotate_image_plugin(const wxString &title);
    virtual void process();
    virtual void show(wxCommandEvent&);

    virtual std::string submenu_name() { return "Image processing"; }
    virtual std::string menuentry_name() { return "Sample"; }

    void on_button_90cw(wxCommandEvent& e);
};

#endif // __ROTATE_IMAGE_HPP__
