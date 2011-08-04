#include "layer.hpp"

#include <sstream>

#include "../tools/error_logger.hpp"
#include "../tools/orientation_2d.hpp"
#include "../tools/color_lookup_table.hpp"
#include "../gui/layer_settings_control.hpp"

#include <iostream>

layer::layer(const boost::function<void()> &notifyLayerControl, const boost::function<void()> &notifyLayerSettingsControl):
        notifyLayerControl_(notifyLayerControl),
        notifyLayerSettingsControl_(notifyLayerSettingsControl),
        m_isVisible(true),
        m_isTransformable(true),
        m_hasToBeUpdated(true),
        m_name("Default layer name"),
        m_filename(""),
        m_layer_orientation(LO_0),
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
    std::cout<<p0.x<<" "<<p0.y<<" "<<p1.x<<" "<<p1.y<<std::endl;
    wxRealPoint q0 = m_layer_transform.to_local(p0);
    wxRealPoint q1 = m_layer_transform.to_local(p1);
     std::cout<<q0.x<<" "<<q0.y<<" "<<q1.x<<" "<<q1.y<<std::endl;
   
    ptrLayerType l=crop_local(q0,q1);
    if(l==layer::ptrLayerType())
         return ptrLayerType();
    // fix "off by 1 pixel transform" errors for rotated images
    q1.x -= 1;
    q1.y -= 1;
 
    wxRealPoint r0 = m_layer_transform.from_local(q0);
    wxRealPoint r1 = m_layer_transform.from_local(q1);
    if(r0.x>r1.x) std::swap(r0.x,r1.x);
    if(r0.y>r1.y) std::swap(r0.y,r1.y);

    l->layer_orientation(layer_orientation());
    l->transform() = m_layer_transform;
    l->transform().translation_x(0);
    l->transform().translation_y(0);
    l->transform().translate(r0);

    return l;
}
