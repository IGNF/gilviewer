/**
 * @date 14/06/2012
 * @author Jean-Pascal Burochin
 * Gestion personnalisée de la STL
 */
#ifndef __STL_MANAGER_HPP__
#define __STL_MANAGER_HPP__
#include <string>
#include <vector>
#include <map>

#define DEFAULT_MAP_SEPARATOR ':'

template < class T > void add_new_line ( const T & object , const std::string & path ) ;

template < class T > void write_vector(const std::vector < T > & v_values, const std::string & path);
template < class T > std::vector < T > read_vector(const std::string & path);

template < class key_t , class object_t > void write_map
(
    const std::map < key_t, object_t > & map,
    const std::string & path,
    const char & separator = DEFAULT_MAP_SEPARATOR
);
template < class key_t, class object_t > std::map < key_t, object_t > read_map
(
    const std::string & path,
    const char & separator = DEFAULT_MAP_SEPARATOR
);

template < class T > void remove_unsorted_element(typename std::vector < T >::iterator & it, const std::vector < T > & set);

template < class T > typename std::vector < T >::const_iterator find_nearest_sorted_value(const T & value, std::vector < T > & set);

template < class T > typename std::vector < T >::const_iterator find_nearest_sorted_value(const T & value, const typename std::vector < T >::const_iterator & begin_it, const typename std::vector < T >::const_iterator & end_it);

std::string GetFormatedTime();

std::vector < std::string > read_directory(const std::string & path, const std::string & ext);

std::string remove_extension(const std::string & path, const char separator = '.');
std::string get_extension(const std::string & path, const char separator = '.');

#include "STL_manager.inl"
#endif
