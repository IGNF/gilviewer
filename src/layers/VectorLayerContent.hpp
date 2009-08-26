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

#ifndef __VECTOR_LAYER_CONTENT_HPP__
#define __VECTOR_LAYER_CONTENT_HPP__

#ifdef UNIX
#include "libshp/shapefil.h"
#else
#include "shapelib/shapefil.h"
#endif

#include <string>

class wxDC;

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

    virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const float zoomFactor, const float translationX, const float translationY) = 0;

    inline SHPHandle GetHandle() { return m_SHPHandle; }
    void SetHandle( const SHPHandle &handle ) { m_SHPHandle = handle; }

    void ShapefileFileName( const std::string &name ) { m_shapefileFileName = name; }
    std::string ShapefileFileName() { return m_shapefileFileName; }

	signed short FlagPRJ() const { return m_flagPRJ; }
	void FlagPRJ( signed short flagPRJ );

	void FlagDBF( bool flagDBF ) { m_flagDBF = flagDBF; }
	bool FlagDBF() const { return m_flagDBF; }

	void DrawAttribute( const int toDraw ) { m_drawAttribute = toDraw; }
	int DrawAttribute() const { return m_drawAttribute; }

	int NumberOfEntities() { return m_numberOfEntities; }
	int ShapeType() { return m_shapeType; }
	std::string ShapeTypeAsString();

	/// Cette methode permet de lire le fichier DBF associe au fichier SHP
	void ReadAttributes( const std::string &DBFfilename );

	virtual void Clear()
	{
		// TODO : Separer vector layer de SHP et crees a la main !
	}

	std::string GetInfos();

	virtual void Save( const std::string &name ) = 0;

	/// Les attributs, si ils existent ... On met tous dans des wxString, ce n'est que pour de l'affichage ...
	//std::map< wxString , std::vector< wxString > > m_attributesDBF;
	std::vector < wxString > m_dbfAttributesNames;
	std::vector < std::vector < wxString > > m_dbfAttributesValues;

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
