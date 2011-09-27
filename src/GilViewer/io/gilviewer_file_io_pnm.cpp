#include "gilviewer_file_io_pnm.hpp"
#include <boost/gil/extension/io_new/pnm_all.hpp>
#include "gilviewer_io_factory.hpp"

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

shared_ptr<layer> gilviewer_file_io_pnm::load(const string &filename, const ptrdiff_t top_left_x, const ptrdiff_t top_left_y, const ptrdiff_t dim_x, const ptrdiff_t dim_y)
{
    return load_gil_image<pnm_tag>(filename, point_t(top_left_x, top_left_y), point_t(dim_x, dim_y));
}

void gilviewer_file_io_pnm::save(shared_ptr<layer> layer, const string &filename)
{
    save_gil_view<pnm_tag>(layer, filename);
}

string gilviewer_file_io_pnm::build_and_get_infos(const std::string &filename)
{
    image_read_info< pnm_tag > info = read_image_info(filename, pnm_tag());
    ostringstream infos_str;
    infos_str << "Type: P" << info._type;
    switch(info._type)
    {
    case 1 : infos_str << " (Mono ASCII)\n"; break;
    case 2 : infos_str << " (Gray ASCII)\n"; break;
    case 3 : infos_str << " (Color ASCII)\n"; break;
    case 4 : infos_str << " (Mono Binary)\n"; break;
    case 5 : infos_str << " (Gray Binary)\n"; break;
    case 6 : infos_str << " (Color Binary)\n"; break;
    default: infos_str << " (\?\?\?)\n"; break;
    }
    infos_str << "Dimensions: " << info._width << "x" << info._height << "\n";
    infos_str << "Maximum value: " << info._max_value << "\n";
    return infos_str.str();
}

shared_ptr<gilviewer_file_io_pnm> create_gilviewer_file_io_pnm()
{
    return shared_ptr<gilviewer_file_io_pnm>(new gilviewer_file_io_pnm());
}

bool gilviewer_file_io_pnm::Register(gilviewer_io_factory *factory)
{
    factory->Register("pnm", create_gilviewer_file_io_pnm);
    factory->Register("pbm", create_gilviewer_file_io_pnm);
    factory->Register("pgm", create_gilviewer_file_io_pnm);
    factory->Register("ppm", create_gilviewer_file_io_pnm);
    pair<string,string> familly_description = make_pair<string,string>("Image files","PNM images");
    factory->metadata().insert( make_pair<string,pair<string,string> >( "pnm", familly_description ) );
    factory->metadata().insert( make_pair<string,pair<string,string> >( "pbm", familly_description ) );
    factory->metadata().insert( make_pair<string,pair<string,string> >( "pgm", familly_description ) );
    factory->metadata().insert( make_pair<string,pair<string,string> >( "ppm", familly_description ) );
    return true;
}
