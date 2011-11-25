#include "layer.hpp"

#include <sstream>

#include "../tools/error_logger.hpp"
#include "../tools/orientation_2d.hpp"
#include "../tools/color_lookup_table.hpp"
#include "../gui/layer_settings_control.hpp"

#include <iostream>

#include <wx/gdicmn.h>

layer::layer(const boost::function<void()> &notifyLayerControl, const boost::function<void()> &notifyLayerSettingsControl):
        notifyLayerControl_(notifyLayerControl),
        notifyLayerSettingsControl_(notifyLayerSettingsControl),
        m_isVisible(true),
        m_isTransformable(true),
        m_hasToBeUpdated(true),
        m_name("Default layer name"),
        m_filename(""),
        m_ori(boost::shared_ptr<orientation_2d>(new orientation_2d)),
        m_hasOri(false),
        m_infos(""),
        m_point_width(3), m_line_width(3),
        m_line_style(wxSOLID),
        m_polygon_border_width(3),
        m_polygon_border_style(wxSOLID), m_polygon_inner_style(wxSOLID),
        m_point_color(*wxRED), m_line_color(*wxBLUE), m_polygon_border_color(*wxLIGHT_GREY), m_polygon_inner_color(*wxGREEN), m_text_color(*wxRED) {
    
    static unsigned int countId=0;
    ++countId;
    m_id=countId;
}

const boost::shared_ptr<orientation_2d> &layer::orientation() const
{
    return m_ori;
}

boost::shared_ptr<color_lookup_table> layer::colorlookuptable()
{
    return boost::shared_ptr<color_lookup_table>();
}

layer_settings_control* layer::build_layer_settings_control(unsigned int index, layer_control* parent)
{
    return new layer_settings_control(parent);
}

void layer::add_orientation( const std::string &image_filename )
{
    boost::shared_ptr<orientation_2d> ori = boost::shared_ptr<orientation_2d>(new orientation_2d);
    if( ori->read_ori_from_image_file(image_filename) )
    {
        this->orientation(ori);
        this->has_ori(true);
    }
    else
        this->has_ori(false);
}

layer::ptrLayerType layer::crop(const wxRealPoint& p0, const wxRealPoint& p1) const {
    // compute local coordinates
    wxRealPoint q0 = transform().to_local(p0);
    wxRealPoint q1 = transform().to_local(p1);

    return crop_local(q0,q1);
}
