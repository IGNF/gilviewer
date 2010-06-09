/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets.


Homepage:

	http://code.google.com/p/gilviewer

Copyright:

	Institut Geographique National (2009)

Authors:

	Olivier Tournaire, Adrien Chauve




    GilViewer is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GilViewer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with GilViewer.  If not, see <http://www.gnu.org/licenses/>.

***********************************************************************/

#include <iostream>
#include <sstream>
#include <limits>
#include <utility>

#include <boost/filesystem.hpp>

#include <boost/gil/algorithm.hpp>
#include "boost/gil/extension/numeric/sampler.hpp"
#include "boost/gil/extension/numeric/resample.hpp"

#include <wx/dc.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/log.h>
#include <wx/config.h>

#include "../tools/Orientation2D.h"
#include "../tools/ColorLookupTable.h"
#include "../layers/image_types.hpp"
#include "../gui/ImageLayerSettingsControl.hpp"

#include "ImageLayer.hpp"
#include "image_layer_screen_image_functor.hpp"
#include "image_layer_min_max_functor.hpp"
#include "image_layer_histogram_functor.hpp"
#include "image_layer_to_string_functor.hpp"
#include "image_layer_infos_functor.hpp"


class alpha_image_type : public boost::gil::gray8_image_t {};

using namespace std;
using namespace boost::gil;

image_layer::image_layer(const image_ptr &image, const std::string &name_, const std::string &filename_, const view_ptr& v):
        m_img(image), m_view(v)
{
    if(!v) m_view.reset(new view_t(boost::gil::view(m_img->value)));
    m_startInput[0] = m_startInput[1] = 0;
    m_sizeInput[0] = m_sizeInput[1] = 0;

    m_minmaxResult = apply_operation(m_view->value, any_view_min_max());

    name(name_);
    filename(filename_);

    alpha(255);
    //TODO
    intensity_min(m_minmaxResult.first);
    intensity_max(m_minmaxResult.second);
    transparent(false);
    transparency_max(0.);
    transparency_min(0.);
    gamma(1.);

    m_cLUT = boost::shared_ptr<color_lookup_table>(new color_lookup_table);

    /*
    ostringstream oss;
    oss << "Dimensions: " << m_view->value.width()  << " x " << m_view->value.height() << "\n";
    oss << "Nb channels: " << GetNbComponents() << "\n";
    oss << "Pixels type: " << GetTypeChannel() << "\n";

    m_infos = oss.str();
    */

    channels(0,1,2);
    alpha_channel(false,0);
}

layer::ptrLayerType image_layer::create_image_layer(const image_ptr &image, const string &name)
{
    return ptrLayerType(new image_layer(image, name));
}

template<class ImageType>
layer::ptrLayerType image_layer::create_image_layer(const ImageType &image, const std::string &name)
{
    image_ptr any_img(new image_t(image));
    return ptrLayerType(new image_layer(any_img, name));
}

void image_layer::update(int width, int height)
{
    // Lecture de la configuration des differentes options ...
    wxConfigBase *pConfig = wxConfigBase::Get();
    if (pConfig == NULL)
        return;

    bool loadWholeImage=false, bilinearZoom=false;

    pConfig->Read(wxT("/Options/LoadWoleImage"), &loadWholeImage, true); //TODO
    pConfig->Read(wxT("/Options/BilinearZoom"), &bilinearZoom, false);

    dev3n8_image_t screen_image(width, height);
    dev3n8_view_t screen_view = boost::gil::view(screen_image);

    if(!m_alpha_img) m_alpha_img.reset(new alpha_image_t);
    m_alpha_img->recreate(screen_view.dimensions());
    alpha_image_t::view_t alpha_view = boost::gil::view(*m_alpha_img);
    fill_pixels(alpha_view, 0);

    std::size_t nb_channels = nb_components();
    if(m_red>=nb_channels)
        m_red=nb_channels-1;
    if(m_green>=nb_channels)
        m_green=nb_channels-1;
    if(m_blue>=nb_channels)
        m_blue=nb_channels-1;
    channel_converter_functor my_cc(intensity_min(), intensity_max(), *m_cLUT, m_red, m_green, m_blue);
    apply_operation( m_view->value, screen_image_functor(screen_view, my_cc, m_zoomFactor, m_translationX, m_translationY, alpha_view, m_transparencyMin, m_transparencyMax, m_alpha, transparent()));

    wxImage monImage(screen_view.width(), screen_view.height(), interleaved_view_get_raw_data(screen_view), true);
    monImage.SetAlpha(interleaved_view_get_raw_data(boost::gil::view(*m_alpha_img)), true);

    m_bitmap = boost::shared_ptr<wxBitmap>(new wxBitmap(monImage));
}

