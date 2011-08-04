#include "../config/config.hpp"
#if GILVIEWER_USE_GDALJP2

#include "gilviewer_file_io_gdal_jp2.hpp"
#include <boost/gil/extension/io_new/jpeg_all.hpp>
#include "gilviewer_io_factory.hpp"
#include "gdal_priv.h"
#include <iostream>
#include <fstream>
#include "cpl_conv.h" // for CPLMalloc()

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

#include <boost/gil/extension/io_new/tiff_all.hpp>

shared_ptr<layer> gilviewer_file_io_gdal_jp2::load(const string &filename, const ptrdiff_t top_left_x, const ptrdiff_t top_left_y, const ptrdiff_t dim_x, const ptrdiff_t dim_y)
{
    static bool passer=false;
    if(!passer)
            GDALAllRegister(); 
typedef boost::gil::point2<std::ptrdiff_t> point_t;
       if ( !exists(filename) )
        {
            GILVIEWER_LOG_ERROR("File " + filename + " does not exist");
            return layer::ptrLayerType();
        }

   GDALDataset* hDataset = (GDALDataset*)GDALOpen( filename.c_str(), GA_ReadOnly );
    if( hDataset == NULL )
    {
        GILVIEWER_LOG_ERROR("Pb to open file " + filename);
        return layer::ptrLayerType();
    }

    unsigned int width=GDALGetRasterXSize( hDataset );
    unsigned int height=GDALGetRasterYSize( hDataset );

        path path(system_complete(filename));
        #if BOOST_FILESYSTEM_VERSION > 2
        string ext(path.extension().string());
        #else
        string ext(path.extension());
        #endif

        image_layer::image_ptr image(new image_layer::image_t);
        point_t origin( max(top_left_x, (ptrdiff_t)0), max(top_left_y, (ptrdiff_t)0) );
        //point_t origin( 0,0 );
        point_t size(dim_x,dim_y);
        if(dim_x==0 && dim_y==0)
            size = point_t(width,height);
        
//        image_read_settings<TagType> settings(origin, size );

        try
        {

            if(hDataset->GetRasterCount()==3){
                unsigned char * jpp=new unsigned char[width*height*3];
                int *panBandMap = new int[3];
                panBandMap[0]=1;
                panBandMap[1]=2;
                panBandMap[2]=3;
                
                 CPLErr status=hDataset->RasterIO(GF_Read, 0   , 0,width, height,(void*)jpp,width,height,GDT_Byte ,3, panBandMap,0,0,0);
                 delete [] panBandMap;
                if (status!=CE_None)        {
                    GILVIEWER_LOG_ERROR("Pb to read file " + filename + " with GDAL ");
                    return layer::ptrLayerType();
                }
                GDALClose( (GDALDatasetH) hDataset );
                 boost::gil::rgb8_image_t rgb_im (width,height);
                 boost::gil::rgb8_view_t outputView = boost::gil::view(rgb_im);
                 unsigned int count=0;
                 unsigned int offset=width*height;
                 unsigned int offset2=width*height*2;
                for (boost::gil::rgb8_view_t::iterator viewiterator = outputView.begin(); viewiterator != outputView.end(); ++viewiterator)
                    {  *viewiterator=boost::gil::rgb8_pixel_t((int)jpp[count],
                                                              (int)jpp[count+offset],
                                                              (int)jpp[count+offset2]);
                         count++;
                    }
                image->value=rgb_im;
                delete [] jpp;
            }
            else if(hDataset->GetRasterCount()==1){
                unsigned short * jpp=new unsigned short[width*height];
                int *panBandMap = new int[1];
                panBandMap[0]=1;
                
                 CPLErr status=hDataset->RasterIO(GF_Read, 0   , 0,width, height,(void*)jpp,width,height,GDT_UInt16 ,1, panBandMap,0,0,0);
                 delete [] panBandMap;
                if (status!=CE_None)        {
                    GILVIEWER_LOG_ERROR("Pb to read file " + filename + " with GDAL ");
                    return layer::ptrLayerType();
                }
                GDALClose( (GDALDatasetH) hDataset );
                 boost::gil::gray16_image_t rgb_im (width,height);
                 boost::gil::gray16_view_t outputView = boost::gil::view(rgb_im);
                 unsigned int count=0;
                for (boost::gil::gray16_view_t::iterator viewiterator = outputView.begin(); viewiterator != outputView.end(); ++viewiterator)
                    {  *viewiterator=boost::gil::gray16_pixel_t(jpp[count]);
                         count++;
                    }
                image->value=rgb_im;
                delete [] jpp;
            }
        }
            catch( const std::exception &e )
        {
            //GILVIEWER_LOG_EXCEPTION("Image read error: " + filename);
            return layer::ptrLayerType();
        }

        #if BOOST_FILESYSTEM_VERSION > 2
        layer::ptrLayerType layer(new image_layer(image, path.stem().string(), path.string()));
        #else
        layer::ptrLayerType layer(new image_layer(image, path.stem(), path.string()));
        #endif
        
        layer->add_orientation(filename);

        layer->infos( build_and_get_infos(filename) );

        return layer;
        
            
    //return load_gil_image<jpeg_tag>(filename, point_t(top_left_x, top_left_y), point_t(dim_x, dim_y));
}

