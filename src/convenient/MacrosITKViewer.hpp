/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets.


Homepage: 

	http://launchpad.net/gilviewer
	
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

#ifndef MACROSITKVIEWER_HPP_
#define MACROSITKVIEWER_HPP_

/**
 * Un ensemble de macros pour faciliter la vie de l'utilisateur ...
 */

/**
 * Comme son nom l'indique, cette macro permet d'ajouter les evenements "classiques" de la barre d'outils a la table d'evenements.
 * Voir FrameViewer.cpp pour un exemple d'utilisation
 */
#define ADD_ITKVIEWER_EVENTS_TO_TABLE(classname) \
	EVT_TOOL(wxID_NEW, classname::OnNewLayer) \
	EVT_TOOL(wxID_OPEN, classname::OnOpenLayer) \
	EVT_TOOL(ID_BASIC_SNAPSHOT, classname::OnSnapShot) \
	EVT_TOOL(ID_SHOW_HIDE_LAYER_CONTROL, classname::OnShowHideLayerControl) \
	EVT_TOOL(ID_MODE_NAVIGATION, classname::OnModeNavigation) \
	EVT_TOOL(ID_MODE_GEOMETRY_MOVING, classname::OnModeGeometryMoving) \
	EVT_TOOL(ID_MODE_EDITION, classname::OnModeEdition) \
	EVT_TOOL(ID_MODE_CAPTURE, classname::OnModeCapture) \
	EVT_TOOL(ID_MODE_SELECTION, classname::OnModeSelection) \
	EVT_TOOL(ID_GEOMETRY_NULL, classname::OnGeometryNull) \
	EVT_TOOL(ID_GEOMETRY_POINT, classname::OnGeometryPoint) \
	EVT_TOOL(ID_GEOMETRY_CIRCLE, classname::OnGeometryCircle) \
	EVT_TOOL(ID_GEOMETRY_RECTANGLE, classname::OnGeometryRectangle) \
	EVT_TOOL(ID_GEOMETRY_LINE, classname::OnGeometryLine) \
	EVT_TOOL(ID_GEOMETRY_POLYGONE, classname::OnGeometryPolygone) \
	EVT_TOOL(ID_SINGLE_CROP, classname::OnSingleCrop) \
	EVT_TOOL(ID_MULTI_CROP, classname::OnMultiCrop)



/**
 * Comme son nom l'indique, cette macro permet de declarer les evenements "classiques" de la table d'evenements.
 * Voir FrameViewer.hpp pour un exemple d'utilisation
 */
#define DECLARE_ITKVIEWER_METHODS_FOR_EVENTS_TABLE() \
	void OnOpenLayer(wxCommandEvent& event); \
	void OnNewLayer(wxCommandEvent& event); \
	void OnSnapShot(wxCommandEvent& event); \
	void OnShowHideLayerControl(wxCommandEvent& event); \
	void OnModeNavigation(wxCommandEvent& event); \
	void OnModeGeometryMoving(wxCommandEvent& event); \
	void OnModeEdition(wxCommandEvent& event); \
	void OnModeCapture(wxCommandEvent& event); \
	void OnModeSelection(wxCommandEvent& event); \
	void OnGeometryNull(wxCommandEvent& event); \
	void OnGeometryPoint(wxCommandEvent& event); \
	void OnGeometryCircle(wxCommandEvent& event); \
	void OnGeometryRectangle(wxCommandEvent& event); \
	void OnGeometryLine(wxCommandEvent& event); \
	void OnGeometryPolygone(wxCommandEvent& event); \
	void OnSingleCrop(wxCommandEvent& event); \
	void OnMultiCrop(wxCommandEvent& event);

/**
 * Comme son nom l'indique, cette macro permet d'implementer les evenements "classiques" de la table d'evenements.
 * Voir FrameViewer.cpp pour un exemple d'utilisation
 */
#define IMPLEMENTS_ITKVIEWER_METHODS_FOR_EVENTS_TABLE(classname,variablePanelViewer) \
	void classname::OnOpenLayer(wxCommandEvent& event) \
	{ \
		variablePanelViewer->GetLayerControl()->OnOpenLayer(event); \
	} \
	void classname::OnNewLayer(wxCommandEvent& event) \
	{ \
		variablePanelViewer->GetLayerControl()->OnNewLayer(event); \
	} \
	void classname::OnSnapShot(wxCommandEvent& event) \
	{ \
		variablePanelViewer->OnSnapshot(event); \
	} \
	void classname::OnShowHideLayerControl(wxCommandEvent& event) \
	{ \
		variablePanelViewer->GetLayerControl()->Show(!variablePanelViewer->GetLayerControl()->IsVisible()); \
	} \
	void classname::OnModeNavigation(wxCommandEvent& event) \
	{ \
		variablePanelViewer->SetModeNavigation(); \
		wxLogMessage( _("Passage en mode Navigation !") ); \
	} \
	void classname::OnModeGeometryMoving(wxCommandEvent& event) \
	{ \
		variablePanelViewer->SetModeGeometryMoving(); \
		wxLogMessage( _("Passage en mode Geometry Moving !") ); \
	} \
	void classname::OnModeEdition(wxCommandEvent& event) \
	{ \
		variablePanelViewer->SetModeEdition(); \
		wxLogMessage( _("Passage en mode Edition !") ); \
	} \
	void classname::OnModeCapture(wxCommandEvent& event) \
	{ \
		variablePanelViewer->SetModeCapture(); \
		wxLogMessage( _("Passage en mode Capture !") ); \
	} \
	void classname::OnModeSelection(wxCommandEvent& event) \
	{ \
		variablePanelViewer->SetModeSelection(); \
		wxLogMessage( _("Passage en mode Selection !") ); \
	} \
	void classname::OnGeometryNull(wxCommandEvent& event) \
	{ \
		variablePanelViewer->SetGeometryNull(); \
		wxLogMessage( _("Passage en Geometry Nulle !") ); \
	} \
	void classname::OnGeometryPoint(wxCommandEvent& event) \
	{ \
		variablePanelViewer->SetGeometryPoint(); \
		wxLogMessage( _("Passage en Geometry Point !") ); \
	} \
	void classname::OnGeometryCircle(wxCommandEvent& event) \
	{ \
		variablePanelViewer->SetGeometryCircle(); \
		wxLogMessage( _("Passage en Geometry Circle !") ); \
	} \
	void classname::OnGeometryRectangle(wxCommandEvent& event) \
	{ \
		variablePanelViewer->SetGeometryRectangle(); \
		wxLogMessage( _("Passage en Geometry Rectangle !") ); \
	} \
	void classname::OnGeometryLine(wxCommandEvent& event) \
	{ \
		variablePanelViewer->SetGeometryLine(); \
		wxLogMessage( _("Passage en Geometry Line !") ); \
	} \
	void classname::OnGeometryPolygone(wxCommandEvent& event) \
	{ \
		variablePanelViewer->SetGeometryPolygone(); \
		wxLogMessage( _("Passage en Geometry Polygone !") ); \
	} \
	void classname::OnSingleCrop(wxCommandEvent& event) \
	{ \
		variablePanelViewer->SingleCrop(); \
	} \
	void classname::OnMultiCrop(wxCommandEvent& event) \
	{ \
		variablePanelViewer->MultiCrop(); \
	}


#endif /* MACROSITKVIEWER_HPP_ */
