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

#ifndef TYPEINFOS_H_
#define TYPEINFOS_H_


template<typename T> struct TypeInfos;
//{
//private:
//	static std::string GetType(){return "Undefined !";}
//};
template<> struct TypeInfos<signed char>
{
	static std::string GetType(){return "signed char";}
};
template<> struct TypeInfos<unsigned char>
{
	static std::string GetType(){return "unsigned char";}
};
template<> struct TypeInfos<signed short>
{
	static std::string GetType(){return "signed short";}
};
template<> struct TypeInfos<unsigned short>
{
	static std::string GetType(){return "unsigned short";}
};
template<> struct TypeInfos<signed int>
{
	static std::string GetType(){return "signed int";}
};
template<> struct TypeInfos<unsigned int>
{
	static std::string GetType(){return "unsigned int";}
};
template<> struct TypeInfos<signed long>
{
	static std::string GetType(){return "signed long";}
};
template<> struct TypeInfos<unsigned long>
{
	static std::string GetType(){return "unsigned long";}
};
template<> struct TypeInfos<float>
{
	static std::string GetType(){return "float";}
};
template<> struct TypeInfos<double>
{
	static std::string GetType(){return "double";}
};




#endif /*TYPEINFOS_H_*/
