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

#include <sstream>

#include <wx/dc.h>
#include <wx/log.h>
#include <wx/config.h>

#include "GilViewer/layers/VectorLayer.hpp"
#include "GilViewer/layers/VectorLayerArc.hpp"
#include "GilViewer/layers/VectorLayerPoint.hpp"
#include "GilViewer/layers/VectorLayerPolygon.hpp"
#include "GilViewer/layers/VectorLayerMultiGeometries.hpp"

void VectorLayer::Init()
{
	ZoomFactor(1.);
	TranslationX(0.);
	TranslationY(0.);

	HasToBeUpdated(true);
	IsTransformable(true);
	m_isTextVisible = true;
	m_textFont = wxNullFont;
}

void VectorLayer::SetDefaultDisplayParameters()
{
	wxConfigBase *pConfig = wxConfigBase::Get();

	/// Points
	int redPoint = 255, bluePoint = 0, greenPoint = 0;
	int widthPoint = 3;
	if ( pConfig )
	{
		pConfig->Read(_T("/Options/VectorLayerPoint/Color/Red"), &redPoint, 255);
		pConfig->Read(_T("/Options/VectorLayerPoint/Color/Green"), &greenPoint, 0);
		pConfig->Read(_T("/Options/VectorLayerPoint/Color/Blue"), &bluePoint, 0);
		pConfig->Read(_T("/Options/VectorLayerPoint/Width"), &widthPoint, 3);
	}
	SetPointsStyle( wxColour(redPoint,greenPoint,bluePoint) , widthPoint );
	/// Points

	/// Lignes
	int redLine = 255, blueLine = 0, greenLine = 0;
	int widthLine = 3;
	int styleLine = wxSOLID;
	if ( pConfig )
	{
		pConfig->Read(_T("/Options/VectorLayerLine/Color/Red"), &redLine, 255);
		pConfig->Read(_T("/Options/VectorLayerLine/Color/Green"), &greenLine, 0);
		pConfig->Read(_T("/Options/VectorLayerLine/Color/Blue"), &blueLine, 0);
		pConfig->Read(_T("/Options/VectorLayerLine/Width"), &widthLine, 3);
		pConfig->Read(_T("/Options/VectorLayerLine/Style"), &styleLine, wxSOLID);
	}
	SetLinesStyle( wxColour(redLine,greenLine,blueLine) , widthLine , styleLine );
	/// Lignes

	/// Polygones
	int redRing = 255, blueRing = 0, greenRing = 0;
	int redShape = 255, blueShape = 0, greenShape = 0;
	int ringWidth = 1;
	int penStyle = wxSOLID;
	int brushStyle = wxSOLID;
	if ( pConfig )
	{
		pConfig->Read(_T("/Options/VectorLayerPolygon/Ring/Color/Red"), &redRing, 255);
		pConfig->Read(_T("/Options/VectorLayerPolygon/Ring/Color/Green"), &greenRing, 0);
		pConfig->Read(_T("/Options/VectorLayerPolygon/Ring/Color/Blue"), &blueRing, 0);
		pConfig->Read(_T("/Options/VectorLayerPolygon/Ring/Width"), &ringWidth, 1);

		pConfig->Read(_T("/Options/VectorLayerPolygon/Shape/Color/Red"), &redShape, 255);
		pConfig->Read(_T("/Options/VectorLayerPolygon/Shape/Color/Green"), &greenShape, 0);
		pConfig->Read(_T("/Options/VectorLayerPolygon/Shape/Color/Blue"), &blueShape, 0);

		pConfig->Read(_T("/Options/VectorLayerPolygon/Style/Pen"), &penStyle, wxSOLID);
		pConfig->Read(_T("/Options/VectorLayerPolygon/Style/Brush"), &brushStyle, wxSOLID);
	}

	PolygonsRingsColour( wxColour(redRing,greenRing,blueRing) );
	PolygonsInsideColour( wxColour(redShape,greenShape,blueShape) );
	PolygonsRingsStyle( penStyle );
	PolygonsInsideStyle( brushStyle );
	PolygonsRingsWidth( ringWidth );
	/// Polygones
}

