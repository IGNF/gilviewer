#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <vector>
#include <string>

std::vector<std::string> ogr_available_formats_extensions();
std::vector<std::string> simple_available_formats_extensions();

class filesystem_utils
{
public:
    filesystem_utils() {}
    virtual ~filesystem_utils() {}

    static std::vector<std::string> all_files_from_path(const std::string &dir_path, const std::string &ext, bool case_sensitive = false, bool recursive = false);
};

namespace gilviewer_utils
{
    std::string build_wx_wildcard_from_io_factory();
}

#endif // __UTILS_HPP__
