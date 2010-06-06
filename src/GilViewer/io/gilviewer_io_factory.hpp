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

//#include <boost/thread/detail/singleton.hpp>

#include "../tools/PatternSingleton.hpp"
#include "../tools/PatternFactory.hpp"
#include "gilviewer_file_io.hpp"

//typedef boost::detail::thread::singleton<PatternFactory<boost::shared_ptr<gilviewer_file_io> > > gilviewer_io_factory;
typedef PatternSingleton< PatternFactory< gilviewer_file_io,
                                          std::string,
                                          boost::function< boost::shared_ptr<gilviewer_file_io> () >,
                                          boost::shared_ptr<gilviewer_file_io>
                                        >
                        > gilviewer_io_factory;

void register_all_image_file_formats();
void register_all_vector_file_formats();
void register_all_file_formats();

#endif // GILVIEWER_IO_FACTORY_HPP