VectorLayer::VectorLayer( const std::string &layerName , int layerType , signed short flagPRJ , bool flagDBF ) :
	m_isEditable(true),
	m_isFromFile(false),
	m_layerType(layerType)
{
	Name(layerName);
	Init();

	// L'idee ici, c'est la creation d'un calque "a la main"
	// "A la main", ca veut dire pas à partir d'un fichier ...
	// Donc, ce calque permet la saisie manuelle (a implementer) ou par ajout dans le code (voir les methodes Add***)
	if ( m_layerType == SHPT_POINT )
	{
		m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerPoint() );
	}
	else if ( m_layerType == SHPT_POINTZ )
	{
		m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerPointZ() );
	}
	else if ( m_layerType == SHPT_POINTM )
	{
		m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerPointM() );
	}
	else if ( m_layerType == SHPT_MULTIPOINT )
	{
		std::ostringstream oss;
		oss << "Les multipoints ne sont pas geres.\nDesole ... " << std::endl;
		oss << "File : " <<__FILE__ << std::endl;
		oss << "Line : " << __LINE__ << std::endl;
		oss << "Function : " << __FUNCTION__ << std::endl;
		throw std::logic_error( oss.str() );
	}
	else if ( m_layerType == SHPT_MULTIPOINTZ )
	{
		std::ostringstream oss;
		oss << "Les multipointsZ ne sont pas geres.\nDesole ... " << std::endl;
		oss << "File : " <<__FILE__ << std::endl;
		oss << "Line : " << __LINE__ << std::endl;
		oss << "Function : " << __FUNCTION__ << std::endl;
		throw std::logic_error( oss.str() );
	}
	else if ( m_layerType == SHPT_MULTIPOINTM )
	{
		std::ostringstream oss;
		oss << "Les multipointsM ne sont pas geres.\nDesole ... " << std::endl;
		oss << "File : " <<__FILE__ << std::endl;
		oss << "Line : " << __LINE__ << std::endl;
		oss << "Function : " << __FUNCTION__ << std::endl;
		throw std::logic_error( oss.str() );
	}
	else if ( m_layerType == SHPT_ARC )
	{
		m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerArc() );
	}
	else if ( m_layerType == SHPT_ARCZ )
	{
		m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerArcZ() );
	}
	else if ( m_layerType == SHPT_ARCM )
	{
		m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerArcM() );
	}
	else if ( m_layerType == (int)SHPT_POLYGON )
	{
		m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerPolygon() );
	}
	else if ( m_layerType == SHPT_POLYGONZ )
	{
		m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerPolygonZ() );
	}
	else if ( m_layerType == SHPT_POLYGONM )
	{
		m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerPolygonM() );
	}
	else if ( m_layerType == SHPT_MULTIPATCH )
	{
		std::ostringstream oss;
		oss << "Les multipatch ne sont pas geres.\nDesole ... " << std::endl;
		oss << "File : " <<__FILE__ << std::endl;
		oss << "Line : " << __LINE__ << std::endl;
		oss << "Function : " << __FUNCTION__ << std::endl;
		throw std::logic_error( oss.str() );
	}
	else
	{
		std::ostringstream oss;
		oss << "Le type de geometrie est incorrect ..." << std::endl;
		oss << "File : " <<__FILE__ << std::endl;
		oss << "Line : " << __LINE__ << std::endl;
		oss << "Function : " << __FUNCTION__ << std::endl;
		throw std::logic_error( oss.str() );
	}
	m_layerContent->FlagPRJ( flagPRJ );
	m_layerContent->FlagDBF( flagDBF );
	SetDefaultDisplayParameters();
	notifyLayerSettingsControl_();
}

VectorLayer::VectorLayer(const std::string &layerName , const std::string &fileName) :
	m_isEditable(false),
	m_isFromFile(true)
{
	Name(layerName);
	boost::filesystem::path full = boost::filesystem::system_complete(fileName);
	//Filename( full.string() );
	Init();

	try
	{
		AddVectorLayerContent(fileName);
	}
	catch(const std::exception &e)
	{
		std::ostringstream oss;
		oss << std::endl << "Exception propagee depuis :" << std::endl;
		oss << e.what();
		throw std::logic_error(oss.str());
	}
	SetDefaultDisplayParameters();
	notifyLayerSettingsControl_();
}

VectorLayer::VectorLayer( const std::string &layerName , signed short flagPRJ , bool flagDBF ) :
    m_isEditable(true),
    m_isFromFile(false)
{
    Name(layerName);
    Init();

    m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerMultiGeometries() );

    m_layerContent->FlagPRJ( flagPRJ );
    m_layerContent->FlagDBF( flagDBF );
	SetDefaultDisplayParameters();
	notifyLayerSettingsControl_();
}

