/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets. GilViewer is distributed under the CeCILL-B licence. 
See Licence_CeCILL-B_V1-en.txt or http://www.cecill.info for more details.


Homepage: 

	http://launchpad.net/gilviewer
	
Copyright:
	
	Institut Geographique National (2009)

Authors: 

	Olivier Tournaire, Adrien Chauve

	
	

This software is governed by the CeCILL-B license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL-B
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-B license and that you accept its terms.
 
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
