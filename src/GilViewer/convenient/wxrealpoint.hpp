/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets.


Homepage:

        http://code.google.com/p/gilviewer

Copyright:

        Institut Geographique National (2011)

Authors:

        Mathieu Brédif




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

#ifndef GILVIEWER_WXREALPOINT_HPP
#define GILVIEWER_WXREALPOINT_HPP

#include <wx/gdicmn.h>
#include <cmath>
#include <iostream>

// convenience wxRealPoint operators
#if (wxMAJOR_VERSION < 3 && wxMINOR_VERSION < 9)
inline wxRealPoint& operator+=(wxRealPoint& p, const wxRealPoint&q)
{
    p=p+q;
    return p;
}
#endif

inline wxRealPoint operator-(const wxRealPoint&p)
{
    return wxRealPoint(-p.x,-p.y);
}


#if (wxMAJOR_VERSION < 3 && wxMINOR_VERSION < 9)
inline wxRealPoint operator*(double d, const wxRealPoint&p)
{
    return wxRealPoint(d*p.x,d*p.y);
}
#endif

inline double squared_distance(const wxRealPoint& p, const wxRealPoint &q)
{
    wxRealPoint diff(q - p);
    return diff.x * diff.x + diff.y * diff.y;
}

inline double dot(const wxRealPoint& p, const wxRealPoint &q)
{
    return p.x*q.x+p.y*q.y;
}

inline bool operator<(const wxRealPoint& p, const wxRealPoint &q)
{
    return (p.x<q.x) || (p.x==q.x && p.y<q.y);
}


#endif // GILVIEWER_WXREALPOINT_HPP