void image_layer::draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const
{
    dc.DrawBitmap(*m_bitmap, x, y, transparent); //-m_translationX+x*m_zoomFactor, -m_translationX+y*m_zoomFactor
}

unsigned int image_layer::nb_components() const
{
    return apply_operation(m_view->value, nb_components_functor());
}

string image_layer::type_channel() const
{
    return apply_operation(m_view->value, type_channel_functor());
}

void image_layer::histogram(vector< vector<double> > &histo, double &min, double &max) const
{
    min = m_minmaxResult.first;
    max = m_minmaxResult.second;
    histo.clear();
    histo.resize( nb_components() );
    for (unsigned int i=0;i<nb_components();++i)
        histo[i].resize(256);
    apply_operation( m_view->value, histogram_functor(histo,min,max) );
}

string image_layer::pixel_value(int i, int j) const
{
    ostringstream oss;
    oss.precision(6);
    oss<<"(";
    i = static_cast<int>(floor(i*m_zoomFactor-m_translationX));
    j = static_cast<int>(floor(j*m_zoomFactor-m_translationY));
    apply_operation(m_view->value, any_view_image_position_to_string_functor(i,j, oss));
    oss<<")";
    return oss.str();
}

boost::shared_ptr<color_lookup_table> image_layer::GetColorLookupTable()
{
    return m_cLUT;
}

void image_layer::orientation(const boost::shared_ptr<orientation_2d> &orientation)
{
    m_ori->OriginX( orientation->OriginX() );
    m_ori->OriginY( orientation->OriginY() );
    m_ori->Step( orientation->Step() );
    m_ori->ZoneCarto( orientation->ZoneCarto() );
    m_ori->SizeX( orientation->SizeX() );
    m_ori->SizeY( orientation->SizeY() );
}

layer::ptrLayerType image_layer::crop(int& x0, int& y0, int& x1, int& y1) const
{
    if(x0<0) x0=0;
    if(y0<0) y0=0;
    if(x1>m_view->value.width ()) x1=m_view->value.width ();
    if(y1>m_view->value.height()) y1=m_view->value.height();
    if(x0>=x1 || y0>=y1) return ptrLayerType();
    view_t::type crop = subimage_view(m_view->value, x0, y0, x1-x0, y1-y0 );
    view_ptr crop_ptr(new view_t(crop));
    boost::filesystem::path file(boost::filesystem::system_complete(filename()));
    std::ostringstream oss;
    oss << ".crop" <<x0<<"_"<<y0<<"_"<<x1-x0<<"x"<<y1-y0;
    file.replace_extension(oss.str() + file.extension());
    std::string name = name + oss.str();
    return ptrLayerType(new image_layer(m_img, name, file.string(), crop_ptr) );
}

vector<string> image_layer::get_available_formats_extensions() const
{
    return vector<string>();
}

string image_layer::available_formats_wildcard() const
{
    ostringstream wildcard;
    wildcard << "All supported image files (*.tif;*.tiff;*.png;*.jpg;*.jpeg)|*.tif;*.tiff;*.png;*.jpg;*.jpeg|";
    wildcard << "TIFF (*.tif;*.tiff)|*.tif;*.tiff|";
    wildcard << "PNG (*.png)|*.png|";
    wildcard << "JPEG (*.jpg;*.jpeg)|*.jpg;*.jpeg|";
    return wildcard.str();
}

layer_settings_control* image_layer::build_layer_settings_control(unsigned int index, layer_control* parent)
{
    return new image_layer_settings_control(index, parent);
}

double image_layer::center_x() {return m_view->value.width()/2.;}
double image_layer::center_y() {return m_view->value.height()/2.;}
