#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <vector>
#include <string>
#ifdef WIN32
	#pragma warning(disable : 4251)
	#pragma warning(disable : 4275)
#endif
namespace gilviewer_utils
{
    std::vector<std::string> all_files_from_path(const std::string &dir_path, const std::string &ext, bool case_sensitive = false, bool recursive = false);
    std::string build_wx_wildcard_from_io_factory(const std::string& family = "", const std::string& group= "");
}

#include <boost/filesystem.hpp>
#if defined(BOOST_FILESYSTEM_VERSION) && (BOOST_FILESYSTEM_VERSION > 2)
    #define BOOST_FILESYSTEM_STRING(x) (x.string())
#else
    #define BOOST_FILESYSTEM_STRING(x) (x)
#endif

#endif // __UTILS_HPP__
