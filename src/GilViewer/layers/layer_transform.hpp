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

class layer_transform {
public:
    layer_transform() :
            m_zoomFactor(1.),
            m_translationX(0.), m_translationY(0.),
            m_coordinates(1)
    {}
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

    // local<->global transforms. Default: pixel-centered
    // double,double -> double,double transformation
    inline void from_local(double lx, double ly, double& gx, double& gy) const
    {
        gx = (              lx +m_translationX)/m_zoomFactor;
        gy = (m_coordinates*ly +m_translationY)/m_zoomFactor;
    }

    inline void to_local(double gx, double gy, double& lx, double& ly) const
    {
        lx =                m_zoomFactor*gx -m_translationX;
        ly = m_coordinates*(m_zoomFactor*gy -m_translationY); // should mathematically be a division by m_coordinates, but since it is either 1 or -1, multiplication is fine
    }

    inline void from_local_int(double lx, double ly, double& gx, double& gy, double delta) const
    {
        gx = (              lx +m_translationX+delta-0.5)/m_zoomFactor;
        gy = (m_coordinates*ly +m_translationY+delta-0.5)/m_zoomFactor;
    }
    inline void to_local_int(double gx, double gy, double& lx, double& ly, double delta) const
    {
        lx =                m_zoomFactor*gx -m_translationX-delta;
        ly = m_coordinates*(m_zoomFactor*gy -m_translationY-delta); // should mathematically be a division by m_coordinates, but since it is either 1 or -1, multiplication is fine
    }

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


private:

    //gestion de la geometrie
    double m_zoomFactor;
    double m_translationX, m_translationY;
    double m_resolution;

    // 1 --> image; -1 --> cartographic coordinates
    int m_coordinates;
};

#endif // __LAYER_TRANSFORM_HPP__
