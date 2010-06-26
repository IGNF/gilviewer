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

#include "ColorLookupTable.h"

color_lookup_table::color_lookup_table():m_clut(3*256)
{
    // Default LUT in gray levels
    for (unsigned int i = 0; i < m_clut.size(); ++i)
        m_clut[i]=i%256;
}

void color_lookup_table::load_from_binary_file(const std::string &fileCLUT)
{
    std::ifstream ficCLUT(fileCLUT.c_str(), std::ios::binary);
    for (unsigned int i=0; i<m_clut.size(); ++i)
        ficCLUT.read( (char*) &(m_clut[i]), sizeof(unsigned char));
    ficCLUT.close();
}

void color_lookup_table::create_random()
{
    std::srand( clock() );
    for (unsigned int i=0;i<3*256;++i)
        m_clut[i] = (unsigned char)( (double(std::rand()) / RAND_MAX) * 255 + 1 );
}

void color_lookup_table::load_from_text_file(const std::string &fileCLUT)
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

