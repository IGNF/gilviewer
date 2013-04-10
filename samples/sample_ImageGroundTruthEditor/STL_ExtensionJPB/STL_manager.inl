/**
 * @date 14/06/2012
 * @author Jean-Pascal Burochin
 * Gestion personnalisée de la STL
 */
#include "STL_manager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "../constantes.h"

template < class T > void add_new_line ( const T & object , const std::string & path )
{
    std :: ofstream file ;

    file . open ( path . c_str ( ) , std :: ios_base :: out | std :: ios_base :: app ) ;
    if ( ! file . good ( ) )
    {
        JPB_THROW ( "echec de l'ouverture du fichier (\"" + path + "\")" )
    }

    file << object << std :: endl ;
    file . close ( ) ;
}

template < class T > void write_vector ( const std :: vector < T > & set , const std :: string & path )
{
    typename std :: vector < T > :: const_iterator it ;
    std :: ofstream file ;

    file . open ( path . c_str ( ) ) ;
    if ( ! file . good ( ) )
    {
        JPB_THROW("echec de l'ouverture du fichier (\"" + path + "\")")
    }

    try
    {
        for ( it = set . begin ( ) ; it < set . end ( ) ; ++ it )
        {
            file << * it << std :: endl ;
        }
        file . close() ;
    }
    JPB_CATCH ( "échec de l'écriture du fichier \"" + path + "\"" )
}

template < class T > std :: vector < T > read_vector ( const std :: string & path )
{
    T elem ;
    std :: vector < T > res ;
    std :: ifstream file ;

    file . open ( path . c_str ( ) ) ;
    if ( ! file . good ( ) )
    {
        JPB_THROW("echec de l'ouverture du fichier (\"" + path + "\")")
    }

    try
    {
        res . clear ( ) ;
        for ( bool is_read_ok = file >> elem ; is_read_ok ; is_read_ok = file >> elem )
        {
            res . push_back ( elem ) ;
        }
        file . close() ;
    }
    JPB_CATCH ( "échec de la lecture du fichier \"" + path + "\"" )

    return res ;
}

template < class key_t , class object_t > void write_map
(
    const std :: map < key_t , object_t > & map ,
    const std :: string & path ,
    const char & separator
)
{
    typename std :: map < key_t , object_t > :: const_iterator map_it ;
    std :: ofstream file ;

    file . open ( path . c_str ( ) ) ;
    if ( ! file . good ( ) )
    {
        JPB_THROW("echec de l'ouverture du fichier (\"" + path + "\")")
    }

    try
    {
        for ( map_it = map . begin ( ) ; map_it != map . end () ; ++ map_it )
        {
            file << map_it -> first << " " << separator << " " << map_it -> second << std :: endl ;
        }
        file . close() ;
    }
    JPB_CATCH ( "échec de l'écriture du fichier \"" + path + "\"" )
}

template < class key_t , class object_t > std :: map < key_t , object_t > read_map
(
    const std :: string & path ,
    const char & separator
)
{
    std :: map < key_t , object_t > map ;
    std :: string line ;
    std :: string key_string , object_string ;
    key_t key ;
    object_t object ;
    std :: ifstream file ;

    file . open ( path . c_str ( ) ) ;
    if ( ! file . good ( ) )
    {
        JPB_THROW("echec de l'ouverture du fichier (\"" + path + "\")")
    }

    try
    {
        map . clear ( ) ;
        for ( bool is_read_ok = file >> line ; is_read_ok ; is_read_ok = file >> line )
        {
            std :: istringstream issLine ( line ) ;
            std :: getline ( issLine , key_string , separator ) ;
            std :: getline ( issLine , object_string , separator ) ;
            std :: istringstream key_converter ( key_string ) ;
            std :: istringstream object_converter ( object_string ) ;
            key_converter >> key ;
            object_converter >> object ;
            std :: pair < key_t , object_t > element ( key , object ) ;
            map . insert ( element ) ;
        }
        file . close() ;
    }
    JPB_CATCH("échec de la lecture du fichier \"" + path + "\"" )

    return map ;
}

template < class T > void remove_unsorted_element ( typename std :: vector < T > :: iterator & it , std :: vector < T > & set )
{
    std :: swap ( * it , set . back ( ) ) ;
    set . pop_back ( ) ;
}

template < class T > typename std :: vector < T > :: const_iterator find_nearest_sorted_value ( const T & value , const std :: vector < T > & set )
{
    typename std :: vector < T > :: const_iterator begin_it , end_it ;

    begin_it = set . begin ( ) ;
    end_it = set . end ( ) ;
    return find_nearest_sorted_value ( value , begin_it , end_it ) ;
}

template < class T > typename std :: vector < T > :: const_iterator find_nearest_sorted_value ( const T & value , const typename std :: vector < T > :: const_iterator & begin_it , const typename std :: vector < T > :: const_iterator & end_it )
{
    typename std :: vector < T > :: const_iterator nearest_value_it ;

    if ( end_it == begin_it )
    {
        JPB_THROW ( "Vecteur vide" )
    }
    if ( value <= * begin_it )
    {
        return begin_it ;
    }
    if ( value >= * ( end_it - 1) )
    {
        return end_it - 1 ;
    }

    for ( nearest_value_it = begin_it ; nearest_value_it < end_it ; ++ nearest_value_it )
    {
        if ( * nearest_value_it <= value && value <= * ( nearest_value_it + 1 ) )
        {
            if ( value - ( * nearest_value_it ) < * ( nearest_value_it + 1 ) - value )
            {
                return nearest_value_it ;
            }
            else
            {
                return nearest_value_it + 1 ;
            }
        }
    }

    return nearest_value_it;
}
