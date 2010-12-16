#include "sample_plugin.hpp"
#include "plugin_base.hpp"

#include "../gui/layer_control.hpp"
#include "../gui/panel_manager.hpp"

#include "sample_plugin_functor.hpp"
#include "../layers/image_types.hpp"
#include "../layers/image_layer.hpp"

#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>

#include <iostream>
using namespace std;

IMPLEMENT_PLUGIN(sample_plugin)

#include <boost/variant/static_visitor.hpp>
#include <boost/variant/apply_visitor.hpp>
struct sample_plugin_visitor// : public boost::static_visitor<>
{
    typedef void result_type;
    template <typename ViewType>
            result_type operator()(ViewType& v) const { boost::gil::apply_operation(v, sample_plugin_functor()); }
};

sample_plugin::sample_plugin() : plugin_base() {}

void sample_plugin::process()
{
    layer_control *lc = panel_manager::instance()->panels_list()[0]->layercontrol();
    layer_control::iterator itb=lc->begin(), ite=lc->end();
    unsigned int i=0;
    for(;itb!=ite;++itb,++i)
    {
        bool selected = lc->rows()[i]->m_nameStaticText->selected();
        if(selected)
            cout << "Layer " << (*itb)->filename() << " is selected" << endl;
        else
            cout << "Layer " << (*itb)->filename() << " is not selected" << endl;

        boost::shared_ptr<image_layer> imagelayer = boost::dynamic_pointer_cast<image_layer>((*itb));

        {
            //image_layer::view_t v = *(il->view());
            //image_layer::view_ptr v = il->view();
            //imagelayer->view()->value;
            //boost::gil::view(imagelayer->image()->value);
           // boost::gil::gray8_image_t i;
            //boost::gil::rotated180_view(boost::gil::view(i));
            //rotate_functor()(boost::gil::view(i));

            using namespace boost::gil;

            //apply_operation(imagelayer->view()->value, rotate_functor());
            //boost::gil::apply_operation(imagelayer->variant_view()->value, sample_plugin_functor());
            //boost::gil::apply_operation(boost::gil::view(imagelayer->image()->value), type_channel_functor());
            //boost::apply_visitor( sample_plugin_visitor(), boost::ref(imagelayer->variant_view()->value) );
            //boost::apply_visitor( sample_plugin_visitor(), boost::ref(imagelayer->variant_view()->value) );



            // Works, but commented to avoid compilation warning
            //sample_plugin_visitor spv;



            //imagelayer->variant_view()->value.apply_visitor( spv );
        }
    }
    cout << "sample_plugin::process()" << endl;
}

wxWindow* sample_plugin::gui()
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer1;
    bSizer1 = new wxBoxSizer( wxVERTICAL );

    wxButton* m_button1 = new wxButton( this, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
    bSizer1->Add( m_button1, 0, wxALL, 5 );

    this->SetSizer( bSizer1 );
    this->Layout();

    m_button1->Connect( wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(sample_plugin::OnButton), NULL, this);

    Show(true);

    return this;
}

void sample_plugin::OnButton(wxCommandEvent& e)
{
        process();
}
