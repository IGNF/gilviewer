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
#include <wx/config.h>
#include <wx/progdlg.h>

#include "VectorLayerPoint.hpp"

GenericVectorLayerPoint::GenericVectorLayerPoint() : VectorLayerContent(),
	m_pointsColour( wxColour(255,0,0) ),
	m_width(3)
{}

void GenericVectorLayerPoint::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const float zoomFactor, const float translationX, const float translationY)
{
	wxPen penColour( m_pointsColour , m_width );
	dc.SetPen( penColour );
	// On draw des lines pq c'est beaucoup plus rapide et qu'en plus ca permet de gerer l'epaisseur ...
	// On suppose que si le SHPHandle est NULL, ca a explose avant ... En plus, pour un calque a la main, il n'y en a pas ...
	//if ( m_SHPHandle != NULL )
	//{
		// On met en place 2 methodes pour eviter de parcourir 2 fois les elements :
		//     - la premiere avec les etiquettes
		//     - la seconde sans les etiquettes
		if ( FlagDBF() && m_drawAttribute > 0 )
		{
			for (unsigned int i=0;i<m_points.size();i++)
			{
				dc.DrawLine( (0.5+m_points[i].first + translationX ) / zoomFactor, (0.5+m_flagPRJ*m_points[i].second +translationY)/zoomFactor , (0.5+m_points[i].first + translationX ) / zoomFactor, (0.5+m_flagPRJ*m_points[i].second +translationY)/zoomFactor );
				dc.DrawText( m_dbfAttributesValues[m_drawAttribute-1][i] , (0.5+m_points[i].first + translationX ) / zoomFactor , (0.5+m_flagPRJ*m_points[i].second +translationY)/zoomFactor );
			}
		}
		else
			for (unsigned int i=0;i<m_points.size();++i)
			{
				double x = (0.5+m_points[i].first + translationX ) / zoomFactor;
				double y = (0.5+m_flagPRJ*m_points[i].second +translationY)/zoomFactor;
				dc.DrawLine( x,y , x,y );
				//dc.DrawPoint( x,y );
			}
	//}
}

VectorLayerPoint::VectorLayerPoint(const SHPHandle &handle , const std::string &shapefileFileName)
{
	m_SHPHandle = handle;
	//m_shapefileFileName = boost::filesystem::basename(shapefileFileName);
	m_shapefileFileName = shapefileFileName;
    // On construit le vecteur qui contient les points
    if ( m_SHPHandle != NULL )
    {
        SHPGetInfo( m_SHPHandle , &m_numberOfEntities , &m_shapeType , m_minBound , m_maxBound );
		unsigned int i;

		wxProgressDialog *progress = new wxProgressDialog(_("Reading shapefile ..."),_("Reading ..."),m_numberOfEntities,NULL,wxPD_AUTO_HIDE|wxPD_ELAPSED_TIME|wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME);

		wxString m;
		m << _("File : ") << wxString(m_shapefileFileName.c_str(), *wxConvCurrent) << _("\n");
		m << _("Reading ") << static_cast<unsigned int>(m_numberOfEntities) << _(" objects");

        for (i=0;i<static_cast<unsigned int>(m_numberOfEntities);++i)
		{
			if (i%3000==0)
				progress->Update(i, m);
            SHPObject* obj = SHPReadObject( m_SHPHandle , i );
            m_points.push_back( std::make_pair<double,double>(obj->padfX[0] , obj->padfY[0]) );
        }
		delete progress;
    }
}

void VectorLayerPoint::Save( const std::string &name )
{
	m_SHPHandle = SHPCreate( name.c_str() , SHPT_POINT );
	boost::filesystem::path full = boost::filesystem::system_complete(name);
	m_shapefileFileName = full.string();
	//Filename( full.string() );
	m_drawAttribute = 0;

	if ( m_flagPRJ == CARTOGRAPHIC_COORDINATES )
	{
		CreatePRJ();
	}

	unsigned int i;
	for (i=0;i<m_points.size();++i)
	{
		SHPObject* object = SHPCreateSimpleObject( SHPT_POINT , 1 , &(m_points[i].first) , &(m_points[i].second) , NULL );
		SHPComputeExtents( object );
		SHPWriteObject( m_SHPHandle , -1 , object );
	}
	SHPClose( m_SHPHandle );
}

void VectorLayerPoint::AddPoint( double x , double y )
{
	m_points.push_back( std::make_pair(x,y) );
	m_numberOfEntities++;
}

void VectorLayerPoint::AddPoints( const std::vector<double> &x , const std::vector<double> &y )
{
	if ( x.size() != y.size() )
		return;
	std::vector<double>::const_iterator itbx = x.begin(), itby = y.begin();
	for(;itbx!=x.end();++itbx,++itby)
		AddPoint(*itbx,*itby);
}

VectorLayerPointM::VectorLayerPointM(const SHPHandle &handle , const std::string &shapefileFileName)
{
	m_SHPHandle = handle;
	//m_shapefileFileName = boost::filesystem::basename(shapefileFileName);
	m_shapefileFileName = shapefileFileName;
    // On construit le vecteur qui contient les points
    if ( m_SHPHandle != NULL )
    {
        SHPGetInfo( m_SHPHandle , &m_numberOfEntities , &m_shapeType , m_minBound , m_maxBound );
		unsigned int i;

		wxProgressDialog *progress = new wxProgressDialog(_("Reading shapefile ..."),_("Reading ..."),m_numberOfEntities,NULL,wxPD_AUTO_HIDE|wxPD_ELAPSED_TIME|wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME);

		wxString m;
		m << _("File : ") << wxString(m_shapefileFileName.c_str(), *wxConvCurrent) << _("\n");
		m << _("Reading ") << static_cast<unsigned int>(m_numberOfEntities) << _(" objects");
        for (i=0;i<static_cast<unsigned int>(m_numberOfEntities);++i)
		{
			if (i%3000==0)
				progress->Update(i, m);
            SHPObject* obj = SHPReadObject( m_SHPHandle , i );
            m_points.push_back( std::make_pair<double,double>(obj->padfX[0] , obj->padfY[0]) );
            m_measures.push_back( obj->padfM[0] );
        }
		delete progress;
    }
}

VectorLayerPointZ::VectorLayerPointZ(const SHPHandle &handle , const std::string &shapefileFileName)
{
	m_SHPHandle = handle;
	//m_shapefileFileName = boost::filesystem::basename(shapefileFileName);
	m_shapefileFileName = shapefileFileName;
    // On construit le vecteur qui contient les points
    if ( m_SHPHandle != NULL )
    {
        SHPGetInfo( m_SHPHandle , &m_numberOfEntities , &m_shapeType , m_minBound , m_maxBound );
		unsigned int i;

		wxProgressDialog *progress = new wxProgressDialog(_("Reading shapefile ..."),_("Reading ..."),m_numberOfEntities,NULL,wxPD_AUTO_HIDE|wxPD_ELAPSED_TIME|wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME);

		wxString m;
		m << _("File : ") << wxString(m_shapefileFileName.c_str(), *wxConvCurrent) << _("\n");
		m << _("Reading ") << static_cast<unsigned int>(m_numberOfEntities) << _(" objects");
        for (i=0;i<static_cast<unsigned int>(m_numberOfEntities);++i)
		{
			if (i%3000==0)
				progress->Update(i, m);
            SHPObject* obj = SHPReadObject( m_SHPHandle , i );
            m_points.push_back( std::make_pair<double,double>(obj->padfX[0] , obj->padfY[0]) );
            m_Z.push_back( obj->padfZ[0] );
        }
		delete progress;
    }
}
