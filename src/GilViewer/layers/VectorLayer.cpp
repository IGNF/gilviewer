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

#include <boost/filesystem.hpp>

#include <sstream>
using namespace std;

#include <wx/dc.h>
#include <wx/log.h>
#include <wx/config.h>

#include "GilViewer/layers/VectorLayer.hpp"

void VectorLayer::Init()
{
    m_isTextVisible = true;
    m_textFont = wxNullFont;
    build_infos();
}

void VectorLayer::SetDefaultDisplayParameters()
{
    wxConfigBase *pConfig = wxConfigBase::Get();

    /// Points
    int redPoint = 255, bluePoint = 0, greenPoint = 0;
    int widthPoint = 3;
    if ( pConfig )
    {
        pConfig->Read(wxT("/Options/VectorLayerPoint/Color/Red"), &redPoint, 255);
        pConfig->Read(wxT("/Options/VectorLayerPoint/Color/Green"), &greenPoint, 0);
        pConfig->Read(wxT("/Options/VectorLayerPoint/Color/Blue"), &bluePoint, 0);
        pConfig->Read(wxT("/Options/VectorLayerPoint/Width"), &widthPoint, 3);
    }
    //SetPointsStyle( wxColour(redPoint,greenPoint,bluePoint) , widthPoint );
    /// Points

    /// Lignes
    int redLine = 255, blueLine = 0, greenLine = 0;
    int widthLine = 3;
    int styleLine = wxSOLID;
    if ( pConfig )
    {
        pConfig->Read(wxT("/Options/VectorLayerLine/Color/Red"), &redLine, 255);
        pConfig->Read(wxT("/Options/VectorLayerLine/Color/Green"), &greenLine, 0);
        pConfig->Read(wxT("/Options/VectorLayerLine/Color/Blue"), &blueLine, 0);
        pConfig->Read(wxT("/Options/VectorLayerLine/Width"), &widthLine, 3);
        pConfig->Read(_T("/Options/VectorLayerLine/Style"), &styleLine, wxSOLID);
    }
    //SetLinesStyle( wxColour(redLine,greenLine,blueLine) , widthLine , styleLine );
    /// Lignes

    /// Polygones
    int redRing = 255, blueRing = 0, greenRing = 0;
    int redShape = 255, blueShape = 0, greenShape = 0;
    int ringWidth = 1;
    int penStyle = wxSOLID;
    int brushStyle = wxSOLID;
    if ( pConfig )
    {
        pConfig->Read(wxT("/Options/VectorLayerPolygon/Ring/Color/Red"), &redRing, 255);
        pConfig->Read(wxT("/Options/VectorLayerPolygon/Ring/Color/Green"), &greenRing, 0);
        pConfig->Read(wxT("/Options/VectorLayerPolygon/Ring/Color/Blue"), &blueRing, 0);
        pConfig->Read(wxT("/Options/VectorLayerPolygon/Ring/Width"), &ringWidth, 1);

        pConfig->Read(wxT("/Options/VectorLayerPolygon/Shape/Color/Red"), &redShape, 255);
        pConfig->Read(wxT("/Options/VectorLayerPolygon/Shape/Color/Green"), &greenShape, 0);
        pConfig->Read(wxT("/Options/VectorLayerPolygon/Shape/Color/Blue"), &blueShape, 0);

        pConfig->Read(wxT("/Options/VectorLayerPolygon/Style/Pen"), &penStyle, wxSOLID);
        pConfig->Read(wxT("/Options/VectorLayerPolygon/Style/Brush"), &brushStyle, wxSOLID);
    }
    //PolygonsRingsColour( wxColour(redRing,greenRing,blueRing) );
    //PolygonsInsideColour( wxColour(redShape,greenShape,blueShape) );
    //PolygonsRingsStyle( penStyle );
    //PolygonsInsideStyle( brushStyle );
    //PolygonsRingsWidth( ringWidth );
    /// Polygones

    set_point_color(wxColour(redPoint,greenPoint,bluePoint));
    set_point_width(widthPoint);

    set_line_color(wxColour(redLine,greenLine,blueLine));
    set_line_width(widthLine);
    set_line_style(styleLine);

    set_polygon_border_color(wxColour(redRing,greenRing,blueRing));
    set_polygon_inner_color(wxColour(redShape,greenShape,blueShape));
    set_polygon_border_width(ringWidth);
    set_polygon_border_style(penStyle);
    set_polygon_inner_style(brushStyle);
}

VectorLayer::VectorLayer( const string &layerName , signed short flagPRJ , bool flagDBF ) :
        m_isFromFile(false)
{
    /*
    Name(layerName);

    m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerMultiGeometries() );

    m_layerContent->FlagPRJ( flagPRJ );
    m_layerContent->FlagDBF( flagDBF );
    SetDefaultDisplayParameters();
    notifyLayerSettingsControl_();
    Init();
    */
}

