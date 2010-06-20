#ifndef __SAMPLE_PLUGIN_HPP__
#define __SAMPLE_PLUGIN_HPP__

#include "plugin_base.hpp"



#include <boost/gil/image.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/gil/image_view_factory.hpp>
#include <boost/gil/image_view.hpp>

class sample_plugin : public plugin_base
{
public:
    sample_plugin();
    virtual void process();
    virtual wxWindow* gui();

    virtual std::string submenu_name() { return "Image processing"; }
    virtual std::string menuentry_name() { return "Sample"; }

    void OnButton(wxCommandEvent& e);

};

#endif // __SAMPLE_PLUGIN_HPP__
