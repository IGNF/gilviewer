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
#include <wx/pen.h>
#include <wx/brush.h>
#include <wx/config.h>
#include <wx/progdlg.h>

#include "VectorLayerPolygon.hpp"

GenericVectorLayerPolygon::GenericVectorLayerPolygon() : VectorLayerContent(),
	m_ringsColour( wxColour(255,0,0) ),
	m_shapesColour( wxColour(0,0,255) ),
	m_ringsWidth(1),
	m_penStyle( wxSOLID ),
	m_brushStyle( wxSOLID )
{}

void GenericVectorLayerPolygon::Clear()
{
	m_polygons.clear();
	m_wxpolygons.clear();
	m_numberOfEntities = 0;
}

void GenericVectorLayerPolygon::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const float zoomFactor, const float translationX, const float translationY)
{
	wxPen penColour( m_ringsColour , m_ringsWidth , m_penStyle );
	penColour.SetWidth(m_ringsWidth);
	wxBrush brushColour( m_shapesColour , m_brushStyle );

	dc.SetPen( penColour );
	dc.SetBrush( brushColour );
	//if ( m_SHPHandle != NULL )
	//{
		// On met en place 2 methodes pour eviter de parcourir 2 fois les elements :
		//     - la premiere avec les etiquettes
		//     - la seconde sans les etiquettes
		if ( FlagDBF() && m_drawAttribute > 0 )
		{
			for (unsigned int i=0;i<m_wxpolygons.size();++i)
			{
				for (unsigned int j=0;j<m_wxpolygons[i].size();++j)
				{
					m_wxpolygons[i][j].x = (0.5+m_polygons[i][j].first + translationX ) / zoomFactor;
					m_wxpolygons[i][j].y = (0.5+m_flagPRJ*m_polygons[i][j].second + translationY ) / zoomFactor;
				}
				dc.DrawPolygon( m_wxpolygons[i].size() , &(m_wxpolygons[i].front()) );
				dc.DrawText( m_dbfAttributesValues[m_drawAttribute-1][i] , m_wxpolygons[i][0].x , m_wxpolygons[i][0].y );
			}
		}
		else
		{
			for (unsigned int i=0;i<m_wxpolygons.size();++i)
			{
				for (unsigned int j=0;j<m_wxpolygons[i].size();++j)
				{
					m_wxpolygons[i][j].x = (0.5+m_polygons[i][j].first + translationX ) / zoomFactor;
					m_wxpolygons[i][j].y = (0.5+m_flagPRJ*m_polygons[i][j].second + translationY ) / zoomFactor;
				}
				dc.DrawPolygon( m_wxpolygons[i].size() , &(m_wxpolygons[i].front()) );
			}
		}
	//}
}

VectorLayerPolygon::VectorLayerPolygon(const SHPHandle &handle , const std::string &shapefileFileName)
{
	m_SHPHandle = handle;
	//m_shapefileFileName = boost::filesystem::basename(shapefileFileName);
	m_shapefileFileName = shapefileFileName;
	// On construit le vecteur qui contient les points
	if ( m_SHPHandle != NULL )
	{
        SHPGetInfo( m_SHPHandle , &m_numberOfEntities , &m_shapeType , m_minBound , m_maxBound );
		unsigned int i, j, k;

		wxProgressDialog *progress = new wxProgressDialog(_("Reading shapefile ..."),_("Reading ..."),m_numberOfEntities,NULL,wxPD_AUTO_HIDE|wxPD_ELAPSED_TIME|wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME);
		wxString log;
		log << _("VectorLayerPolygon : ") << m_numberOfEntities << _(" elements");
		::wxLogMessage(log);

		wxString m;
		m << _("File : ") << wxString(m_shapefileFileName.c_str(), *wxConvCurrent) << _("\n");
		m << _("Reading ") << static_cast<unsigned int>(m_numberOfEntities) << _(" objects");

		for (i=0;i<static_cast<unsigned int>(m_numberOfEntities);++i)
		{
			if (i%3000==0)
				progress->Update(i, m);
			SHPObject* obj = SHPReadObject( m_SHPHandle , i );

			// Longueur de chaque ring du polygone
			std::vector<unsigned int> ringsLengths;

			for (k=0;k<static_cast<unsigned int>(obj->nParts)-1;++k)
				ringsLengths.push_back( static_cast<unsigned int>(obj->panPartStart[k+1])-static_cast<unsigned int>(obj->panPartStart[k]) );

			ringsLengths.push_back( obj->nVertices - static_cast<unsigned int>(obj->panPartStart[obj->nParts-1]) );
			// Pour chaque ring, on ajoute un ring a m_polygons
			for (k=0;k<ringsLengths.size();++k)
			{
				m_wxpolygons.push_back( simplewxPolygonRingType() );
				m_polygons.push_back( simplePolygonRingType() );
				for (j=static_cast<unsigned int>(obj->panPartStart[k]);j<static_cast<unsigned int>(obj->panPartStart[k])+ringsLengths[k];++j)
				{
					(m_polygons.back()).push_back( std::make_pair<double,double>(obj->padfX[j] , obj->padfY[j]) );
					(m_wxpolygons.back()).push_back( wxPoint( obj->padfX[j] , obj->padfY[j] ) );
				}
			}
		}
		delete progress;
	}
}

