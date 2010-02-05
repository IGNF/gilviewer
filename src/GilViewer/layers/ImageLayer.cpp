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
#include <boost/algorithm/string.hpp>   

#include <boost/gil/algorithm.hpp>
#include <boost/gil/extension/dynamic_image/any_image.hpp>
#include <boost/gil/extension/io/tiff_dynamic_io.hpp>
#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include "boost/gil/extension/numeric/sampler.hpp"
#include "boost/gil/extension/numeric/resample.hpp"

#include <wx/dc.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/log.h>
#include <wx/config.h>

#include "GilViewer/tools/ColorLookupTable.h"
#include "GilViewer/layers/image_types.hpp"

#include "ImageLayer.hpp"
#include "image_layer_screen_image_functor.hpp"
#include "image_layer_min_max_functor.hpp"
#include "image_layer_get_any_image_functor.hpp"
#include "image_layer_histogram_functor.hpp"
#include "image_layer_to_string_functor.hpp"
#include "image_layer_infos_functor.hpp"

class any_image_types  : public all_image_types {};
class alpha_image_type : public boost::gil::gray8_image_t {};

using namespace std;
using namespace boost::gil;

ImageLayer::ImageLayer(const boost::shared_ptr<image_t> &image, const string &name):
        m_img(image)
{
    m_startInput[0] = m_startInput[1] = 0;
    m_sizeInput[0] = m_sizeInput[1] = 0;

    m_minmaxResult = apply_operation(view(*m_img), any_view_min_max());

    Name(name);

    Alpha(255);
    //TODO
    IntensityMin(m_minmaxResult.first);
    IntensityMax(m_minmaxResult.second);
    IsTransparent(false);
    TransparencyMax(0.);
    TransparencyMin(0.);
    Gamma(1.);

    ZoomFactor(1.);
    TranslationX(0.);
    TranslationY(0.);

    IsVisible(true);
    IsTransformable(true);
    HasToBeUpdated(true);

    m_cLUT = boost::shared_ptr<ColorLookupTable>(new ColorLookupTable);

//	m_infos = m_visuITK->GetInfos();
    m_infos = "";

    ostringstream oss;
    oss << "Dimensions: " << m_img->width()  << " x " << m_img->height() << "\n";
    oss << "Nb channels: " << GetNbComponents() << "\n";
    oss << "Pixels type: " << GetTypeChannel() << "\n";

    m_infos += oss.str();

    SetChannels(0,1,2);
    SetAlphaChannel(false,0);
}



Layer::ptrLayerType ImageLayer::CreateImageLayer(const boost::shared_ptr<image_t> &image, const string &name)
{
    return ptrLayerType(new ImageLayer(image, name));
}


Layer::ptrLayerType ImageLayer::CreateImageLayer(const string &filename)
{
    if ( !boost::filesystem::exists(filename) )
    {
        ostringstream oss;
        oss << "File does not exist: "<<filename<< " ! " << endl;
        oss << "File : " <<__FILE__ << endl;
        oss << "Line : " << __LINE__ << endl;
        oss << "Function : " << __FUNCTION__ << endl;
        wxLogMessage( wxString(oss.str().c_str(), *wxConvCurrent) );
        return ptrLayerType();
    }


    std::string ext(boost::filesystem::extension(filename));
    boost::to_lower(ext);

//	image_read_info< tiff_tag > info = read_image_info(filename, tiff_tag());

    boost::shared_ptr<image_t> image(new image_t);

	try
	{
		if (ext == ".tif" || ext == ".tiff")
			tiff_read_image(filename, *image);
		else if (ext == ".jpg" || ext == ".jpeg")
			jpeg_read_image(filename, *image);
		else if (ext == ".png")
			png_read_image(filename, *image);
	}
	catch( const exception &e )
    {
        ostringstream oss;
        oss << "Read error: "<<filename<< " ! " << endl;
        oss << "File : " <<__FILE__ << endl;
        oss << "Line : " << __LINE__ << endl;
        oss << "Function : " << __FUNCTION__ << endl;
        wxLogMessage( wxString(oss.str().c_str(), *wxConvCurrent) );
        return ptrLayerType();
    }

    //Creation de la couche image
    ptrLayerType maLayer(new ImageLayer(image, filename));


    // Le m_name est sette dans la fonction appelle a la ligne precedente ...
    // Calcul du chemin complet du fichier
    boost::filesystem::path full = boost::filesystem::system_complete(filename);
    maLayer->Filename( full.string() );

    //Lecture de l'ori et test d'existence
    try
    {
	Orientation2D ori;
	ori.ReadOriFromImageFile(filename);
        maLayer->Orientation(ori);
        maLayer->HasOri(true);
    }
    catch (exception &e)
    {
        ostringstream message;
        message << "No orientation for image " << filename;
        message << "\n" << e.what();
        wxLogMessage( wxString(message.str().c_str(), *wxConvCurrent) );
        maLayer->HasOri(false);
    }

    return maLayer;
}

