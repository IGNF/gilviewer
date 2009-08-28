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

class ColorLookupTable
{
public:
	ColorLookupTable();
	virtual ~ColorLookupTable(){}

	///Charge une CLUT depuis un fichier binaire (de taille 768 octets = 256 * 3)
	void LoadFromBinaryFile(const std::string &fileCLUT);
	///Charge une CLUT depuis un fichier ASCII (3 colonnes RGB, 256 lignes)
	void LoadFromTextFile(const std::string &fileCLUT);
	/// Cree une LUT aleatoire
	void createRandom();

	///Recupere la couleur a partir de l'indice (0 a 255)
//	itk::RGBPixel<unsigned char> GetColor(unsigned int index) const;

	const std::vector<unsigned char>& getData() const { return m_clut; }

private:
	///Tableau de taille 768
	std::vector<unsigned char> m_clut;


};

#endif /*COLORLOOKUPTABLE_H_*/
