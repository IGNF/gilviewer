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
#include "gilviewer_file_io_serialization_xml.hpp"

#include <boost/filesystem/convenience.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <fstream>

#include "gilviewer_io_factory.hpp"

#include "../layers/simple_vector_layer.hpp"

using namespace boost;
using namespace boost::archive;
using namespace std;

boost::shared_ptr<layer> gilviewer_file_io_serialization_xml::load(const string &filename, const ptrdiff_t top_left_x, const ptrdiff_t top_left_y, const ptrdiff_t dim_x, const ptrdiff_t dim_y)
{
    simple_vector_layer simple_layer;
    {
        ifstream ifs(filename.c_str());
        xml_iarchive ia(ifs);
        ia >> BOOST_SERIALIZATION_NVP(simple_layer);
    }
    return boost::shared_ptr<layer>(new simple_vector_layer(simple_layer));
}

void gilviewer_file_io_serialization_xml::save(boost::shared_ptr<layer> layer, const string &filename)
{
    boost::shared_ptr<simple_vector_layer> simple_layer = dynamic_pointer_cast<simple_vector_layer>(layer);
    if(!simple_layer)
        throw invalid_argument("Bad layer type (not a simple_vector_layer)!\n");

    ofstream ofs(filename.c_str());
    {
        xml_oarchive oa(ofs);
        oa << BOOST_SERIALIZATION_NVP(*simple_layer);
    }
}

boost::shared_ptr<gilviewer_file_io_serialization_xml> create_gilviewer_file_io_serialization_xml()
{
    return boost::shared_ptr<gilviewer_file_io_serialization_xml>(new gilviewer_file_io_serialization_xml());
}

bool gilviewer_file_io_serialization_xml::Register(gilviewer_io_factory *factory)
{
    factory->insert("xml", "Vector","Serialization", create_gilviewer_file_io_serialization_xml);
    return true;
}
