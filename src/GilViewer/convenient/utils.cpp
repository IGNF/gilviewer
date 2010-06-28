#include "utils.hpp"

using namespace std;

vector<string> ogr_available_formats_extensions()
{
    vector<string> extensions;
    extensions.push_back("shp");
    extensions.push_back("SHP");
    extensions.push_back("kml");
    extensions.push_back("KML");
    return extensions;
}

vector<string> simple_available_formats_extensions()
{
    vector<string> extensions;
    extensions.push_back("txt");
    extensions.push_back("TXT");
    extensions.push_back("xml");
    extensions.push_back("XML");
    extensions.push_back("bin");
    extensions.push_back("BIN");
    return extensions;
}

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/case_conv.hpp>

using namespace boost::algorithm;
using namespace boost::filesystem;

vector<string> filesystem_utils::all_files_from_path(const string &dir_path, const string &ext, bool case_sensitive, bool recursive)
{
    vector<string> all_files;

    string extension_with_case = ext;
    if ( case_sensitive )
        to_upper( extension_with_case );

    if ( !exists(dir_path) )
        return all_files;
    directory_iterator end_itr; // default construction yields past-the-end
    for (directory_iterator itr(dir_path); itr != end_itr; ++itr)
    {
        if ( is_regular_file(itr->status()) )
        {
            if ( ext != "" )
            {
                string filename_extension_with_case = itr->path().string();
                if ( case_sensitive )
                    to_upper( filename_extension_with_case ) ;
                if ( extension( filename_extension_with_case ) == extension_with_case )
                    all_files.push_back( itr->path().string() );
            }
            else
                all_files.push_back( itr->path().string() );
        }
        else if ( is_directory(itr->status()) )
        {
            vector<string> files_in_recursion = all_files_from_path( itr->path().string() , extension_with_case , true );
            all_files.insert( all_files.end() , files_in_recursion.begin() , files_in_recursion.end() );
        }
    }

    return all_files;
}
