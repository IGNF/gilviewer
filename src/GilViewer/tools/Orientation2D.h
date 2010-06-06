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

class orientation_2d
{
public:
        orientation_2d();
        orientation_2d(const double origineX, const double origineY, const double step,const unsigned int zoneCarto, const unsigned int tailleX, const unsigned int tailleY);

	///Accesseurs/Setteurs
	double OriginX() const { return m_originX; }
	void OriginX( const double x) { m_originX = x; }
	double OriginY() const { return m_originY; }
	void OriginY( const double y) { m_originY = y; }

	double Step() const { return m_step; }
	void Step( const double s) { m_step = s; }

	unsigned int ZoneCarto() const { return m_zoneCarto; }
	void ZoneCarto( const unsigned int zone) { m_zoneCarto = zone; }

	unsigned int SizeX() const { return m_sizeX; }
	void SizeX( const unsigned int size) { m_sizeX = size; }
	unsigned int SizeY() const { return m_sizeY; }
	void SizeY( const unsigned int size) { m_sizeY = size; }

	///Passage de pixel a image et inversement
	inline void ImageToMap(const int col, const int lig, double &x, double &y) const;
	inline void MapToImage(const double x, const double y, int &col, int &lig) const;

	///IO : renvoit des exceptions si mauvais format ou pbs en lecture
	void ReadOriFromOriFile(const std::string &filename);
	void ReadOriFromTFWFile(const std::string &filename);
	void ReadOriFromImageFile(const std::string &filename);

	void SaveOriToFile(const std::string &filename);

	std::string Affiche() const;

private:

	///Origine en X et Y de la couche 2D
	double m_originX, m_originY;
	///Resolution en X et Y de la couche 2D
	double m_step;

	unsigned int m_zoneCarto;
	unsigned int m_sizeX, m_sizeY;

};

inline void orientation_2d::ImageToMap(const int col, const int lig, double &x, double &y) const
{
	x = m_originX + col * m_step;
	y = m_originY - lig * m_step;
}

inline void orientation_2d::MapToImage(const double x, const double y, int &col, int &lig) const
{
	col = static_cast<int>( std::floor((x - m_originX ) / m_step + 0.5)); //0.5 pour le round
	lig = -static_cast<int>( std::floor((y - m_originY ) / m_step + 0.5));
}

typedef orientation_2d Orientation2D;

#endif /*VIEWERORIENTATION2D_H_*/
