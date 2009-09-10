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

#ifndef __IMAGE_LAYER_HPP__
#define __IMAGE_LAYER_HPP__

#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/gil/extension/matis/float_images.hpp>
#include <boost/gil/extension/io/tiff_dynamic_io.hpp>

#include "layers/Layer.hpp"

using namespace boost::gil;

class ImageLayer : public Layer
{
	public:
	//GIL   :  gray8_image_t, gray8s_image_t, gray16_image_t, gray16s_image_t, gray32_image_t, gray32s_image_t,
        typedef any_image< boost::mpl::vector<rgb8_image_t, gray8_image_t , rgb16_image_t, gray16_image_t , rgb32_image_t, gray32_image_t, gray32F_image_t , gray64F_image_t> > usable_images_t;
	typedef usable_images_t::view_t usable_views_t;


		virtual ~ImageLayer() {}


		static ptrLayerType CreateImageLayer(const std::string &fileName);
		static ptrLayerType CreateImageLayer(const boost::shared_ptr<usable_images_t> &image, const std::string &name ="Image Layer");

		///ATTENTION ici l'image est recopiée dans une any_image !!
		template<class ImageType>
		static ptrLayerType CreateImageLayer(const ImageType &image, const std::string &name ="Image Layer")
		{
			boost::shared_ptr<usable_images_t> any_img(new usable_images_t(image));
			return boost::shared_ptr<ImageLayer> (new ImageLayer(any_img, name));
		}

		virtual void Update(const int width, const int height);
		virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent);

		virtual unsigned int GetNbComponents() const ;
		std::string GetTypeChannel() const;
		virtual void Histogram(std::vector< std::vector<double> > &histo, double &min, double &max) const;
		virtual std::string GetPixelValue(const int i, const int j) const;

		virtual boost::shared_ptr<ColorLookupTable> GetColorLookupTable() { return m_cLUT; }

		virtual void Orientation(const Orientation2D &orientation)
		{
			m_ori.OriginX( orientation.OriginX() );
			m_ori.OriginY( orientation.OriginY() );
			m_ori.Step( orientation.Step() );
			m_ori.ZoneCarto( orientation.ZoneCarto() );
			m_ori.SizeX( orientation.SizeX() );
			m_ori.SizeY( orientation.SizeY() );
		}
		virtual void SetChannels(const unsigned int red, const unsigned int green, const unsigned int blue)
		{
			m_red = red;
			m_green = green;
			m_blue = blue;
		}
		virtual void GetChannels(unsigned int &red, unsigned int &green, unsigned int &blue)
		{
			red = m_red;
			green = m_green;
			blue = m_blue;
		}
		virtual void SetAlphaChannel(const bool useAlphaChannel, const unsigned int alphaChannel)
		{
			m_useAlphaChannel = useAlphaChannel;
			m_alphaChannel = alphaChannel;
		}
		virtual void GetAlphaChannel(bool &useAlphaChannel, unsigned int &alphaChannel)
		{
			useAlphaChannel = m_useAlphaChannel;
			alphaChannel = m_alphaChannel;
		}

		virtual void Alpha(const unsigned char alpha) { m_alpha=alpha; }
		virtual inline unsigned char Alpha() const { return m_alpha; }
		virtual void IntensityMin(const double intensity) { m_intensityMin=intensity; }
		virtual double IntensityMin() const { return m_intensityMin; }
		virtual void IntensityMax(const double intensity) { m_intensityMax=intensity; }
		virtual double IntensityMax() const { return m_intensityMax; }
		virtual void Gamma(const double gamma) { m_gamma=gamma; }
		virtual double Gamma() const { return m_gamma; }
		virtual void TransparencyMin(const double t) { m_transparencyMin=t; }
		virtual double TransparencyMin() const { return m_transparencyMin; }
		virtual void TransparencyMax(const double t) { m_transparencyMax=t; }
		virtual double TransparencyMax() const { return m_transparencyMax; }
		virtual void IsTransparent(const bool t) { m_isTransparent=t; }
		virtual bool IsTransparent() const { return m_isTransparent; }


		virtual void Save(const std::string &name);
		virtual ptrLayerType crop( int x0 , int y0 , int width , int height );

	private:
		ImageLayer(const boost::shared_ptr<usable_images_t> &image, const std::string &name ="Image Layer");

		boost::shared_ptr<usable_images_t> m_img;
		gray8_image_t m_canal_alpha;

		int m_startInput[2];
		float m_startfInput[2];
		int m_sizeInput[2];
		float m_dx, m_dy;

		std::pair<float, float> m_minmaxResult;

		boost::shared_ptr<wxBitmap> m_bitmap;
		unsigned int m_red, m_green, m_blue;
		bool m_useAlphaChannel;
		unsigned int m_alphaChannel;
		//CLUT
		boost::shared_ptr<ColorLookupTable> m_cLUT;

		unsigned char m_alpha;
		double m_intensityMin;
		double m_intensityMax;

		double m_transparencyMin;
		double m_transparencyMax;
		bool m_isTransparent;

		double m_gamma;

		float m_oldZoomFactor;
};


#endif // __IMAGE_LAYER_HPP__
