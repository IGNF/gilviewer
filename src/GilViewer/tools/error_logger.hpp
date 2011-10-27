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

#ifndef __GILVIEWER_WX_ERROR_LOGGER_HPP__
#define __GILVIEWER_WX_ERROR_LOGGER_HPP__

#include <string>

class gilviewer_wx_error_logger
{
public:
    gilviewer_wx_error_logger() {}
    static void log_error(const std::string& message);
    static void log_exception(const std::string& message);
    static void log_warning(const std::string& message);
    static void log_message(const std::string& message);

private:
    static void log_common(const std::string& message);
};

#endif // __GILVIEWER_WX_ERROR_LOGGER_HPP__
