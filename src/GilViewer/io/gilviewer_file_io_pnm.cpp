#include "gilviewer_file_io_pnm.hpp"
#include "gilviewer_io_factory.hpp"

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

string gilviewer_file_io_pnm::get_infos(const std::string &filename)
{
    if(!_info_read)
        _info = read_image_info(filename, pnm_tag());
    ostringstream infos_str;
    infos_str << "Type: P" << _info._type;
    switch(_info._type)
    {
    case 1 : infos_str << " (Mono ASCII)\n"; break;
    case 2 : infos_str << " (Gray ASCII)\n"; break;
    case 3 : infos_str << " (Color ASCII)\n"; break;
    case 4 : infos_str << " (Mono Binary)\n"; break;
    case 5 : infos_str << " (Gray Binary)\n"; break;
    case 6 : infos_str << " (Color Binary)\n"; break;
    default: infos_str << " (\?\?\?)\n"; break;
    }
    infos_str << "Dimensions: " << _info._width << "x" << _info._height << "\n";
    infos_str << "Maximum value: " << _info._max_value << "\n";
    return infos_str.str();
}

boost::shared_ptr<gilviewer_file_io_pnm> create_gilviewer_file_io_pnm()
{
    return boost::shared_ptr<gilviewer_file_io_pnm>(new gilviewer_file_io_pnm());
}

bool gilviewer_file_io_pnm::Register(gilviewer_io_factory *factory)
{
    factory->insert("pnm", "Image","PNM", create_gilviewer_file_io_pnm);
    factory->insert("pbm", "Image","PNM", create_gilviewer_file_io_pnm);
    factory->insert("pgm", "Image","PNM", create_gilviewer_file_io_pnm);
    factory->insert("ppm", "Image","PNM", create_gilviewer_file_io_pnm);
    return true;
}
