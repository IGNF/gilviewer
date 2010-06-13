#include "sample_plugin.hpp"
#include "plugin_base.hpp"

#include <iostream>

using namespace std;

IMPLEMENT_PLUGIN(sample_plugin)

sample_plugin::sample_plugin() : plugin_base() {}

void sample_plugin::process()
{
    cout << "sample_plugin::process()" << endl;
}

wxWindow* sample_plugin::gui()
{
    return NULL;
}

void sample_plugin::OnButton(wxCommandEvent& e)
{
        process();
}
