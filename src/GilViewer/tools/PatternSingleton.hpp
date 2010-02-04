/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets.


Homepage: 

	http://code.google.com/p/gilviewer
	
Copyright:
	
	Institut Geographique National (2009)

Authors: 

	Olivier Tournaire, Adrien Chauve

	
	

    GilViewer is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GilViewer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public 
    License along with GilViewer.  If not, see <http://www.gnu.org/licenses/>.
 
***********************************************************************/

#ifndef __PATTERN_SINGLETON_HPP__
#define __PATTERN_SINGLETON_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

/**
* @brief Classe de <a href="http://home.earthlink.net/~huston2/dp/singleton.html">Singleton</a>.
*
* Un singleton permet de n'avoir qu'une seule et unique instance d'un objet. Le constructeur de la classe est alors prive,
* et pour obtenir cette l'instance existante de l'objet, on fait appel a la methode Instance().<br>
* L'utilisation est assez simple. Par exemple, on peut deriver la classe MySingleton :<br>
* @code
* class SingletonObject : public PatternSingleton<SingletonObject>
* {
*	friend class PatternSingleton<SingletonObject>;
*	// ...
* }
* @endcode
* Il est important que le constructeur soit cache. L'utilisateur ne peut ainsi pas l'appeler et doit obligatoirement
* passer par la methode PatternSingleton::Instance(). Avec les shared_ptr de boost, cela donne :<br>
* @code
* boost::shared_ptr<SingletonObject> unique_object = SingletonObject::Instance();
* @endcode
*/

template <typename T> class PatternSingleton
{
protected:
    /// Constructeur vide
    PatternSingleton () {}
    /// Constructeur par recopie
    PatternSingleton (const PatternSingleton& ) {}
    /// Destructeur
    ~PatternSingleton () {}

public:
    /// Cette methode permet d'obtenir ou de creer l'instance unique de l'objet
//    static boost::shared_ptr<T> Instance ()
//    {
//        if ( m_singleton == boost::shared_ptr<T>() )
//        {
//            // Double-Checked Locking Pattern !
//            boost::mutex::scoped_lock lock(m_mutex);
//            if ( m_singleton == boost::shared_ptr<T>() )
//                m_singleton = boost::shared_ptr<T> (new T);
//        }
//        return (static_cast< boost::shared_ptr<T> > (m_singleton));
//    }

    static T* Instance ()
    {
        if ( !m_singleton )
        {
            // Double-Checked Locking Pattern !
            boost::mutex::scoped_lock lock(m_mutex);
            if ( !m_singleton )
                m_singleton = new T;
        }
        return m_singleton;
    }

//private:
//    /// Le pointeur sur l'instance unique de l'objet
    static T* m_singleton;
//    /// Un mutex pour le <i>Double-Checked Locking Pattern</i> !
    static boost::mutex m_mutex;
};

// Initialisation des variables statiques
template <typename T> T* PatternSingleton<T>::m_singleton=0;
template <typename T> boost::mutex PatternSingleton<T>::m_mutex;

#endif // __PATTERN_SINGLETON_HPP__
