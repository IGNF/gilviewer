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

#ifndef __SAMPLE_VECTOR_LAYER_APP_H__
#define __SAMPLE_VECTOR_LAYER_APP_H__

#include <wx/app.h>

class sample_vector_layer_viewer;

class sample_vector_layer_app: public wxApp
{
	bool OnInit();

	// Langage
	void set_langage(unsigned int language_id);

        sample_vector_layer_viewer *m_frame;

};

DECLARE_APP(sample_vector_layer_app);

#endif /* __SAMPLE_VECTOR_LAYER_APP_H__ */
