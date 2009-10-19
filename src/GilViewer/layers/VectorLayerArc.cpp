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

#include <boost/filesystem.hpp>

#include <wx/dc.h>
#include <wx/log.h>
#include <wx/progdlg.h>

#include "GilViewer/layers/VectorLayerArc.hpp"

inline void limite(double &x)
{
	if (x< -500)
		x = -500;
	else if (x> 4000)
		x = 4000;
}

GenericVectorLayerArc::GenericVectorLayerArc() : VectorLayerContent(),
	m_arcsColour( wxColour(255,0,0) ),
	m_width(1),
	m_penStyle(wxSOLID)
{}

void GenericVectorLayerArc::Clear()
{
	m_arcs.clear();
	m_numberOfEntities = 0;
}

void GenericVectorLayerArc::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const double zoomFactor, const double translationX, const double translationY, const double resolution)
{
	wxPen penColour( m_arcsColour , m_width , m_penStyle );
	dc.SetPen( penColour );
	{
		// On met en place 2 methodes pour eviter de parcourir 2 fois les elements :
		//     - la premiere avec les etiquettes
		//     - la seconde sans les etiquettes

		const double delta = 0.5 * resolution;

		if ( FlagDBF() && m_drawAttribute > 0 )
		{
			const std::vector < wxString > & tab_att = m_dbfAttributesValues[m_drawAttribute-1];
			double sizex = 0, sizey = 0;
			if (tab_att[0].length() != 0)
			{
				wxSize text_size = dc.GetTextExtent(tab_att[0]);
				sizex = text_size.GetWidth() * 1. / tab_att[0].length();
				sizey = text_size.GetHeight();
			}
			for (unsigned int i=0;i<m_arcs.size();++i)
			{
				const simpleArcType &local_tab = m_arcs[i];
				for (unsigned int j=0;j<local_tab.size()-1;++j)
				{
					double posx1 = (delta +local_tab[j].first+translationX)/zoomFactor;
					limite(posx1);
					double posy1 = (delta+m_flagPRJ*local_tab[j].second+translationY)/zoomFactor ;
					limite(posy1);
					double posx2 = (delta+local_tab[j+1].first+translationX)/zoomFactor;
					limite(posx2);
					double posy2 = (delta+m_flagPRJ*local_tab[j+1].second+translationY)/zoomFactor;
					limite(posy2);
					dc.DrawLine( posx1, posy1, posx2, posy2);
				}
				double posx = ((delta+local_tab[0].first+translationX)/zoomFactor+(delta+local_tab[local_tab.size()-1].first+translationX)/zoomFactor)/2.;
				double posy = ((delta+m_flagPRJ*local_tab[0].second+translationY)/zoomFactor + (delta+m_flagPRJ*local_tab[local_tab.size()-1].second+translationY)/zoomFactor)/2.;

				double text_size_x = sizex * tab_att[i].length();
				if ((posx +text_size_x > 0) && (posy + sizey>0) && (posx < dc.GetSize().GetWidth()) && (posy < dc.GetSize().GetHeight()))
					dc.DrawText( tab_att[i] , posx,  posy);
			}
		}
		else
			for (unsigned int i=0;i<m_arcs.size();++i)
			{
				const simpleArcType &local_tab = m_arcs[i];
				for (unsigned int j=0;j<local_tab.size()-1;++j)
				{
					double posx1 = (delta+local_tab[j].first+translationX)/zoomFactor;
					limite(posx1);
					double posy1 = (delta+m_flagPRJ*local_tab[j].second+translationY)/zoomFactor ;
					limite(posy1);
					double posx2 = (delta+local_tab[j+1].first+translationX)/zoomFactor;
					limite(posx2);
					double posy2 = (delta+m_flagPRJ*local_tab[j+1].second+translationY)/zoomFactor;
					limite(posy2);
					dc.DrawLine( posx1, posy1, posx2, posy2);
				}
			}
	}
}

