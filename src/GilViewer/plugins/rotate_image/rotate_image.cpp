#include "rotate_image.hpp"
#include "GilViewer/plugins/plugin_base.hpp"

#include "GilViewer/gui/layer_control.hpp"
#include "GilViewer/gui/panel_manager.hpp"

#include "GilViewer/layers/image_types.hpp"
#include "GilViewer/layers/image_layer.hpp"

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

    rotate_image_plugin_functor(const shared_ptr<image_layer> &layer) : m_layer(layer) {}

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
            GILVIEWER_LOG_ERROR("[sample_plugin::process] Error: empty panel manager list")
            return;
        }
        layer_control *lc = v_pv[0]->layercontrol();

        lc->add_layer(image_layer::create_image_layer(m_layer->image(),
                                                      m_layer->name()+"_rotated",
                                                      m_layer->filename(),
                                                      m_variant_ptr) );
    }

    dynamic_xy_step_type<any_view_type>::type m_dest;
    const shared_ptr<image_layer> m_layer;
};

struct rotate_image_plugin_visitor : public boost::static_visitor<void>
{
    typedef void result_type;
    rotate_image_plugin_visitor(const shared_ptr<image_layer> &layer) : m_functor(layer) {}

    template <typename ViewType>
    result_type operator()(ViewType& v) { apply_operation(v, m_functor); }

    rotate_image_plugin_functor m_functor;
};

rotate_image_plugin::rotate_image_plugin(const wxString &title) : wx_plugin_base(title) {}

void rotate_image_plugin::process()
{
    GILVIEWER_LOG_MESSAGE("[rotate_image_plugin::process] Starting...")
    std::vector<panel_viewer*> v_pv = panel_manager::instance()->panels_list();
    if(v_pv.empty())
    {
        GILVIEWER_LOG_ERROR("[rotate_image_plugin::process] Empty panel manager list")
        return;
    }

    layer_control *lc = v_pv[0]->layercontrol();
    layer_control::iterator itb=lc->begin(), ite=lc->end();
    unsigned int i=0;

    if(!lc->rows().size())
    {
        GILVIEWER_LOG_MESSAGE("[rotate_image_plugin::process] No layer loaded...")
        return;
    }

    for(;itb!=ite;++itb,++i)
    {
        bool selected = lc->rows()[i]->m_nameStaticText->selected();
        ostringstream os;
        os << "Layer " << (*itb)->filename();
        if(selected)
            os << " is selected";
        else
            os << " is not selected";
        GILVIEWER_LOG_MESSAGE(oss.str())

        shared_ptr<image_layer> imagelayer = dynamic_pointer_cast<image_layer>((*itb));

        if(selected && imagelayer)
        {
            ostringstream os;
            os << "Layer " << (*itb)->filename() << " is an image layer and is selected --> applying rotate visitor";
            GILVIEWER_LOG_MESSAGE(os.str())

            rotate_image_plugin_visitor spv(imagelayer);
            imagelayer->variant_view()->value.apply_visitor( spv );
        }
    }
}

void rotate_image_plugin::show(wxCommandEvent&e)
{
    GILVIEWER_LOG_MESSAGE("[rotate_image_plugin::show] start")
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer1;
    bSizer1 = new wxBoxSizer( wxVERTICAL );

    wxButton* m_button1 = new wxButton( this, wxID_ANY, wxT("Rotate!"), wxDefaultPosition, wxDefaultSize, 0 );
    bSizer1->Add( m_button1, 0, wxALL, 5 );

    this->SetSizer( bSizer1 );
    this->Layout();

    m_button1->Connect( wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(rotate_image_plugin::on_button_90cw), NULL, this);

    Show(true);
}

void rotate_image_plugin::on_button_90cw(wxCommandEvent& e)
{
    GILVIEWER_LOG_MESSAGE("[rotate_image_plugin::on_button_90cw] start")
    process();
}
