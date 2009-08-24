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

#ifndef __VECTOR_LAYER_HPP__
#define __VECTOR_LAYER_HPP__

#include <map>

#include <boost/bind.hpp>

class VectorLayerContent;

#include "gui/VectorLayerSettingsControl.hpp"

//ShapeLib
#include "shapelib/shapefil.h"

#include "layers/Layer.hpp"

class VectorLayer : public Layer
{
public:
	/// Creation d'un calque "a la main"
	/// @param layerName Le nom du layer dans le LayerControl
	/// @param layerType Le type du layer parmi (cf. http://shapelib.maptools.org/shp_api.html, section "Shape Types")
	/// @param flagPRJ Indique si l'on est dans un système projeté : -1 si oui, +1 sinon (par défaut)
	/// @param flagDBF Indique si on souhaite gerer des attributs : true si oui, false sinon (par défaut) [NON GERE !!!]
    VectorLayer(const std::string &layerName , int layerType , signed short flagPRJ = 1 , bool flagDBF = false );
    /// Constructeur a partir d'un nom de calque et d'un fichier shapefile
    /// @param layerName Le nom du calque
    /// @param shapefileFileName Le chemin vers le fichier shapefile
    VectorLayer(const std::string &layerName,const std::string &shapefileFileName);
    /// Simplement pour construire un calque "foutoir" (voir l'implementation)
    VectorLayer(const std::string &layerName , signed short flagPRJ = 1 , bool flagDBF = false );

	~VectorLayer() {};

    static ptrLayerType CreateVectorLayer(const std::string &layerName , int layerType , signed short flagPRJ = 1 , bool flagDBF = false );
    static ptrLayerType CreateVectorLayer(const std::string &layerName , const std::string &fileName);
    static ptrLayerType CreateVectorLayer(const std::string &layerName , signed short flagPRJ = 1 , bool flagDBF = false );

    virtual void AddVectorLayerContent( const std::string &shapefileFileName );

    /// Cette methode recupere les donnees d'affichage par defaut dans les parametres et les affecte au nouveau calque
    virtual void SetDefaultDisplayParameters();

    const int & Type() { return m_layerType; }

    virtual void AddPoint( double x , double y );
    virtual void AddPoints( const std::vector<double> &x , const std::vector<double> &y );
    virtual void AddText( double x , double y , const std::string &text, const wxColour &color = *wxBLACK);
    virtual void AddLine( double x1 , double y1 , double x2 , double y2 );
    virtual void AddPolyline( const std::vector<double> &x , const std::vector<double> &y );
    virtual void AddPolygon( const std::vector<double> &x , const std::vector<double> &y );

    // Pour le FunnyVectorLayer
    virtual void AddCircle( double x , double y , double radius );
    virtual void AddSpline( std::vector<std::pair<double,double> > points );
    virtual void AddEllipse(double x_center, double y_center, double a, double b);
    virtual void AddEllipse(double x_center, double y_center, double a, double b, double theta);

    virtual void SetBrushVectorLayerMultiGeometries( const wxBrush &brush );
    virtual void SetBrushVectorLayerMultiGeometries( unsigned char red , unsigned char green , unsigned char blue , int style , unsigned char alpha=wxALPHA_OPAQUE );
    virtual void SetPenVectorLayerMultiGeometries( const wxPen &pen );
    virtual void SetPenVectorLayerMultiGeometries( unsigned char red , unsigned char green , unsigned char blue , int width = 1 , int style = wxSOLID , unsigned char alpha=wxALPHA_OPAQUE );
    // Fin FunnyVectorLayer

	virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent);
	virtual void Update(const int width, const int height) {};

    void GetInfos();
    virtual void Save(const std::string &name);

    virtual ptrLayerType crop( int x0 , int y0 , int width , int height ) { return ptrLayerType(); }

    virtual void PointsColour( const wxColour &colour , bool update = true );
	virtual wxColour PointsColour();
    virtual void PointsWidth( const unsigned int width , bool update = true );
	virtual unsigned int PointsWidth();
    virtual void SetPointsStyle( const wxColour &colour , const unsigned int width , bool update = true );

    virtual void LinesColour( const wxColour &colour , bool update = true );
	virtual wxColour LinesColour();
    virtual void LinesWidth( const unsigned int width , bool update = true );
	virtual unsigned int LinesWidth();
    virtual void LinesPenStyle( const int style , bool update = true );
	virtual int LinesPenStyle();
    virtual void SetLinesStyle( const wxColour &colour , const unsigned int width , int style , bool update = true );

	virtual void PolygonsRingsColour( const wxColour &colour , bool update = true );
	virtual wxColour PolygonsRingsColour();
	virtual void PolygonsInsideColour( const wxColour &colour , bool update = true );
	virtual wxColour PolygonsInsideColour();
	virtual void PolygonsRingsStyle( const unsigned int style , bool update = true );
	virtual unsigned int PolygonsRingsStyle();
	virtual void PolygonsInsideStyle( const unsigned int style , bool update = true );
	virtual unsigned int PolygonsInsideStyle();
	virtual void PolygonsRingsWidth( const unsigned int width , bool update = true );
	virtual unsigned int PolygonsRingsWidth();

	// TODO
	//virtual void TextsColour( const wxColour &colour );
	virtual void TextsFont( const wxFont &font , bool update = true ) { m_textFont = font; if (update) notifyLayerSettingsControl_(); }
	virtual wxFont TextsFont() { return m_textFont; }
	virtual void TextsVisibility( bool value , bool update = true ) { m_isTextVisible = value; if (update) notifyLayerSettingsControl_(); }
	virtual bool TextsVisibility() { return m_isTextVisible; }

	boost::shared_ptr<VectorLayerContent> LayerContent() { return m_layerContent; }
	inline bool IsEditable() { return m_isEditable; }
	inline void IsEditable( bool editable ) { m_isEditable = editable; }

	virtual void Clear();

	virtual std::string Filename() const;

private:
	void Init();

    boost::shared_ptr<VectorLayerContent> m_layerContent;

    bool m_isEditable;
    bool m_isFromFile;

    // Pour l'affichage de texte
    bool m_isTextVisible;
    wxFont m_textFont;
    std::vector< std::pair<double,double> > m_textCoordinates;
    std::vector< std::string > m_textValue;
    std::vector< wxColour > m_textColour;

    int m_layerType;
};


#endif // __VECTOR_LAYER_HPP__