VectorLayerArc::VectorLayerArc(const SHPHandle &handle , const std::string &shapefileFileName)
{
	m_SHPHandle = handle;
	//m_shapefileFileName = boost::filesystem::basename(shapefileFileName);
	m_shapefileFileName = shapefileFileName;
	// On construit le vecteur qui contient les points
	if (m_SHPHandle != NULL)
	{
		SHPGetInfo(m_SHPHandle, &m_numberOfEntities, &m_shapeType, m_minBound, m_maxBound);
		unsigned int i, j, k;

		wxString mes;
		mes << _("File : ") << wxString(m_shapefileFileName.c_str(), *wxConvCurrent) << wxT("\n");
		mes << _("Reading ") << static_cast<unsigned int>(m_numberOfEntities) << _(" objetcs ...");
		wxProgressDialog* progress = new wxProgressDialog( _("Reading file ...") , mes , static_cast<unsigned int>(m_numberOfEntities) , NULL , wxPD_APP_MODAL|wxPD_AUTO_HIDE|wxPD_ELAPSED_TIME|wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME );
		for (i=0; i<static_cast<unsigned int>(m_numberOfEntities); ++i)
		{
			if (i%3000==0)
				progress->Update(i);
			SHPObject* obj = SHPReadObject(m_SHPHandle, i);
			// On construit un vecteur de longueur pour chaque partie de l'arc
			std::vector<unsigned int> partLengths;
			for (k=0; k<static_cast<unsigned int>(obj->nParts)-1; ++k)
				partLengths.push_back( static_cast<unsigned int>(obj->panPartStart[k+1])-static_cast<unsigned int>(obj->panPartStart[k]) );
			partLengths.push_back(obj->nVertices - static_cast<unsigned int>(obj->panPartStart[obj->nParts-1]) );
			// Pour chaque partie de l'arc, on ajoute un arc a m_arcs
			for (k=0; k<partLengths.size(); ++k)
			{
				m_arcs.push_back(simpleArcType() );
				for (j=static_cast<unsigned int>(obj->panPartStart[k]); j<static_cast<unsigned int>(obj->panPartStart[k])+partLengths[k]; ++j)
					(m_arcs.back()).push_back(std::make_pair<double,double>(obj->padfX[j] , obj->padfY[j]) );
				}
			}
			delete progress;
		}
	}

void VectorLayerArc::Save( const std::string &name )
{
	m_SHPHandle = SHPCreate( name.c_str() , SHPT_ARC );
	boost::filesystem::path full = boost::filesystem::system_complete(name);
	m_shapefileFileName = full.string();
	//Filename( full.string() );
	m_drawAttribute = 0;

	if ( m_flagPRJ == CARTOGRAPHIC_COORDINATES )
	{
		CreatePRJ();
	}

	unsigned int i, j;
	for (i=0;i<m_arcs.size();++i)
	{
		std::vector<double> X, Y;
		for (j=0;j<m_arcs[i].size();++j)
		{
			X.push_back( m_arcs[i][j].first );
			Y.push_back( m_arcs[i][j].second );
		}
		SHPObject* object = SHPCreateSimpleObject( SHPT_ARC , m_arcs[i].size() , &(X.front()) , &(Y.front()) , NULL );
		SHPComputeExtents( object );
		SHPWriteObject( m_SHPHandle , -1 , object );
	}
	SHPClose( m_SHPHandle );
}

void VectorLayerArc::AddLine( double x1 , double y1 , double x2 , double y2 )
{
	simpleArcType arc;
	arc.push_back( std::make_pair(x1,y1) );
	arc.push_back( std::make_pair(x2,y2) );
	m_arcs.push_back( arc );
	m_numberOfEntities++;
}

void VectorLayerArc::AddPolyline( const std::vector<double> &x , const std::vector<double> &y )
{
	if ( x.size() != y.size() )
	{
		wxString mes;
		mes << _("x and y are not of the same size!\n");
		::wxLogMessage( mes );
		return;
	}

	simpleArcType arc;
	std::vector<double>::const_iterator itbx = x.begin(), itex = x.end();
	std::vector<double>::const_iterator itby = y.begin(), itey = y.end();
	for (;itbx!=itex;++itbx,++itby)
		arc.push_back( std::make_pair( *itbx , *itby ) );
	m_arcs.push_back( arc );
	m_numberOfEntities++;
}

