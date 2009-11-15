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

#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include "boost/gil/extension/numeric/sampler.hpp"
#include "boost/gil/extension/numeric/resample.hpp"
#include <boost/gil/algorithm.hpp>

#include <wx/dc.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/log.h>
#include <wx/config.h>

#include "GilViewer/tools/ColorLookupTable.h"

#include "image_layer_screen_image_functor.hpp"
#include "image_layer_min_max_functor.hpp"
#include "image_layer_get_any_image_functor.hpp"
#include "image_layer_histogram_functor.hpp"
#include "image_layer_to_string_functor.hpp"
#include "image_layer_infos_functor.hpp"

#include "ImageLayer.hpp"

using namespace std;
using namespace boost::gil;

ImageLayer::ImageLayer(const boost::shared_ptr<usable_images_t> &image, const string &name):
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


Layer::ptrLayerType ImageLayer::CreateImageLayer(const boost::shared_ptr<usable_images_t> &image, const string &name)
{
    return boost::shared_ptr<ImageLayer> (new ImageLayer(image, name));
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
        cout << oss.str() << endl;
        wxLogMessage( wxString(oss.str().c_str(), *wxConvCurrent) );
        return boost::shared_ptr<Layer>();
        //throw logic_error( oss.str() );
    }


    const string ext(boost::filesystem::extension(filename));

//	image_read_info< tiff_tag > info = read_image_info(filename, tiff_tag());

    boost::shared_ptr<usable_images_t> image(new usable_images_t);

    if (ext == ".TIF" || ext == ".TIFF" || ext == ".tif" || ext == ".tiff")
        tiff_read_image(filename, *image);
    else if (ext == ".JPG" || ext == ".JPEG" || ext == ".jpg" || ext == ".jpeg")
        jpeg_read_image(filename, *image);
    else if (ext == ".PNG" || ext == ".png")
        png_read_image(filename, *image);


    //Creation de la couche image
    boost::shared_ptr<ImageLayer> maLayer(new ImageLayer(image, filename));


    // Le m_name est sette dans la fonction appelle a la ligne precedente ...
    // Calcul du chemin complet du fichier
    boost::filesystem::path full = boost::filesystem::system_complete(filename);
    maLayer->Filename( full.string() );

    //Lecture de l'ori et test d'existence
    try
    {
        maLayer->m_ori.ReadOriFromImageFile(filename);
        maLayer->m_hasOri = true;
    }
    catch (exception &e) // Putain, ca ca fait chier, ca me fout plein de warnings ('e' n'est pas utilise)!
    {
        ostringstream message;
        message << "No orientation for image " << filename;
        message << "\n" << e.what();
        wxLogMessage( wxString(message.str().c_str(), *wxConvCurrent) );
        maLayer->m_hasOri = false;
    }

    return maLayer;
}


template<typename T>
inline int iRound(const T x)
{
    using namespace std;
    return static_cast<int>(floor(x + 0.5));
}

void ImageLayer::Update(const int width, const int height)
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




    m_canal_alpha.recreate(screen_view.dimensions());
    gray8_view_t alpha_view = view(m_canal_alpha);
    fill_pixels(alpha_view, 0);

    channel_converter_functor my_cc(IntensityMin(), IntensityMax(), *m_cLUT);
    apply_operation( view(*m_img), screen_image_functor(screen_view, my_cc, m_zoomFactor, m_translationX, m_translationY, alpha_view, m_transparencyMin, m_transparencyMax, m_alpha, IsTransparent()));


    wxImage monImage(screen_view.width(), screen_view.height(), interleaved_view_get_raw_data(screen_view), true);
    monImage.SetAlpha(interleaved_view_get_raw_data(view(m_canal_alpha)), true);

    m_bitmap = boost::shared_ptr<wxBitmap>(new wxBitmap(monImage));



////TODO loadWholeImage


}


void ImageLayer::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent)
{
    dc.DrawBitmap(*m_bitmap, x, y, transparent); //-m_translationX+x*m_zoomFactor, -m_translationX+y*m_zoomFactor
}


void ImageLayer::Save(const string &name)
{
    string ext = boost::filesystem::extension(name);
    if ( ext == ".tiff" || ext == ".tif" || ext == ".TIF" || ext == ".TIFF" )
        tiff_write_view( name.c_str() , view(*m_img) );
    else if ( ext == ".jpeg" || ext == ".jpg" || ext == ".JPEG" || ext == ".JPG" )
        jpeg_write_view( name.c_str() , view(*m_img) );
    else if ( ext == ".png" || ext == ".PNG" )
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

string ImageLayer::GetPixelValue(const int i, const int j) const
{
    ostringstream oss;
    oss.precision(6);
    oss<<"(";
    apply_operation(view(*m_img), any_view_image_position_to_string_functor(static_cast<int>(-m_translationX+i*m_zoomFactor), static_cast<int>(-m_translationY+j*m_zoomFactor), oss));
    oss<<")";
    return oss.str();
}

void ImageLayer::crop( int x0 , int y0 , int width , int height , string filename )
{
    usable_views_t crop_view = subimage_view(view(*m_img), x0 , y0 , width , height );

    if ( filename != "" )
    {
        string ext = boost::filesystem::extension(filename);
        if ( ext == ".tiff" || ext == ".tif" || ext == ".TIF" || ext == ".TIFF" )
            tiff_write_view( filename.c_str() , crop_view );
        else if ( ext == ".jpeg" || ext == ".jpg" || ext == ".JPEG" || ext == ".JPG" )
            jpeg_write_view( filename.c_str() , crop_view );
        else if ( ext == ".png" || ext == ".PNG" )
            png_write_view( filename.c_str() , crop_view );
    }

    /*
    	// TODO : only works for gray8 images ...
    	boost::shared_ptr<usable_images_t> any_img = apply_operation( crop_view , get_any_image_functor() );

    	return boost::shared_ptr<ImageLayer> ( new ImageLayer(any_img) );
    	*/
}
