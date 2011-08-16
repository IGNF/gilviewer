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
#include <boost/shared_array.hpp>

#include "../layers/layer.hpp"

class orientation_2d;
class color_lookup_table;

// forward declaration of image types
struct image_type;
struct view_type;
struct variant_view_type;
class alpha_image_type;




class image_layer : public layer
{
public:


    typedef image_type       image_t;
    typedef variant_view_type variant_view_t;
    typedef alpha_image_type alpha_image_t;
    typedef boost::shared_ptr<image_t      > image_ptr;
    typedef boost::shared_ptr<variant_view_t       > variant_view_ptr;
    typedef boost::shared_ptr<alpha_image_t> alpha_image_ptr;

    //image_layer(const image_ptr &image, const std::string &name ="Image Layer", const std::string& filename="", const view_ptr& view=view_ptr() );
    image_layer(const image_ptr &image, const std::string &name ="Image Layer", const std::string& filename="", const variant_view_ptr& variant_view=variant_view_ptr() );
    image_layer(const variant_view_ptr &variant_view_, const std::string &name_, const std::string &filename_="");
    virtual ~image_layer() {}
/*
    // local<->global transforms. Default: pixel-centered
    virtual wxRealPoint from_local(const wxRealPoint &p) const
    {
        //wxPoint res=rotated_coordinate(p);
        //wxRealPoint res=rotated_coordinate_from_local(p);
        return transform().from_local(p);
    }

    virtual wxRealPoint to_local(const wxRealPoint &p) const
    {
        //wxRealPoint res=transform().to_local(p);
        //return rotated_coordinate_to_local(p);
        return transform().to_local(p);
    }
*/

protected:
    void init();


public:
    static ptrLayerType create_image_layer(const image_ptr &image, const std::string &name ="Image Layer");
    static ptrLayerType create_image_layer(const variant_view_ptr &variant_view_, const std::string &name);

    virtual void update(int width, int height);
    virtual void draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const;

    virtual unsigned int nb_components() const ;
    std::string type_channel() const;
    virtual boost::shared_ptr<const histogram_type> histogram(double &min, double &max) const;
    virtual std::string pixel_value(const wxRealPoint& p) const;

    virtual boost::shared_ptr<color_lookup_table> colorlookuptable();

    virtual void orientation(const boost::shared_ptr<orientation_2d> &orientation);

    virtual bool snap( eSNAP snap, double d2[], const wxRealPoint& p, wxRealPoint& psnap );

    virtual void channels(unsigned int red, unsigned int green, unsigned int blue)
    {
        m_red   = red;
        m_green = green;
        m_blue  = blue;
    }
    virtual void channels(unsigned int &red, unsigned int &green, unsigned int &blue) const
    {
        red   = m_red;
        green = m_green;
        blue  = m_blue;
    }
    virtual void alpha_channel(bool useAlphaChannel, unsigned int alphaChannel)
    {
        m_useAlphaChannel = useAlphaChannel;
        m_alphaChannel = alphaChannel;
    }
    virtual void alpha_channel(bool &useAlphaChannel, unsigned int &alphaChannel) const
    {
        useAlphaChannel = m_useAlphaChannel;
        alphaChannel = m_alphaChannel;
    }

    virtual void alpha(unsigned char alpha) { m_alpha=alpha; }
    virtual inline unsigned char alpha() const { return m_alpha; }
    virtual void intensity_min(double intensity) { m_intensityMin=intensity; }
    virtual double intensity_min() const { return m_intensityMin; }
    virtual void intensity_max(double intensity) { m_intensityMax=intensity; }
    virtual double intensity_max() const { return m_intensityMax; }
    virtual void gamma(double gamma);
    virtual double gamma() const { return m_gamma; }
    virtual void transparency_min(double t) { m_transparencyMin=t; }
    virtual double transparency_min() const { return m_transparencyMin; }
    virtual void transparency_max(double t) { m_transparencyMax=t; }
    virtual double transparency_max() const { return m_transparencyMax; }
    virtual void transparent(bool t) { m_isTransparent=t; }
    virtual bool transparent() const { return m_isTransparent; }

    virtual ptrLayerType crop_local(const wxRealPoint& p0, const wxRealPoint& p1) const;

    virtual image_ptr image() const { return m_img; }
    virtual variant_view_ptr  variant_view() const { return m_variant_view; }

    virtual std::vector<std::string> available_formats_extensions() const;
    virtual std::string available_formats_wildcard() const;
    virtual bool saveable() const {return true;}
    virtual std::string get_layer_type_as_string() const {return "Image";}

    virtual layer_settings_control* build_layer_settings_control(unsigned int index, layer_control* parent);

    inline virtual double center_x();
    inline virtual double center_y();
    
    unsigned int width() const  ;
    unsigned int height() const ;
    
        protected:

    image_ptr       m_img;
    variant_view_ptr        m_variant_view;
    alpha_image_ptr m_alpha_img;

    double m_dx, m_dy;

    std::pair<float, float> m_minmaxResult;

    boost::shared_ptr<wxBitmap> m_bitmap;
    unsigned int m_red, m_green, m_blue;
    bool m_useAlphaChannel;
    unsigned int m_alphaChannel;
    //CLUT
    boost::shared_ptr<color_lookup_table> m_cLUT;

    unsigned char m_alpha;
    double m_intensityMin;
    double m_intensityMax;

    double m_transparencyMin;
    double m_transparencyMax;
    bool m_isTransparent;



    double m_gamma;

    double m_oldZoomFactor;

    boost::shared_array<float> m_gamma_array;
    static unsigned int m_gamma_array_size;
    
/*
    wxRealPoint rotated_coordinate_to_local(const wxRealPoint& pt)const;
    wxRealPoint rotated_coordinate_from_local(const wxRealPoint& pt)const;
*/
};

#endif // __IMAGE_LAYER_HPP__