Layer::ptrLayerType VectorLayer::CreateVectorLayer( const std::string &layerName , int layerType , signed short flagPRJ , bool flagDBF )
{
	Layer::ptrLayerType ptrLayer(new VectorLayer(layerName,layerType,flagPRJ,flagDBF));
	ptrLayer->notifyLayerSettingsControl_();
	ptrLayer->SetDefaultDisplayParameters();
	return ptrLayer;
}

Layer::ptrLayerType VectorLayer::CreateVectorLayer(const std::string &layerName , const std::string &fileName)
{
	boost::filesystem::path full = boost::filesystem::system_complete(fileName);
	if ( !boost::filesystem::exists(fileName) )
	{
		std::ostringstream oss;
		oss << "Le fichier demande n'existe pas : "<<fileName<< " ! " << std::endl;
		oss << "File : " <<__FILE__ << std::endl;
		oss << "Line : " << __LINE__ << std::endl;
		oss << "Function : " << __FUNCTION__ << std::endl;
		throw std::logic_error( oss.str() );
	}

	try
	{
		Layer::ptrLayerType ptrLayer(new VectorLayer(layerName,fileName));
		ptrLayer->Name(fileName);
		boost::filesystem::path full = boost::filesystem::system_complete(fileName);
		ptrLayer->Filename( full.string() );
		ptrLayer->notifyLayerSettingsControl_();
		ptrLayer->SetDefaultDisplayParameters();
		return ptrLayer;
	}
	catch(const std::exception &e)
	{
		std::ostringstream oss;
		oss << std::endl << "Exception propagee depuis :" << std::endl;
		oss << e.what();
		throw std::logic_error(oss.str());
	}
}

Layer::ptrLayerType VectorLayer::CreateVectorLayer( const std::string &layerName , signed short flagPRJ , bool flagDBF )
{
    Layer::ptrLayerType ptrLayer(new VectorLayer(layerName,flagPRJ,flagDBF));
	ptrLayer->notifyLayerSettingsControl_();
	ptrLayer->SetDefaultDisplayParameters();
    return ptrLayer;
}

