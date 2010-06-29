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

#include "../io/gilviewer_io_factory.hpp"

namespace gilviewer_utils
{
    string build_wx_wildcard_from_io_factory()
    {
        /*
        typedef multimap<string, pair<string, string> > metadata_type;
        typedef metadata_type::const_iterator metadata_iterator;
        const multimap<string, pair<string, string> > &metadata = gilviewer_io_factory::instance()->metadata();
        // First, fecth all available families
        map<string,vector<string> > families;
        metadata_iterator it=metadata.begin();
        for(;it!=metadata.end();++it)
            families.insert()
            */
        // Currently simple version without descriptions
        vector<string> ext = gilviewer_io_factory::instance()->available_identifiers();
        vector<string>::iterator it = ext.begin();
        ostringstream wildcard;
        wildcard << "All available formats |";
        for(;it!=ext.end();++it)
        {
            string current_ext = *it;
            wildcard << "*." << current_ext << ";";
            to_upper(current_ext);
            wildcard << "*." << current_ext << ";";
        }
        wildcard << "|" << "Custom format |*.*";
        return wildcard.str();
    }
}
