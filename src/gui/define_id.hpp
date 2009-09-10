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

#ifndef __DEFINE_ID_HPP__
#define __DEFINE_ID_HPP__

///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     File defining all ID's used in the application
///     All ID's used in the application MUST be declared here
///
///////////////////////////////////////////////////////////////////////////////////////////////////

///
/// Layer control
///
#define MAX_LAYERS							100
#define MAX_PLUGINS							500

///
/// HistogramPlotter
///
#define HISTOGRAM_UP_MARGIN					15
#define HISTOGRAM_BOTTOM_MARGIN				15
#define HISTOGRAM_LEFT_MARGIN				15
#define HISTOGRAM_RIGHT_MARGIN				15

enum
{
	ID_INFO									=	wxID_HIGHEST + 1000,
	ID_SAVE									=	ID_INFO + MAX_LAYERS,
	ID_DELETE								=	ID_SAVE + MAX_LAYERS,
	ID_SETTINGS								=	ID_DELETE + MAX_LAYERS,
	ID_DATABASE								=	ID_SETTINGS + MAX_LAYERS,
	ID_VISIBILITY							=	ID_DATABASE + MAX_LAYERS,
	ID_TRANSFORMATION						=	ID_VISIBILITY + MAX_LAYERS,
	ID_VISIBILITY_BUTTON					=	ID_TRANSFORMATION + MAX_LAYERS + 1,
	ID_TRANSFORMATION_BUTTON,
	ID_GLOBAL_SETTINGS_BUTTON,

	// Menu contextuel
	ID_POPMENU_SCREENSHOT,
	ID_POPMENU_REMOVE,
	ID_POPMENU_VISIBILITY					=	ID_POPMENU_REMOVE + MAX_LAYERS,
	ID_POPMENU_TRANSFORMABILITY				=	ID_POPMENU_VISIBILITY + MAX_LAYERS,

	// Text control ID's
	ID_ALPHA_RANGE_CHECK,
	ID_ALPHA_CHANNEL_CHECK,

	ID_SHOW_HIDE_LAYER_CONTROL,
	ID_SHOW_HIDE_LOG_WINDOW,
	ID_BASIC_SNAPSHOT,
	ID_ADVANCED_SNAPSHOT,

	ID_APPLICATION_SETTINGS_PATHS_APPLY_BUTTON,
	ID_APPLICATION_SETTINGS_OPTIONS_APPLY_BUTTON,

	// Dans la toolbar de l'appli, les boutons pour changer de mode
	ID_MODE_NAVIGATION, //déplacement dans la scène 2D (translation/zoom des images)
	ID_MODE_GEOMETRY_MOVING, //déplacement de la primitive active
	ID_MODE_EDITION, //modification de primitives existantes
	ID_MODE_CAPTURE, //saisie de nouvelles primitives
	ID_MODE_SELECTION, //sélection raster

	//Boutons pour changer de géométrie courante
	ID_GEOMETRY_NULL,
	ID_GEOMETRY_POINT,
	ID_GEOMETRY_CIRCLE,
	ID_GEOMETRY_RECTANGLE,
	ID_GEOMETRY_LINE,
	ID_GEOMETRY_POLYGONE,

	// Boutons de crop
	ID_SINGLE_CROP,
	ID_MULTI_CROP,

	ID_CHOICE_PANELS,
	ID_CHOICE_LAYERS		=	ID_CHOICE_PANELS + MAX_PLUGINS,

	// Celui-ci doit TOUJOURS etre le dernier !!!
	ID_FIRST_PLUGIN		=	ID_CHOICE_LAYERS + MAX_PLUGINS,
};


#endif // __DEFINE_ID_HPP__
