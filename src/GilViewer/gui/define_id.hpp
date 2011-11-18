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
        ID_REFRESH								=	ID_SETTINGS + MAX_LAYERS,
        ID_VISIBILITY							=	ID_REFRESH + MAX_LAYERS,
	ID_TRANSFORMATION						=	ID_VISIBILITY + MAX_LAYERS,
	ID_VISIBILITY_BUTTON					=	ID_TRANSFORMATION + MAX_LAYERS + 1,
	ID_TRANSFORMATION_BUTTON,
	ID_GLOBAL_SETTINGS_BUTTON,
        ID_DELETE_ALL_ROWS,

	// Text control ID's
	ID_ALPHA_RANGE_CHECK,
	ID_ALPHA_CHANNEL_CHECK,

	ID_SHOW_HIDE_LAYER_CONTROL,
	ID_SHOW_HIDE_LOG_WINDOW,
	ID_BASIC_SNAPSHOT,

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

	// Bouton de crop
        ID_CROP,

        MULTI_GEOMETRIES_TYPE,

        // Plugins IDs
//        FIRST_GILVIEWER_PLUGIN,
//       LAST_GILVIEWER_PLUGIN = FIRST_GILVIEWER_PLUGIN + 100,

        LAST_GILVIEWER_ID
};


#endif // __DEFINE_ID_HPP__
