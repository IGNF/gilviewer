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

#ifndef COLORLOOKUPTABLE_H_
#define COLORLOOKUPTABLE_H_

#include <vector>
#include <string>

/**
 * @brief LUT (Look Up Table) handling
 *
 * Creates by default a gray levels LUT. Can load LUT in the same format as ImageJ.<br>
 * Stored in a binary file, the LUT must contain 768 octets: <b>256</b> values for each of the <b>3</b> channels.<br>
 * Stored in a text file, the LUT must have <b>3 columns</b> (R, G and B) and <b>256</b> lines.<br>
 *
 **/
class color_lookup_table
{
public:
    color_lookup_table();
    virtual ~color_lookup_table(){}

    /// Loads a LUT from a binary file (the file must contain 768 octets = 256 * 3)
    void load_from_binary_file(const std::string &fileCLUT);
    /// Loads a LUT from a text file (the file must 3 columns RGB and 256 lines)
    void load_from_text_file(const std::string &fileCLUT);
    /// Creates a random LUT
    void create_random();

    /// Returns the LUT data container
    const std::vector<unsigned char>& get_data() const { return m_clut; }

private:
    /// The LUT data container
    std::vector<unsigned char> m_clut;
};

typedef color_lookup_table ColorLookupTable;

#endif /*COLORLOOKUPTABLE_H_*/
