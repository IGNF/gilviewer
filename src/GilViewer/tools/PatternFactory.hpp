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
#include <stdexcept>

#include <boost/function.hpp>

/**
* @brief Classe de Factory.
*
* Implémentation d'une factory très simple à partir de "Modern C++ Design" d'Alexandrescu.
* (simple = AssocMap non opimisée, pas de gestionnaire d'erreur... à améliorer !)
* A noter l'utilisation de boost::function pour pouvoir récupérer le résultat d'un bind comme un creator,
* ce qui n'est pas possible directement avec un <TAbstractProduct* (*) () classique.
*
*/

template
<
	class TAbstractProduct,
	typename TIdentifierType = std::string,
	typename TProductCreator = boost::function<TAbstractProduct* ()>
>
class PatternFactory
{

	public:
		virtual ~PatternFactory() {}

		bool Register(const TIdentifierType& id, TProductCreator creator)
		{
			return associations_.insert( typename AssocMapType::value_type(id, creator) ).second;
		}

		bool Unregister(const TIdentifierType& id)
		{
			return associations_.erase(id) == 1;
		}

		virtual TAbstractProduct* createObject(const TIdentifierType& id)
		{
			typename AssocMapType::const_iterator i = associations_.find(id);
			if(i != associations_.end())
			{
				return (i->second)();
			}
			throw std::logic_error("Unknown object type passed to factory !\n");
		}

	private:
		typedef std::map<TIdentifierType, TProductCreator> AssocMapType;
		AssocMapType associations_;

};

#endif /* PATTERNFACTORY_HPP_ */