void VectorLayer::AddVectorLayerContent(const std::string &shapefileFileName)
{
	m_layerType = SHPT_NULL;
    // On determine le type de LayerContent a creer
    SHPHandle handle = SHPOpen( shapefileFileName.c_str() , "rb+" );
	if ( handle == NULL )
	{
		// Si le handle est NULL, c'est peut etre qu'il n'y a pas de SHX. On previent ...
		std::string basename = boost::filesystem::basename(shapefileFileName);
		std::string path = boost::filesystem::path(shapefileFileName).branch_path().string();

		wxString message;
		if ( !boost::filesystem::exists(path+"/"+basename+".shx") )
		{
    		std::ostringstream oss;
    		oss << "There is no shx file for " << shapefileFileName.c_str() << " !" << std::endl;
    		oss << "File : " <<__FILE__ << std::endl;
    		oss << "Line : " << __LINE__ << std::endl;
    		oss << "Function : " << __FUNCTION__ << std::endl;
    		throw std::logic_error( oss.str() );
		}
		std::ostringstream oss;
		oss << "Handle NULL sur le fichier " << shapefileFileName.c_str() << " !" << std::endl;
		oss << "File : " <<__FILE__ << std::endl;
		oss << "Line : " << __LINE__ << std::endl;
		oss << "Function : " << __FUNCTION__ << std::endl;
		throw std::logic_error( oss.str() );
		return;
	}
	else
    {
        int nbEntities;
        double minBound[4], maxBound[4];
        SHPGetInfo( handle , &nbEntities , &m_layerType , minBound , maxBound );

		// On regarde si il existe un fchier PRJ (-1 : oui ; +1 : non)
		signed short flagPRJ = CARTOGRAPHIC_COORDINATES;
		std::string basename = boost::filesystem::basename(shapefileFileName);
		std::string path = boost::filesystem::path(shapefileFileName).branch_path().string();

		if ( !boost::filesystem::exists(path+"/"+basename+".prj") )
		{
			wxString message;
			message << _("There is no PRJ file for ") << wxString(shapefileFileName.c_str(), *wxConvCurrent) << _(" !");
			::wxLogMessage( message , _("Info") );
			flagPRJ = IMAGE_COORDINATES;
		}

		// On regarde si il existe un fichier DBF
		bool flagDBF = false;

		if ( !boost::filesystem::exists(path+"/"+basename+".dbf") )
		{
			wxString message;
			message << _("There is no DBF file for ") << wxString(shapefileFileName.c_str(), *wxConvCurrent) << _(" !");
			::wxLogMessage( message , _("Info") );
		}
		else
			flagDBF = true;

		if ( m_layerType == SHPT_POINT )
        {
			m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerPoint(handle,shapefileFileName) );
        }
        else if ( m_layerType == SHPT_POINTZ )
        {
			m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerPointZ(handle,shapefileFileName) );
        }
        else if ( m_layerType == SHPT_POINTM )
        {
			m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerPointM(handle,shapefileFileName) );
        }
        else if ( m_layerType == SHPT_MULTIPOINT )
		{
    		std::ostringstream oss;
    		oss << "Les multipoints ne sont pas geres.\nDesole ... " << std::endl;
    		oss << "File : " <<__FILE__ << std::endl;
    		oss << "Line : " << __LINE__ << std::endl;
    		oss << "Function : " << __FUNCTION__ << std::endl;
    		throw std::logic_error( oss.str() );
        }
        else if ( m_layerType == SHPT_MULTIPOINTZ )
		{
    		std::ostringstream oss;
    		oss << "Les multipointsZ ne sont pas geres.\nDesole ... " << std::endl;
    		oss << "File : " <<__FILE__ << std::endl;
    		oss << "Line : " << __LINE__ << std::endl;
    		oss << "Function : " << __FUNCTION__ << std::endl;
    		throw std::logic_error( oss.str() );
        }
        else if ( m_layerType == SHPT_MULTIPOINTM )
		{
    		std::ostringstream oss;
    		oss << "Les multipointsM ne sont pas geres.\nDesole ... " << std::endl;
    		oss << "File : " <<__FILE__ << std::endl;
    		oss << "Line : " << __LINE__ << std::endl;
    		oss << "Function : " << __FUNCTION__ << std::endl;
    		throw std::logic_error( oss.str() );
        }
        else if ( m_layerType == SHPT_ARC )
        {
			m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerArc(handle,shapefileFileName) );
        }
        else if ( m_layerType == SHPT_ARCZ )
        {
			m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerArcZ(handle,shapefileFileName) );
        }
        else if ( m_layerType == SHPT_ARCM )
        {
			m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerArcM(handle,shapefileFileName) );
        }
        else if ( m_layerType == SHPT_POLYGON )
		{
			m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerPolygon(handle,shapefileFileName) );
        }
        else if ( m_layerType == SHPT_POLYGONZ )
		{
			m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerPolygonZ(handle,shapefileFileName) );
        }
        else if ( m_layerType == SHPT_POLYGONM )
		{
			m_layerContent = boost::shared_ptr<VectorLayerContent>( new VectorLayerPolygonM(handle,shapefileFileName) );
        }
    	else if ( m_layerType == SHPT_MULTIPATCH )
    	{
    		std::ostringstream oss;
    		oss << "Les multipatch ne sont pas geres.\nDesole ... " << std::endl;
    		oss << "File : " <<__FILE__ << std::endl;
    		oss << "Line : " << __LINE__ << std::endl;
    		oss << "Function : " << __FUNCTION__ << std::endl;
    		throw std::logic_error( oss.str() );
    	}
        else
        {
    		std::ostringstream oss;
    		oss << "Probleme lors de la lecture du type de geometrie.\nFichier invalide ..." << std::endl;
    		oss << "File : " <<__FILE__ << std::endl;
    		oss << "Line : " << __LINE__ << std::endl;
    		oss << "Function : " << __FUNCTION__ << std::endl;
    		throw std::logic_error( oss.str() );
        }
		m_layerContent->ShapefileFileName( shapefileFileName );
		m_layerContent->FlagPRJ( flagPRJ );
		m_layerContent->FlagDBF( flagDBF );
		if ( flagDBF )
			m_layerContent->ReadAttributes( path+"/"+basename+".dbf" );
		SetDefaultDisplayParameters();
		notifyLayerSettingsControl_();
    }
}

void VectorLayer::GetInfos()
{
	m_infos = m_layerContent->GetInfos();
}

void VectorLayer::Save(const std::string &name)
{
	m_layerContent->Save(name);
}

