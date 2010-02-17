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

#ifndef __VECTOR_LAYER_HPP__
#define __VECTOR_LAYER_HPP__

#include <map>

#include <boost/bind.hpp>

#include "GilViewer/gui/VectorLayerSettingsControl.hpp"
#include "GilViewer/layers/VectorLayerContent.hpp"
#include "GilViewer/layers/Layer.hpp"

//ShapeLib
#include "shapelib/shapefil.h"

class VectorLayer : public Layer
{
public:
    /// Constructeur a partir d'un nom de calque et d'un fichier shapefile
    /// @param layerName Le nom du calque
    /// @param shapefileFileName Le chemin vers le fichier shapefile
    //VectorLayer(const std::string &layerName,const std::string &shapefileFileName);
    /// Simplement pour construire un calque "foutoir" (voir l'implementation)
    VectorLayer(const std::string &layerName , signed short flagPRJ , bool flagDBF);

	virtual ~VectorLayer() {};

    //static ptrLayerType CreateVectorLayer(const std::string &layerName , const std::string &fileName);
    static ptrLayerType CreateVectorLayer(const std::string &layerName , signed short flagPRJ = 1 , bool flagDBF = false );

    /// Cette methode recupere les donnees d'affichage par defaut dans les parametres et les affecte au nouveau calque
    virtual void SetDefaultDisplayParameters();

    int Type() const { return m_layerType; }

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

	virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const;
	virtual void Update(int width, int height) {};

    void build_infos();
    virtual std::string get_layer_type_as_string() const {return "Vector";}
    virtual void Save(const std::string &name) const;

    void set_inner_color(const wxColour &colour, bool update = true );
    void set_border_color(const wxColour &colour, bool update = true );
    void set_width(unsigned int width, bool update = true );
    void set_inner_style(unsigned int style, bool update = true);
    void set_border_style(unsigned int style, bool update = true);
    void set_style( const wxColour &inner_color, const wxColour &border_color, unsigned int inner_style, unsigned int border_style, unsigned int width , bool update = true );
    inline wxColour get_inner_color() const {return m_layerContent->get_inner_color();}
    inline wxColour get_border_color() const {return m_layerContent->get_border_color();}
    inline unsigned int get_width() const {return m_layerContent->get_width();}
    inline unsigned int get_inner_style() const {return m_layerContent->get_inner_style();}
    inline unsigned int get_border_style() const {return m_layerContent->get_border_style();}

	// TODO
	//virtual void TextsColour( const wxColour &colour );
	virtual void TextsFont( const wxFont &font , bool update = true ) { m_textFont = font; if (update) notifyLayerSettingsControl_(); }
	virtual const wxFont& TextsFont() const { return m_textFont; }
	virtual void TextsVisibility( bool value , bool update = true ) { m_isTextVisible = value; if (update) notifyLayerSettingsControl_(); }
	virtual bool TextsVisibility() const { return m_isTextVisible; }

    boost::shared_ptr<VectorLayerContent> LayerContent() { return m_layerContent; }

	virtual void Clear();

	virtual std::string Filename() const;
	virtual void Filename(const std::string &filename) {m_filename=filename;}

    virtual std::vector<std::string> get_available_formats_extensions() const;
    virtual std::string get_available_formats_wildcard() const;

    virtual LayerSettingsControl* build_layer_settings_control(unsigned int index, LayerControl* parent);
    virtual bool is_saveable() const {return true;}

private:
	void Init();

    boost::shared_ptr<VectorLayerContent> m_layerContent;

    bool m_isFromFile;

    // Pour l'affichage de texte
    bool m_isTextVisible;
    wxFont m_textFont;
    std::vector< std::pair<double,double> > m_textCoordinates;
    std::vector< std::string > m_textValue;
    std::vector< wxColour > m_textColour;

protected:
    int m_layerType;
};


#endif // __VECTOR_LAYER_HPP__
