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

#include <sstream>
#include <stdexcept>
#include <fstream>

#include <boost/filesystem.hpp>

#include <wx/log.h>
#include <wx/string.h>

#include "layers/VectorLayerContent.hpp"

#include "wrappers/WrapperShapelibDBF.hpp"

VectorLayerContent::VectorLayerContent() :
	m_SHPHandle(NULL), m_shapefileFileName(""), m_flagPRJ(IMAGE_COORDINATES), m_flagDBF(false), m_drawAttribute(0), m_numberOfEntities(0), m_shapeType(SHPT_NULL)
{
	m_minBound[0] = m_maxBound[0] = 0.;
	m_minBound[1] = m_maxBound[1] = 0.;
	m_minBound[2] = m_maxBound[2] = 0.;
	m_minBound[3] = m_maxBound[3] = 0.;
}

void VectorLayerContent::FlagPRJ(signed short flagPRJ)
{
	// Le flag doit obligatoirement avoir la valeur +1 ou -1
	if (flagPRJ == CARTOGRAPHIC_COORDINATES || flagPRJ == IMAGE_COORDINATES)
		m_flagPRJ = flagPRJ;
	else
	{
		std::ostringstream oss;
		oss << "Bad flag value ! Value must be +1 (IMAGE_COORDINATES) or -1 (CARTOGRAPHIC_COORDINATES) !" << std::endl;
		oss << "flagPRJ = " << flagPRJ << std::endl;
		oss << "File : " << __FILE__ << std::endl;
		oss << "Line : " << __LINE__ << std::endl;
		oss << "Function : " << __FUNCTION__ << std::endl;
		throw std::logic_error(oss.str());
	}

}

void VectorLayerContent::ReadAttributes(const std::string &DBFfilename)
{
	wxString mes;
	mes << wxT("Lecture du fichier DBF ") << wxString(DBFfilename.c_str(), *wxConvCurrent);
	::wxLogMessage(mes);

	try
	{
		WrapperShapelibDBF fileDBF(DBFfilename);

		int nbFields = fileDBF.GetFieldCount();
		int nbRecords = fileDBF.GetRecordCount();
		mes.Clear();
		mes << nbFields << wxT(" champs dans le fichier ...\n") << nbRecords << wxT(" enregistrements dans le fichier ...");
		wxLogMessage(mes);

		unsigned int i, j;
		for (j = 0; j < static_cast<unsigned int> (nbFields); ++j)
		{
			std::vector< wxString> attributeValues;
			std::string fieldName = fileDBF.GetFieldName(j);
			for (i = 0; i < static_cast<unsigned int> (nbRecords); ++i)
				attributeValues.push_back( wxString(fileDBF.ReadAttributeAsString(i,j).c_str(),*wxConvCurrent) );

			//m_attributesDBF.insert( std::make_pair< wxString , std::vector< wxString > >( std::string( fieldName ) , attributeValues ) );
			m_dbfAttributesNames.push_back(wxString(fieldName.c_str(), *wxConvCurrent));
			m_dbfAttributesValues.push_back(attributeValues);
			wxString ml;
			ml << attributeValues.size();
			wxLogMessage(ml);
		}
	}
	catch( const WrapperShapelibDBFException& e)
	{
		mes.Clear();
		mes << wxString(e.what(),*wxConvCurrent);
		wxLogMessage(mes);
	}
}

std::string VectorLayerContent::ShapeTypeAsString()
{
	switch (m_shapeType)
	{
	case SHPT_NULL:
		return "Invalid or unset geometries";
		break;
	case SHPT_POINT:
		return "SHPT_POINT";
		break;
	case SHPT_ARC:
		return "SHPT_ARC";
		break;
	case SHPT_POLYGON:
		return "SHPT_POLYGON";
		break;
	case SHPT_MULTIPOINT:
		return "SHPT_MULTIPOINT";
		break;
	case SHPT_POINTZ:
		return "SHPT_POINTZ";
		break;
	case SHPT_ARCZ:
		return "SHPT_ARCZ";
		break;
	case SHPT_POLYGONZ:
		return "SHPT_POLYGONZ";
		break;
	case SHPT_MULTIPOINTZ:
		return "SHPT_MULTIPOINTZ";
		break;
	case SHPT_POINTM:
		return "SHPT_POINTM";
		break;
	case SHPT_ARCM:
		return "SHPT_ARCM";
		break;
	case SHPT_POLYGONM:
		return "SHPT_POLYGONM";
		break;
	case SHPT_MULTIPOINTM:
		return "SHPT_MULTIPOINTM";
		break;
	case SHPT_MULTIPATCH:
		return "SHPT_MULTIPATCH";
		break;
	default:
		return "Unknown geometries";
	}
}

void VectorLayerContent::CreatePRJ()
{
	std::ofstream prj(boost::filesystem::change_extension(m_shapefileFileName, ".prj").string().c_str());
	prj.close();
}
std::string VectorLayerContent::GetInfos()
{
	std::ostringstream oss;
	oss << "Number of entities = " << m_numberOfEntities << "\n";
	oss << "Entities type = " << ShapeTypeAsString() << "\n";
	oss << "Minimal bounds :\n";
	oss << "      X = " << m_minBound[0] << "\n";
	oss << "      Y = " << m_minBound[1] << "\n";
	oss << "Maximal bounds :\n";
	oss << "      X = " << m_maxBound[0] << "\n";
	oss << "      Y = " << m_maxBound[1] << "\n";
	return oss.str();
}
