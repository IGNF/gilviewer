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

#include "WrapperShapelibDBF.hpp"

WrapperShapelibDBF::WrapperShapelibDBF(const std::string &filename, eAccessType access) :
	m_dbfHandle(NULL), m_accessType(access), m_fieldCount(0), m_recordCount(0)
{
	if (m_accessType == READ_ONLY)
		m_accessTypeAsString = "rb";
	else
		m_accessTypeAsString = "rb+";

	m_dbfHandle = DBFOpen(filename.c_str(), m_accessTypeAsString.c_str());

	if (m_dbfHandle)
		Init();
}

WrapperShapelibDBF::WrapperShapelibDBF() :
	m_dbfHandle(NULL), m_accessType(READ_WRITE)
{
	m_accessTypeAsString = "rb+";
}

void WrapperShapelibDBF::Create(const std::string &filename)
{
	m_dbfHandle = DBFCreate(filename.c_str());
	if (!m_dbfHandle)
		throw WrapperShapelibDBFException("DBFHandle non valide ! Avez vous bien appele la methode Create() apres le constructeur vide ?", __LINE__, __FILE__, __FUNCTION__);

	// Pas besoin de tester le handle, on aura throwe avant ...
	Init();
}

void WrapperShapelibDBF::Init()
{
	m_fieldCount = DBFGetFieldCount(m_dbfHandle);
	m_recordCount = DBFGetRecordCount(m_dbfHandle);
}

void WrapperShapelibDBF::Close()
{
	if (m_dbfHandle)
		DBFClose(m_dbfHandle);
}

int WrapperShapelibDBF::GetFieldIndex(const std::string &fieldname)
{
	if (m_dbfHandle)
		return DBFGetFieldIndex(m_dbfHandle, fieldname.c_str());
	throw WrapperShapelibDBFException("DBFHandle non valide ! Avez vous bien appele la methode Create() apres le constructeur vide ?", __LINE__, __FILE__, __FUNCTION__);
}

DBFFieldType WrapperShapelibDBF::GetFieldInfo(int fieldIndex, std::string& fieldname, int &fieldwidth, int &nbDecimals) const
{
	if (fieldIndex >= m_fieldCount || fieldIndex < 0)
		throw WrapperShapelibDBFException("Index invalide !", __LINE__, __FILE__, __FUNCTION__);
	if (m_dbfHandle)
	{
		char charfieldname[256];
		DBFFieldType fieldtype = DBFGetFieldInfo(m_dbfHandle, fieldIndex, charfieldname, &fieldwidth, &nbDecimals);
		fieldname = std::string(charfieldname);
		return fieldtype;
	}
	throw WrapperShapelibDBFException("DBFHandle non valide ! Avez vous bien appele la methode Create() apres le constructeur vide ?", __LINE__, __FILE__, __FUNCTION__);
}
std::string WrapperShapelibDBF::GetFieldName(int fieldIndex) const
{
	std::string fieldname;
	int i;
	GetFieldInfo(fieldIndex, fieldname, i, i);
	return fieldname;
}

int WrapperShapelibDBF::AddField(std::string& fieldname, const DBFFieldType &fieldtype, int fieldwidth, int nbDecimals)
{
	if (m_accessType == READ_WRITE && m_dbfHandle)
		return DBFAddField(m_dbfHandle, fieldname.c_str(), fieldtype, fieldwidth, nbDecimals);
	else
	{
		if (m_accessType != READ_WRITE)
			throw WrapperShapelibDBFException("Ajout de champs impossible ! Le fichier a ete ouvert en lecture seule !", __LINE__, __FILE__, __FUNCTION__);
		else
			// (!m_dbfHandle)
			throw WrapperShapelibDBFException("DBFHandle non valide ! Avez vous bien appele la methode Create() apres le constructeur vide ?", __LINE__, __FILE__, __FUNCTION__);
	}
}

int WrapperShapelibDBF::ReadIntegerAttribute(int shape, int field) const
{
	if (m_dbfHandle)
		return DBFReadIntegerAttribute(m_dbfHandle, shape, field);
	throw WrapperShapelibDBFException("DBFHandle non valide ! Avez vous bien appele la methode Create() apres le constructeur vide ?", __LINE__, __FILE__, __FUNCTION__);
}