Layer::ptrLayerType VectorLayer::CreateVectorLayer( const string &layerName , signed short flagPRJ , bool flagDBF )
{
    Layer::ptrLayerType ptrLayer(new VectorLayer(layerName,flagPRJ,flagDBF));
    ptrLayer->notifyLayerSettingsControl_();
    ptrLayer->SetDefaultDisplayParameters();
    return ptrLayer;
}

void VectorLayer::build_infos()
{
    //m_infos = m_layerContent->GetInfos();
}

void VectorLayer::Save(const string &name) const
{
    m_layerContent->Save(name);
}

void VectorLayer::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const
{
    // On draw les geometries
    m_layerContent->Draw(dc, x, y, transparent, ZoomFactor(), TranslationX(), TranslationY(), Resolution());
    // ... puis le texte si besoin ...
    if ( m_isTextVisible )
    {
        if ( m_textFont != wxNullFont )
            dc.SetFont(m_textFont);
        vector< pair<double,double> >::const_iterator itbc = m_textCoordinates.begin(), itec = m_textCoordinates.end();
        vector< string >::const_iterator itbv = m_textValue.begin();
        vector< wxColour >::const_iterator itbcolor = m_textColour.begin();
        for (;itbc!=itec;++itbc,++itbv,++itbcolor)
        {
            wxDCTextColourChanger dcfortextcolor(dc,*itbcolor);
            wxCoord x = wxCoord((itbc->first+TranslationX())/ZoomFactor());
            wxCoord y = wxCoord((m_layerContent->FlagPRJ()*itbc->second+TranslationY())/ZoomFactor());
            dc.DrawText( wxString(itbv->c_str(), *wxConvCurrent), x, y);
        }
    }
}

void VectorLayer::AddPoint( double x , double y )
{
    /*
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
    {
        dfl->AddPoint(x,y);
        return;
    }
    */
}

void VectorLayer::AddPoints( const vector<double> &x , const vector<double> &y )
{
    /*
    boost::shared_ptr<VectorLayerPoint> layerPoint = boost::dynamic_pointer_cast<VectorLayerPoint>( m_layerContent );
    if ( layerPoint )
        layerPoint->AddPoints(x,y);
        */
}

void VectorLayer::AddText( double x , double y , const string &text, const wxColour &color )
{
    m_textCoordinates.push_back( make_pair(x,y) );
    m_textValue.push_back( text );
    m_textColour.push_back(color);
}

void VectorLayer::AddLine( double x1 , double y1 , double x2 , double y2 )
{
    /*
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
    {
        dfl->AddLine(x1,y1,x2,y2);
        return;
    }
    */
}

void VectorLayer::AddPolyline( const vector<double> &x , const vector<double> &y )
{
    /*
    boost::shared_ptr<VectorLayerArc> layerArc = boost::dynamic_pointer_cast<VectorLayerArc>( m_layerContent );
    if ( layerArc )
        layerArc->AddPolyline(x,y);
        */
}

void VectorLayer::AddPolygon( const vector<double> &x , const vector<double> &y )
{
    /*
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
        dfl->AddPolygon(x,y);
        */
}

void VectorLayer::AddCircle( double x , double y , double radius )
{
    /*
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
        dfl->AddCircle(x,y,radius);
        */
}

void VectorLayer::AddSpline( vector<pair<double,double> > points )
{
    /*
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
        dfl->AddSpline(points);
        */
}

void VectorLayer::AddEllipse(double x_center, double y_center, double a, double b)
{
    /*
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
        dfl->AddEllipse(x_center,y_center,a,b);
        */
}

void VectorLayer::AddEllipse(double x_center, double y_center, double a, double b, double theta)
{
    /*
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
        dfl->AddEllipse(x_center,y_center,a,b,theta);
        */
}

void VectorLayer::Clear()
{
    m_layerContent->Clear();
    m_textCoordinates.clear();
    m_textValue.clear();
}

string VectorLayer::Filename() const
{
    if(!m_layerContent) return m_filename;
    boost::filesystem::path full = boost::filesystem::system_complete(m_layerContent->ShapefileFileName());
    return full.string();
}

vector<string> VectorLayer::get_available_formats_extensions() const
{
    return vector<string>();
}

string VectorLayer::get_available_formats_wildcard() const
{
    ostringstream wildcard;
    wildcard << "All supported files (*.shp)|*.shp;*.SHP";
    wildcard << wxT("Shapefile (*.shp)|*.shp;*.SHP");
    return wildcard.str();
}

LayerSettingsControl* VectorLayer::build_layer_settings_control(unsigned int index, LayerControl* parent)
{
    return new VectorLayerSettingsControl(index, parent);
}
