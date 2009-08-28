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

#include <boost/filesystem.hpp>

#include <wx/dc.h>
#include <wx/log.h>
#include <wx/progdlg.h>

#include "layers/VectorLayerArc.hpp"

inline void limite(float &x)
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

void GenericVectorLayerArc::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const float zoomFactor, const float translationX, const float translationY)
{
	wxPen penColour( m_arcsColour , m_width , m_penStyle );
	dc.SetPen( penColour );
	{
		// On met en place 2 methodes pour eviter de parcourir 2 fois les elements :
		//     - la premiere avec les etiquettes
		//     - la seconde sans les etiquettes
		if ( FlagDBF() && m_drawAttribute > 0 )
		{
			const std::vector < wxString > & tab_att = m_dbfAttributesValues[m_drawAttribute-1];
			float sizex = 0, sizey = 0;
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
					float posx1 = (0.5+local_tab[j].first+translationX)/zoomFactor;
					limite(posx1);
					float posy1 = (0.5+m_flagPRJ*local_tab[j].second+translationY)/zoomFactor ;
					limite(posy1);
					float posx2 = (0.5+local_tab[j+1].first+translationX)/zoomFactor;
					limite(posx2);
					float posy2 = (0.5+m_flagPRJ*local_tab[j+1].second+translationY)/zoomFactor;
					limite(posy2);
					dc.DrawLine( posx1, posy1, posx2, posy2);
				}
				float posx = ((0.5+local_tab[0].first+translationX)/zoomFactor+(0.5+local_tab[local_tab.size()-1].first+translationX)/zoomFactor)/2.;
				float posy = ((0.5+m_flagPRJ*local_tab[0].second+translationY)/zoomFactor + (0.5+m_flagPRJ*local_tab[local_tab.size()-1].second+translationY)/zoomFactor)/2.;

				float text_size_x = sizex * tab_att[i].length();
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
					float posx1 = (0.5+local_tab[j].first+translationX)/zoomFactor;
					limite(posx1);
					float posy1 = (0.5+m_flagPRJ*local_tab[j].second+translationY)/zoomFactor ;
					limite(posy1);
					float posx2 = (0.5+local_tab[j+1].first+translationX)/zoomFactor;
					limite(posx2);
					float posy2 = (0.5+m_flagPRJ*local_tab[j+1].second+translationY)/zoomFactor;
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
		mes << _("File : ") << wxString(m_shapefileFileName.c_str(), *wxConvCurrent) << _("\n");
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
		mes << _("x and y are not of the same size !\n");
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
		mes << _("File : ") << wxString(m_shapefileFileName.c_str(), *wxConvCurrent) << _("\n");
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
		m << _("File : ") << wxString(m_shapefileFileName.c_str(), *wxConvCurrent) << _("\n");
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
