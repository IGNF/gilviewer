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
#include <boost/variant/apply_visitor.hpp>

#include <iostream>
#include <sstream>

#include <gdal/ogrsf_frmts.h>

#include <wx/dc.h>

#include "GilViewer/gui/VectorLayerSettingsControl.hpp"

using namespace std;
using namespace boost::filesystem;

ogr_vector_layer::ogr_vector_layer(const string &layer_name, const string &filename)
{
    try
    {
        OGRDataSource *poDS;

        poDS = OGRSFDriverRegistrar::Open(filename.c_str(), FALSE);
        if( poDS == NULL )
        {
            printf( "Open failed.\n" );
            exit( 1 );
        }

        for(int i=0;i<poDS->GetLayerCount();++i)
        {
            OGRLayer *poLayer = poDS->GetLayer(i);
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
                if( OGRPoint *poPoint = dynamic_cast<OGRPoint*>(poFeature->GetGeometryRef()) )
                    m_geometries_features.push_back(std::make_pair<OGRPoint*,OGRFeature*>(poPoint,poFeature));
                else if(OGRPolygon *poPolygon = dynamic_cast<OGRPolygon*>(poFeature->GetGeometryRef()))
                    m_geometries_features.push_back(std::make_pair<OGRPolygon*,OGRFeature*>(poPolygon,poFeature));
                else if(OGRLineString *poLine = dynamic_cast<OGRLineString*>(poFeature->GetGeometryRef()) )
                    m_geometries_features.push_back(std::make_pair<OGRLineString*,OGRFeature*>(poLine,poFeature));
                else if(OGRMultiPoint* poMPoint = dynamic_cast<OGRMultiPoint*>(poFeature->GetGeometryRef()))
                    m_geometries_features.push_back(std::make_pair<OGRMultiPoint*,OGRFeature*>(poMPoint,poFeature));
                else if(OGRMultiPolygon* poMPolygon = dynamic_cast<OGRMultiPolygon*>(poFeature->GetGeometryRef()))
                    m_geometries_features.push_back(std::make_pair<OGRMultiPolygon*,OGRFeature*>(poMPolygon,poFeature));
                else if(OGRMultiLineString* poMLine = dynamic_cast<OGRMultiLineString*>(poFeature->GetGeometryRef()))
                    m_geometries_features.push_back(std::make_pair<OGRMultiLineString*,OGRFeature*>(poMLine,poFeature));
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
    notifyLayerSettingsControl_();

    Name(layer_name);
}

ogr_vector_layer::~ogr_vector_layer()
{
    for(unsigned int i=0;i<m_geometries_features.size();++i)
        OGRFeature::DestroyFeature(m_geometries_features[i].second);
}

Layer::ptrLayerType ogr_vector_layer::CreateVectorLayer(const string &layerName , const string &fileName)
{
    path full = system_complete(fileName);
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
        ptrLayer->Name(fileName);
        path full = system_complete(fileName);
        ptrLayer->Filename( full.string() );
        ptrLayer->notifyLayerSettingsControl_();
        ptrLayer->SetDefaultDisplayParameters();
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

class draw_geometry_visitor : public boost::static_visitor<>
{
public:
    draw_geometry_visitor(wxDC &dc, wxCoord x, wxCoord y, bool transparent, double r, double z, double tx, double ty):
            m_dc(dc),
            m_x(x), m_y(y),
            m_transparent(transparent),
            m_r(r), m_z(z), m_tx(tx), m_ty(ty)
    {
        m_delta=0.5*r;
    }

    template <typename T>
    void operator()(T* operand) const
    {
        //throw std::invalid_argument("Unhandled geometry type!");
        cout << "Unhandled geometry type!" << endl;
    }
private:
    wxDC &m_dc;
    wxCoord m_x, m_y;
    bool m_transparent;
    double m_r, m_z, m_tx, m_ty, m_delta;
};

template <>
void draw_geometry_visitor::operator()(OGRPoint* operand) const
{
    double x = (m_delta+operand->getX()+m_tx)/m_z;
    double y = (m_delta+operand->getY()+m_ty)/m_z;
    m_dc.DrawLine(x,y,x,y);
}

template <>
void draw_geometry_visitor::operator()(OGRLineString* operand) const
{
    for(int j=0;j<operand->getNumPoints()-1;++j)
    {
        double x1 = (m_delta+operand->getX(j)+m_tx)/m_z;
        double y1 = (m_delta+operand->getY(j)+m_ty)/m_z;
        double x2 = (m_delta+operand->getX(j+1)+m_tx)/m_z;
        double y2 = (m_delta+operand->getY(j+1)+m_ty)/m_z;
        m_dc.DrawLine(x1,y1,x2,y2);
    }
}

template <>
void draw_geometry_visitor::operator()(OGRMultiLineString* operand) const
{
    for(int i=0;i<operand->getNumGeometries();++i)
    {
        OGRLineString* l = (OGRLineString*)operand->getGeometryRef(i);
        for(int j=0;j<l->getNumPoints()-1;++j)
        {
            double x1 = (m_delta+l->getX(j)+m_tx)/m_z;
            double y1 = (m_delta+l->getY(j)+m_ty)/m_z;
            double x2 = (m_delta+l->getX(j+1)+m_tx)/m_z;
            double y2 = (m_delta+l->getY(j+1)+m_ty)/m_z;
            m_dc.DrawLine(x1,y1,x2,y2);
        }
    }
}

void ogr_vector_layer::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const
{
    wxPen penColour( wxColour(255,0,0) , 3);
    dc.SetPen( penColour );
    draw_geometry_visitor visitor(dc,x,y,transparent,Resolution(),ZoomFactor(),TranslationX(),TranslationY());
    for(unsigned int i=0;i<m_geometries_features.size();++i)
        boost::apply_visitor( visitor, m_geometries_features[i].first );
}

LayerSettingsControl* ogr_vector_layer::build_layer_settings_control(unsigned int index, LayerControl* parent)
{
    return new VectorLayerSettingsControl(index, parent);
}

// TODO: notify, settings control, shared_ptr ...
