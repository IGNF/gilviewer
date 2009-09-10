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
