/**
 * @date 20/09/2012
 * @author Jean-Pascal Burochin
 * Exploitation de la stl (étendue aux répertoires)
 */
#include "STL_manager.hpp"
#include <stdio.h>
#include <dirent.h>
#include <ctime>
#include <iomanip>

#define DIRECTORY_PATH_SEPARATOR '/'

std::string GetFormatedTime()
{
    std::ostringstream res;
    struct tm * p_date;
    time_t t;

    t = time(0);
    p_date = localtime(& t);

    res.fill('0');
    res << (1900 + p_date->tm_year);
    res << std::setw(2) << (1 + p_date->tm_mon);
    res << std::setw(2) << p_date->tm_mday;
    res << std::setw(2) << p_date->tm_hour;
    res << std::setw(2) << p_date->tm_min;
    res << std::setw(2) << p_date->tm_sec;

    return res.str();
}

void read_directory
(
    const std::string & path,
    const std::string & ext,
    std::vector < std::string > & v_paths,
    const std::string & prefix_path
)
{
    DIR * p_directory;
    struct dirent * p_entry;
    std::string data_path, new_prefix_path;

    p_directory = opendir(path.c_str());
    if(p_directory == NULL)
    {
        JPB_THROW("echec de l'ouverture du fichier \"" + path + "\"")
    }

    p_entry = readdir(p_directory);
    for(; p_entry != NULL; p_entry = readdir(p_directory))
    {
        data_path = p_entry->d_name;
        if(data_path == "." || data_path == "..")
        {
            continue ;
        }
        if(p_entry->d_type == DT_DIR)
        {
            new_prefix_path = prefix_path + data_path + DIRECTORY_PATH_SEPARATOR;
            read_directory(path + DIRECTORY_PATH_SEPARATOR + data_path, ext, v_paths, new_prefix_path);
        }
        else
        {
            if(data_path.find(ext) != std::string::npos)
            {
                v_paths.push_back(prefix_path + data_path);
            }
        }
    }
    closedir(p_directory);
}

std::vector < std::string > read_directory(const std::string & path, const std::string & ext)
{
    std::vector < std::string > v_paths;

    read_directory(path, ext, v_paths, "");

    return v_paths;
}

std::string remove_extension(const std::string & path, const char separator)
{
    std::string res;
    std::string::size_type idx;

    idx = path.rfind(separator);

    if(idx != std::string::npos)
    {
        res = path.substr(0, idx);
    }
    else
    {
        res = path;
    }

    return res;
}

std::string get_extension(const std::string & path, const char separator)
{
    std::string res;
    std::string::size_type idx;

    idx = path.rfind(separator);

    res = "";
    if(idx != std::string::npos)
    {
        res = path.substr(idx, path.length());
    }

    return res;
}
