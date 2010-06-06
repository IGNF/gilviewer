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

#ifndef COLORLOOKUPTABLE_H_
#define COLORLOOKUPTABLE_H_

#include <vector>
#include <string>



/*
 * \class ColorLookupTable
 * \brief Classe de gestion des LUT
 *
 * Classe qui crée par défaut une lut en niveaux de gris, et charge des LUTs au format dérivé d'ImageJ.
 *
 */

class color_lookup_table
{
public:
        color_lookup_table();
        virtual ~color_lookup_table(){}

	///Charge une CLUT depuis un fichier binaire (de taille 768 octets = 256 * 3)
	void LoadFromBinaryFile(const std::string &fileCLUT);
	///Charge une CLUT depuis un fichier ASCII (3 colonnes RGB, 256 lignes)
	void LoadFromTextFile(const std::string &fileCLUT);
	/// Cree une LUT aleatoire
        void createRandom();

	const std::vector<unsigned char>& getData() const { return m_clut; }

private:
	///Tableau de taille 768
	std::vector<unsigned char> m_clut;


};

typedef color_lookup_table ColorLookupTable;

#endif /*COLORLOOKUPTABLE_H_*/
