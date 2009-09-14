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

