/**
 * @author Jean-Pascal Burochin
 * @date 23/10/2012
 * @project e-PLU, WP3 : détection de façades aveugles
 *  (en vue du calcul d'enveloppes constructible)
 * @object Éditeur de rectangle dans des images
 */
#ifndef __ImageGroundTruthApplication__
#define __ImageGroundTruthApplication__
#include <wx/app.h>

class ImageGroundTruthEditor;

class ImageGroundTruthApplication: public wxApp
{
    bool OnInit();

    ImageGroundTruthEditor * mp_frame;
};

DECLARE_APP(ImageGroundTruthApplication)

#endif /* _GILVIEWER_H_ */
