#include "Layer.hpp"

#include <sstream>

#include "../tools/error_logger.hpp"
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

void layer::add_orientation( const std::string &image_filename )
{
    try
    {
        boost::shared_ptr<orientation_2d> ori = boost::shared_ptr<orientation_2d>(new orientation_2d);
        ori->ReadOriFromImageFile(image_filename);
        this->Orientation(ori);
        this->HasOri(true);
    }
    catch (const std::exception &e)
    {
        std::ostringstream oss;
        oss << "No orientation for image " << image_filename << "\n" << e.what();
        error_logger::log_wx_log_message(oss.str());
        this->HasOri(false);
    }
}
