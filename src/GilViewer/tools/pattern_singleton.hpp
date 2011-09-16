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
* @brief <a href="http://home.earthlink.net/~huston2/dp/singleton.html">Singleton</a> class.
*
* Its use is rather simple. For instance, you can subclass PatternSingleton:
* @code
* class SingletonObject : public PatternSingleton<SingletonObject>
* {
*	friend class PatternSingleton<SingletonObject>;
*	// ...
* }
* @endcode
* To retrieve the object, you should use the instance() method:
* @code
* boost::shared_ptr<SingletonObject> unique_object = SingletonObject::instance();
* @endcode
*/

template <typename T> class PatternSingleton : public T
{
protected:
    PatternSingleton () {}
    PatternSingleton (const PatternSingleton& ) {}
    ~PatternSingleton () {}

public:
    /// Retrieve the singleton object unique instance (or create it on the first call)
    static T* instance ()
    {
        if ( !m_singleton )
        {
            // Double-Checked Locking Pattern
            boost::mutex::scoped_lock lock(m_mutex);
            if ( !m_singleton )
                m_singleton = new T;
        }
        return m_singleton;
    }

private:
    /// A pointer on the object unique instance
    static T* m_singleton;
    /// Mutex for the <i>Double-Checked Locking Pattern</i>
    static boost::mutex m_mutex;
};

// Static members initialization
template <typename T> T* PatternSingleton<T>::m_singleton=0;
template <typename T> boost::mutex PatternSingleton<T>::m_mutex;

#endif // __PATTERN_SINGLETON_HPP__
