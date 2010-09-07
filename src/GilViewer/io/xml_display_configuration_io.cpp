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

#include "GilViewer/io/xml_display_configuration_io.hpp"

#include <sstream>
#include <boost/filesystem.hpp>

#include <wx/log.h>
#include <wx/msgdlg.h>

#include "tinyxml/tinyxml.h"

#include "../gui/LayerControl.hpp"
#include "../gui/PanelViewer.hpp"
#include "../tools/orientation_2d.hpp"
#include "../tools/color_lookup_table.hpp"

void xml_display_configuration_io::read( layer_control* layerControl , const std::string filename )
{
    TiXmlDocument doc( filename.c_str() );
    if ( !doc.LoadFile() )
    {
        wxLogMessage( _("Error opening file!") );
        return;
    }

    TiXmlHandle hDoc(&doc);
    TiXmlHandle hRoot(0);

    // On check que le noeud root est bien "DisplayConfiguration"
    TiXmlElement *elem = hDoc.FirstChildElement("DisplayConfiguration").Element();
    // should always have a valid root but handle gracefully if it does
    if ( !elem )
        return;

    bool is_visible = false, is_transformable = false, is_transparent = false, is_image;
    int alpha;
    double gamma;
    double min_intensities, max_intensities;
    double min_transparency, max_transparency;
    double zoom_factor = -1., translation_x = -1., translation_y = -1.;
    int alpha_channel = -1;
    bool use_alpha_channel = false;
    std::string lut_path;
    int red = 0, green = 0, blue = 0;
    std::string path;
    int points_red, points_green, points_blue, points_alpha;
    int points_width;
    int lines_red, lines_green, lines_blue, lines_alpha;
    int lines_width, lines_style;
    int polygons_rings_width, polygons_rings_red, polygons_rings_green, polygons_rings_blue, polygons_rings_alpha;
    int /*polygons_rings_width,*/ polygons_rings_style;
    int polygons_inside_red, polygons_inside_green, polygons_inside_blue, polygons_inside_alpha;
    int /*polygons_inside_width,*/ polygons_inside_style;

    int i;

    // La, c'est OK ... On itere sur les noeuds enfants : "Layers" et "Orientation"
    TiXmlNode *child = 0;
    while( (child = elem->IterateChildren( child )) )
    {
        if ( std::string(child->Value()) == "Layers" )
        {
            // La, il faut iterer sur l'ensemble des layers de la configuration
            TiXmlNode *childLayers = 0;
            while( (childLayers = child->IterateChildren( childLayers )) )
            {
                is_image = false;
                if ( std::string(childLayers->Value()) == "Layer" )
                {
                    // La, il faut iterer sur les differents aspects du layer
                    TiXmlNode *childLayer = 0;
                    while( (childLayer = childLayers->IterateChildren( childLayer )) )
                    {
                        if ( std::string(childLayer->Value()) == "Type" )
                        {
                            std::string type = childLayer->ToElement()->Attribute( "value" );
                            if ( type == "Image" )
                                is_image = true;
                            else if ( type != "Vector" )
                            {
                                wxString mes;
                                mes << _("Invalid attribute for the layer type: ") << wxString(type.c_str(), *wxConvCurrent);
                                wxLogMessage( mes );
                                return;
                            }
                        }
                        else if ( std::string(childLayer->Value()) == "Path" )
                        {
                            path = childLayer->ToElement()->Attribute( "value" );
                            if ( !boost::filesystem::exists(path) )
                            {
                                wxString mes;
                                mes << _("File ") << wxString(path.c_str(), *wxConvCurrent) << _(" does not exist!");
                                wxLogMessage( mes );
                                return;
                            }
                        }
                        else if ( std::string(childLayer->Value()) == "Appearance" )
                        {
                            // La, il faut iterer sur les noeuds enfants, en prenant en compte le type de layer
                            TiXmlNode *childAppearance = 0;
                            while( (childAppearance = childLayer->IterateChildren( childAppearance )) )
                            {
                                // Quelque soit le type de layer, il y a toujours les noeuds 'Visibility' et 'Transformability'
                                if ( std::string(childAppearance->Value()) == "Visibility" )
                                {
                                    childAppearance->ToElement()->Attribute( "value" , &i );
                                    wxString mes;
                                    mes << i;
                                    ::wxLogMessage( mes );
                                    if ( !(i == 0 || i == 1) )
                                    {
                                        wxLogMessage( _("[Node Layer - Appearance - Visibility] Error while reading attribute 'value'!") );
                                        return;
                                    }
                                    if ( i == 0 )
                                        is_visible = false;
                                    else
                                        is_visible = true;
                                }
                                else if ( std::string(childAppearance->Value()) == "Transformability" )
                                {
                                    childAppearance->ToElement()->Attribute( "value" , &i );
                                    if ( !(i == 0 || i == 1) )
                                    {
                                        ::wxLogMessage( _("[Node Layer - Appearance - Transformability] Error while reading attribute 'value' !") );
                                        return;
                                    }
                                    if ( i == 0 )
                                        is_transformable = false;
                                    else
                                        is_transformable = true;
                                }
                                else if ( std::string(childAppearance->Value()) == "ZoomFactor" )
                                {
                                    childAppearance->ToElement()->Attribute( "value" , &zoom_factor );
                                    if ( zoom_factor < 0 )
                                    {
                                        ::wxLogMessage( _("[Node Layer - Appearance - ZoomFactor] Error while reading attribute 'value' !") );
                                        return;
                                    }
                                }
                                else if ( std::string(childAppearance->Value()) == "TranslationX" )
                                {
                                    childAppearance->ToElement()->Attribute( "value" , &translation_x );
                                }
                                else if ( std::string(childAppearance->Value()) == "TranslationY" )
                                {
                                    childAppearance->ToElement()->Attribute( "value" , &translation_y );
                                }
                                // Maintenant, on distingue les cas 'Image' et 'Vector'
                                if ( is_image )
                                {
                                    // Alpha
                                    if ( std::string(childAppearance->Value()) == "Alpha" )
                                    {
                                        childAppearance->ToElement()->Attribute( "value" , &alpha );
                                        if ( alpha < 0 || alpha > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Alpha] Error while reading attribute 'value' !") );
                                            return;
                                        }
                                    }
                                    // Gamma
                                    else if ( std::string(childAppearance->Value()) == "Gamma" )
                                    {
                                        childAppearance->ToElement()->Attribute( "value" , &gamma );
                                        if ( gamma < 0 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Gamma] Error while reading attribute 'value' !") );
                                            return;
                                        }
                                    }
                                    // Intensities
                                    else if ( std::string(childAppearance->Value()) == "Intensities" )
                                    {
                                        childAppearance->ToElement()->Attribute( "min" , &min_intensities );
                                        childAppearance->ToElement()->Attribute( "max" , &max_intensities );
                                    }
                                    // Transparency
                                    else if ( std::string(childAppearance->Value()) == "Transparency" )
                                    {
                                        childAppearance->ToElement()->Attribute( "value" , &i );
                                        if ( !(i == 0 || i == 1) )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Transparency] Error while reading attribute 'min' !") );
                                            return;
                                        }
                                        if ( i == 0 )
                                            is_transparent = false;
                                        else
                                            is_transparent = true;
                                        childAppearance->ToElement()->Attribute( "min" , &min_transparency );
                                        childAppearance->ToElement()->Attribute( "max" , &max_transparency );
                                    }
                                    // Channels
                                    else if ( std::string(childAppearance->Value()) == "Channels" )
                                    {
                                        childAppearance->ToElement()->Attribute( "red" , &red );
                                        childAppearance->ToElement()->Attribute( "green" , &green );
                                        childAppearance->ToElement()->Attribute( "blue" , &blue );
                                    }
                                    // Alpha channel
                                    else if ( std::string(childAppearance->Value()) == "AlphaChannel" )
                                    {
                                        childAppearance->ToElement()->Attribute( "value" , &i );
										use_alpha_channel = i ? true : false;
                                        childAppearance->ToElement()->Attribute( "channel" , &i );
                                        alpha_channel = i;
                                    }
                                    // LUT
                                    else if ( std::string(childAppearance->Value()) == "LUT" )
                                    {
                                        lut_path = childAppearance->ToElement()->Attribute( "path" );
                                    }
                                }
                                else
                                {
                                    // Points
                                    if ( std::string(childAppearance->Value()) == "Points" )
                                    {
                                        childAppearance->FirstChild( "Colour" )->ToElement()->Attribute( "red" , &points_red );
                                        if ( points_red < 0 || points_red > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Points - Colour] Error while reading attribute 'red' !") );
                                            return;
                                        }
                                        childAppearance->FirstChild( "Colour" )->ToElement()->Attribute( "green" , &points_green );
                                        if ( points_green < 0 || points_green > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Points - Colour] Error while reading attribute 'green' !") );
                                            return;
                                        }
                                        childAppearance->FirstChild( "Colour" )->ToElement()->Attribute( "blue" , &points_blue );
                                        if ( points_blue < 0 || points_blue > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Points - Colour] Error while reading attribute 'blue' !") );
                                            return;
                                        }
                                        childAppearance->FirstChild( "Colour" )->ToElement()->Attribute( "alpha" , &points_alpha );
                                        if ( points_alpha < 0 || points_alpha > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Points - Colour] Error while reading attribute 'alpha' !") );
                                            return;
                                        }
                                        childAppearance->FirstChild( "Width" )->ToElement()->Attribute( "value" , &points_width );
                                        if ( points_width < 0 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Points - Width] Error while reading attribute 'width' !") );
                                            return;
                                        }
                                    }
                                    // Lines
                                    else if ( std::string(childAppearance->Value()) == "Lines" )
                                    {
                                        childAppearance->FirstChild( "Colour" )->ToElement()->Attribute( "red" , &lines_red );
                                        if ( lines_red < 0 || lines_red > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Lines - Colour] Error while reading attribute 'red' !") );
                                            return;
                                        }
                                        childAppearance->FirstChild( "Colour" )->ToElement()->Attribute( "green" , &lines_green );
                                        if ( lines_green < 0 || lines_green > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Lines - Colour] Error while reading attribute 'green' !") );
                                            return;
                                        }
                                        childAppearance->FirstChild( "Colour" )->ToElement()->Attribute( "blue" , &lines_blue );
                                        if ( lines_blue < 0 || lines_blue > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Lines - Colour] Error while reading attribute 'blue' !") );
                                            return;
                                        }
                                        childAppearance->FirstChild( "Colour" )->ToElement()->Attribute( "alpha" , &lines_alpha );
                                        if ( lines_alpha < 0 || lines_alpha > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Lines - Colour] Error while reading attribute 'alpha' !") );
                                            return;
                                        }
                                        childAppearance->FirstChild( "Width" )->ToElement()->Attribute( "value" , &lines_width );
                                        if ( lines_width < 0 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Lines - Width] Error while reading attribute 'width' !") );
                                            return;
                                        }
                                        childAppearance->FirstChild( "Style" )->ToElement()->Attribute( "value" , &lines_style );
                                        if ( lines_style < 0 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Lines - Style] Error while reading attribute 'style' !") );
                                            return;
                                        }
                                    }
                                    // Polygons
                                    else if ( std::string(childAppearance->Value()) == "Polygons" )
                                    {
                                        childAppearance->FirstChild( "Rings" )->FirstChild( "Colour" )->ToElement()->Attribute( "red" , &polygons_rings_red );
                                        if ( polygons_rings_red < 0 || polygons_rings_red > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Polygons - Rings - Color] Error while reading attribute 'red' !") );
                                            return;
                                        }
                                        childAppearance->FirstChild( "Rings" )->FirstChild( "Colour" )->ToElement()->Attribute( "green" , &polygons_rings_green );
                                        if ( polygons_rings_green < 0 || polygons_rings_green > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Polygons - Rings - Color] Error while reading attribute 'green' !") );
                                            return;
                                        }
                                        childAppearance->FirstChild( "Rings" )->FirstChild( "Colour" )->ToElement()->Attribute( "blue" , &polygons_rings_blue );
                                        if ( polygons_rings_blue < 0 || polygons_rings_blue > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Polygons - Rings - Color] Error while reading attribute 'blue' !") );
                                            return;
                                        }
                                        childAppearance->FirstChild( "Rings" )->FirstChild( "Colour" )->ToElement()->Attribute( "alpha" , &polygons_rings_alpha );
                                        if ( polygons_rings_alpha < 0 || polygons_rings_alpha > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Polygons - Rings - Color] Error while reading attribute 'alpha' !") );
                                            return;
                                        }
                                        childAppearance->FirstChild( "Rings" )->FirstChild( "Style" )->ToElement()->Attribute( "value" , &polygons_rings_style );
                                        if ( polygons_rings_style < 0 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Polygons - Style] Error while reading attribute 'style' !") );
                                            return;
                                        }
                                        childAppearance->FirstChild( "Rings" )->FirstChild( "Width" )->ToElement()->Attribute( "value" , &polygons_rings_width );
                                        childAppearance->FirstChild( "Inside" )->FirstChild( "Colour" )->ToElement()->Attribute( "red" , &polygons_inside_red );
                                        if ( polygons_inside_red < 0 || polygons_inside_red > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Polygons - Inside - Color] Error while reading attribute 'red' !") );
                                            return;
                                        }
                                        childAppearance->FirstChild( "Inside" )->FirstChild( "Colour" )->ToElement()->Attribute( "green" , &polygons_inside_green );
                                        if ( polygons_inside_green < 0 || polygons_inside_green > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Polygons - Inside - Color] Error while reading attribute 'green' !") );
                                            return;
                                        }
                                        childAppearance->FirstChild( "Inside" )->FirstChild( "Colour" )->ToElement()->Attribute( "blue" , &polygons_inside_blue );
                                        if ( polygons_inside_blue < 0 || polygons_inside_blue > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Polygons - Inside - Color] Error while reading attribute 'blue' !") );
                                            return;
                                        }
                                        // J'en ai marre de coder a peu pres proprement, je me mets en mode goret
                                        childAppearance->FirstChild( "Inside" )->FirstChild( "Colour" )->ToElement()->Attribute( "alpha" , &polygons_inside_alpha );
                                        if ( polygons_inside_alpha < 0 || polygons_inside_alpha > 255 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Polygons - Inside - Color] Error while reading attribute 'alpha' !") );
                                            return;
                                        }
                                        // J'en ai marre de coder a peu pres proprement, je me mets en mode goret
                                        childAppearance->FirstChild( "Inside" )->FirstChild( "Style" )->ToElement()->Attribute( "value" , &polygons_inside_style );
                                        if ( polygons_inside_style < 0 )
                                        {
                                            ::wxLogMessage( _("[Node Layer - Appearance - Polygons - Style] Error while reading attribute 'style' !") );
                                            return;
                                        }
                                    }
                                }
                            }
                        }
                        // Tout le reste est invalide, sauf si c'est un commentaire ...
                        else if ( dynamic_cast<TiXmlComment*>( childLayer ) == NULL )
                        {
                            wxString mes;
                            mes << _("[Layer] Invalid node: ") << wxString(childLayer->Value(), *wxConvCurrent);
                            ::wxLogMessage( mes );
                            return;
                        }
                    }
                }
                // Tout le reste est invalide, sauf si c'est un commentaire ...
                else if ( dynamic_cast<TiXmlComment*>( childLayers ) == NULL )
                {
                    wxString mes;
                    mes << _("Invalid node: ") << wxString(childLayers->Value(), *wxConvCurrent);
                    ::wxLogMessage( mes );
                    return;
                }

                // Si c'est un commentaire, on passe au noeud suivant ...
                if ( dynamic_cast<TiXmlComment*>( childLayers ) )
                    continue;

                if ( is_image )
                {
                    // Si on est arrive ici, on a tout ce qu'il faut pour creer un calque image
                    // On fait un petit log avec tous les parametres ...
                    /*
					wxString messsage;
					messsage << _("Image layer\n");
					messsage << _("   Path = ") << wxString(path.c_str(), *wxConvCurrent) << _("\n");
					messsage << _("   Appearance\n");
					messsage << _("      Visibility = ") << is_visible << _("\n");
					messsage << _("      Transformability = ") << is_transformable << _("\n");
					messsage << _("      ZoomFactor = ") << zoom_factor << _("\n");
					messsage << _("      TranslationX = ") << translation_x << _("\n");
					messsage << _("      TranslationY = ") << translation_y << _("\n");
					messsage << _("      Alpha = ") << alpha << _("\n");
					messsage << _("      Gamma = ") << gamma << _("\n");
					messsage << _("      Red = ") << red << _("\n");
					messsage << _("      Green = ") << green << _("\n");
					messsage << _("      Blue = ") << blue << _("\n");
					messsage << _("      Intensities\n");
					messsage << _("         min = ") << min_intensities << _("\n");
					messsage << _("         max = ") << max_intensities << _("\n");
					messsage << _("      Transparency = ") << is_transparent << _("\n");
					messsage << _("         min = ") << min_transparency << _("\n");
					messsage << _("         max = ") << max_transparency << _("\n\n");
					::wxLogMessage( messsage );
					*/

                    ImageLayerParameters parameters;
                    parameters.path = path;
                    parameters.visible = is_visible;
                    parameters.transformable = is_transformable;
                    parameters.alpha = alpha;
                    parameters.gamma = gamma;
                    parameters.intensity_min = min_intensities;
                    parameters.intensity_max = max_intensities;
                    parameters.transparent = is_transparent;
                    parameters.transparency_min = min_transparency;
                    parameters.transparency_max = max_transparency;
                    parameters.zoom_factor = zoom_factor;
                    parameters.translation_x = translation_x;
                    parameters.translation_y = translation_y;
                    parameters.red = red;
                    parameters.green = green;
                    parameters.blue = blue;
                    parameters.useAlphaChannel = use_alpha_channel;
                    parameters.alphaChannel = alpha_channel;
                    parameters.lut_file = lut_path;

                    layerControl->create_new_image_layer_with_parameters(parameters);
                }
                else
                {
                    /*
					wxString messsage;
					messsage << _("Vector layer\n");
					messsage << _("   Path = ") << wxString(path.c_str(), *wxConvCurrent) << _("\n");
					messsage << _("   Appearance\n");
					messsage << _("      Visibility = ") << is_visible << _("\n");
					messsage << _("      Transformability = ") << is_transformable << _("\n");
					messsage << _("      ZoomFactor = ") << zoom_factor << _("\n");
					messsage << _("      TranslationX = ") << translation_x << _("\n");
					messsage << _("      TranslationY = ") << translation_y << _("\n");
					messsage << _("   Points\n");
					messsage << _("      color = (") << points_red << _(",") << points_green << _(",") << points_blue << _(")\n");
					messsage << _("      width = ") << points_width << _("\n");
					messsage << _("   Lines\n");
					messsage << _("      color = (") << lines_red << _(",") << lines_green << _(",") << lines_blue << _(")\n");
					messsage << _("      width = ") << lines_width << _("\n");
					messsage << _("      style = ") << lines_style << _("\n");
					::wxLogMessage( messsage );
					*/

                    VectorLayerParameters parameters;
                    parameters.path = path;
                    parameters.visible = is_visible;
                    parameters.transformable = is_transformable;
                    parameters.pointsColor = wxColour(points_red,points_green,points_blue,points_alpha);
                    parameters.pointsWidth = points_width;
                    parameters.linesWidth = lines_width;
                    parameters.linesStyle = lines_style;
                    parameters.linesColor = wxColour(lines_red,lines_green,lines_blue,lines_alpha);
                    parameters.polygonsInsideColor = wxColour(polygons_inside_red,polygons_inside_green,polygons_inside_blue,polygons_inside_alpha);
                    parameters.polygonsRingsColor = wxColour(polygons_rings_red,polygons_rings_green,polygons_rings_blue,polygons_rings_alpha);
                    parameters.polygonsInsideStyle = polygons_inside_style;
                    parameters.polygonsRingsStyle = polygons_rings_style;
                    parameters.polygonsRingsWidth = polygons_rings_width;
                    parameters.zoom_factor = zoom_factor;
                    parameters.translation_x = translation_x;
                    parameters.translation_y = translation_y;

                    layerControl->create_new_vector_layer_with_parameters( parameters );
                }

                is_image = false;
            }
        }
        else if ( std::string(child->Value()) == "Orientation" )
        {
            // On check les valeurs de l'orientation du viewer
            TiXmlNode *childOrientation = 0;
            while( (childOrientation = child->IterateChildren( childOrientation )) )
            {
                if ( std::string(childOrientation->Value()) == "IsSet" )
                {
                    int i;
                    childOrientation->ToElement()->Attribute( "value" , &i );
                    if ( !(i == 0 || i == 1) )
                    {
                        ::wxLogMessage( _("[Node Orientation - IsSet] Error while reading attribute value' !") );
                        return;
                    }
                    if ( i == 0 )
                        layerControl->m_isOrientationSet = false;
                    else
                        layerControl->m_isOrientationSet = true;
                    wxString mes;
                    mes << _("Node 'IsSet': ") << i;
                    ::wxLogMessage( mes );
                }
                else if ( std::string(childOrientation->Value()) == "ViewerOrientation" )
                {
                    wxString mes;
                    double d;
                    int i;
                    mes << _("Node 'ViewerOrientation' : \n");
                    // OriginX
                    childOrientation->ToElement()->Attribute( "originX" , &d );
                    layerControl->m_ori->origin_x(d);
                    mes << _("   Attribute 'originX' : ") << d << wxT("\n");
                    // OriginY
                    childOrientation->ToElement()->Attribute( "originY" , &d );
                    layerControl->m_ori->origin_y(d);
                    mes << _("   Attribute 'originY' : ") << d << wxT("\n");
                    // SizeX
                    childOrientation->ToElement()->Attribute( "sizeX" , &i );
                    layerControl->m_ori->size_x(i);
                    mes << _("   Attribute 'sizeX' : ") << i << wxT("\n");
                    //SizeY
                    childOrientation->ToElement()->Attribute( "sizeY" , &i );
                    layerControl->m_ori->size_y(i);
                    mes << _("   Attribute 'sizeY' : ") << i << wxT("\n");
                    // Step
                    childOrientation->ToElement()->Attribute( "step" , &d );
                    if ( d < 0. )
                    {
                        ::wxLogMessage( _("[Node Orientation - ViewerOrientation] Error while reading attribute 'step' !") );
                        return;
                    }
                    layerControl->m_ori->step(d);
                    mes << _("   Attribute 'Step' : ") << d << wxT("\n");
                    // Zone carto
                    childOrientation->ToElement()->Attribute( "ZoneCarto" , &i );
                    layerControl->m_ori->zone_carto(i);
                    mes << _("   Attribute 'ZoneCarto' : ") << i << wxT("\n");

                    ::wxLogMessage( mes );
                }
                // Tout le reste est invalide, sauf si c'est un commentaire ...
                else if ( dynamic_cast<TiXmlComment*>( childOrientation ) == NULL )
                {
                    wxString mes;
                    mes << _("Invalid node: ") << wxString(childOrientation->Value(), *wxConvCurrent);
                    ::wxLogMessage( mes );
                    return;
                }
            }
        }
        // Tout le reste est invalide, sauf si c'est un commentaire ...
        else if ( dynamic_cast<TiXmlComment*>( child ) == NULL )
        {
            wxString mes;
            mes << _("Invalid node: ") << wxString(child->Value(), *wxConvCurrent);
            ::wxLogMessage( mes );
            return;
        }
    }
    // Finalement, on fait un petit refresh ...
    layerControl->m_basicDrawPane->Refresh();
}

