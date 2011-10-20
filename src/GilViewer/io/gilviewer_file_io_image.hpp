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
#ifndef GILVIEWER_FILE_IO_IMAGE_HPP
#define GILVIEWER_FILE_IO_IMAGE_HPP

#include "gilviewer_file_io.hpp"

#include <boost/gil/utilities.hpp>
#include <boost/filesystem/operations.hpp>

#include <boost/variant/static_visitor.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <boost/gil/extension/io_new/detail/read_image.hpp>
#include <boost/gil/extension/io_new/detail/read_image_info.hpp>

//#include <xutility>
#if _WINDOWS
#   include <boost/config/platform/win32.hpp>
#endif

#include "../layers/image_layer.hpp"
#include "../layers/image_types.hpp"
#include "../convenient/macros_gilviewer.hpp"
#include "../convenient/utils.hpp"

template <class TagType> struct write_gil_view_visitor : public boost::static_visitor<>
{
    write_gil_view_visitor(const std::string& filename) : m_filename(filename), m_tag(TagType()) {}

    template <typename ViewType>
    result_type operator()(const ViewType& v) const { write_view( m_filename , v, m_tag ); }

private:
    std::string m_filename;
    TagType m_tag;
};

class layer;

class gilviewer_file_io_image : public gilviewer_file_io
{
public:
    virtual ~gilviewer_file_io_image() {}

    typedef boost::gil::point2<std::ptrdiff_t> point_t;
    template <class TagType> boost::shared_ptr<layer> load_gil_image(const std::string &filename, const point_t &top_left, const point_t &dim)
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

        image_layer::image_ptr image(new image_layer::image_t);

        image_read_info<TagType> info = read_image_info(filename , TagType() );
        point_t origin( max(top_left.x, (ptrdiff_t)0), max(top_left.y, (ptrdiff_t)0) );
        point_t size = dim;
        if(dim.x==-1 && dim.y==-1)
            size = point_t(info._width, info._height);
        image_read_settings<TagType> settings(origin, size );

        try
        {
            read_image(filename
                       , image->value
                       , settings);
        }
            catch( const std::exception &e )
        {
            GILVIEWER_LOG_EXCEPTION("Image read error: " + filename);
            return layer::ptrLayerType();
        }

        layer::ptrLayerType layer(new image_layer(image, BOOST_FILESYSTEM_STRING(path.stem()), path.string()));
        layer->add_orientation(filename);
        layer->infos( build_and_get_infos(filename) );

        return layer;
    }

    template <class TagType> void save_gil_view(boost::shared_ptr<layer> layer, const std::string &filename)
    {
        using namespace boost;
        using namespace std;

        shared_ptr<image_layer> imagelayer = dynamic_pointer_cast<image_layer>(layer);
        if(!imagelayer)
            throw invalid_argument("Bad layer type (not an image layer)!\n");

        write_gil_view_visitor<TagType> writer(filename);
        try
        {
            apply_visitor( writer, imagelayer->variant_view()->value );
        }
        catch( const std::exception &e )
        {
            GILVIEWER_LOG_EXCEPTION("Image write error: " + filename);
        }
    }
};

#endif // GILVIEWER_FILE_IO_IMAGE_HPP