void VectorLayerPolygon::Save( const std::string &name )
{
	m_SHPHandle = SHPCreate( name.c_str() , SHPT_POLYGON );
	boost::filesystem::path full = boost::filesystem::system_complete(name);
	m_shapefileFileName = full.string();
	//Filename( full.string() );
	m_drawAttribute = 0;

	if ( m_flagPRJ == CARTOGRAPHIC_COORDINATES )
	{
		CreatePRJ();
	}

	unsigned int i, j;
	for (i=0;i<m_polygons.size();++i)
	{
		std::vector<double> X, Y;
		for (j=0;j<m_polygons[i].size();++j)
		{
			X.push_back( m_polygons[i][j].first );
			Y.push_back( m_polygons[i][j].second );
		}
		SHPObject* object = SHPCreateSimpleObject( SHPT_POLYGON , m_polygons[i].size() , &(X.front()) , &(Y.front()) , NULL );
		SHPComputeExtents( object );
		SHPWriteObject( m_SHPHandle , -1 , object );
	}
	SHPClose( m_SHPHandle );
}

void VectorLayerPolygon::AddPolygon( const std::vector<double> &x , const std::vector<double> &y )
{
	if (x.size() != y.size())
		return;
	m_wxpolygons.push_back( simplewxPolygonRingType() );
	m_polygons.push_back( simplePolygonRingType() );
	for (unsigned int j=0;j<x.size();++j)
	{
		(m_polygons.back()).push_back( std::make_pair<double,double>(x[j] , y[j]) );
		(m_wxpolygons.back()).push_back( wxPoint( x[j] , y[j] ) );
	}
	m_numberOfEntities++;
}

