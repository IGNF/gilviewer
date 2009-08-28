/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets. GilViewer is distributed under the CeCILL-B licence. 
See Licence_CeCILL-B_V1-en.txt or http://www.cecill.info for more details.


Homepage: 

	http://launchpad.net/gilviewer
	
Copyright:
	
	Institut Geographique National (2009)

Authors: 

	Olivier Tournaire, Adrien Chauve

	
	

This software is governed by the CeCILL-B license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL-B
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-B license and that you accept its terms.
 
***********************************************************************/

#include <iostream>
#include <sstream>
#include <limits>
#include <utility>

#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
#include <boost/gil/extension/io/tiff_dynamic_io.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <boost/gil/extension/numeric/sampler.hpp>
#include <boost/gil/extension/numeric/resample.hpp>
#include <boost/gil/algorithm.hpp>

#include <wx/dc.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/log.h>
#include <wx/config.h>

#include "tools/ColorLookupTable.h"

#include "ImageLayer.hpp"

#include "image_layer_channel_converter_functor.hpp"
#include "image_layer_min_max_functor.hpp"
#include "image_layer_get_any_image_functor.hpp"
#include "image_layer_histogram_functor.hpp"
#include "image_layer_transparency_functor.hpp"
#include "image_layer_to_string_functor.hpp"
#include "image_layer_infos_functor.hpp"

ImageLayer::ImageLayer(const boost::shared_ptr<usable_images_t> &image, const std::string &name):
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

	std::ostringstream oss;
	oss << "Dimensions : " << m_img->width()  << " x " << m_img->height() << "\n";
	oss << "Nb canaux : " << GetNbComponents() << "\n";
	oss << "Type pixels : " << GetTypeChannel() << "\n";

	m_infos += oss.str();

	SetChannels(0,1,2);
	SetAlphaChannel(false,0);
}


Layer::ptrLayerType ImageLayer::CreateImageLayer(const boost::shared_ptr<usable_images_t> &image, const std::string &name)
{
	return boost::shared_ptr<ImageLayer> (new ImageLayer(image, name));
}


