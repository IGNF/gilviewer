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

#ifndef PATTERNFACTORY_HPP_
#define PATTERNFACTORY_HPP_

#include <map>
#include <string>
#include <vector>
#include <stdexcept>

#include <boost/function.hpp>

/**
* @brief Factory class
*
* This implementation is inspired by "Modern C++ Design" (A. Alexandrescu).
* We however use in this implementation a boost::function to let the ability to use the result of a boost::bind as a creator.
*
* Template parameters description:
* <ul>
*   <li><i>TIdentifierType</i>  : the identifier type to associate with a <i>creator</i>. Defaults to <i>std::string</i></li>
*   <li><i>TProductCreator</i>  : the <i>creator</i> function. Defaults to <i>boost::function<TAbstractProduct* ()></i></li>
*   <li><i>ReturnType</i>       : the return type of the creator. Defaults to <i>TAbstractProduct*</i></li>
*   <li><i>MetadataContainer</i>: the metadata container type. Defaults to <i>std::map<std::string, std::string></i></li>
* </ul>
*
* @todo Policy based error handling
*
*/

template < class TAbstractProduct,
           typename TIdentifierType   = std::string,
           typename TProductCreator   = boost::function<TAbstractProduct* ()>,
           typename ReturnType        = TAbstractProduct*,
           typename MetadataContainer = std::map<std::string, std::string> > class PatternFactory
{
public:
    virtual ~PatternFactory() {}

    /// Register a <i>creator</i> with its <i>identifier</i> in the factory
    bool Register(const TIdentifierType& id, TProductCreator creator)
    {
        return associations_.insert( typename AssocMapType::value_type(id, creator) ).second;
    }

    /// @brief Unregister a <i>creator</i> from the factory
    /// @param id Identifier associated to the <i>creator</i> to be removed
    bool Unregister(const TIdentifierType& id)
    {
        return associations_.erase(id) == 1;
    }

    virtual ReturnType create_object(const TIdentifierType& id)
    {
        typename AssocMapType::const_iterator i = associations_.find(id);
        if(i != associations_.end())
        {
            return (i->second)();
        }
        throw std::logic_error("Unknown object type passed to factory !\n");
    }

    std::vector<TIdentifierType> available_identifiers()
    {
        std::vector<TIdentifierType> ids;
        for(typename AssocMapType::const_iterator it=associations_.begin(); it!=associations_.end(); ++it)
            ids.push_back(it->first);
        return ids;
    }

    const MetadataContainer& metadata() const { return m_metadata; }
    MetadataContainer& metadata() { return m_metadata; }

private:
    typedef std::map<TIdentifierType, TProductCreator> AssocMapType;
    AssocMapType associations_;
    MetadataContainer m_metadata;
};

#endif /* PATTERNFACTORY_HPP_ */