VectorLayerArcM::VectorLayerArcM(const SHPHandle &handle , const std::string &shapefileFileName)
{
	m_SHPHandle = handle;
	//m_shapefileFileName = boost::filesystem::basename(shapefileFileName);
	m_shapefileFileName = shapefileFileName;
	// On construit le vecteur qui contient les points
	if (m_SHPHandle != NULL)
	{
		SHPGetInfo(m_SHPHandle, &m_numberOfEntities, &m_shapeType, m_minBound, m_maxBound);

		wxString mes;
		mes << _("File : ") << wxString(m_shapefileFileName.c_str(), *wxConvCurrent) << wxT("\n");
		mes << _("Reading ") << static_cast<unsigned int>(m_numberOfEntities) << _(" objetcs ...");
		wxProgressDialog* progress = new wxProgressDialog( _("Reading file ...") , mes , static_cast<unsigned int>(m_numberOfEntities) , NULL , wxPD_APP_MODAL|wxPD_AUTO_HIDE|wxPD_ELAPSED_TIME|wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME );
		unsigned int i, j, k;
		for (i=0; i<static_cast<unsigned int>(m_numberOfEntities); ++i)
		{
			if (i%3000==0)
				progress->Update(i);
			SHPObject* obj = SHPReadObject(m_SHPHandle, i);
			//wxString mes;
			// On construit un vecteur de longueur pour chaque partie de 'arc
			std::vector<unsigned int> partLengths;
			for (k=0; k<static_cast<unsigned int>(obj->nParts)-1; ++k)
				partLengths.push_back( static_cast<unsigned int>(obj->panPartStart[k+1])-static_cast<unsigned int>(obj->panPartStart[k]) );
			partLengths.push_back(obj->nVertices - static_cast<unsigned int>(obj->panPartStart[obj->nParts-1]) );
			// Pour chaque partie de l'arc, on ajoute un arc a m_arcs
			for (k=0; k<partLengths.size(); ++k)
			{
				m_arcs.push_back(simpleArcType() );
				for (j=static_cast<unsigned int>(obj->panPartStart[k]); j<static_cast<unsigned int>(obj->panPartStart[k])+partLengths[k]; ++j)
					(m_arcs.back()).push_back(std::make_pair<double,double>(obj->padfX[j] , obj->padfY[j]) );
			}
		}
		delete progress;
	}
}

VectorLayerArcZ::VectorLayerArcZ(const SHPHandle &handle, const std::string &shapefileFileName)
{
	m_SHPHandle = handle;
	//m_shapefileFileName = boost::filesystem::basename(shapefileFileName);
	m_shapefileFileName = shapefileFileName;
	// On construit le vecteur qui contient les points
	if (m_SHPHandle != NULL)
	{
		SHPGetInfo(m_SHPHandle, &m_numberOfEntities, &m_shapeType, m_minBound, m_maxBound);
		unsigned int i, j, k;

		wxProgressDialog *progress = new wxProgressDialog(_("Reading shapefile ..."),_("Reading ..."),m_numberOfEntities,NULL,wxPD_AUTO_HIDE|wxPD_ELAPSED_TIME|wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME);

		wxString m;
		m << _("File : ") << wxString(m_shapefileFileName.c_str(), *wxConvCurrent) << wxT("\n");
		m << _("Reading ") << static_cast<unsigned int>(m_numberOfEntities) << _(" objects");

		for (i=0; i<static_cast<unsigned int>(m_numberOfEntities); ++i)
		{
			if (i%3000==0)
				progress->Update(i, m);
			SHPObject* obj = SHPReadObject(m_SHPHandle, i);

			// On construit un vecteur de longueur pour chaque partie de 'arc
			std::vector<unsigned int> partLengths;
			for (k=0; k<static_cast<unsigned int>(obj->nParts)-1; ++k)
				partLengths.push_back( static_cast<unsigned int>(obj->panPartStart[k+1])-static_cast<unsigned int>(obj->panPartStart[k]) );
			partLengths.push_back(obj->nVertices - static_cast<unsigned int>(obj->panPartStart[obj->nParts-1]) );
			// Pour chaque partie de l'arc, on ajoute un arc a m_arcs
			for (k=0; k<partLengths.size(); ++k)
			{
				m_arcs.push_back(simpleArcType() );
				for (j=static_cast<unsigned int>(obj->panPartStart[k]); j<static_cast<unsigned int>(obj->panPartStart[k])+partLengths[k]; ++j)
					(m_arcs.back()).push_back(std::make_pair<double,double>(obj->padfX[j] , obj->padfY[j]) );
				}
			}
			delete progress;
		}
}
