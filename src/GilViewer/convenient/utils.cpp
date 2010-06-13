#include "utils.hpp"

using namespace std;

vector<string> ogr_available_formats_extensions()
{
    vector<string> extensions;
    extensions.push_back("shp");
    extensions.push_back("SHP");
    extensions.push_back("kml");
    extensions.push_back("KML");
    return extensions;
}

vector<string> simple_available_formats_extensions()
{
    vector<string> extensions;
    extensions.push_back("txt");
    extensions.push_back("TXT");
    extensions.push_back("xml");
    extensions.push_back("XML");
    extensions.push_back("bin");
    extensions.push_back("BIN");
    return extensions;
}
