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
#ifndef GILVIEWER_FILE_IO_IMAGEEXR_HPP
#define GILVIEWER_FILE_IO_IMAGEEXR_HPP

#include "GilViewer/io/gilviewer_file_io_image.hpp"
#include "GilViewer/plugins/plugin_base.hpp"

#include <ImfOutputFile.h>
#include <ImfPixelType.h>
#include <ImfInputFile.h>
#include <ImfChannelList.h>
#include <ImathBox.h>
#include <ImathFun.h>
#include <ImfHeader.h>
#include <half.h>
#include <ImfArray.h>
#include <ImfXdr.h>
#include <ImfPreviewImageAttribute.h>
#include <ImfChannelListAttribute.h>
#include <Iex.h>


class gilviewer_file_io_exr : public gilviewer_file_io
{
public:
    virtual ~gilviewer_file_io_exr() {}

    virtual boost::shared_ptr<layer> load(const std::string &filename, const std::ptrdiff_t top_left_x=0, const std::ptrdiff_t top_left_y=0, const std::ptrdiff_t dim_x=0, const std::ptrdiff_t dim_y=0);
    virtual void save(boost::shared_ptr<layer> layer, const std::string &filename);

    virtual std::string build_and_get_infos(const std::string &filename);
    void ReadEXRHeader(const std::string adresse, int& width, int& height);

    virtual bool Register(gilviewer_io_factory *factory);
    friend boost::shared_ptr<gilviewer_file_io_exr> create_gilviewer_file_io_exr();
private:
    std::string exr_channels;
    Imf::PixelType exr_pixel_type;
    int components;

};

#endif // GILVIEWER_FILE_IO_IMAGEEXR_HPP
