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

#ifndef WRAPPERSHAPELIBDBF_HPP_
#define WRAPPERSHAPELIBDBF_HPP_

#include <string>
#include <stdexcept>
#include <sstream>

#include "shapelib/shapefil.h"
class WrapperShapelibDBFException : public std::exception
{
public:
	/// Constructeur vide
	WrapperShapelibDBFException() throw() : m_message("") {};
	/// Constructeur � partir d'une cha�ne de caract�res
	/// @param error_msg Le message de l'exception
	WrapperShapelibDBFException(const std::string& error_msg) : m_message(error_msg) {};
	/// Constructeur � partir d'une cha�ne de caract�res et d'infos suppl�mentaires
	/// @param error_msg Le message de l'exception
	/// @param line La ligne � laquelle s'est produite l'exception (on peut utiliser la macro __LINE__)
	/// @param file Le fichier dans lequel s'est produite l'exception (on peut utiliser la macro __FILE__)
	/// @param function La fonction dans laquelle s'est produite l'exception (on peut utiliser la macro __FUNCTION__)
	WrapperShapelibDBFException(const std::string& error_msg, int line = -1, const std::string &file = "undefined", const std::string &function = "undefined") {setMessage(error_msg,line,file,function);};
	virtual ~WrapperShapelibDBFException() throw() {};
	/// Renvoie le message de l'exception
	/// @return Le message de l'exception
	virtual const char * what() const throw() { return m_message.c_str(); }
	/// Construction du message de l'exception
	/// @param error_msg Le message principale de l'exception
	/// @param line La ligne � laquelle s'est produite l'exception (on peut utiliser la macro __LINE__)
	/// @param file Le fichier dans lequel s'est produite l'exception (on peut utiliser la macro __FILE__)
	/// @param function La fonction dans laquelle s'est produite l'exception (on peut utiliser la macro __FUNCTION__)
	void setMessage(const std::string& error_msg, int line = -1, const std::string &file = "undefined", const std::string &function = "undefined")
	{
		std::ostringstream string_line;
		m_message = "!!!!!!!!!!!!!!!!\nWrapperShapelibDBFException :\n";
		m_message += "\tline : ";
		if ( line != -1 )
		{
			string_line << line;
			m_message += string_line.str();
		}
		else
			m_message += "undefined";
		m_message += "\n\tfunction : ";
		m_message += function;
		m_message += "\n\tfile : ";
		m_message += file;
		m_message += "\n\tdescription : ";
		m_message += error_msg;
		m_message += "\n!!!!!!!!!!!!!!!!\n";
	}

private:
	/// Le message de l'exception
	std::string m_message;
};

/**
 * Cette classe est un wrapper extremement simple sur la shapelib, et en particulier sa gestion des fichiers d'attributs (*.dbf).
 */
class WrapperShapelibDBF
{
public:
	enum eAccessType
	{
		READ_ONLY,
		READ_WRITE
	};

	typedef struct
	{
	} DBFFieldInfo;

	/// Ce constructeur doit etre utilise si le fichier existe sur le disque !
	WrapperShapelibDBF( const std::string &filename , eAccessType access = READ_ONLY );
	/// Ce constructeur est a utiliser si le fichier est a creer ! Il est obligatoire d'appeler la methode Create() avant toute manipulation !
	WrapperShapelibDBF();

	void Create( const std::string &filename );
	/// Juste pour ... je sais pas ... Appellee de toute facon dans le destructeur
	void Close();

	/// Renvoie le nombre de champs
	int GetFieldCount() const { return m_fieldCount; }
	/// Renvoie le nombre d'enregistrements. Pour un fichier shp donne, il y a autant d'enregistrements que de geometries.
	int GetRecordCount() const { return m_recordCount; }
	/// Renvoie l'index d'un champ. La comparaison n'est pas sensible a la casse ...
	/// @param fieldname Le champ a tester
	/// @ return -1 en cas d'echec
	int GetFieldIndex( const std::string &fieldname );
	/// Renvoie les infos sur un champ
	/// @param fieldIndex Le champ a requeter
	/// ...
	DBFFieldType GetFieldInfo(int fieldIndex , std::string& fieldname , int &fieldwidth , int &nbDecimals ) const;
	/// Renvoie le nom du champs
	std::string GetFieldName(int fieldIndex) const;
	/// ...
	/// @return -1 en cas d'echec
	int AddField(std::string& fieldname , const DBFFieldType &fieldtype , int fieldwidth , int nbDecimals = 0 );

	/// The DBFReadIntegerAttribute() will read the value of one field and return it as an integer. This can be used even with FTString fields, though the returned value will be zero if not interpretable as a number.
	int ReadIntegerAttribute(int shape , int field) const;
	/// The DBFReadDoubleAttribute() will read the value of one field and return it as a double. This can be used even with FTString fields, though the returned value will be zero if not interpretable as a number.
	double ReadDoubleAttribute(int shape , int field) const;
	/// The DBFReadStringAttribute() will read the value of one field and return it as a string. This function may be used on any field type (including FTInteger and FTDouble) and will return the string representation stored in the .dbf file. The returned pointer is to an internal buffer which is only valid untill the next DBF function call. It's contents may be copied with normal string functions such as strcpy(), or strdup(). If the TRIM_DBF_WHITESPACE macro is defined in shapefil.h (it is by default) then all leading and trailing space (ASCII 32) characters will be stripped before the string is returned.
	std::string ReadStringAttribute( int shape, int field ) const;
	/// Lit un attribut de n'importe quel type et renoive sa valeur sous forme de string ...
	std::string ReadAttributeAsString(int shape , int field) const;
	/// This function will return TRUE if the indicated field is NULL valued otherwise FALSE. Note that NULL fields are represented in the .dbf file as having all spaces in the field. Reading NULL fields will result in a value of 0.0 or an empty string with the other DBFRead*Attribute() functions.
	bool IsAttributeNULL( int shape, int field ) const;
	/// The DBFWriteIntegerAttribute() function is used to write a value to a numeric field (FTInteger, or FTDouble). If the write succeeds the value TRUE will be returned, otherwise FALSE will be returned. If the value is too large to fit in the field, it will be truncated and FALSE returned.
	bool WriteIntegerAttribute( int shape, int field , int fieldValue );
	/// The DBFWriteDoubleAttribute() function is used to write a value to a numeric field (FTInteger, or FTDouble). If the write succeeds the value TRUE will be returned, otherwise FALSE will be returned. If the value is too large to fit in the field, it will be truncated and FALSE returned.
	bool WriteDoubleAttribute( int shape, int field , double fieldValue );
	/// The DBFWriteStringAttribute() function is used to write a value to a string field (FString). If the write succeeds the value TRUE willbe returned, otherwise FALSE will be returned. If the value is too large to fit in the field, it will be truncated and FALSE returned.
	bool WriteStringAttribute( int shape, int field , std::string fieldValue );
	/// The DBFWriteNULLAttribute() function is used to clear the indicated field to a NULL value. In the .dbf file this is represented by setting the entire field to spaces. If the write succeeds the value TRUE willbe returned, otherwise FALSE will be returned.
	bool WriteNULLAttribute( int shape, int field );

	virtual ~WrapperShapelibDBF();

private:
	void Init();
	/// Pour faciliter le travail
	std::string BuildExceptionMessage();
	/// Le handle sur le fichier DBF : si il est NULL, ca ne va vraiment pas ...
	DBFHandle m_dbfHandle;
	eAccessType m_accessType;
	std::string m_accessTypeAsString;
	int m_fieldCount;
	int m_recordCount;
};

#endif /* WRAPPERSHAPELIBDBF_HPP_ */