void xml_display_configuration_io::write( const layer_control* layerControl , const std::string filename )
{
    if ( layerControl->rows().size() == 0 )
    {
        ::wxMessageBox(_("There is no layer to save!"),_("Error!"));
        return;
    }
    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    doc.LinkEndChild( decl );

    // Ca, c'est la racine ...
    TiXmlElement *root = new TiXmlElement( "DisplayConfiguration" );
    doc.LinkEndChild( root );

    // Un petit commentaire
    TiXmlComment *commentDisplayConfiguration = new TiXmlComment( "GilViewer - Display configuration" );
    root->LinkEndChild( commentDisplayConfiguration );

    // On passe maintenant aux differents layers. Un noeud "Layers" les contient tous
    TiXmlElement *elementLayers = new TiXmlElement( "Layers" );
    root->LinkEndChild( elementLayers );

    layer_control::const_iterator it = layerControl->begin();
    unsigned int i = 0;
    for (;it!=layerControl->end();++it,++i)
    {
        if ( (*it)->filename() == "" )
            continue;
        bool is_image = false;
        std::ostringstream oss;
        oss << "Layer " << i;
        TiXmlComment *commentLayer = new TiXmlComment( oss.str().c_str() );
        elementLayers->LinkEndChild( commentLayer );
        TiXmlElement *elementLayer = new TiXmlElement( "Layer" );
        elementLayers->LinkEndChild( elementLayer );

        // Type
        TiXmlElement *elemType = new TiXmlElement( "Type" );
        elementLayer->LinkEndChild( elemType );
        if ( (*it)->layer_type_as_string() == "Vector" )
            elemType->SetAttribute("value","Vector");
        else
        {
            is_image = true;
            elemType->SetAttribute("value","Image");
        }

        // Chemin
        TiXmlElement *elemPath = new TiXmlElement( "Path" );
        elementLayer->LinkEndChild( elemPath );
        elemPath->SetAttribute("value",(*it)->filename().c_str());

        // Tout ce qui concerne les aspects de visualisation
        TiXmlElement *elemAppearance = new TiXmlElement( "Appearance" );
        elementLayer->LinkEndChild( elemAppearance );

        // Visibilite
        TiXmlElement *elemVisibility = new TiXmlElement( "Visibility" );
        elemAppearance->LinkEndChild( elemVisibility );
        elemVisibility->SetAttribute("value",(*it)->visible());

        // Transformabilite
        TiXmlElement *elemTransformability = new TiXmlElement( "Transformability" );
        elemAppearance->LinkEndChild( elemTransformability );
        elemTransformability->SetAttribute("value",(*it)->transformable());

        // Zoom
        TiXmlElement *elemZoom = new TiXmlElement( "ZoomFactor" );
        elemAppearance->LinkEndChild( elemZoom );
        elemZoom->SetDoubleAttribute("value",(*it)->zoom_factor());

        // Translations
        TiXmlElement *elemTranslationX = new TiXmlElement( "TranslationX" );
        elemAppearance->LinkEndChild( elemTranslationX );
        elemTranslationX->SetDoubleAttribute("value",(*it)->translation_x());
        TiXmlElement *elemTranslationY = new TiXmlElement( "TranslationY" );
        elemAppearance->LinkEndChild( elemTranslationY );
        elemTranslationY->SetDoubleAttribute("value",(*it)->translation_y());

        // Ca, ca n'est valable que si le calque est une image
        if (is_image)
        {
            // Alpha
            TiXmlElement *elemAlpha = new TiXmlElement( "Alpha" );
            elemAppearance->LinkEndChild( elemAlpha );
            elemAlpha->SetAttribute("value",(*it)->alpha());

            // Gamma
            TiXmlElement *elemGamma = new TiXmlElement( "Gamma" );
            elemAppearance->LinkEndChild( elemGamma );
            elemGamma->SetDoubleAttribute("value",(*it)->gamma());

            // Intensities
            TiXmlElement *elemIntensity = new TiXmlElement( "Intensities" );
            elemAppearance->LinkEndChild( elemIntensity );
            elemIntensity->SetDoubleAttribute("min",(*it)->intensity_min());
            elemIntensity->SetDoubleAttribute("max",(*it)->intensity_max());

            // Transparency
            TiXmlElement *elemTransparency = new TiXmlElement( "Transparency" );
            elemAppearance->LinkEndChild( elemTransparency );
            elemTransparency->SetAttribute("value",(*it)->transparent());
            elemTransparency->SetDoubleAttribute("min",(*it)->transparency_min());
            elemTransparency->SetDoubleAttribute("max",(*it)->transparency_max());

            // Channels
            TiXmlElement *elemChannels = new TiXmlElement( "Channels" );
            elemAppearance->LinkEndChild( elemChannels );
            // Useless initialization to avoid compiler warning ...
            unsigned int red=0, green=0, blue=0;
            (*it)->channels(red,green,blue);
            elemChannels->SetAttribute("red",red);
            elemChannels->SetAttribute("green",green);
            elemChannels->SetAttribute("blue",blue);

            // Alpha channel
            TiXmlElement *elemAlphaChannel = new TiXmlElement( "AlphaChannel" );
            elemAppearance->LinkEndChild( elemAlphaChannel );
            // Useless initialization to avoid compiler warning ...
            bool isUsed=true;
            unsigned int channelIndex=0;
            (*it)->alpha_channel(isUsed,channelIndex);
            elemAlphaChannel->SetAttribute("value",isUsed);
            elemAlphaChannel->SetAttribute("channel",channelIndex);

             // LUT
            TiXmlElement *elemLUT = new TiXmlElement( "LUT" );
            elemAppearance->LinkEndChild( elemLUT );
            elemLUT->SetAttribute("path",(*it)->colorlookuptable()->lut_file());
        }
        else
        {
            wxColour colour;

            // Points
            TiXmlElement *elemPoints = new TiXmlElement( "Points" );
            elemAppearance->LinkEndChild( elemPoints );
            TiXmlElement *elemPointsColour = new TiXmlElement( "Colour" );
            colour = (*it)->point_color();
            elemPoints->LinkEndChild( elemPointsColour );
            elemPointsColour->SetAttribute("red",colour.Red());
            elemPointsColour->SetAttribute("green",colour.Green());
            elemPointsColour->SetAttribute("blue",colour.Blue());
            elemPointsColour->SetAttribute("alpha",colour.Alpha());
            TiXmlElement *elemPointsWidth = new TiXmlElement( "Width" );
            elemPoints->LinkEndChild( elemPointsWidth );
            elemPointsWidth->SetAttribute("value",(*it)->point_width());

            // Lines
            TiXmlElement *elemLines = new TiXmlElement( "Lines" );
            elemAppearance->LinkEndChild( elemLines );
            TiXmlElement *elemLinesColour = new TiXmlElement( "Colour" );
            colour = (*it)->line_color();
            elemLines->LinkEndChild( elemLinesColour );
            elemLinesColour->SetAttribute("red",colour.Red());
            elemLinesColour->SetAttribute("green",colour.Green());
            elemLinesColour->SetAttribute("blue",colour.Blue());
            elemLinesColour->SetAttribute("alpha",colour.Alpha());
            TiXmlElement *elemLinesWidth = new TiXmlElement( "Width" );
            elemLines->LinkEndChild( elemLinesWidth );
            elemLinesWidth->SetAttribute("value",(*it)->line_width());
            TiXmlElement *elemLinesPenStyle = new TiXmlElement( "Style" );
            elemLines->LinkEndChild( elemLinesPenStyle );
            elemLinesPenStyle->SetAttribute("value",(*it)->line_style());

            // Polygons
            TiXmlElement *elemPolygons = new TiXmlElement( "Polygons" );
            elemAppearance->LinkEndChild( elemPolygons );
            TiXmlElement *elemPolygonsRings = new TiXmlElement( "Rings" );
            elemPolygons->LinkEndChild( elemPolygonsRings );
            TiXmlElement *elemPolygonsRingsColour = new TiXmlElement( "Colour" );
            elemPolygonsRings->LinkEndChild( elemPolygonsRingsColour );
            colour = (*it)->polygon_border_color();
            elemPolygonsRingsColour->SetAttribute("red",colour.Red());
            elemPolygonsRingsColour->SetAttribute("green",colour.Green());
            elemPolygonsRingsColour->SetAttribute("blue",colour.Blue());
            elemPolygonsRingsColour->SetAttribute("alpha",colour.Alpha());
            TiXmlElement *elemPolygonsRingsWidth = new TiXmlElement( "Width" );
            elemPolygonsRings->LinkEndChild( elemPolygonsRingsWidth );
            elemPolygonsRingsWidth->SetAttribute("value",(*it)->polygon_border_width());
            TiXmlElement *elemPolygonsRingsStyle = new TiXmlElement( "Style" );
            elemPolygonsRings->LinkEndChild( elemPolygonsRingsStyle );
            elemPolygonsRingsStyle->SetAttribute("value",(*it)->polygon_border_style());
            TiXmlElement *elemPolygonsInside = new TiXmlElement( "Inside" );
            elemPolygons->LinkEndChild( elemPolygonsInside );
            TiXmlElement *elemPolygonsInsideColour = new TiXmlElement( "Colour" );
            elemPolygonsInside->LinkEndChild( elemPolygonsInsideColour );
            colour = (*it)->polygon_inner_color();
            elemPolygonsInsideColour->SetAttribute("red",colour.Red());
            elemPolygonsInsideColour->SetAttribute("green",colour.Green());
            elemPolygonsInsideColour->SetAttribute("blue",colour.Blue());
            elemPolygonsInsideColour->SetAttribute("alpha",colour.Alpha());
            TiXmlElement *elemPolygonsInsideStyle = new TiXmlElement( "Style" );
            elemPolygonsInside->LinkEndChild( elemPolygonsInsideStyle );
            elemPolygonsInsideStyle->SetAttribute("value",(*it)->polygon_inner_style());
        }
    }

    // Une fois les layers traites, on se concentre sur ce qui gere l'orientation
    TiXmlElement *elementOrientation = new TiXmlElement( "Orientation" );
    root->LinkEndChild( elementOrientation );
    TiXmlElement *elemOrientationSet = new TiXmlElement( "IsSet" );
    elementOrientation->LinkEndChild( elemOrientationSet );
    elemOrientationSet->SetAttribute("value",layerControl->m_isOrientationSet);
    TiXmlElement *elemOrientationViewer = new TiXmlElement( "ViewerOrientation" );
    elementOrientation->LinkEndChild( elemOrientationViewer );
    elemOrientationViewer->SetDoubleAttribute("originX",layerControl->m_ori->origin_x());
    elemOrientationViewer->SetDoubleAttribute("originY",layerControl->m_ori->origin_y());
    elemOrientationViewer->SetAttribute("sizeX",layerControl->m_ori->size_x());
    elemOrientationViewer->SetAttribute("sizeY",layerControl->m_ori->size_y());
    elemOrientationViewer->SetDoubleAttribute("step",layerControl->m_ori->step());
    elemOrientationViewer->SetAttribute("ZoneCarto",layerControl->m_ori->zone_carto());
    /*
	if ( &max_transparency == NULL )
	{
		wxLogMessage( _("[Noeud Layer - Appearance - Transparency] Erreur a la lecture de l'attribut 'max' !") );
		return;
	}
	*/

    doc.SaveFile( filename.c_str() );
}
