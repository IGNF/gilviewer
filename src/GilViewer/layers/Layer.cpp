#include "Layer.hpp"

#include <sstream>

#include "../tools/error_logger.hpp"
#include "../tools/Orientation2D.h"
#include "../tools/ColorLookupTable.h"
#include "../gui/LayerSettingsControl.hpp"

layer::layer(const boost::function<void()> &notifyLayerControl, const boost::function<void()> &notifyLayerSettingsControl):
        notifyLayerControl_(notifyLayerControl),
        notifyLayerSettingsControl_(notifyLayerSettingsControl),
        m_isVisible(true),
        m_isTransformable(true),
        m_hasToBeUpdated(true),
        m_name("Default layer name"),
        m_filename(""),
        m_zoomFactor(1.),
        m_translationX(0.), m_translationY(0.),
        m_ori(boost::shared_ptr<orientation_2d>(new orientation_2d)),
        m_hasOri(false),
        m_infos(""),
        m_point_width(3), m_line_width(3),
        m_line_style(wxSOLID),
        m_polygon_border_width(3),
        m_polygon_border_style(wxSOLID), m_polygon_inner_style(wxSOLID),
        m_point_color(*wxRED), m_line_color(*wxBLUE), m_polygon_border_color(*wxLIGHT_GREY), m_polygon_inner_color(*wxGREEN), m_text_color(*wxRED) {}

const boost::shared_ptr<orientation_2d> &layer::orientation() const
{
    return m_ori;
}

boost::shared_ptr<color_lookup_table> layer::GetColorLookupTable()
{
    return boost::shared_ptr<color_lookup_table>();
}

layer_settings_control* layer::build_layer_settings_control(unsigned int index, layer_control* parent)
{
    return new layer_settings_control(parent);
}

void layer::add_orientation( const std::string &image_filename )
{
    try
    {
        boost::shared_ptr<orientation_2d> ori = boost::shared_ptr<orientation_2d>(new orientation_2d);
        ori->ReadOriFromImageFile(image_filename);
        this->orientation(ori);
        this->has_ori(true);
    }
    catch (const std::exception &e)
    {
        std::ostringstream oss;
        oss << "No orientation for image " << image_filename << "\n" << e.what();
        error_logger::log_wx_log_message(oss.str());
        this->has_ori(false);
    }
}
