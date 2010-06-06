#include "gilviewer_file_io.hpp"

#include <boost/filesystem/convenience.hpp>

#include <sstream>

#include "../layers/ogr_vector_layer.hpp"

using namespace boost;
using namespace std;

#include "../config/config.hpp"

boost::shared_ptr<layer> gilviewer_file_io::load(const string& filename)
{
    if ( !filesystem::exists(filename) )
    {
        ostringstream oss;
        oss << "File does not exist: "<<filename<< " ! " << endl;
        oss << "File : " <<__FILE__ << endl;
        oss << "Line : " << __LINE__ << endl;
        oss << "Function : " << __FUNCTION__ << endl;
        throw logic_error( oss.str() );
    }

    try
    {
#if GILVIEWER_USE_GDAL
        layer::ptrLayerType ptrLayer(new ogr_vector_layer(filesystem::basename(filename),filename));
        return ptrLayer;
#endif // GILVIEWER_USE_GDAL
        return boost::shared_ptr<layer>();
    }
    catch(const std::exception &e)
    {
        ostringstream oss;
        oss << endl << "Exception propagated from:" << endl;
        oss << e.what();
        throw logic_error(oss.str());
    }
}
