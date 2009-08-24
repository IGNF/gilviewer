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

#include <fstream>
#include <ctime>
#include <cstdlib>

#include "tools/ColorLookupTable.h"

ColorLookupTable::ColorLookupTable():m_clut(3*256)
{
	//Initialisation par défaut à une LUT en niveaux de gris
	for (unsigned int i = 0; i < m_clut.size(); ++i)
	{
		m_clut[i]=i%256;
	}
}

void ColorLookupTable::LoadFromBinaryFile(const std::string &fileCLUT)
{

	std::ifstream ficCLUT(fileCLUT.c_str());

	for (unsigned int i=0; i<m_clut.size(); ++i)
	{
		//ficCLUT>>m_clut[i]; // ne marche pas ... pourquoi ?!
		ficCLUT.read( (char*) &(m_clut[i]), sizeof(unsigned char));
	}

	ficCLUT.close();
//	for (unsigned int i=0; i<256; ++i)
//	{
//		ficCLUT>>m_clut[3*i+1];
//	}
//	for (unsigned int i=0; i<256; ++i)
//	{
//		ficCLUT>>m_clut[3*i+2];
//	}

//	//fabriquer la LUt gray
//	std::ofstream ficCLUTout("gray.lut", std::ios_base::out | std::ios_base::binary);//(fileCLUT.c_str());
//
//	for (unsigned int i=0; i<256; ++i) ficCLUTout<<(unsigned char)i;
//	for (unsigned int i=0; i<256; ++i) ficCLUTout<<(unsigned char)i;
//	for (unsigned int i=0; i<256; ++i) ficCLUTout<<(unsigned char)i;
//
//
//	ficCLUTout.close();

	//A nettoyer tout ça ! en verifiant que le fichier fait 768 octets, et en utilisant directement un foreach pour remplir le tableau depuis le fichier

}

#include <iostream>

void ColorLookupTable::createRandom()
{
	std::srand( clock() );

	for (unsigned int i=0;i<3*256;++i)
		m_clut[i] = (unsigned char)( (double(std::rand()) / RAND_MAX) * 255 + 1 );
}

void ColorLookupTable::LoadFromTextFile(const std::string &fileCLUT)
{

//	std::ifstream ficCLUT("/home/achauve/Logiciels/ImageJ/luts/sepia.lut");//(fileCLUT.c_str());
//
//	unsigned int i = 0;
//	int temp;
//	while (ficCLUT.good() && i<m_clut.size())
//	{
//
//		ficCLUT>>temp;
//		m_clut[i]=static_cast<unsigned char>(temp);
//		++i;
//	}
//
//	ficCLUT.close();

	//A nettoyer tout ça ! en verifiant que le fichier fait 768 octets, et en utilisant directement un foreach pour remplir le tableau depuis le fichier

}

//itk::RGBPixel<unsigned char> ColorLookupTable::GetColor(unsigned int index) const
//{
//	if(index<0 || index>=m_clut.size())
//		std::cout<<"Erreur LUT !\n";
//	itk::RGBPixel<unsigned char> result;
//	result.SetRed(m_clut[index]);
//	result.SetGreen(m_clut[256+index]);
//	result.SetBlue(m_clut[2*256 +index]);
//
//	return result;
//}