void VectorLayer::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent)
{
	// On draw les geometries
	m_layerContent->Draw(dc, x, y, transparent, ZoomFactor(), TranslationX(), TranslationY(), Resolution());
	// ... puis le texte si besoin ...
	if ( m_isTextVisible )
	{
		if ( m_textFont != wxNullFont )
			dc.SetFont(m_textFont);
		std::vector< std::pair<double,double> >::const_iterator itbc = m_textCoordinates.begin(), itec = m_textCoordinates.end();
		std::vector< std::string >::const_iterator itbv = m_textValue.begin();
		std::vector< wxColour >::const_iterator itbcolor = m_textColour.begin();
		for (;itbc!=itec;++itbc,++itbv,++itbcolor)
		{
			wxDCTextColourChanger dcfortextcolor(dc,*itbcolor);
			dc.DrawText( wxString( itbv->c_str() , *wxConvCurrent ) , (itbc->first+TranslationX())/ZoomFactor() , (m_layerContent->FlagPRJ()*itbc->second+TranslationY())/ZoomFactor() );
		}
	}
}

void VectorLayer::PointsColour( const wxColour &colour , bool update )
{
	boost::shared_ptr<GenericVectorLayerPoint> layerPoint = boost::dynamic_pointer_cast<GenericVectorLayerPoint>( m_layerContent );
	if ( layerPoint )
		layerPoint->PointsColour( colour );
	if ( update )
		notifyLayerSettingsControl_();
}

wxColour VectorLayer::PointsColour()
{
	boost::shared_ptr<GenericVectorLayerPoint> layerPoint = boost::dynamic_pointer_cast<GenericVectorLayerPoint>( m_layerContent );
	if ( layerPoint )
		return layerPoint->PointsColour();
	return wxColour(0,0,0);
}

void VectorLayer::PointsWidth( const unsigned int width , bool update )
{
	boost::shared_ptr<GenericVectorLayerPoint> layerPoint = boost::dynamic_pointer_cast<GenericVectorLayerPoint>( m_layerContent );
	if ( layerPoint )
		layerPoint->PointsWidth( width );
	if ( update )
		notifyLayerSettingsControl_();
}

unsigned int VectorLayer::PointsWidth()
{
	boost::shared_ptr<GenericVectorLayerPoint> layerPoint = boost::dynamic_pointer_cast<GenericVectorLayerPoint>( m_layerContent );
	if ( layerPoint )
		return layerPoint->PointsWidth();
	return 0;
}

void VectorLayer::SetPointsStyle( const wxColour &colour , const unsigned int width , bool update )
{
	PointsColour( colour , false );
	PointsWidth( width , false );
	if ( update )
		notifyLayerSettingsControl_();
}

void VectorLayer::LinesColour( const wxColour &colour , bool update )
{
	boost::shared_ptr<GenericVectorLayerArc> layerArc = boost::dynamic_pointer_cast<GenericVectorLayerArc>( m_layerContent );
	if ( layerArc )
		layerArc->LinesColour( colour );
	if ( update )
		notifyLayerSettingsControl_();
}

wxColour VectorLayer::LinesColour()
{
	boost::shared_ptr<GenericVectorLayerArc> layerArc = boost::dynamic_pointer_cast<GenericVectorLayerArc>( m_layerContent );
	if ( layerArc )
		return layerArc->LinesColour();
	return wxColour(0,0,0);
}

void VectorLayer::LinesWidth( const unsigned int width , bool update )
{
	boost::shared_ptr<GenericVectorLayerArc> layerArc = boost::dynamic_pointer_cast<GenericVectorLayerArc>( m_layerContent );
	if ( layerArc )
		layerArc->LinesWidth( width );
	if ( update )
		notifyLayerSettingsControl_();
}

unsigned int VectorLayer::LinesWidth()
{
	boost::shared_ptr<GenericVectorLayerArc> layerArc = boost::dynamic_pointer_cast<GenericVectorLayerArc>( m_layerContent );
	if ( layerArc )
		return layerArc->LinesWidth();
	return 0;
}

void VectorLayer::LinesPenStyle( const int style , bool update )
{
	boost::shared_ptr<GenericVectorLayerArc> layerArc = boost::dynamic_pointer_cast<GenericVectorLayerArc>( m_layerContent );
	if ( layerArc )
		layerArc->LinesPenStyle( style );
	if ( update )
		notifyLayerSettingsControl_();
}

