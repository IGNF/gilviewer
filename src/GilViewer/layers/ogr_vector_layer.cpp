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
#include "ogr_vector_layer.hpp"

#include <boost/filesystem.hpp>
#include <boost/variant/detail/apply_visitor_unary.hpp>


#include <iostream>
#include <sstream>

#include <gdal/ogrsf_frmts.h>

#include "GilViewer/gui/VectorLayerSettingsControl.hpp"
#include "GilViewer/gui/define_id.hpp"

#include "draw_geometry_visitor.hpp"

using namespace std;
using namespace boost::filesystem;

ogr_vector_layer::ogr_vector_layer(const string &layer_name, const string &filename):
        m_layertype(MULTI_GEOMETRIES_TYPE),
        m_center_x(0.), m_center_y(0.),
        m_nb_geometries(0),
        m_coordinates(1),
        m_point_width(3), m_line_width(3),
        m_line_style(wxSOLID),
        m_polygon_border_width(3),
        m_polygon_border_style(wxSOLID), m_polygon_inner_style(wxSOLID),
        m_point_color(*wxRED), m_line_color(*wxBLUE), m_polygon_border_color(*wxLIGHT_GREY), m_polygon_inner_color(*wxGREEN)
{
    try
    {
        OGRDataSource *poDS;

        poDS = OGRSFDriverRegistrar::Open(filename.c_str(), FALSE);
        if( poDS == NULL )
        {
            string error_message("Open failed. Did you registered OGR formats (you MUST call OGRRegisterAll() ...)?\n");
            error_message += CPLGetLastErrorMsg();
            throw invalid_argument(error_message.c_str());
        }

        for(int i=0;i<poDS->GetLayerCount();++i)
        {
            OGRLayer *poLayer = poDS->GetLayer(i);

            // TODO: handle spatial reference
            OGRSpatialReference *spatref = poLayer->GetSpatialRef();
            build_infos(spatref);

            // TODO: compute extent to get center
            compute_center(poLayer,poDS->GetLayerCount());

            // TODO: read features
            OGRFeature *poFeature;

            poLayer->ResetReading();
            while( (poFeature = poLayer->GetNextFeature()) != NULL )
            {
                // TODO: read fields
                /*
                OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
                for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
                {
                    OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );

                    if( poFieldDefn->GetType() == OFTInteger )
                        printf( "%d,", poFeature->GetFieldAsInteger( iField ) );
                    else if( poFieldDefn->GetType() == OFTReal )
                        printf( "%.3f,", poFeature->GetFieldAsDouble(iField) );
                    else if( poFieldDefn->GetType() == OFTString )
                        printf( "%s,", poFeature->GetFieldAsString(iField) );
                    else
                        printf( "%s,", poFeature->GetFieldAsString(iField) );
                }
                */
                if(OGRLinearRing *poLine = dynamic_cast<OGRLinearRing*>(poFeature->GetGeometryRef()) )
                {
                    m_geometries_features.push_back(std::make_pair<OGRLinearRing*,OGRFeature*>(poLine,poFeature));
                    m_nb_geometries+=poLine->getNumPoints();
                }
                else if(OGRLineString *poLine = dynamic_cast<OGRLineString*>(poFeature->GetGeometryRef()) )
                {
                    m_geometries_features.push_back(std::make_pair<OGRLineString*,OGRFeature*>(poLine,poFeature));
                    m_nb_geometries+=poLine->getNumPoints();
                }
                else if(OGRMultiLineString* poMLine = dynamic_cast<OGRMultiLineString*>(poFeature->GetGeometryRef()))
                {
                    m_geometries_features.push_back(std::make_pair<OGRMultiLineString*,OGRFeature*>(poMLine,poFeature));
                    for(int i=0;i<poMLine->getNumGeometries();++i)
                        m_nb_geometries+=((OGRLineString*)(poMLine->getGeometryRef(i)))->getNumPoints();
                }
                else if(OGRMultiPoint* poMPoint = dynamic_cast<OGRMultiPoint*>(poFeature->GetGeometryRef()))
                {
                    m_geometries_features.push_back(std::make_pair<OGRMultiPoint*,OGRFeature*>(poMPoint,poFeature));
                    m_nb_geometries+=poMPoint->getNumGeometries();
                }
                else if(OGRMultiPolygon* poMPolygon = dynamic_cast<OGRMultiPolygon*>(poFeature->GetGeometryRef()))
                {
                    m_geometries_features.push_back(std::make_pair<OGRMultiPolygon*,OGRFeature*>(poMPolygon,poFeature));
                    for(int i=0;i<poMLine->getNumGeometries();++i)
                    {
                        OGRPolygon* one_polygon =(OGRPolygon*)(poMPolygon->getGeometryRef(i));
                        m_nb_geometries+=one_polygon->getExteriorRing()->getNumPoints();
                        for(int i=0;i<one_polygon->getNumInteriorRings();++i)
                            m_nb_geometries+=one_polygon->getInteriorRing(i)->getNumPoints();
                    }
                }
                else if( OGRPoint *poPoint = dynamic_cast<OGRPoint*>(poFeature->GetGeometryRef()) )
                {
                    m_geometries_features.push_back(std::make_pair<OGRPoint*,OGRFeature*>(poPoint,poFeature));
                    ++m_nb_geometries;
                }
                else if(OGRPolygon *poPolygon = dynamic_cast<OGRPolygon*>(poFeature->GetGeometryRef()))
                {
                    m_geometries_features.push_back(std::make_pair<OGRPolygon*,OGRFeature*>(poPolygon,poFeature));
                    m_nb_geometries+=poPolygon->getExteriorRing()->getNumPoints();
                    for(int i=0;i<poPolygon->getNumInteriorRings();++i)
                        m_nb_geometries+=poPolygon->getInteriorRing(i)->getNumPoints();
                }
            }
        }
        OGRDataSource::DestroyDataSource( poDS );
    }
    catch(const exception &e)
    {
        ostringstream oss;
        oss << endl << "Exception propagated from:" << endl;
        oss << e.what();
        throw logic_error(oss.str());
    }

    Name(layer_name);
    Filename( system_complete(filename).string() );
    notifyLayerSettingsControl_();
    SetDefaultDisplayParameters();
    m_layertype = MULTI_GEOMETRIES_TYPE;
}

