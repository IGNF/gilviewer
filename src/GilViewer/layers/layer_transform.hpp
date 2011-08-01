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

/*
wxPoint vector_layer_ghost::from_local(const wxPoint &p, double delta) const
{
    return wxPoint(
            wxCoord((p.x +m_translationX+delta)/m_zoomFactor),
            wxCoord((p.y +m_translationY+delta)/m_zoomFactor));
}

wxPoint vector_layer_ghost::to_local(const wxPoint &p, double delta) const
{
    return wxPoint(
            wxCoord(m_zoomFactor*p.x -m_translationX+0.5-delta),
            wxCoord(m_zoomFactor*p.y -m_translationY+0.5-delta));
}

wxPoint cgal_vector_layer::from_local(const layer_transform& trans, double x, double y, int coordinates = 1 )//IMAGE_COORDINATES
{
    double delta = trans.resolution()*0.5;
    return wxPoint( static_cast<wxCoord>((delta+            x+trans.translation_x())/trans.zoom_factor()),
                    static_cast<wxCoord>((delta+coordinates*y+trans.translation_y())/trans.zoom_factor()) );
}
wxPoint ogr_vector_layer::from_local(const layer_transform& trans, double x, double y, int coordinates = 1 )//IMAGE_COORDINATES
{
        double delta = 0.5*trans.resolution();
        return wxPoint(
                static_cast<wxCoord>((delta+            x+trans.translation_x())/trans.zoom_factor()),
                static_cast<wxCoord>((delta+coordinates*y+trans.translation_y())/trans.zoom_factor())
        );
}

wxPoint simple_vector_layer::from_local(const layer_transform& trans, double x, double y, int coordinates) const {
      double delta = 0.5*trans.resolution();
        return wxPoint(
                static_cast<wxCoord>((delta+            x+trans.translation_x())/trans.zoom_factor()),
                static_cast<wxCoord>((delta+coordinates*y+trans.translation_y())/trans.zoom_factor())
        );
    }

*/


class layer_transform {
public:
    layer_transform() :
            m_zoomFactor(1.),
            m_translationX(0.), m_translationY(0.),
            m_coordinates(1)
    {}
    layer_transform(const layer_transform& l):
            m_zoomFactor(l.m_zoomFactor),
            m_translationX(l.m_translationX), m_translationY(l.m_translationY),
            m_coordinates(l.m_coordinates)
    {}
    layer_transform& operator =(const layer_transform& l){
            m_zoomFactor=l.m_zoomFactor;
            m_translationX=l.m_translationX;m_translationY=l.m_translationY;
            m_coordinates=l.m_coordinates;
            return *this;
    }

    void zoom_factor(double zoomFactor) { m_zoomFactor = zoomFactor; }
    inline double zoom_factor() const { return m_zoomFactor; }
    void translation_x(double dx) { m_translationX = dx; }
    inline double translation_x() const { return m_translationX; }
    void translation_y(double dy) { m_translationY = dy; }
    inline double translation_y() const { return m_translationY; }

    // local<->global transforms. Default: pixel-centered
    inline wxPoint from_local(const wxPoint &p, double delta=0.5) const
    {
        return from_local(p.x,p.y,delta);
    }

    wxPoint from_local(double x, double y, double delta=0.5) const
    {
        return wxPoint(
                wxCoord((              x +m_translationX+delta)/m_zoomFactor),
                wxCoord((m_coordinates*y +m_translationY+delta)/m_zoomFactor));
    }

    wxPoint to_local(const wxPoint &p, double delta=0.5) const
    {
        return wxPoint(
                wxCoord(m_zoomFactor*p.x -m_translationX+0.5-delta),
                wxCoord(m_coordinates*(m_zoomFactor*p.y -m_translationY-delta)+0.5));
    }


    void resolution(double r) { m_resolution = r; }
    inline double resolution() const { return m_resolution; }

    void translate(const wxPoint &p)
    {
        m_translationX += p.x * m_zoomFactor;
        m_translationY += p.y * m_zoomFactor;
    }

    void zoom(double z, const wxPoint &p)
    {
        m_translationX += p.x * m_zoomFactor * (z - 1);
        m_translationY += p.y * m_zoomFactor * (z - 1);
        m_zoomFactor *= z;
    }

    inline void coordinates(int c) {m_coordinates=c;}

private:

    //gestion de la geometrie
    double m_zoomFactor;
    double m_translationX, m_translationY;
    double m_resolution;

    // 1 --> image; -1 --> cartographic coordinates
    int m_coordinates;
};

#endif // __LAYER_TRANSFORM_HPP__
