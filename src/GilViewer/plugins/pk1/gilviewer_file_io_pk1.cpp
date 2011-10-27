
#include "gilviewer_file_io_pk1.hpp"

#include <boost/filesystem/convenience.hpp>
#include <boost/variant/get.hpp>


#include "GilViewer/io/gilviewer_io_factory.hpp"
#include "GilViewer/layers/simple_vector_layer.hpp"

using namespace boost;
using namespace std;

#include <iostream>
#include <sstream>
#include <fstream>

#include "GilViewer/plugins/plugin_base.hpp"
IMPLEMENT_PLUGIN(gilviewer_file_io_pk1);

boost::shared_ptr<layer> gilviewer_file_io_pk1::load(const string &filename, const ptrdiff_t top_left_x, const ptrdiff_t top_left_y, const ptrdiff_t dim_x, const ptrdiff_t dim_y)
{
    boost::shared_ptr<simple_vector_layer> shr_ptr=boost::shared_ptr<simple_vector_layer>(new simple_vector_layer(filesystem::basename(filename)));
    std::ifstream file(filename.c_str());
		unsigned int offset=3;
    while(!file.eof()){
        std::string ligne;
        std::getline(file,ligne);
        std::stringstream ss;
        ss<< ligne;
		if(!ss.good())
			continue;
        std::string nom;
        double x,y;
		ss>>nom>>x>>y;
         shr_ptr->add_line(x-offset,y,x+offset,y);
         shr_ptr->add_line(x,y-offset,x,y+offset);
         shr_ptr->add_text(x,y,nom);
    }
    return shr_ptr;
}

void gilviewer_file_io_pk1::save(boost::shared_ptr<layer> layer, const string &filename)
{
    std::cout<<"TO DO";
}

boost::shared_ptr<gilviewer_file_io_pk1> create_gilviewer_file_io_pk1()
{
    return boost::shared_ptr<gilviewer_file_io_pk1>(new gilviewer_file_io_pk1());
}

bool gilviewer_file_io_pk1::Register(gilviewer_io_factory *factory)
{
    factory->insert("pk1", "Vector","PK1", create_gilviewer_file_io_pk1);
    return true;
}


