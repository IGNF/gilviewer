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
#ifndef GILVIEWER_FILE_IO_HPP
#define GILVIEWER_FILE_IO_HPP

#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/gil/extension/io_new/jpeg_all.hpp>
#include <boost/gil/extension/io_new/png_all.hpp>
#include <boost/gil/extension/io_new/tiff_all.hpp>

#include <string>

#include "../layers/image_layer.hpp"
#include "../layers/image_types.hpp"
#include "../tools/error_logger.hpp"
#include "../convenient/macros_gilviewer.hpp"

class layer;

class gilviewer_file_io
{
public:
    virtual ~gilviewer_file_io() {}

    virtual boost::shared_ptr<layer> load(const std::string &filename) { return boost::shared_ptr<layer>(); }
    template <class TagType> boost::shared_ptr<layer> load_gil_image(const std::string &filename)
    {
        using namespace boost;
        using namespace boost::gil;
        using namespace boost::filesystem;
        using namespace std;

        if ( !exists(filename) )
        {
            GILVIEWER_LOG_ERROR("File " + filename + " does not exist");
            return layer::ptrLayerType();
        }

        path path(system_complete(filename));
        string ext(path.extension());

        image_layer::image_ptr image(new image_layer::image_t);

        try
        {
            read_image(filename
                       , image->value
                       , TagType());
        }
            catch( const std::exception &e )
        {
            GILVIEWER_LOG_EXCEPTION("Image read error: " + filename);
            return layer::ptrLayerType();
        }

        layer::ptrLayerType layer(new image_layer(image, path.stem(), path.string()));
        layer->add_orientation(filename);

        layer->infos( build_and_get_infos(filename) );

        return layer;
    }

    virtual void save(boost::shared_ptr<layer> layer, const std::string &filename)=0;

    virtual std::string build_and_get_infos(const std::string &filename) { return ""; }

protected:
    gilviewer_file_io() : m_familly(""), m_description("") {}
    /// This attribute describes the main familly of the reader (e.g. vector, image, ...)
    std::string m_familly;
    /// This attribute gives the individual description of the reader (e.g. TIFF images, SHP files ...)
    std::string m_description;
};

#endif // GILVIEWER_FILE_IO_HPP
