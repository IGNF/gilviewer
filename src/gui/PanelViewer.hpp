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

#ifndef PANELVIEWER_HPP_
#define PANELVIEWER_HPP_

#include <wx/dnd.h>
#include <wx/panel.h>

#include "layers/Layer.hpp"

#include "convenient/MacrosITKViewer.hpp"

class ApplicationSettings;
class LayerControl;
class wxLogWindow;
class wxToolBar;
class wxMenuBar;
class VectorLayerGhost;

#if wxUSE_DRAG_AND_DROP
	class ITKViewerFileDropTarget;
#endif // wxUSE_DRAG_AND_DROP

class PanelViewer : public wxPanel
{
public:
#if wxUSE_DRAG_AND_DROP
	friend class ITKViewerFileDropTarget;
#endif // wxUSE_DRAG_AND_DROP

	static void Register(wxFrame* parent);


	virtual ~PanelViewer() {}

	void AddLayer( const Layer::ptrLayerType &layer );

	LayerControl* GetLayerControl() const;
	ApplicationSettings* GetApplicationSettings() const;

	// On la met en public pour pouvoir y acceder depuis le FrameViewer (salete de windows, il faut bien le reconnaitre ...)
	DECLARE_ITKVIEWER_METHODS_FOR_EVENTS_TABLE();
	void OnMouseWheel(wxMouseEvent& event);

	//wxToolBar* GetToolBar();
	wxMenuBar* GetMenuBar();
	bool InitToolbar();



	// Drag and Drop
	//virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

	virtual void OpenCustomFormat(const std::string &filename);

	void SetModeNavigation();
	void SetModeGeometryMoving();
	void SetModeEdition();
	void SetModeCapture();
	void SetModeSelection();

	void SetGeometryNull();
	void SetGeometryPoint();
	void SetGeometryCircle();
	void SetGeometryRectangle();
	void SetGeometryLine();
	void SetGeometryPolygone();

	void OnSnapshot(wxCommandEvent& event);

	void SingleCrop();
	void MultiCrop();

	// TODO : tout passer en minuscule (ou en majuscule)
	enum eMode //mode de gestion des événements
	{
		MODE_NAVIGATION, //déplacement dans la scène 2D (translation/zoom des images)
		MODE_GEOMETRY_MOVING, //déplacement de la primitive active
		MODE_EDITION, //modification de primitives existantes
		MODE_CAPTURE, //saisie de nouvelles primitives
		MODE_SELECTION //sélection raster
	};

	enum eGEOMETRY //choix de primitives
	{
		GEOMETRY_NULL,
		GEOMETRY_POINT,
		GEOMETRY_CIRCLE,
		GEOMETRY_RECTANGLE,
		GEOMETRY_LINE,
		GEOMETRY_POLYGONE
	};

	inline eMode GetCurrentMode() { return m_mode; }
	inline eGEOMETRY GetCurrentGeometry() { return m_geometry; }
	inline const VectorLayerGhost & GetVectorLayerGhost() { return m_ghostLayer; }

	DECLARE_EVENT_TABLE();

protected:
	wxFrame* m_parent;

	void ShowLayerControl(bool show) const;

	///Recuperer les coord images du pointeur souris par rapport a la premiere couche image
	bool GetCoordImage(const int mouseX, const int mouseY, int &i, int &j) const;
	bool GetSubPixCoordImage(const int mouseX, const int mouseY, double &i, double&j) const;

	//wxToolBar* m_toolBar;
	wxMenuBar* m_menuBar;
	wxMenu *m_menuMain;
	bool m_mouseMovementStarted;
	float m_mouseMovementInitX;
	float m_mouseMovementInitY;

	wxPoint m_translationDrag;

	// Le controle des couches
	LayerControl* m_layerControl;
	ApplicationSettings* m_applicationSettings;

	//ref sur le ghostLayer du LayerControl
	VectorLayerGhost &m_ghostLayer;

	void executeMode();
	virtual void executeModeNavigation();
	virtual void executeModeGeometryMoving();
	virtual void executeModeCapture();
	virtual void executeModeEdition();
	virtual void executeModeSelection();

	/// Flag indiquant le mode navigation, saisie, ...)
	eMode m_mode;
	///Flag indiquant la primitive géométrique en cours quand on n'est pas en mode navigation (point, rectangle, ...)
	eGEOMETRY m_geometry;

	///Ajoute un point à la géométrie courante
	void GeometryAddPoint(const wxPoint& pt);
	///Mets à jour la géométrie avec le point sous la souris lors d'un MouseMove, sans le sélectionner définitivement
	void GeometryUpdateAbsolute(const wxPoint& pt);
	void GeometryUpdateRelative(const wxPoint& translation);
	///Fin du clic, on ferme (ou pas ?) la géométrie et on exécute les traitements
	void GeometryEnd();

	///Déplacement de la géométrie
	void GeometryMoveAbsolute(const wxPoint& pt);
	void GeometryMoveRelative(const wxPoint& translation);

	///Déplacement de la scene
	void SceneMove(const wxPoint& translation);

	void OnPaint(wxPaintEvent& evt);
	void UpdateStatusBar(const int i, const int j);
	void OnSize( wxSizeEvent &e );

	// Mouse events
	void OnLeftDown(wxMouseEvent &event);
	void OnLeftUp(wxMouseEvent &event);
	void OnMouseMove(wxMouseEvent &event);
	void OnMiddleDown(wxMouseEvent &event);
	void OnRightDown(wxMouseEvent &event);
	void OnLeftDClick(wxMouseEvent &event);
	void OnRightDClick(wxMouseEvent &event);

	void OnKeydown(wxKeyEvent& event);

	void ShowPopUpMenu(const wxPoint& pos);
	void OnPopUpMenu(wxCommandEvent& event);

	void OnQuit(wxCommandEvent& event);

	void Zoom(float zoomFactor, wxMouseEvent &event);

	void UpdateIfTransformable();

	///pour ne créer des panels qu'à partir de la factory (PanelManager)
	PanelViewer(wxFrame* parent);
	friend PanelViewer* createPanelViewer(wxFrame* parent);

};

#if wxUSE_DRAG_AND_DROP
class ITKViewerFileDropTarget : public wxFileDropTarget
{
public:
	ITKViewerFileDropTarget( PanelViewer* panelviewer ) : m_panelViewer(panelviewer){}
	virtual ~ITKViewerFileDropTarget() {}
	bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);
private:
	PanelViewer* m_panelViewer;
};
#endif // wxUSE_DRAG_AND_DROP

#endif /*PANELVIEWER_HPP_*/
