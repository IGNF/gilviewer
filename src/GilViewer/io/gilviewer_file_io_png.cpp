#include "gilviewer_file_io_png.hpp"
#include <boost/gil/extension/io_new/png_all.hpp>
#include "gilviewer_io_factory.hpp"

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

shared_ptr<layer> gilviewer_file_io_png::load(const string &filename, const ptrdiff_t top_left_x, const ptrdiff_t top_left_y, const ptrdiff_t dim_x, const ptrdiff_t dim_y)
{
    return load_gil_image<png_tag>(filename, point_t(top_left_x, top_left_y), point_t(dim_x, dim_y));
}

void gilviewer_file_io_png::save(shared_ptr<layer> layer, const string &filename)
{
    save_gil_view<png_tag>(layer, filename);
}

string gilviewer_file_io_png::build_and_get_infos(const std::string &filename)
{
    // TODO
    return "TODO";
}

shared_ptr<gilviewer_file_io_png> create_gilviewer_file_io_png()
{
    return shared_ptr<gilviewer_file_io_png>(new gilviewer_file_io_png());
}

bool gilviewer_file_io_png::Register()
{
    gilviewer_io_factory::instance()->Register("png", create_gilviewer_file_io_png);
    pair<string,string> familly_description = make_pair<string,string>("Image files","PNG images");
    pair< string, pair<string,string> > to_insert = make_pair< string, pair<string,string> >( "png", familly_description );
    gilviewer_io_factory::instance()->metadata().insert( to_insert );
    return true;
}

bool register_png_ok = gilviewer_file_io_png::Register();