int VectorLayer::LinesPenStyle()
{
	boost::shared_ptr<GenericVectorLayerArc> layerArc = boost::dynamic_pointer_cast<GenericVectorLayerArc>( m_layerContent );
	if ( layerArc )
		return layerArc->LinesPenStyle();
	return 0;
}

void VectorLayer::SetLinesStyle( const wxColour &colour , const unsigned int width , int style , bool update )
{
	LinesColour( colour , false);
	LinesWidth( width , false );
	LinesPenStyle( style , false);
	if ( update )
		notifyLayerSettingsControl_();
}

void VectorLayer::PolygonsRingsColour( const wxColour &colour , bool update )
{
	boost::shared_ptr<GenericVectorLayerPolygon> layerPolygon = boost::dynamic_pointer_cast<GenericVectorLayerPolygon>( m_layerContent );
	if ( layerPolygon )
		layerPolygon->PolygonsRingsColour( colour );
	if ( update )
		notifyLayerSettingsControl_();
}

wxColour VectorLayer::PolygonsRingsColour()
{
	boost::shared_ptr<GenericVectorLayerPolygon> layerPolygon = boost::dynamic_pointer_cast<GenericVectorLayerPolygon>( m_layerContent );
	if ( layerPolygon )
		return layerPolygon->PolygonsRingsColour();
	return wxColour(0,0,0);
}

void VectorLayer::PolygonsInsideColour( const wxColour &colour , bool update )
{
	boost::shared_ptr<GenericVectorLayerPolygon> layerPolygon = boost::dynamic_pointer_cast<GenericVectorLayerPolygon>( m_layerContent );
	if ( layerPolygon )
		layerPolygon->PolygonsInsideColour( colour );
	if ( update )
		notifyLayerSettingsControl_();
}

wxColour VectorLayer::PolygonsInsideColour()
{
	boost::shared_ptr<GenericVectorLayerPolygon> layerPolygon = boost::dynamic_pointer_cast<GenericVectorLayerPolygon>( m_layerContent );
	if ( layerPolygon )
		return layerPolygon->PolygonsInsideColour();
	return wxColour(0,0,0);
}

void VectorLayer::PolygonsRingsStyle( const unsigned int style , bool update )
{
	boost::shared_ptr<GenericVectorLayerPolygon> layerPolygon = boost::dynamic_pointer_cast<GenericVectorLayerPolygon>( m_layerContent );
	if ( layerPolygon )
		layerPolygon->PolygonsRingsStyle( style );
	if ( update )
		notifyLayerSettingsControl_();
}

unsigned int VectorLayer::PolygonsRingsStyle()
{
	boost::shared_ptr<GenericVectorLayerPolygon> layerPolygon = boost::dynamic_pointer_cast<GenericVectorLayerPolygon>( m_layerContent );
	if ( layerPolygon )
		return layerPolygon->PolygonsRingsStyle();
	return 0;
}

void VectorLayer::PolygonsInsideStyle( const unsigned int style , bool update )
{
	boost::shared_ptr<GenericVectorLayerPolygon> layerPolygon = boost::dynamic_pointer_cast<GenericVectorLayerPolygon>( m_layerContent );
	if ( layerPolygon )
		layerPolygon->PolygonsInsideStyle( style );
	if ( update )
		notifyLayerSettingsControl_();
}

unsigned int VectorLayer::PolygonsInsideStyle()
{
	boost::shared_ptr<GenericVectorLayerPolygon> layerPolygon = boost::dynamic_pointer_cast<GenericVectorLayerPolygon>( m_layerContent );
	if ( layerPolygon )
		return layerPolygon->PolygonsInsideStyle();
	return 0;
}


void VectorLayer::PolygonsRingsWidth( const unsigned int width , bool update )
{
	boost::shared_ptr<GenericVectorLayerPolygon> layerPolygon = boost::dynamic_pointer_cast<GenericVectorLayerPolygon>( m_layerContent );
	if ( layerPolygon )
		layerPolygon->PolygonsRingsWidth( width );
	if ( update )
		notifyLayerSettingsControl_();
}

unsigned int VectorLayer::PolygonsRingsWidth()
{
	boost::shared_ptr<GenericVectorLayerPolygon> layerPolygon = boost::dynamic_pointer_cast<GenericVectorLayerPolygon>( m_layerContent );
	if ( layerPolygon )
		return layerPolygon->PolygonsRingsWidth();
	return 0;
}