ogr_vector_layer::~ogr_vector_layer()
{
    for(unsigned int i=0;i<m_geometries_features.size();++i)
        OGRFeature::DestroyFeature(m_geometries_features[i].second);
}

Layer::ptrLayerType ogr_vector_layer::CreateVectorLayer(const string &layerName , const string &fileName)
{
    if ( !exists(fileName) )
    {
        ostringstream oss;
        oss << "File does not exist: "<<fileName<< " ! " << endl;
        oss << "File : " <<__FILE__ << endl;
        oss << "Line : " << __LINE__ << endl;
        oss << "Function : " << __FUNCTION__ << endl;
        throw logic_error( oss.str() );
    }

    try
    {
        Layer::ptrLayerType ptrLayer(new ogr_vector_layer(layerName,fileName));
        return ptrLayer;
    }
    catch(const exception &e)
    {
        ostringstream oss;
        oss << endl << "Exception propagated from:" << endl;
        oss << e.what();
        throw logic_error(oss.str());
    }
}

void ogr_vector_layer::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const
{
    wxPen point_pen(m_point_color,m_point_width);
    wxPen line_pen(m_line_color,m_line_width,m_line_style);
    wxPen polygon_pen(m_polygon_border_color,m_polygon_border_width,m_polygon_border_style);
    wxBrush polygon_brush(m_polygon_inner_color,m_polygon_inner_style);

    // TODO: image or geographic coordinates
    // dc.SetAxisOrientation();
    draw_geometry_visitor visitor(dc,point_pen,line_pen,polygon_pen,polygon_brush,x,y,transparent,Resolution(),ZoomFactor(),TranslationX(),TranslationY(),m_coordinates);
    for(unsigned int i=0;i<m_geometries_features.size();++i)
        boost::apply_visitor( visitor, m_geometries_features[i].first );
}

