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
#include <stdexcept>

#include <boost/filesystem.hpp>
#include "orientation_2d.hpp"

using namespace std;

void orientation_2d::read_ori_from_image_file(const string &filename)
{
    if ( !boost::filesystem::exists(filename) )
    {
        ostringstream oss;
        oss << "Image file does not exist: "<<filename<< " ! " << endl;
        oss << "File : " <<__FILE__ << endl;
        oss << "Line : " << __LINE__ << endl;
        oss << "Function : " << __FUNCTION__ << endl;
        throw logic_error( oss.str() );
    }

    string basename = boost::filesystem::basename(filename);
    string path = boost::filesystem::path(filename).branch_path().string();

    try
    {
        read_ori_from_ori_file(path+"/"+basename+".ori");
    }
    catch (const logic_error &)
    {
        // Si on catche cette erreur, cela signifie que le .ori n'existe pas
        // On essaie de lire un .tfw
        read_ori_from_tfw_file(path+"/"+basename+".tfw");
    }
}

void orientation_2d::read_ori_from_ori_file(const string &filename)
{
    if ( !boost::filesystem::exists(filename) )
    {
        ostringstream oss;
        oss << "Image file does not exist: "<<filename<< " ! " << endl;
        oss << "File : " <<__FILE__ << endl;
        oss << "Line : " << __LINE__ << endl;
        oss << "Function : " << __FUNCTION__ << endl;
        throw logic_error( oss.str() );
    }

    ifstream fileOri(filename.c_str() , ifstream::in );

#ifndef WIN32
    //Renvoit des exceptions en cas d'erreur de lecture/ouverture de fichier
    // Ca, sous windows, ca me lance direct une exception ...
    fileOri.exceptions ( ifstream::eofbit | ifstream::failbit | ifstream::badbit );
#endif

    //Verification de la presence du carto
    string carto;
    fileOri >> carto;

    // On suppose un formattage comme suit :
    // CARTO
    // OriginX (en m)   OriginY (en m)
    // Zone carto
    // TailleX    TailleY   (taille de l'image en pixels)
    // Pas en X (>0 - en m)    Pas en Y (<0 - en m)

    if (carto != "CARTO")
    {
        ostringstream oss;
        oss << "Bad ORI format!" << endl;
        oss << "File : " <<__FILE__ << endl;
        oss << "Line : " << __LINE__ << endl;
        oss << "Function : " << __FUNCTION__ << endl;
        throw logic_error( oss.str() );
    }

    fileOri >> m_originX >> m_originY;
    fileOri >> m_zoneCarto;
    fileOri >> m_sizeX >> m_sizeY;
    double pasX, pasY;
    fileOri >> pasX >> pasY;

    if(pasX != pasY)
    {
        ostringstream oss;
        oss << "Unsupported orientaton: X and Y steps are different!" << endl;
        oss << "File : " <<__FILE__ << endl;
        oss << "Line : " << __LINE__ << endl;
        oss << "Function : " << __FUNCTION__ << endl;
        throw logic_error( oss.str() );
    }
    else
	m_step = pasX;

    fileOri.close();

}

void orientation_2d::read_ori_from_tfw_file(const string &filename)
{
    if ( !boost::filesystem::exists(filename) )
    {
        ostringstream oss;
        oss << "TFW file does not exist: "<<filename<< " ! " << endl;
        oss << "File : " <<__FILE__ << endl;
        oss << "Line : " << __LINE__ << endl;
        oss << "Function : " << __FUNCTION__ << endl;
        throw logic_error( oss.str() );
    }

    ifstream fileTFW(filename.c_str() , ifstream::in );

#ifndef WIN32
    //Renvoit des exceptions en cas d'erreur de lecture/ouverture de fichier
    // Ca, sous windows, ca me lance direct une exception ...
    fileTFW.exceptions ( ifstream::eofbit | ifstream::failbit | ifstream::badbit );
#endif

    // On suppose un formattage comme suit :
    // Pas en X (>0 - en m)
    // Sais pas
    // Sais pas
    // Pas en Y (<0 - en m)
    // OriginX (en m)
    // OriginY (en m)

    double temp, pasX, pasY;
    fileTFW >> pasX;
    //	pasX *= 1000.;
    fileTFW >> temp;
    fileTFW >> temp;
    fileTFW >> pasY;
    pasY *= -1.;
    fileTFW >> m_originX;
    //	m_originX *= 1000.;
    fileTFW >> m_originY;
    //	m_originY *= 1000.;

    if(pasX != pasY)
    {
        ostringstream oss;
        oss << "Unsupported orientaton: X and Y steps are different!" << endl;
        oss << "File : " <<__FILE__ << endl;
        oss << "Line : " << __LINE__ << endl;
        oss << "Function : " << __FUNCTION__ << endl;
        throw logic_error( oss.str() );
    }
    else
        m_step = pasX;

    fileTFW.close();
}

ostream& operator<< (ostream &o, const orientation_2d &ori)
{
    o << "CARTO\n";
    o << ori.m_originX   << "\t" << ori.m_originY << endl;
    o << ori.m_zoneCarto << endl;
    o << ori.m_sizeX     << "\t" << ori.m_sizeY   << endl;
    o << ori.m_step      << "\t" << ori.m_step    << endl;
    return o;
}
