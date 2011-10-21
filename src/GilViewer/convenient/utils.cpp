#include "utils.hpp"

using namespace std;


#include <set>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include "GilViewer/io/gilviewer_io_factory.hpp"
#include "GilViewer/tools/pattern_singleton.hpp"

using namespace boost::algorithm;
using namespace boost::filesystem;

namespace gilviewer_utils
{
    vector<string> all_files_from_path(const string &dir_path, const string &ext, bool case_sensitive, bool recursive)
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

    void build_wx_wildcard_from_io_factory_aux(const std::string& family, const std::string& group, const vector<factory_key>& id, ostringstream& wildcard)
    {
        ostringstream oss1;
        ostringstream oss2;
        bool first = true;
        for(vector<factory_key>::const_iterator it = id.begin();it!=id.end();++it)
        {
            if(family!="" && it->family != family) continue;
            if(group !="" && it->group  != group ) continue;
            if(!first)
            {
                oss1 << " ";
                oss2 << ";";
            }
            string current_ext = it->extension;
            oss1 << current_ext;
            oss2 << "*." << current_ext << ";";
            to_upper(current_ext);
            oss2 << "*." << current_ext;
            first = false;
        }
        if(!first)
        {
            if(wildcard.tellp()>0) wildcard << "|";
            if(pos && pos != -1) wildcard << "|";
            wildcard << (family==""?(group==""?"Supported":group):family) << " files (" << oss1.str() << ")|" << oss2.str();
        }
    }


    string build_wx_wildcard_from_io_factory(const std::string& family, const std::string& group)
    {
        ostringstream wildcard;
        vector<factory_key> id = PatternSingleton<gilviewer_io_factory>::instance()->available_identifiers();

        build_wx_wildcard_from_io_factory_aux(family, group, id, wildcard);

        std::set<std::string> families;
        std::set<std::string> groups;

        for(vector<factory_key>::const_iterator it = id.begin();it!=id.end();++it)
        {
            if(family!="" && it->family != family) continue;
            if(group !="" && it->group  != group ) continue;
            families.insert(it->family);
            groups.insert  (it->group);
        }
        if(family=="")
        {
            for(std::set<std::string>::const_iterator it = families.begin();it!=families.end();++it)
                build_wx_wildcard_from_io_factory_aux(*it, group, id, wildcard);
        }
        if(group=="")
        {
            for(std::set<std::string>::const_iterator it = groups.begin();it!=groups.end();++it)
                build_wx_wildcard_from_io_factory_aux("", *it, id, wildcard);
        }
        if(wildcard.tellp()>0) wildcard << "|";
        wildcard << "All files (*)|*.*";
        return wildcard.str();
    }
}
