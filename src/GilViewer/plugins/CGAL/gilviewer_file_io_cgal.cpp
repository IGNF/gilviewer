#include "gilviewer_file_io_cgal.hpp"

#include <boost/filesystem/convenience.hpp>
#include <boost/variant/get.hpp>

#include "GilViewer/io/gilviewer_io_factory.hpp"
#include "GilViewer/plugins/CGAL/cgal_vector_layer.hpp"

using namespace boost;
using namespace std;

#include <iostream>

shared_ptr<layer> gilviewer_file_io_cgal::load(const string &filename, const ptrdiff_t top_left_x, const ptrdiff_t top_left_y, const ptrdiff_t dim_x, const ptrdiff_t dim_y)
{
    return shared_ptr<layer>(new cgal_vector_layer(filesystem::basename(filename),filename));
}

void gilviewer_file_io_cgal::save(shared_ptr<layer> layer, const string &filename)
{
    shared_ptr<cgal_vector_layer> cgal_layer = dynamic_pointer_cast<cgal_vector_layer>(layer);
    if(!cgal_layer)
        throw invalid_argument("Bad layer type!\n");
    cgal_layer->save(filename);
}

boost::shared_ptr<gilviewer_file_io_cgal> create_gilviewer_file_io_cgal()
{
    return shared_ptr<gilviewer_file_io_cgal>(new gilviewer_file_io_cgal());
}

bool gilviewer_file_io_cgal::Register(gilviewer_io_factory *factory)
{
    factory->insert("cgal", "Vector", "CGAL", create_gilviewer_file_io_cgal);
    return true;
}

#include "GilViewer/plugins/plugin_base.hpp"
IMPLEMENT_PLUGIN(gilviewer_file_io_cgal);
