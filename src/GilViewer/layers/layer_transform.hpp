/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets.


Homepage:

    http://code.google.com/p/gilviewer

Copyright:

    Institut Geographique National (2009)

Authors:

        Olivier Tournaire, Adrien Chauve, Mathieu Brédif




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

#ifndef __LAYER_TRANSFORM_HPP__
#define __LAYER_TRANSFORM_HPP__


#include <iostream>
#ifdef WIN32
	#pragma warning(disable : 4251)
	#pragma warning(disable : 4275)
#endif
#include <wx/gdicmn.h>
class layer_transform {
    void rotated_coordinate_to_local(double gx, double gy, double& lx, double& ly) const;
    void rotated_coordinate_from_local(double lx, double ly, double& gx, double& gy) const;

public:
    enum layerOrientation{
        LO_0 = 0,
        LO_90 = 1,
        LO_180 = 2,
        LO_270 = 3
             };

    layer_transform() :
            m_zoomFactor(1.),
            m_translationX(0.), m_translationY(0.),
            m_coordinates(1),
            m_h(0),m_w(0),m_layer_orientation(LO_0)
    {}
    layer_transform(const layer_transform& l):
            m_zoomFactor(l.m_zoomFactor),
            m_translationX(l.m_translationX), m_translationY(l.m_translationY),
            m_coordinates(l.m_coordinates),
            m_h(l.m_h),m_w(l.m_w),m_layer_orientation(l.m_layer_orientation)
    {}
    layer_transform& operator =(const layer_transform& l){
        m_zoomFactor=l.m_zoomFactor;
        m_translationX=l.m_translationX;
        m_translationY=l.m_translationY;
        m_coordinates=l.m_coordinates;
        m_h=l.m_h;
        m_w=l.m_w;
        m_layer_orientation=l.m_layer_orientation;
        return *this;
    }

    void zoom_factor(double zoomFactor) { m_zoomFactor = zoomFactor; }
    inline double zoom_factor() const { return m_zoomFactor; }
    void translation_x(double dx) { m_translationX = dx; }
    inline double translation_x() const { return m_translationX; }
    void translation_y(double dy) { m_translationY = dy; }
    inline double translation_y() const { return m_translationY; }
    inline void coordinates(int c) {m_coordinates=c;}
    inline int coordinates() const { return m_coordinates; }
    void resolution(double r) { m_resolution = r; }
    inline double resolution() const { return m_resolution; }

    inline unsigned int  h() const { return m_h; }
    inline unsigned int  w() const { return m_w; }
    inline layerOrientation orientation() const { return m_layer_orientation; }
    void orientation(layerOrientation ori,unsigned int w,unsigned int h){m_layer_orientation = ori;m_w = w;m_h = h; }
    

    // local<->global transforms. Default: pixel-centered
    // double,double -> double,double transformation
    void from_local(double lx, double ly, double& gx, double& gy) const;
    void to_local  (double gx, double gy, double& lx, double& ly) const;
    void from_local_int(double lx, double ly, double& gx, double& gy, double delta) const;
    void to_local_int  (double gx, double gy, double& lx, double& ly, double delta) const;

    // double,double  -> wxRealPoint transformations
    inline wxRealPoint from_local(double x, double y) const { wxRealPoint q; from_local(x,y,q.x,q.y); return q; }
    inline wxRealPoint   to_local(double x, double y) const { wxRealPoint q;   to_local(x,y,q.x,q.y); return q; }

    // double,double  -> wxPoint transformations
    inline wxPoint from_local_int(double lx, double ly, double delta=0.5) const
    {
        double x,y;
        from_local_int(lx, ly, x, y, delta);
        return wxPoint(static_cast<wxCoord>(x),static_cast<wxCoord>(y));
    }

    inline wxPoint to_local_int(double gx, double gy, double delta=0.5) const
    {
        double x,y;
        to_local_int(gx, gy, x, y, delta);
        return wxPoint(static_cast<wxCoord>(x),static_cast<wxCoord>(y));
    }

    // transformations from a (x,y) template
    template<typename T> inline wxRealPoint from_local(const T& p) const { return from_local(p.x,p.y); }
    template<typename T> inline wxRealPoint   to_local(const T& p) const { return   to_local(p.x,p.y); }
    template<typename T> inline wxPoint from_local_int(const T& p, double delta=0.5) const { return from_local_int(p.x,p.y,delta); }
    template<typename T> inline wxPoint   to_local_int(const T& p, double delta=0.5) const { return   to_local_int(p.x,p.y,delta); }



    void translate(const wxRealPoint &p)
    {
        m_translationX += p.x * m_zoomFactor;
        m_translationY += p.y * m_zoomFactor;
    }

    // zoom_factor is multiplied by z, while keeping from_local(p) fixed
    void zoom(double zoom, double x, double y)
    {
        m_translationX += x * m_zoomFactor * (zoom - 1);
        m_translationY += y * m_zoomFactor * (zoom - 1);
        m_zoomFactor *= zoom;
    }

    void reset(){*this=layer_transform();}

private:

    //gestion de la geometrie
    double m_zoomFactor;
    double m_translationX, m_translationY;
    double m_resolution;

    // 1 --> image; -1 --> cartographic coordinates
    int m_coordinates;
    
    unsigned int m_h;
    unsigned int m_w;
    layerOrientation m_layer_orientation;
};


/// snapping ///

#include "../convenient/wxrealpoint.hpp"
// should be moved to its own header?...
enum eSNAP //snap modes (may be bitwise-combined using the '&' operator)
{
    SNAP_NONE = 0,
    SNAP_GRID = 1,
    SNAP_LINE = 2,
    SNAP_INTERSECTION = 4,
    SNAP_POINT = 8,
    SNAP_ALL = 15,
    SNAP_MAX_ID = SNAP_ALL+1,
};

template<typename P>
bool snap_segment(const layer_transform& trans, double invzoom2, double d2[], const wxRealPoint& q, const P& _p0, const P& _p1, wxRealPoint& psnap)
{
    wxRealPoint p0(_p0.x,_p0.y), p1(_p1.x,_p1.y);
    wxRealPoint u(q -p0);
    wxRealPoint v(p1-p0);
    double t = dot(u,v)/dot(v,v);
    if(t<0 || t> 1) return false;
    wxRealPoint proj(p0+t*v);
    double d = squared_distance(q,proj)*invzoom2;
    if(d >= d2[SNAP_LINE] ) return false;
    for(unsigned int k=0; k<SNAP_LINE; ++k) d2[k]=0;
    d2[SNAP_LINE] = d;
    psnap = trans.from_local(proj);
    return true;

}

template<typename P>
bool snap_point(const layer_transform& trans, double invzoom2, double d2[], const wxRealPoint& q, const P& _p, wxRealPoint& psnap )
{
    wxRealPoint p(_p.x,_p.y);
    double d = squared_distance(q,p)*invzoom2;
    if(d > d2[SNAP_POINT] ) return false;
    for(unsigned int k=0; k<SNAP_POINT; ++k) d2[k]=0;
    d2[SNAP_POINT] = d;
    psnap = trans.from_local(p);
    return true;
}


#endif // __LAYER_TRANSFORM_HPP__
