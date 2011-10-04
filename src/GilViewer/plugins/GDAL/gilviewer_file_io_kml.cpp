#include "gilviewer_file_io_kml.hpp"

#include <boost/variant/get.hpp>

#include <gdal/ogrsf_frmts.h>

#include "GilViewer/io/gilviewer_io_factory.hpp"

#include "ogr_vector_layer.hpp"
#include <sstream>

using namespace boost;
using namespace std;

shared_ptr<layer> gilviewer_file_io_kml::load(const string &filename, const ptrdiff_t top_left_x, const ptrdiff_t top_left_y, const ptrdiff_t dim_x, const ptrdiff_t dim_y)
{
    return gilviewer_file_io::load(filename);
}

void gilviewer_file_io_kml::save(shared_ptr<layer> layer, const string &filename)
{
    shared_ptr<ogr_vector_layer> ogr_layer = dynamic_pointer_cast<ogr_vector_layer>(layer);
    if(!ogr_layer)
        throw invalid_argument("Bad layer type!\n");

    OGRSFDriver *poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName( "KML" );
    if( poDriver == NULL )
        throw invalid_argument("KML driver is not available. Did you forget to call OGRRegisterAll()?\n");

    OGRDataSource *poDS = poDriver->CreateDataSource( filename.c_str(), NULL );
    if( poDS == NULL )
    {
        ostringstream oss;
        oss << "Creation of output file " << filename << " failed.\n";
        throw ios_base::failure(oss.str());
    }

    OGRLayer *poLayer = poDS->CreateLayer( "", NULL, wkbUnknown, NULL );
    if( poLayer == NULL )
        throw ios_base::failure("Creation of layer failed.\n");

    std::vector<std::pair<geometry_types,OGRFeature*> > geometries = ogr_layer->geometries_features();
    typedef std::vector<std::pair<geometry_types,OGRFeature*> >::const_iterator const_iterator;
    const_iterator itb=geometries.begin(), ite=geometries.end();
    for(;itb!=ite;++itb)
    {
        OGRFeature *poFeature = OGRFeature::CreateFeature( poLayer->GetLayerDefn() );
        // --------- Point --------- //
        {
            try
            {
                OGRPoint *poPoint = boost::get<OGRPoint*>(itb->first);
                poFeature->SetGeometry(poPoint);
                if( poLayer->CreateFeature( poFeature ) != OGRERR_NONE )
                    throw invalid_argument("Failed to create feature in shapefile.\n");
                OGRFeature::DestroyFeature( poFeature );
                continue;
            }
            catch(const bad_get& e)
            {
            }
        }
        // --------- MultiPoint --------- //
        {
            try
            {
                OGRMultiPoint *poMPoint = boost::get<OGRMultiPoint*>(itb->first);
                poFeature->SetGeometry(poMPoint);
                if( poLayer->CreateFeature( poFeature ) != OGRERR_NONE )
                    throw invalid_argument("Failed to create feature in shapefile.\n");
                OGRFeature::DestroyFeature( poFeature );
                continue;
            }
            catch(const bad_get& e)
            {
            }
        }
        // --------- Polygon --------- //
        {
            try
            {
                OGRPolygon *poPolygon = boost::get<OGRPolygon*>(itb->first);
                poFeature->SetGeometry(poPolygon);
                if( poLayer->CreateFeature( poFeature ) != OGRERR_NONE )
                    throw invalid_argument("Failed to create feature in shapefile.\n");
                OGRFeature::DestroyFeature( poFeature );
                continue;
            }
            catch(const bad_get& e)
            {
            }
        }
        // --------- MultiPolygon --------- //
        {
            try
            {
                OGRMultiPolygon *poMPolygon = boost::get<OGRMultiPolygon*>(itb->first);
                poFeature->SetGeometry(poMPolygon);
                if( poLayer->CreateFeature( poFeature ) != OGRERR_NONE )
                    throw invalid_argument("Failed to create feature in shapefile.\n");
                OGRFeature::DestroyFeature( poFeature );
                continue;
            }
            catch(const bad_get& e)
            {
            }
        }
        // --------- LinearRing --------- //
        {
            try
            {
                OGRLinearRing *poLinear = boost::get<OGRLinearRing*>(itb->first);
                poFeature->SetGeometry(poLinear);
                if( poLayer->CreateFeature( poFeature ) != OGRERR_NONE )
                    throw invalid_argument("Failed to create feature in shapefile.\n");
                OGRFeature::DestroyFeature( poFeature );
                continue;
            }
            catch(const bad_get& e)
            {
            }
        }
        // --------- LineString --------- //
        {
            try
            {
                OGRLineString *poLinear = boost::get<OGRLineString*>(itb->first);
                poFeature->SetGeometry(poLinear);
                if( poLayer->CreateFeature( poFeature ) != OGRERR_NONE )
                    throw invalid_argument("Failed to create feature in shapefile.\n");
                OGRFeature::DestroyFeature( poFeature );
                continue;
            }
            catch(const bad_get& e)
            {
            }
        }
        // --------- MultiLineString --------- //
        {
            try
            {
                OGRMultiLineString *poMLinear = boost::get<OGRMultiLineString*>(itb->first);
                poFeature->SetGeometry(poMLinear);
                if( poLayer->CreateFeature( poFeature ) != OGRERR_NONE )
                    throw invalid_argument("Failed to create feature in shapefile.\n");
                OGRFeature::DestroyFeature( poFeature );
                continue;
            }
            catch(const bad_get& e)
            {
            }
        }
    }
    OGRDataSource::DestroyDataSource( poDS );
}

boost::shared_ptr<gilviewer_file_io_kml> create_gilviewer_file_io_kml()
{
    return shared_ptr<gilviewer_file_io_kml>(new gilviewer_file_io_kml());
}

bool gilviewer_file_io_kml::Register(gilviewer_io_factory *factory)
{
    factory->Register("kml", create_gilviewer_file_io_kml);
    pair<string,string> familly_description = make_pair<string,string>("Vector files","KML files");
    factory->metadata().insert( make_pair<string,pair<string,string> >( "kml", familly_description ) );
    return true;
}
