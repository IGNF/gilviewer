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
        GILVIEWER_LOG_WARNING("File " + filename + " does not exist");
        return false;
    }

    string basename = boost::filesystem::basename(filename);
    string path = boost::filesystem::path(filename).branch_path().string();

    if( read_ori_from_ori_file(path+"/"+basename+".ori") ) return true;
    if( read_ori_from_ori_file(path+"/"+basename+".ORI") ) return true;
    if( read_ori_from_tfw_file(path+"/"+basename+".tfw") ) return true;
    if( read_ori_from_tfw_file(path+"/"+basename+".TFW") ) return true;
    if( read_ori_from_tfw_file(path+"/"+basename+".jgw") ) return true;
    if( read_ori_from_tfw_file(path+"/"+basename+".JGW") ) return true;
    if( read_ori_from_hdr_file(path+"/"+basename+".hdr") ) return true;
    if( read_ori_from_hdr_file(path+"/"+basename+".HDR") ) return true;
    return false;
}

bool orientation_2d::read_ori_from_ori_file(const string &filename)
{
    if ( !boost::filesystem::exists(filename) ) return false;
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


#include <boost/algorithm/string/case_conv.hpp>
bool orientation_2d::read_ori_from_hdr_file(const string &filename)
{
    if ( !boost::filesystem::exists(filename) ) return false;
    ifstream fic(filename.c_str() , ifstream::in );

    if (!fic.good()) return false;

    unsigned int nc, nl;
    double pas_x, pas_y;
    double position_origine_x, position_origine_y;

    unsigned short good = 0;

    while (fic.good())
    {
        std::string s;
        fic >> s;
        if (!fic.good())
            break;
        boost::algorithm::to_upper(s);

        //			if ( s == "MAPUNITS") { fic >> s; }
        //			else if( s == "METERS") { fic >> s; }
        //			else
        if (s == "ULXMAP")
        {
            fic >> position_origine_x;
            good |= 1;
        }
        else if (s == "ULYMAP")
        {
            fic >> position_origine_y;
            good |= 2;
        }
        else if (s == "XDIM")
        {
            fic >> pas_x;
            good |= 4;
        }
        else if (s == "YDIM")
        {
            fic >> pas_y;
            good |= 8;
        }
        else if (s == "NROWS")
        {
            fic >> nc;
            good |= 16;
        }
        else if (s == "NCOLS")
        {
            fic >> nl;
            good |= 32;
        }
        else if (s == "PROJECTION")
        {
            std::string systeme;
            fic >> systeme;
            //std::cout << "Fichier HDR, projection lue : " << systeme << std::endl;

            if (systeme == "LAMBERT1")
                m_zoneCarto = 1;
            else if (systeme == "LAMBERT2")
                m_zoneCarto = 2;
            else if (systeme == "LAMBERT3")
                m_zoneCarto = 3;
            else if (systeme == "LAMBERT4")
                m_zoneCarto = 4;
            else if (systeme == "LAMBERT93")
                m_zoneCarto = 93;
            else
                GILVIEWER_LOG_ERROR("Projection non supportée : " << systeme);
        }

        getline(fic, s);
    }

    if (good != 63) return false;

    m_originX = position_origine_x + 0.5 * pas_x;
    m_originY = position_origine_y - 0.5 * pas_y;


    m_sizeX = nc;
    m_sizeY = nl;
    if(pas_x != pas_y)
    {
        GILVIEWER_LOG_ERROR("Unsupported orientaton: X and Y steps are different")
                return false;
    }
    else
        m_step = pas_x;

    fic.close();
    return true;
}


bool orientation_2d::read_ori_from_tfw_file(const string &filename)
{
    if ( !boost::filesystem::exists(filename) ) return false;
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
