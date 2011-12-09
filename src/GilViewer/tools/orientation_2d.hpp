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

#ifndef VIEWERORIENTATION2D_H_
#define VIEWERORIENTATION2D_H_

/*
 * \class Orientation2D
 * \brief Classe de gestion des orientations 2D en géométrie ortho
 *
 * Classe qui lit et écrit les fichiers ORI, et passe de l'indice de pixel aux coordonnées X,Y.
 *
 */

#include <cmath>
#include <sstream>

class orientation_2d
{
public:
    orientation_2d() : m_originX(0), m_originY(0), m_step(1), m_zoneCarto(0), m_sizeX(1), m_sizeY(1) {}
    orientation_2d(const double origineX, const double origineY, const double step,const unsigned int zoneCarto, const unsigned int tailleX, const unsigned int tailleY) : m_originX(origineX), m_originY(origineY), m_step(step), m_zoneCarto(zoneCarto), m_sizeX(tailleX), m_sizeY(tailleY) {}

    /** @name Accessors
      */
    //@{
    double origin_x() const { return m_originX; }
    void origin_x( const double x) { m_originX = x; }
    double origin_y() const { return m_originY; }
    void origin_y( const double y) { m_originY = y; }
    double step() const { return m_step; }
    void step( const double s) { m_step = s; }
    unsigned int zone_carto() const { return m_zoneCarto; }
    void zone_carto( const unsigned int zone) { m_zoneCarto = zone; }
    unsigned int size_x() const { return m_sizeX; }
    void size_x( const unsigned int size) { m_sizeX = size; }
    unsigned int size_y() const { return m_sizeY; }
    void size_y( const unsigned int size) { m_sizeY = size; }
    //@}

    /** @name Coordinates transformations
      */
    //@{
    /// Transform an image position to a cartographic position
    inline void image_to_map(const int col, const int lig, double &x, double &y) const;
    /// Transform a cartographic position to an image position
    inline void map_to_image(const double x, const double y, int &col, int &lig) const;
    //@}


    /** @name Image orientation readers
      */
    //@{
    /// Reads an image orientation from a .ori file
    bool read_ori_from_ori_file(const std::string &filename);
    /// Reads an image orientation from a .hdr file
    bool read_ori_from_hdr_file(const std::string &filename);
    /// Reads an image orientation from a .tfw file
    bool read_ori_from_tfw_file(const std::string &filename);
    /// @brief Reads an image orientation associated to an image file (.ori or .tfw)
    /// @param filename The full image path. The orientation file name must be the same (with a .ori or .tfw extension)
    bool read_ori_from_image_file(const std::string &filename);
    //@}

private:
    /// X cartographic origin
    double m_originX;
    /// Y cartographic origin
    double m_originY;
    /// Step
    double m_step;
    /// Cartographic area (for Lambert projections)
    unsigned int m_zoneCarto;
    /// Image width
    unsigned int m_sizeX;
    /// Image height
    unsigned int m_sizeY;

    /// Dumps an orientation_2d in an output stream
    friend std::ostream& operator<< (std::ostream &o, const orientation_2d &ori);
};

std::ostream& operator<< (std::ostream &o, const orientation_2d &ori);

inline void orientation_2d::image_to_map(const int col, const int lig, double &x, double &y) const
{
    x = m_originX + col * m_step;
    y = m_originY - lig * m_step;
}

inline void orientation_2d::map_to_image(const double x, const double y, int &col, int &lig) const
{
    col = static_cast<int>( std::floor((x - m_originX ) / m_step + 0.5)); // 0.5 pour le round
    lig = -static_cast<int>( std::floor((y - m_originY ) / m_step + 0.5));
}

#endif /*VIEWERORIENTATION2D_H_*/
