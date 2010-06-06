#include "Layer.hpp"

#include "../tools/Orientation2D.h"
#include "../tools/ColorLookupTable.h"
#include "../gui/LayerSettingsControl.hpp"

const boost::shared_ptr<orientation_2d> &layer::Orientation() const
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
