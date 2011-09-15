
#include "gilviewer_file_io_pk1.hpp"

#include <boost/filesystem/convenience.hpp>
#include <boost/variant/get.hpp>


#include "gilviewer_io_factory.hpp"



#include "../layers/simple_vector_layer.hpp"

using namespace boost;
using namespace std;

#include <iostream>
#include <sstream>
#include <fstream>

shared_ptr<layer> gilviewer_file_io_pk1::load(const string &filename, const ptrdiff_t top_left_x, const ptrdiff_t top_left_y, const ptrdiff_t dim_x, const ptrdiff_t dim_y)
{
    shared_ptr<simple_vector_layer> shr_ptr=shared_ptr<simple_vector_layer>(new simple_vector_layer(filesystem::basename(filename)));
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

void gilviewer_file_io_pk1::save(shared_ptr<layer> layer, const string &filename)
{
    std::cout<<"TO DO";
}

boost::shared_ptr<gilviewer_file_io_pk1> create_gilviewer_file_io_pk1()
{
    return shared_ptr<gilviewer_file_io_pk1>(new gilviewer_file_io_pk1());
}

bool gilviewer_file_io_pk1::Register()
{
    gilviewer_io_factory::instance()->Register("pk1", create_gilviewer_file_io_pk1);
    pair<string,string> familly_description = make_pair<string,string>("Images Measurments","PK1 files");
    pair< string, pair<string,string> > to_insert = make_pair< string, pair<string,string> >( "pk1", familly_description );
    gilviewer_io_factory::instance()->metadata().insert( to_insert );
    return true;
}

bool register_pk1_ok = gilviewer_file_io_pk1::Register();


