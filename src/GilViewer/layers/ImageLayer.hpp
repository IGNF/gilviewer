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

#ifndef __IMAGE_LAYER_HPP__
#define __IMAGE_LAYER_HPP__

#include <boost/shared_ptr.hpp>

#include "GilViewer/layers/Layer.hpp"

// forward declaration of image types
class image_type;
class view_type;
class alpha_image_type;

class ImageLayer : public Layer
{
	public:
	typedef image_type       image_t;
	typedef view_type        view_t;
	typedef alpha_image_type alpha_image_t;
	typedef boost::shared_ptr<image_t      > image_ptr;
	typedef boost::shared_ptr<view_t       > view_ptr;
	typedef boost::shared_ptr<alpha_image_t> alpha_image_ptr;

	virtual ~ImageLayer() {}

	static ptrLayerType CreateImageLayer(const std::string &fileName);
	static ptrLayerType CreateImageLayer(const image_ptr &image, const std::string &name ="Image Layer");

	///ATTENTION ici l'image est recopiée dans une any_image !!
	template<class ImageType>
	static ptrLayerType CreateImageLayer(const ImageType &image, const std::string &name ="Image Layer");

	virtual void Update(int width, int height);
	virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const;

	virtual unsigned int GetNbComponents() const ;
	std::string GetTypeChannel() const;
	virtual void Histogram(std::vector< std::vector<double> > &histo, double &min, double &max) const;
	virtual std::string GetPixelValue(int i, int j) const;

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
	virtual void SetChannels(unsigned int red, unsigned int green, unsigned int blue)
	{
		m_red   = red;
		m_green = green;
		m_blue  = blue;
	}
	virtual void GetChannels(unsigned int &red, unsigned int &green, unsigned int &blue) const
	{
		red   = m_red;
		green = m_green;
		blue  = m_blue;
	}
	virtual void SetAlphaChannel(bool useAlphaChannel, unsigned int alphaChannel)
	{
		m_useAlphaChannel = useAlphaChannel;
		m_alphaChannel = alphaChannel;
	}
	virtual void GetAlphaChannel(bool &useAlphaChannel, unsigned int &alphaChannel) const
	{
		useAlphaChannel = m_useAlphaChannel;
		alphaChannel = m_alphaChannel;
	}

	virtual void Alpha(unsigned char alpha) { m_alpha=alpha; }
	virtual inline unsigned char Alpha() const { return m_alpha; }
	virtual void IntensityMin(double intensity) { m_intensityMin=intensity; }
	virtual double IntensityMin() const { return m_intensityMin; }
	virtual void IntensityMax(double intensity) { m_intensityMax=intensity; }
	virtual double IntensityMax() const { return m_intensityMax; }
	virtual void Gamma(double gamma) { m_gamma=gamma; }
	virtual double Gamma() const { return m_gamma; }
	virtual void TransparencyMin(double t) { m_transparencyMin=t; }
	virtual double TransparencyMin() const { return m_transparencyMin; }
	virtual void TransparencyMax(double t) { m_transparencyMax=t; }
	virtual double TransparencyMax() const { return m_transparencyMax; }
	virtual void IsTransparent(bool t) { m_isTransparent=t; }
	virtual bool IsTransparent() const { return m_isTransparent; }


	virtual void Save(const std::string &name);
	virtual ptrLayerType crop(int& x0, int& y0, int& x1, int& y1) const;

	private:
	ImageLayer(const image_ptr &image, const std::string &name ="Image Layer", const view_ptr& view=view_ptr() );

	image_ptr       m_img;
	view_ptr        m_view;
	alpha_image_ptr m_alpha_img;

	int m_startInput[2];
	double m_startfInput[2];
	int m_sizeInput[2];
	double m_dx, m_dy;

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

	double m_oldZoomFactor;
};

#endif // __IMAGE_LAYER_HPP__
