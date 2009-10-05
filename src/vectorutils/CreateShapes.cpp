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

#include <stdexcept>
#include <sstream>
#include <fstream>

#include <boost/filesystem.hpp>

#include "vectorutils/CreateShapes.h"

CreateShapes::CreateShapes()
{
}

CreateShapes::~CreateShapes()
{
}

void CreateShapes::CreateSegments(const std::string &txtFileSegments)
{
	if ( !boost::filesystem::exists(txtFileSegments) )
	{
		std::ostringstream oss;
		oss << "Le fichier texte de segments n'existe pas : "<<txtFileSegments<< " ! " << std::endl;
		oss << "File : " <<__FILE__ << std::endl;
		oss << "Line : " << __LINE__ << std::endl;
		oss << "Function : " << __FUNCTION__ << std::endl;
		throw std::logic_error( oss.str() );
	}

	//ouverture du fichier texte de segments qui doit contenir des x1 y1 x2 y2 pour chaque ligne
	std::ifstream fileSegments(txtFileSegments.c_str() , std::ifstream::in );

	SHPHandle hSHPHandle;
	SHPObject *psShape;

	std::string fileSHP = boost::filesystem::basename(txtFileSegments) + ".shp";
	hSHPHandle = SHPCreate( fileSHP.c_str(), SHPT_ARC);

	double x[2], y[2];
	while(fileSegments.good())
	{
		fileSegments>>x[0]>>y[0]>>x[1]>>y[1];
		//		x1 += 0.5;


		//		psShape = SHPCreateSimpleObject( SHPT_POINT, -1, 0, NULL, NULL, 1, &x, &y, &z, &m);
		psShape = SHPCreateSimpleObject( SHPT_ARC, 2, x, y, NULL);

		SHPWriteObject(hSHPHandle, -1, psShape);

	}

	SHPClose(hSHPHandle);
}