void VectorLayer::AddPoint( double x , double y )
{
	boost::shared_ptr<VectorLayerPoint> layerPoint = boost::dynamic_pointer_cast<VectorLayerPoint>( m_layerContent );
	if ( layerPoint )
	{
		layerPoint->AddPoint(x,y);
		return;
	}
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
    {
        dfl->AddPoint(x,y);
        return;
    }
}

void VectorLayer::AddPoints( const std::vector<double> &x , const std::vector<double> &y )
{
	boost::shared_ptr<VectorLayerPoint> layerPoint = boost::dynamic_pointer_cast<VectorLayerPoint>( m_layerContent );
	if ( layerPoint )
		layerPoint->AddPoints(x,y);
}

void VectorLayer::AddText( double x , double y , const std::string &text, const wxColour &color )
{
	m_textCoordinates.push_back( std::make_pair(x,y) );
	m_textValue.push_back( text );
	m_textColour.push_back(color);
}

void VectorLayer::AddLine( double x1 , double y1 , double x2 , double y2 )
{
	boost::shared_ptr<VectorLayerArc> layerArc = boost::dynamic_pointer_cast<VectorLayerArc>( m_layerContent );
	if ( layerArc )
	{
		layerArc->AddLine(x1,y1,x2,y2);
		return;
	}
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
    {
        dfl->AddLine(x1,y1,x2,y2);
        return;
    }
}

void VectorLayer::AddPolyline( const std::vector<double> &x , const std::vector<double> &y )
{
	boost::shared_ptr<VectorLayerArc> layerArc = boost::dynamic_pointer_cast<VectorLayerArc>( m_layerContent );
	if ( layerArc )
		layerArc->AddPolyline(x,y);
}

void VectorLayer::AddPolygon( const std::vector<double> &x , const std::vector<double> &y )
{
	boost::shared_ptr<VectorLayerPolygon> layerPolygon = boost::dynamic_pointer_cast<VectorLayerPolygon>( m_layerContent );
	if ( layerPolygon )
		layerPolygon->AddPolygon(x,y);
}

void VectorLayer::AddCircle( double x , double y , double radius )
{
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
        dfl->AddCircle(x,y,radius);
}

void VectorLayer::AddSpline( std::vector<std::pair<double,double> > points )
{
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
        dfl->AddSpline(points);
}

void VectorLayer::AddEllipse(double x_center, double y_center, double a, double b)
{
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
        dfl->AddEllipse(x_center,y_center,a,b);
}

void VectorLayer::AddEllipse(double x_center, double y_center, double a, double b, double theta)
{
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
        dfl->AddEllipse(x_center,y_center,a,b,theta);
}

void VectorLayer::SetBrushVectorLayerMultiGeometries( const wxBrush &brush )
{
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
        dfl->SetBrushVectorLayerMultiGeometries(brush);
}

void VectorLayer::SetBrushVectorLayerMultiGeometries( unsigned char red , unsigned char green , unsigned char blue , int style , unsigned char alpha )
{
	wxBrush brush( wxColour(red,green,blue,alpha) , style );
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
        dfl->SetBrushVectorLayerMultiGeometries(brush);
	notifyLayerSettingsControl_();
}

void VectorLayer::SetPenVectorLayerMultiGeometries( const wxPen &pen )
{
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
        dfl->SetPenVectorLayerMultiGeometries(pen);
}

void VectorLayer::SetPenVectorLayerMultiGeometries( unsigned char red , unsigned char green , unsigned char blue , int style , int width , unsigned char alpha )
{
	// Bug wx !!! Inverser width et style !!!
	wxPen pen(wxColour(red,green,blue,alpha) , style , width );
    boost::shared_ptr<VectorLayerMultiGeometries> dfl = boost::dynamic_pointer_cast<VectorLayerMultiGeometries> ( m_layerContent );
    if ( dfl )
        dfl->SetPenVectorLayerMultiGeometries(pen);
	notifyLayerSettingsControl_();
}

void VectorLayer::Clear()
{
	m_layerContent->Clear();
	m_textCoordinates.clear();
	m_textValue.clear();
}

std::string VectorLayer::Filename() const
{
	boost::filesystem::path full = boost::filesystem::system_complete(m_layerContent->ShapefileFileName());
	return full.string();
}
