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

#ifndef __VECTOR_LAYER_CONTENT_HPP__
#define __VECTOR_LAYER_CONTENT_HPP__

#ifdef UNIX
#include "libshp/shapefil.h"
#else
#include "shapelib/shapefil.h"
#endif

#include <string>

class wxDC;
class wxPoint;

/*
* \class VectorLayerContent
* \brief Classe abstraite de geometrie de calque vectoriel
*
* Cette classe permet de stocker le contenu d'un calque vectoriel
*/

enum
{
	CARTOGRAPHIC_COORDINATES	=	-1,
	IMAGE_COORDINATES			=	1
};


class VectorLayerContent
{
public:
	VectorLayerContent();
    virtual ~VectorLayerContent() {};

    virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const double zoomFactor, const double translationX, const double translationY, const double resolution) const = 0;

    inline SHPHandle GetHandle() const { return m_SHPHandle; }
    void SetHandle( const SHPHandle &handle ) { m_SHPHandle = handle; }

    void ShapefileFileName( const std::string &name ) { m_shapefileFileName = name; }
    const std::string& ShapefileFileName() const { return m_shapefileFileName; }

	signed short FlagPRJ() const { return m_flagPRJ; }
	void FlagPRJ( signed short flagPRJ );

	void FlagDBF( bool flagDBF ) { m_flagDBF = flagDBF; }
	bool FlagDBF() const { return m_flagDBF; }

	void DrawAttribute( const int toDraw ) { m_drawAttribute = toDraw; }
	int DrawAttribute() const { return m_drawAttribute; }

	int NumberOfEntities() const { return m_numberOfEntities; }
	int ShapeType() const { return m_shapeType; }
	std::string ShapeTypeAsString() const;

	/// Cette methode permet de lire le fichier DBF associe au fichier SHP
	void ReadAttributes( const std::string &DBFfilename );

	virtual void Clear()
	{
		// TODO : Separer vector layer de SHP et crees a la main !
	}

	std::string GetInfos() const;

	virtual void Save( const std::string &name ) = 0;

	/// Les attributs, si ils existent ... On met tous dans des wxString, ce n'est que pour de l'affichage ...
	//std::map< wxString , std::vector< wxString > > m_attributesDBF;
	std::vector < wxString > m_dbfAttributesNames;
	std::vector < std::vector < wxString > > m_dbfAttributesValues;

	// local->global transform.
	wxPoint FromLocal(double zoomFactor, double translationX, double translationY, double delta, double x, double y) const;

protected:
    /// Le handle sur le fichier shapefile
    SHPHandle m_SHPHandle;
    /// Le chemin vers le fichier shapefile
	std::string m_shapefileFileName;
	/// Flag indiquant si le fichier SHP a un fichier PRJ associe. Si il y a un fichier PRJ, on considere que les geometries contenues dans le SHP sont en coordonnees cartographiques. Sinon, ce sont des coordonnees image.
	/// Le flag est a 1 pour des coordonnees image, a -1 pour des coordonnees cartographiques
	signed short m_flagPRJ;
	// Flag indiquant si il existe un DBF associe au fichier
	bool m_flagDBF;
	/// Flag indiquant quel attribut il faut afficher
	int m_drawAttribute;
    /// Le nombre d'objets dans le calque
    int m_numberOfEntities;
    /// Le type de geometrie
    int m_shapeType;
    /// Les coordonnees minimales de l'emprise
    double m_minBound[4];
    /// Les coordonnees maximales de l'emprise
    double m_maxBound[4];

    virtual void CreatePRJ();
};

#endif // __VECTOR_LAYER_CONTENT_HPP__
