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

#ifndef GILVIEWER_IO_FACTORY_HPP
#define GILVIEWER_IO_FACTORY_HPP

#include "GilViewer/tools/pattern_factory.hpp"
#include "gilviewer_file_io.hpp"

struct factory_key {
    std::string extension;
    std::string family;
    std::string group;

    factory_key(const std::string& e, const std::string& f="", const std::string& g="") : extension(e), family(f), group(g) {}
    inline bool operator<(const factory_key& k) const { return extension<k.extension; }
};

class gilviewer_io_factory : public PatternFactory< gilviewer_file_io,
                                          factory_key,
                                          boost::function< boost::shared_ptr<gilviewer_file_io> () >,
                                          boost::shared_ptr<gilviewer_file_io>
                                        >
{
public:
    inline bool insert(const std::string& e, const std::string& f, const std::string& g, product_creator_type creator)
    {
        return Register(factory_key(e,f,g),creator);
    }
};

void register_all_image_file_formats(gilviewer_io_factory *factory);
void register_all_vector_file_formats(gilviewer_io_factory *factory);
void register_all_file_formats(gilviewer_io_factory *factory);

#endif // GILVIEWER_IO_FACTORY_HPP
