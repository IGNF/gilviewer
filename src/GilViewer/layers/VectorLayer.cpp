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

#include "../layers/VectorLayer.hpp"

void VectorLayer::SetDefaultDisplayParameters()
{
    /// Points
    int redPoint = 255, bluePoint = 0, greenPoint = 0;
    int widthPoint = 3;
    /// Lignes
    int redLine = 255, blueLine = 0, greenLine = 0;
    int widthLine = 3;
    int styleLine = wxSOLID;
    /// Polygones
    int redRing = 255, blueRing = 0, greenRing = 0;
    int redShape = 255, blueShape = 0, greenShape = 0;
    int ringWidth = 1;
    int penStyle = wxSOLID;
    int brushStyle = wxSOLID;

    m_isTextVisible = true;

    set_point_color(wxColour(redPoint,greenPoint,bluePoint));
    set_point_width(widthPoint);

    set_line_color(wxColour(redLine,greenLine,blueLine));
    set_line_width(widthLine);
    set_line_style(styleLine);

    set_polygon_border_color(wxColour(redRing,greenRing,blueRing));
    set_polygon_inner_color(wxColour(redShape,greenShape,blueShape));
    set_polygon_border_width(ringWidth);
    set_polygon_border_style(penStyle);
    set_polygon_inner_style(brushStyle);
}

void VectorLayer::Clear()
{
    using namespace std;

    m_textCoordinates.clear();
    m_textValue.      clear();
    // deallocate memory
    vector<pair<double,double> >().swap(m_textCoordinates);
    vector<string              >().swap(m_textValue      );
}
