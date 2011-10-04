#include "gilviewer_io_factory.hpp"

#include "gilviewer_file_io_bmp.hpp"
#include "gilviewer_file_io_jpg.hpp"
//#include "gilviewer_file_io_key.hpp"
#include "gilviewer_file_io_pk1.hpp"
#include "gilviewer_file_io_png.hpp"
#include "gilviewer_file_io_pnm.hpp"
#include "gilviewer_file_io_serialization_binary.hpp"
#include "gilviewer_file_io_serialization_txt.hpp"
#include "gilviewer_file_io_serialization_xml.hpp"
#include "gilviewer_file_io_tiff.hpp"

void register_all_image_file_formats(gilviewer_io_factory *factory)
{
    gilviewer_file_io_jpg().Register(factory);
    gilviewer_file_io_png().Register(factory);
    gilviewer_file_io_tiff().Register(factory);
    gilviewer_file_io_bmp().Register(factory);
    gilviewer_file_io_pnm().Register(factory);

#if GILVIEWER_USE_STEREOV2
    gilviewer_file_io_stereoV2_jp2().Register(factory);
#endif
}

void register_all_vector_file_formats(gilviewer_io_factory *factory)
{
    gilviewer_file_io_serialization_txt().Register(factory);
    gilviewer_file_io_serialization_xml().Register(factory);
    gilviewer_file_io_serialization_binary().Register(factory);
    gilviewer_file_io_pk1().Register(factory);
//    gilviewer_file_io_key().Register(factory);
}

void register_all_file_formats(gilviewer_io_factory *factory)
{
    register_all_image_file_formats(factory);
    register_all_vector_file_formats(factory);
}