VectorLayerPolygonZ::VectorLayerPolygonZ(const SHPHandle &handle , const std::string &shapefileFileName)
{
	m_SHPHandle = handle;
	m_shapefileFileName = boost::filesystem::basename(shapefileFileName);
	// On construit le vecteur qui contient les points
	if ( m_SHPHandle != NULL )
	{
        SHPGetInfo( m_SHPHandle , &m_numberOfEntities , &m_shapeType , m_minBound , m_maxBound );
		unsigned int i, j, k;

		wxProgressDialog *progress = new wxProgressDialog(_("Reading shapefile ..."),_("Reading ..."),m_numberOfEntities,NULL,wxPD_AUTO_HIDE|wxPD_ELAPSED_TIME|wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME);
		wxString log;
		log << _("VectorLayerPolygonZ : ") << m_numberOfEntities << _(" elements");
		::wxLogMessage(log);

		wxString m;
		m << _("File : ") << wxString(m_shapefileFileName.c_str(), *wxConvCurrent) << _("\n");
		m << _("Reading ") << static_cast<unsigned int>(m_numberOfEntities) << _(" objects");

		for (i=0;i<static_cast<unsigned int>(m_numberOfEntities);++i)
		{
			if (i%3000==0)
				progress->Update(i, m);
			SHPObject* obj = SHPReadObject( m_SHPHandle , i );

			// Longueur de chaque ring du polygone
			std::vector<unsigned int> ringsLengths;

			for (k=0;k<static_cast<unsigned int>(obj->nParts)-1;++k)
				ringsLengths.push_back( static_cast<unsigned int>(obj->panPartStart[k+1])-static_cast<unsigned int>(obj->panPartStart[k]) );

			ringsLengths.push_back( obj->nVertices - static_cast<unsigned int>(obj->panPartStart[obj->nParts-1]) );
			// Pour chaque ring, on ajoute un ring a m_polygons
			// Pour l'instant, on ne lit que le premier ring : sinon, on perd la structure,
			// et cela pose des problemes lorsque l'on veut lire un DBF (on a plus de polygones
			// que d'enregistrements ...)
			for (k=0;k<1/*ringsLengths.size()*/;++k)
			{
				m_wxpolygons.push_back( simplewxPolygonRingType() );
				m_polygons.push_back( simplePolygonRingType() );
				for (j=static_cast<unsigned int>(obj->panPartStart[k]);j<static_cast<unsigned int>(obj->panPartStart[k])+ringsLengths[k];++j)
				{
					(m_polygons.back()).push_back( std::make_pair<double,double>(obj->padfX[j] , obj->padfY[j]) );
					(m_wxpolygons.back()).push_back( wxPoint( obj->padfX[j] , obj->padfY[j] ) );
				}
			}
		}
		delete progress;
	}
}

VectorLayerPolygonM::VectorLayerPolygonM(const SHPHandle &handle , const std::string &shapefileFileName)
{
	m_SHPHandle = handle;
	m_shapefileFileName = boost::filesystem::basename(shapefileFileName);
	// On construit le vecteur qui contient les points
	if ( m_SHPHandle != NULL )
	{
        SHPGetInfo( m_SHPHandle , &m_numberOfEntities , &m_shapeType , m_minBound , m_maxBound );
		unsigned int i, j, k;

		wxProgressDialog *progress = new wxProgressDialog(_("Reading shapefile ..."),_("Reading ..."),m_numberOfEntities,NULL,wxPD_AUTO_HIDE|wxPD_ELAPSED_TIME|wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME);
		wxString log;
		log << _("VectorLayerPolygonM : ") << m_numberOfEntities << _(" elements");
		::wxLogMessage(log);

		wxString m;
		m << _("File : ") << wxString(m_shapefileFileName.c_str(), *wxConvCurrent) << _("\n");
		m << _("Reading ") << static_cast<unsigned int>(m_numberOfEntities) << _(" objects");

		for (i=0;i<static_cast<unsigned int>(m_numberOfEntities);++i)
		{
			if (i%3000==0)
				progress->Update(i, m);
			SHPObject* obj = SHPReadObject( m_SHPHandle , i );

			// Longueur de chaque ring du polygone
			std::vector<unsigned int> ringsLengths;

			for (k=0;k<static_cast<unsigned int>(obj->nParts)-1;++k)
				ringsLengths.push_back( static_cast<unsigned int>(obj->panPartStart[k+1])-static_cast<unsigned int>(obj->panPartStart[k]) );

			ringsLengths.push_back( obj->nVertices - static_cast<unsigned int>(obj->panPartStart[obj->nParts-1]) );
			// Pour chaque ring, on ajoute un ring a m_polygons
			for (k=0;k<ringsLengths.size();++k)
			{
				m_wxpolygons.push_back( simplewxPolygonRingType() );
				m_polygons.push_back( simplePolygonRingType() );
				for (j=static_cast<unsigned int>(obj->panPartStart[k]);j<static_cast<unsigned int>(obj->panPartStart[k])+ringsLengths[k];++j)
				{
					(m_polygons.back()).push_back( std::make_pair<double,double>(obj->padfX[j] , obj->padfY[j]) );
					(m_wxpolygons.back()).push_back( wxPoint( obj->padfX[j] , obj->padfY[j] ) );
				}
			}
		}
		delete progress;
	}
}