LayerSettingsControl* ogr_vector_layer::build_layer_settings_control(unsigned int index, LayerControl* parent)
{
    return new VectorLayerSettingsControl(index, parent);
}

void ogr_vector_layer::compute_center(OGRLayer* layer, int nb_layers)
{
    static double ratio=1./(double)nb_layers;
    OGREnvelope env;
    layer->GetExtent(&env);
    m_center_x+=ratio*(env.MaxX+env.MinX)/2.;
    m_center_y+=ratio*(env.MaxY+env.MinY)/2.;
}

void ogr_vector_layer::build_infos(OGRSpatialReference *spatial_reference)
{
    // Currently only handles infos if the layer has a spatial reference
    m_infos = "";
    ostringstream oss;
    if(spatial_reference)
    {
        // If the layer has a spatial reference, y coordinates must be inverted
        m_coordinates=-1;
        if(spatial_reference->IsGeographic())
            oss << "Geographic system" << std::endl;
        if(spatial_reference->IsLocal())
            oss << "Local system" << std::endl;
        if(spatial_reference->IsProjected())
            oss << "Projected system" << std::endl;
        oss << "PROJCS -> " << spatial_reference->GetAttrValue("PROJCS") << std::endl;
        oss << "GEOGCS -> " << spatial_reference->GetAttrValue("GEOGCS") << std::endl;
        oss << "DATUM -> " << spatial_reference->GetAttrValue("DATUM") << std::endl;
        oss << "SPHEROID -> " << spatial_reference->GetAttrValue("SPHEROID") << std::endl;
        oss << "PROJECTION -> " << spatial_reference->GetAttrValue("PROJECTION") << std::endl;
        m_infos = oss.str();
    }
    else
    {
        std::cout << "No spatial reference defined!" << std::endl;
    }
}

string ogr_vector_layer::get_available_formats_wildcard() const
{
    ostringstream wildcard;
    wildcard << "All supported vector files (*.shp;*.kml)|*.shp;*.kml|";
    wildcard << "SHP (*.shp)|*.shp";
    wildcard << "KML (*.kml)|*.kml";
    return wildcard.str();
}

const std::vector<std::pair<geometry_types,OGRFeature*> >& ogr_vector_layer::get_geometries_features() const
{
    return m_geometries_features;
}

wxPoint ogr_vector_layer::FromLocal(double zoomFactor, double translationX, double translationY, double delta, double x, double y, int coordinates)
{
    return wxPoint( static_cast<wxCoord>((delta+            x+translationX)/zoomFactor),
                    static_cast<wxCoord>((delta+coordinates*y+translationY)/zoomFactor) );
}

void ogr_vector_layer::set_point_color(const wxColor& c, bool update)
{
    m_point_color=c;
    if(update)
        notifyLayerSettingsControl_();
}

wxColor ogr_vector_layer::get_point_color() const {return m_point_color;}

void ogr_vector_layer::set_line_color(const wxColor& c, bool update)
{
    m_line_color=c;
    if(update)
        notifyLayerSettingsControl_();
}

wxColor ogr_vector_layer::get_line_color() {return m_line_color;}

void ogr_vector_layer::set_polygon_border_color(const wxColor& c, bool update)
{
    m_polygon_border_color=c;
    if(update)
        notifyLayerSettingsControl_();
}

wxColor ogr_vector_layer::get_polygon_border_color() {return m_polygon_border_color;}

void ogr_vector_layer::set_polygon_inner_color(const wxColor& c, bool update)
{
    m_polygon_inner_color=c;
    if(update)
        notifyLayerSettingsControl_();
}

wxColor ogr_vector_layer::get_polygon_inner_color() {return m_polygon_inner_color;}

// TODO: notify, settings control, shared_ptr, IMAGE or GEOGRAPHIC coordinates ...
