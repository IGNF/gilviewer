/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets. GilViewer is distributed under the CeCILL-B licence. 
See Licence_CeCILL-B_V1-en.txt or http://www.cecill.info for more details.


Homepage: 

	http://launchpad.net/gilviewer
	
Copyright:
	
	Institut Geographique National (2009)

Authors: 

	Olivier Tournaire, Adrien Chauve

	
	

This software is governed by the CeCILL-B license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL-B
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-B license and that you accept its terms.
 
***********************************************************************/

#include "gui/NewVectorLayerControl.hpp"

#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/sizer.h>

#include "shapelib/shapefil.h"

#include "gui/LayerControl.hpp"

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
