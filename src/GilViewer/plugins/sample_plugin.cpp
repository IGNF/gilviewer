#include "sample_plugin.hpp"
#include "plugin_base.hpp"

#include "../gui/layer_control.hpp"
#include "../gui/panel_manager.hpp"

#include "../layers/image_types.hpp"
#include "../layers/image_layer.hpp"

#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>
#include <boost/gil/image_view_factory.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <iostream>

using namespace std;
using namespace boost;
using namespace boost::gil;

IMPLEMENT_PLUGIN(rotate_image_plugin)

struct rotate_image_plugin_functor
{
    typedef void result_type;

    rotate_image_plugin_functor() {}

    template <typename ViewType>
    result_type operator()(ViewType& v)
    {
        m_dest = rotated90cw_view(v);

        any_variant_view_type m_any_variant(m_dest);
        variant_view_type m_variant(m_any_variant);
        shared_ptr<variant_view_type> m_variant_ptr = shared_ptr<variant_view_type>(new variant_view_type(m_variant));

        std::vector<panel_viewer*> v_pv = panel_manager::instance()->panels_list();
        if(v_pv.empty())
        {
            #if wxMINOR_VERSION < 9
            ::wxLogMessage(wxT("[sample_plugin::process] Error: empty panel manager list"));
            #else
            wxLogMessage(wxT("[sample_plugin::process] Error: empty panel manager list"));
            #endif 
            return;
        }
        layer_control *lc = v_pv[0]->layercontrol();

        // @todo: decent name...
        lc->add_layer( image_layer::create_image_layer( m_variant_ptr, "iuyiu") );
    }

    dynamic_xy_step_type<any_view_type>::type m_dest;
};

struct rotate_image_plugin_visitor : public boost::static_visitor<void>
{
    typedef void result_type;

    template <typename ViewType>
    result_type operator()(ViewType& v) { apply_operation(v, rotate_image_plugin_functor()); }
};

rotate_image_plugin::rotate_image_plugin(const wxString &title) : plugin_base(title) {}

void rotate_image_plugin::process()
{
        #if wxMINOR_VERSION < 9
	::wxLogMessage(wxT("[sample_plugin::process] Starting..."));
        #else
	wxLogMessage(wxT("[sample_plugin::process] Starting..."));
        #endif 
    std::vector<panel_viewer*> v_pv = panel_manager::instance()->panels_list();
    if(v_pv.empty())
    {
        #if wxMINOR_VERSION < 9
        ::wxLogMessage(wxT("[sample_plugin::process] Error: empty panel manager list"));
        #else
        wxLogMessage(wxT("[sample_plugin::process] Error: empty panel manager list"));
        #endif 
        return;
    }

    layer_control *lc = v_pv[0]->layercontrol();
    layer_control::iterator itb=lc->begin(), ite=lc->end();
    unsigned int i=0;

    for(;itb!=ite;++itb,++i)
    {
        bool selected = lc->rows()[i]->m_nameStaticText->selected();
        if(selected)
            cout << "Layer " << (*itb)->filename() << " is selected" << endl;
        else
            cout << "Layer " << (*itb)->filename() << " is not selected" << endl;

        shared_ptr<image_layer> imagelayer = dynamic_pointer_cast<image_layer>((*itb));

        if(selected && imagelayer)
        {
            cout << "Layer " << (*itb)->filename() << " is an image layer" << endl;

            rotate_image_plugin_visitor spv;
            imagelayer->variant_view()->value.apply_visitor( spv );
        }
    }
}

wxWindow* rotate_image_plugin::gui()
{
    #if wxMINOR_VERSION < 9
    ::wxLogMessage(wxT("[sample_plugin::gui] start"));
    #else
    wxLogMessage(wxT("[sample_plugin::gui] start"));
    #endif 
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer1;
    bSizer1 = new wxBoxSizer( wxVERTICAL );

    wxButton* m_button1 = new wxButton( this, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
    bSizer1->Add( m_button1, 0, wxALL, 5 );

    this->SetSizer( bSizer1 );
    this->Layout();

    m_button1->Connect( wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(rotate_image_plugin::on_button_90cw), NULL, this);

    Show(true);

    return this;
}

void rotate_image_plugin::on_button_90cw(wxCommandEvent& e)
{
    #if wxMINOR_VERSION < 9
    ::wxLogMessage(wxT("[sample_plugin::OnButton] start"));
    #else
    wxLogMessage(wxT("[sample_plugin::OnButton] start"));
    #endif 
    process();
}