double WrapperShapelibDBF::ReadDoubleAttribute(int shape, int field) const
{
	if (m_dbfHandle)
		return DBFReadDoubleAttribute(m_dbfHandle, shape, field);
	throw WrapperShapelibDBFException("DBFHandle non valide ! Avez vous bien appele la methode Create() apres le constructeur vide ?", __LINE__, __FILE__, __FUNCTION__);
}

std::string WrapperShapelibDBF::ReadStringAttribute(int shape, int field) const
{
	if (m_dbfHandle)
		return std::string(DBFReadStringAttribute(m_dbfHandle, shape, field));
	throw WrapperShapelibDBFException("DBFHandle non valide ! Avez vous bien appele la methode Create() apres le constructeur vide ?", __LINE__, __FILE__, __FUNCTION__);
}

std::string WrapperShapelibDBF::ReadAttributeAsString(int shape, int field) const
{
	if (!m_dbfHandle)
		throw WrapperShapelibDBFException("DBFHandle non valide ! Avez vous bien appele la methode Create() apres le constructeur vide ?", __LINE__, __FILE__, __FUNCTION__);
	std::string fieldName;
	int i;
	DBFFieldType fieldType = GetFieldInfo(field, fieldName, i,i);
	if (fieldType == FTString)
	{
		return std::string(ReadStringAttribute(shape,field));
	}
	else if (fieldType == FTInteger)
	{
		std::ostringstream oss;
		oss << ReadIntegerAttribute(shape,field);
		return oss.str();
	}
	else if (fieldType == FTDouble)
	{
		std::ostringstream oss;
		oss << ReadDoubleAttribute(shape,field);
		return oss.str();
	}
	else
		return "";
}

bool WrapperShapelibDBF::IsAttributeNULL(int shape, int field) const
{
	if (m_dbfHandle)
		return (bool)DBFIsAttributeNULL(m_dbfHandle, shape, field);
	throw WrapperShapelibDBFException("DBFHandle non valide ! Avez vous bien appele la methode Create() apres le constructeur vide ?", __LINE__, __FILE__, __FUNCTION__);
}

bool WrapperShapelibDBF::WriteIntegerAttribute(int shape, int field, int fieldValue)
{
	if (m_dbfHandle)
		return (bool)DBFWriteIntegerAttribute(m_dbfHandle, shape, field, fieldValue);
	throw WrapperShapelibDBFException("DBFHandle non valide ! Avez vous bien appele la methode Create() apres le constructeur vide ?", __LINE__, __FILE__, __FUNCTION__);
}

bool WrapperShapelibDBF::WriteDoubleAttribute(int shape, int field, double fieldValue)
{
	if (m_dbfHandle)
		return DBFWriteDoubleAttribute(m_dbfHandle, shape, field, fieldValue);
	throw WrapperShapelibDBFException("DBFHandle non valide ! Avez vous bien appele la methode Create() apres le constructeur vide ?", __LINE__, __FILE__, __FUNCTION__);
}

bool WrapperShapelibDBF::WriteStringAttribute(int shape, int field, std::string fieldValue)
{
	if (m_dbfHandle)
		return (bool)DBFWriteStringAttribute(m_dbfHandle, shape, field, fieldValue.c_str());
	throw WrapperShapelibDBFException("DBFHandle non valide ! Avez vous bien appele la methode Create() apres le constructeur vide ?", __LINE__, __FILE__, __FUNCTION__);
}

bool WrapperShapelibDBF::WriteNULLAttribute(int shape, int field)
{
	if (m_dbfHandle)
		return (bool)DBFWriteNULLAttribute(m_dbfHandle, shape, field);
	throw WrapperShapelibDBFException("DBFHandle non valide ! Avez vous bien appele la methode Create() apres le constructeur vide ?", __LINE__, __FILE__, __FUNCTION__);
}

WrapperShapelibDBF::~WrapperShapelibDBF()
{
	// The DBFClose() function will close the indicated xBase file (opened with DBFOpen(), or DBFCreate()), flushing out all
	// information to the file on disk, and recovering any resources associated with having the file open. The file handle (hDBF)
	// should not be used again with the DBF API after calling DBFClose().
	if (m_dbfHandle)
		DBFClose(m_dbfHandle);
}
