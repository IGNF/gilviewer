#ifndef GILVIEWER_FILE_IO_RAW_HPP
#define GILVIEWER_FILE_IO_RAW_HPP

#include <boost/gil/extension/io_new/raw_all.hpp>
#include "GilViewer/io/gilviewer_file_io_image.hpp"

class gilviewer_file_io_raw : public gilviewer_file_io_image<boost::gil::raw_tag>
{
public:
    virtual ~gilviewer_file_io_raw() {}

    virtual std::string get_infos(const std::string &filename);
    virtual void save(boost::shared_ptr<layer> layer, const std::string &filename);

    virtual bool Register(gilviewer_io_factory *factory);
};

#endif // GILVIEWER_FILE_IO_RAW_HPP