Layer::ptrLayerType ImageLayer::CreateImageLayer(const std::string &filename)
{
	if ( !boost::filesystem::exists(filename) )
	{
		std::ostringstream oss;
		oss << "Le fichier image demande n'existe pas : "<<filename<< " ! " << std::endl;
		oss << "File : " <<__FILE__ << std::endl;
		oss << "Line : " << __LINE__ << std::endl;
		oss << "Function : " << __FUNCTION__ << std::endl;
		std::cout << oss.str() << std::endl;
		wxLogMessage( wxString(oss.str().c_str(), *wxConvCurrent) );
		return boost::shared_ptr<Layer>();
		//throw std::logic_error( oss.str() );
	}


	const std::string ext(boost::filesystem::extension(filename));

//	image_read_info< tiff_tag > info = read_image_info(filename, tiff_tag());

	boost::shared_ptr<usable_images_t> image(new usable_images_t);

	if(ext == ".TIF" || ext == ".TIFF" || ext == ".tif" || ext == ".tiff")
		tiff_read_image(filename, *image);
        else if(ext == ".JPG" || ext == ".JPEG" || ext == ".jpg" || ext == ".jpeg")
		jpeg_read_image(filename, *image);
        else if(ext == ".PNG" || ext == ".png")
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
	catch(std::exception &e) // Putain, ca ca fait chier, ca me fout plein de warnings ('e' n'est pas utilise)!
	{
        std::ostringstream message;
        message << "Pas d'orientation pour l'image " << filename;
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
	//TODO
//	monImage.SetAlpha(m_visuITK->GetAlphaMask(), true);

	// Lecture de la configuration des differentes options ...
	wxConfigBase *pConfig = wxConfigBase::Get();
	if (pConfig == NULL)
		return;

	bool loadWholeImage=false, bilinearZoom=false;

	pConfig->Read(_("/Options/LoadWoleImage"), &loadWholeImage, true); //TODO
	pConfig->Read(_("/Options/BilinearZoom"), &bilinearZoom, false);


	if(!loadWholeImage)
	{

		m_startInput[0] = std::max(0, -static_cast<int>(m_translationX));
		m_startInput[1] = std::max(0, -static_cast<int>(m_translationY));
		m_startInput[0] = std::min(m_startInput[0], static_cast<int>(view(*m_img).width()));
		m_startInput[1] = std::min(m_startInput[1], static_cast<int>(view(*m_img).height()));

		m_sizeInput[0] = std::ceil(m_zoomFactor * width)+1;// - m_translationX - m_startInput[0];
		m_sizeInput[1] = std::ceil(m_zoomFactor * height)+1;// - m_translationY - m_startInput[1];

	//	m_sizeInput[0] = std::max(m_sizeInput[0], 0);
	//	m_sizeInput[1] = std::max(m_sizeInput[1], 0);

		m_sizeInput[0] = std::min(m_sizeInput[0], static_cast<int>(view(*m_img).width()) - m_startInput[0]);
		m_sizeInput[1] = std::min(m_sizeInput[1], static_cast<int>(view(*m_img).height()) - m_startInput[1]);

		m_sizeInput[0] = std::min(m_sizeInput[0], static_cast<int>(view(*m_img).width()));
		m_sizeInput[1] = std::min(m_sizeInput[1], static_cast<int>(view(*m_img).height()));



	//	std::cout << "Region : \n";
	//	std::cout << "\tHG : (" << m_startInput[0] << "," << m_startInput[1] << ")\n";
	//	std::cout << "\ttaille : (" << m_sizeInput[0] << "," << m_sizeInput[1] << ")" << std::endl;
	//	std::cout << "\técran : (" << width << "," << height << ")" << std::endl;
	//	std::cout << "\ttranslation : (" << m_translationX << "," << m_translationY << ")" << std::endl;
	//	std::cout << "\tzoom : " << m_zoomFactor << std::endl;

	//	read_image( m_filename, m_img, point_t(m_startInput[0], m_startInput[1]), point_t(m_sizeInput[0], m_sizeInput[1]), tiff_tag() );

		usable_views_t geometric_view = subimage_view(view(*m_img), m_startInput[0], m_startInput[1], m_sizeInput[0], m_sizeInput[1]);

	//	usable_views_t geometric_view = view(m_img);

	//	std::cout << "min,max :" << IntensityMin() << ", " << IntensityMax() << std::endl;

	//	rgb8_image_t visu_img(view(m_img).dimensions());
		channel_converter_functor my_cc(IntensityMin(), IntensityMax(), *m_cLUT);
	//	rgb8_image_t visu_img(geometric_view.dimensions());
	//	rgb8_view_t color_view =  color_converted_view<rgb8_pixel_t>(geometric_view, my_cc);


	//	copy_and_convert_pixels(geometric_view, visu_img._view, my_cc);

		dev3n8_image_t visu_zoom(geometric_view.width()/m_zoomFactor, geometric_view.height()/m_zoomFactor);

		if(bilinearZoom)
			resize_view(any_color_converted_view<dev3n8_pixel_t>(geometric_view, my_cc), view(visu_zoom),  bilinear_sampler());
		else
			resize_view(any_color_converted_view<dev3n8_pixel_t>(geometric_view, my_cc), view(visu_zoom),  nearest_neighbor_sampler());

		wxImage monImage(visu_zoom.width(), visu_zoom.height(), interleaved_view_get_raw_data(view(visu_zoom)), true);

		///Transparence
		if(IsTransparent() || m_alpha<255)
		{
			m_canal_alpha.recreate(visu_zoom.dimensions());

			if (IsTransparent()) //pour l'instant on ne traite que le range de transparence, pas le canal d'alpha (à gérer avec les images n canaux)
				transform_pixels(const_view(visu_zoom), view(m_canal_alpha), apply_transparency_functor(m_transparencyMin, m_transparencyMax, m_alpha));
			else if(m_alpha<255)
				fill_pixels(view(m_canal_alpha), gray8_pixel_t(m_alpha));

			monImage.SetAlpha(interleaved_view_get_raw_data(view(m_canal_alpha)), true);

		}

		m_bitmap = boost::shared_ptr<wxBitmap>(new wxBitmap(monImage));

	}

	else
	{

		///////////////////////////////////////////NEW lent mais OK pour la superposition vector/raster

		channel_converter_functor my_cc(IntensityMin(), IntensityMax(), *m_cLUT);

		m_startInput[0]=0;
		m_startInput[1]=0;

		if(m_zoomFactor==m_oldZoomFactor)
			return;
		else
			m_oldZoomFactor = m_zoomFactor;

		try
		{
			usable_views_t geometric_view = view(*m_img);

			dev3n8_image_t visu_zoom( geometric_view.width()/m_zoomFactor, geometric_view.height()/m_zoomFactor);

			resize_view(any_color_converted_view<dev3n8_pixel_t>(geometric_view, my_cc), view(visu_zoom),  nearest_neighbor_sampler());
			wxImage monImage(visu_zoom.width(), visu_zoom.height(), interleaved_view_get_raw_data(view(visu_zoom)), true);
			////////////////////////////////////////////////


			///Transparence
			if(IsTransparent() || m_alpha<255)
			{
				m_canal_alpha.recreate(visu_zoom.dimensions());

				if (IsTransparent()) //pour l'instant on ne traite que le range de transparence, pas le canal d'alpha (à gérer avec les images n canaux)
					transform_pixels(const_view(visu_zoom), view(m_canal_alpha), apply_transparency_functor(m_transparencyMin, m_transparencyMax, m_alpha));
				else if(m_alpha<255)
					fill_pixels(view(m_canal_alpha), gray8_pixel_t(m_alpha));

				monImage.SetAlpha(interleaved_view_get_raw_data(view(m_canal_alpha)), true);

			}

			m_bitmap = boost::shared_ptr<wxBitmap>(new wxBitmap(monImage));

		}
		catch(...)
		{

		}
	}

}


void ImageLayer::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent)
{
	dc.DrawBitmap(*m_bitmap, (m_translationX+m_startInput[0])/m_zoomFactor, (m_translationY+m_startInput[1])/m_zoomFactor, transparent); //-m_translationX+x*m_zoomFactor, -m_translationX+y*m_zoomFactor
}


