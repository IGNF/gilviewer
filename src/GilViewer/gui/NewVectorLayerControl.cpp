/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets.


Homepage: 

	http://code.google.com/p/gilviewer
	
Copyright:
	
	Institut Geographique National (2009)

Authors: 

	Olivier Tournaire, Adrien Chauve

	
	

    GilViewer is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GilViewer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public 
    License along with GilViewer.  If not, see <http://www.gnu.org/licenses/>.
 
***********************************************************************/

#include "GilViewer/gui/NewVectorLayerControl.hpp"

#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/sizer.h>

#include "shapelib/shapefil.h"

#include "GilViewer/gui/LayerControl.hpp"

BEGIN_EVENT_TABLE(NewVectorLayerControl,wxDialog)
END_EVENT_TABLE()

NewVectorLayerControl::NewVectorLayerControl(wxWindow* parent,wxWindowID id, const wxString& title, const wxPoint &pos, const wxSize &size, const long style):
	wxDialog( (wxWindow*)parent, id, title, pos, size, style), m_parent(parent)
{
	m_associationStringType.push_back( "Points" );
	m_associationStringType.push_back( "PointsZ" );
	m_associationStringType.push_back( "PointsM" );
	m_associationStringType.push_back( "Multipoints" );
	m_associationStringType.push_back( "Arcs" );
	m_associationStringType.push_back( "ArcsZ" );
	m_associationStringType.push_back( "ArcsM" );
	m_associationStringType.push_back( "Polygons" );
	m_associationStringType.push_back( "PolygonsZ" );
	m_associationStringType.push_back( "PolygonsM" );

	m_associationMapStringType.insert( std::make_pair( "Points" , SHPT_POINT ) );
	m_associationMapStringType.insert( std::make_pair( "PointsZ" , SHPT_POINTZ ) );
	m_associationMapStringType.insert( std::make_pair( "PointsM" , SHPT_POINTM ) );
	m_associationMapStringType.insert( std::make_pair( "Multipoints" , SHPT_MULTIPOINT ) );
	m_associationMapStringType.insert( std::make_pair( "Arcs" , SHPT_ARC ) );
	m_associationMapStringType.insert( std::make_pair( "ArcsZ" , SHPT_ARCZ ) );
	m_associationMapStringType.insert( std::make_pair( "ArcsM" , SHPT_ARCM ) );
	m_associationMapStringType.insert( std::make_pair( "Polygons" , SHPT_POLYGON ) );
	m_associationMapStringType.insert( std::make_pair( "PolygonsZ" , SHPT_POLYGONZ ) );
	m_associationMapStringType.insert( std::make_pair( "PolygonsM" , SHPT_POLYGONM ) );

	wxColour bgcolor( 220, 220, 220);
	SetBackgroundColour(bgcolor);
	ClearBackground();

	wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer *nameSizer = new wxStaticBoxSizer(wxHORIZONTAL,this,_("Layer name"));
	sizer->Add(nameSizer, 0, wxEXPAND|wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5);
	m_textName = new wxTextCtrl(this,wxID_ANY);
	m_textName->SetValue(_(""));
	m_textName->SetToolTip(_("Layer name"));
	nameSizer->Add(m_textName, 1, wxEXPAND|wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5);

	wxStaticBoxSizer *typeSizer = new wxStaticBoxSizer(wxHORIZONTAL,this,_("Layer type"));
	sizer->Add(typeSizer, 0, wxEXPAND|wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5);
	wxArrayString layerTypes;
	layerTypes.Add( _("Points") );
	layerTypes.Add( _("PointsZ") );
	layerTypes.Add( _("PointsM") );
	layerTypes.Add( _("Multipoints") );
	layerTypes.Add( _("Arcs") );
	layerTypes.Add( _("ArcsZ") );
	layerTypes.Add( _("ArcsM") );
	layerTypes.Add( _("Polygons") );
	layerTypes.Add( _("PolygonsZ") );
	layerTypes.Add( _("PolygonsM") );

	// Pffffffffffffffff
	wxCArrayString temp( layerTypes );
	m_choiceLayerType = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, layerTypes.GetCount(), temp.GetStrings());
	typeSizer->Add( m_choiceLayerType , 1 , wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxALL , 5 );

	sizer->Add( CreateStdDialogButtonSizer(wxOK|wxCANCEL) , 1 , wxEXPAND|wxALL , 5);

	this->SetSizerAndFit(sizer);
	Layout();
}

std::string NewVectorLayerControl::GetLayerTypeAsString()
{
	return m_associationStringType[ m_choiceLayerType->GetSelection() ];
}

int NewVectorLayerControl::GetLayerType()
{
	std::map< std::string , int >::iterator it = m_associationMapStringType.find( this->GetLayerTypeAsString() );
	if ( it != m_associationMapStringType.end() )
		return it->second;
	// Si on arrive la, c'est vraiment pas bon ...
	return -1;
}

std::string NewVectorLayerControl::GetLayerName()
{
	return std::string( m_textName->GetValue().fn_str() );
}