template<class ImageType>
Layer::ptrLayerType ImageLayer::CreateImageLayer(const ImageType &image, const std::string &name)
{
    boost::shared_ptr<image_t> any_img(new image_t(image));
    return ptrLayerType(new ImageLayer(any_img, name));
}

template<typename T>
inline int iRound(const T x)
{
    using namespace std;
    return static_cast<int>(floor(x + 0.5));
}

void ImageLayer::Update(int width, int height)
{
    // Lecture de la configuration des differentes options ...
    wxConfigBase *pConfig = wxConfigBase::Get();
    if (pConfig == NULL)
        return;

    bool loadWholeImage=false, bilinearZoom=false;

    pConfig->Read(wxT("/Options/LoadWoleImage"), &loadWholeImage, true); //TODO
    pConfig->Read(wxT("/Options/BilinearZoom"), &bilinearZoom, false);



    dev3n8_image_t screen_image(width, height);
    dev3n8_view_t screen_view = view(screen_image);

    if(!m_alpha_img) m_alpha_img.reset(new alpha_image_t);
    m_alpha_img->recreate(screen_view.dimensions());
    alpha_image_t::view_t alpha_view = view(*m_alpha_img);
    fill_pixels(alpha_view, 0);

    channel_converter_functor my_cc(IntensityMin(), IntensityMax(), *m_cLUT);
    apply_operation( view(*m_img), screen_image_functor(screen_view, my_cc, m_zoomFactor, m_translationX, m_translationY, alpha_view, m_transparencyMin, m_transparencyMax, m_alpha, IsTransparent()));

    wxImage monImage(screen_view.width(), screen_view.height(), interleaved_view_get_raw_data(screen_view), true);
    monImage.SetAlpha(interleaved_view_get_raw_data(view(*m_alpha_img)), true);

    m_bitmap = boost::shared_ptr<wxBitmap>(new wxBitmap(monImage));



////TODO loadWholeImage


}


void ImageLayer::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const
{
    dc.DrawBitmap(*m_bitmap, x, y, transparent); //-m_translationX+x*m_zoomFactor, -m_translationX+y*m_zoomFactor
}


void ImageLayer::Save(const string &name)
{
    string ext = boost::filesystem::extension(name);
    boost::to_lower(ext);
    if ( ext == ".tiff" || ext == ".tif" )
        tiff_write_view( name.c_str() , view(*m_img) );
    else if ( ext == ".jpeg" || ext == ".jpg" )
        jpeg_write_view( name.c_str() , view(*m_img) );
    else if ( ext == ".png" )
        png_write_view( name.c_str() , view(*m_img) );
}

unsigned int ImageLayer::GetNbComponents() const
{
    return apply_operation(view(*m_img), nb_components_functor());
}

string ImageLayer::GetTypeChannel() const
{
    return apply_operation(view(*m_img), type_channel_functor());
}

void ImageLayer::Histogram(vector< vector<double> > &histo, double &min, double &max) const
{
    min = m_minmaxResult.first;
    max = m_minmaxResult.second;
    histo.clear();
    histo.resize( GetNbComponents() );
    for (unsigned int i=0;i<GetNbComponents();++i)
        histo[i].resize(256);
    apply_operation( view(*m_img), histogram_functor(histo,min,max) );
}

string ImageLayer::GetPixelValue(int i, int j) const
{
    ostringstream oss;
    oss.precision(6);
    oss<<"(";
    i = static_cast<int>(i*m_zoomFactor-m_translationX);
    j = static_cast<int>(j*m_zoomFactor-m_translationY);
    apply_operation(view(*m_img), any_view_image_position_to_string_functor(i,j, oss));
    oss<<")";
    return oss.str();
}

Layer::ptrLayerType ImageLayer::crop(int x0, int y0, int x1, int y1) const
{
    if(x0<0) x0=0;
    if(y0<0) y0=0;
    if(x1>m_img->width ()) x1=m_img->width ();
    if(y1>m_img->height()) y1=m_img->height();
    if(x0>=x1 || y0>=y1) return ptrLayerType();
    image_t::view_t crop_view = subimage_view(view(*m_img), x0, y0, x1-x0, y1-y0 );
    return ptrLayerType(new ImageLayer(apply_operation(crop_view, get_any_image_functor())) );
}