void gilviewer_file_io_gdal_jp2::save(shared_ptr<layer> layer, const string &filename)
{
    save_gil_view<jpeg_tag>(layer, filename);
}

string gilviewer_file_io_gdal_jp2::build_and_get_infos(const std::string &filename)
{
        return "TODO";
    //image_read_info< jpeg_tag > info = read_image_info(filename, jpeg_tag());
    ostringstream infos_str;
    /*infos_str << "Dimensions: " << info._width << "x" << info._height << "\n";
    infos_str << "Number of components: " << info._num_components << "\n";
    string colorspace_str;
    if(info._color_space==JCS_UNKNOWN)
        colorspace_str="JCS_UNKNOWN";
    else if(info._color_space==JCS_GRAYSCALE)
        colorspace_str="JCS_GRAYSCALE";
    else if(info._color_space==JCS_RGB)
        colorspace_str="JCS_RGB";
    else if(info._color_space==JCS_YCbCr)
        colorspace_str="JCS_YCbCr";
    else if(info._color_space==JCS_CMYK)
        colorspace_str="JCS_CMYK";
    else if(info._color_space==JCS_YCCK)
        colorspace_str="JCS_YCCK";
    infos_str << "Color space: " << colorspace_str << "\n";
    infos_str << "Data precision: " << info._data_precision << "\n";*/
    return infos_str.str();
}

shared_ptr<gilviewer_file_io_gdal_jp2> create_gilviewer_file_io_gdal_jp2()
{
    return shared_ptr<gilviewer_file_io_gdal_jp2>(new gilviewer_file_io_gdal_jp2());
}

bool gilviewer_file_io_gdal_jp2::Register()
{
    


    gilviewer_io_factory::instance()->Register("jp2", create_gilviewer_file_io_gdal_jp2);
    pair<string,string> familly_description = make_pair<string,string>("Image files","JPEG2000 images");
    pair< string, pair<string,string> > to_insert = make_pair< string, pair<string,string> >( "jp2", familly_description );
    gilviewer_io_factory::instance()->metadata().insert( to_insert );


    /*
    gilviewer_io_factory::instance()->Register("jpeg", create_gilviewer_file_io_jpg);
    to_insert = make_pair< string, pair<string,string> >( "jpeg", familly_description );
    gilviewer_io_factory::instance()->metadata().insert( to_insert );
    */
    return true;
}

bool register_jpg2000_ok = gilviewer_file_io_gdal_jp2::Register();

#endif // GILVIEWER_USE_GDALJP2