void ImageLayer::Save(const std::string &name)
{

}

unsigned int ImageLayer::GetNbComponents() const
{
	return apply_operation(const_view(*m_img), nb_components_functor());
}

std::string ImageLayer::GetTypeChannel() const
{
	return apply_operation(const_view(*m_img), type_channel_functor());
}

void ImageLayer::Histogram(std::vector< std::vector<double> > &histo, double &min, double &max) const
{
	min = m_minmaxResult.first;
	max = m_minmaxResult.second;
	histo.clear();
	histo.resize( GetNbComponents() );
	for (unsigned int i=0;i<GetNbComponents();++i)
		histo[i].resize(256);
	apply_operation( const_view(*m_img), histogram_functor(histo,min,max) );
}

template<typename ViewT, typename CoordT>
inline bool isInside(const ViewT& src, const CoordT i, const CoordT j)
{
	return i>=0 && j>=0 && i<src.width() && j<src.height();
}



std::string ImageLayer::GetPixelValue(const int i, const int j) const
{
	std::ostringstream oss;
	oss.precision(6);
	oss<<"(";
	apply_operation(const_view(*m_img), any_view_to_string(static_cast<int>(-m_translationX+i*m_zoomFactor), static_cast<int>(-m_translationY+j*m_zoomFactor), oss));
	oss<<")";
	return oss.str();
}

ImageLayer::ptrLayerType ImageLayer::crop( int x0 , int y0 , int width , int height )
{
	usable_views_t crop_view = subimage_view(view(*m_img), x0 , y0 , width , height );
	tiff_write_view( "test.tif" , crop_view );
	boost::shared_ptr<usable_images_t> any_img = apply_operation( crop_view , get_any_image_functor() );

	std::cout << "iuyiuyiuy" << std::endl;

	return boost::shared_ptr<ImageLayer> ( new ImageLayer(any_img) );
}
