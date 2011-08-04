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
#include "../convenient/macros_gilviewer.hpp"

using namespace std;

bool orientation_2d::read_ori_from_image_file(const string &filename)
{
    if ( !boost::filesystem::exists(filename) )
    {
        GILVIEWER_LOG_ERROR("File " + filename + " does not exist")
        return false;
    }

    string basename = boost::filesystem::basename(filename);
    string path = boost::filesystem::path(filename).branch_path().string();

    if( !read_ori_from_ori_file(path+"/"+basename+".ori") )
    {
        if( !read_ori_from_tfw_file(path+"/"+basename+".tfw") &&
            !read_ori_from_tfw_file(path+"/"+basename+".jgw") )
                return false;
        return true;
    }
    else
        return true;

}

bool orientation_2d::read_ori_from_ori_file(const string &filename)
{
    if ( !boost::filesystem::exists(filename) )
    {
        GILVIEWER_LOG_ERROR("File " + filename + " does not exist")
        return false;
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
        GILVIEWER_LOG_ERROR("Bad ORI format (not CARTO)")
        return false;
    }

    fileOri >> m_originX >> m_originY;
    fileOri >> m_zoneCarto;
    fileOri >> m_sizeX >> m_sizeY;
    double pasX, pasY;
    fileOri >> pasX >> pasY;

    if(pasX != pasY)
    {
        GILVIEWER_LOG_ERROR("Unsupported orientaton: X and Y steps are different")
        return false;
    }
    else
	m_step = pasX;

    fileOri.close();
    return true;
}

bool orientation_2d::read_ori_from_tfw_file(const string &filename)
{
    if ( !boost::filesystem::exists(filename) )
    {
        GILVIEWER_LOG_ERROR("File " + filename + " does not exist")
        return false;
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
        GILVIEWER_LOG_ERROR("Unsupported orientaton: X and Y steps are different")
        return false;
    }
    else
        m_step = pasX;

    fileTFW.close();
    return true;
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
