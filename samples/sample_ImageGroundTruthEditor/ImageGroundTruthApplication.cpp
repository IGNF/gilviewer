/**
 * @author Jean-Pascal Burochin
 * @date 23/10/2012
 * @project e-PLU, WP3 : détection de façades aveugles
 *  (en vue du calcul d'enveloppes constructible)
 * @object Éditeur de rectangle dans des images
 */
#ifdef WIN32
	#pragma warning(disable : 4251)
	#pragma warning(disable : 4275)
#endif
#include "ImageGroundTruthApplication.hpp"
#include "ImageGroundTruthEditor.hpp"
#include "GilViewer/convenient/macros_gilviewer.hpp"
#include "GilViewer/tools/pattern_singleton.hpp"
#include "GilViewer/io/gilviewer_io_factory.hpp"

#include <wx/msgdlg.h>
#include <wx/log.h>

#ifdef __LINUX__
#	include <locale.h>
#endif

#include "GilViewer/config/config.hpp"
#if GILVIEWER_USE_GDAL
#   include <gdal/ogrsf_frmts.h>
#endif // GILVIEWER_USE_GDAL

IMPLEMENT_APP(ImageGroundTruthApplication)

bool ImageGroundTruthApplication::OnInit()
{
#ifdef __LINUX__
    setlocale(LC_ALL, "POSIX");
#endif

    try
    {
        ImageGroundTruthManager * p_manager = new ImageGroundTruthManager();
        mp_frame = new ImageGroundTruthEditor(p_manager, _("Éditeur de boîtes englobantes (e-PLU WP3 2012)"));
        mp_frame->Show();
    }
    catch( const std::exception &e )
    {
        using namespace std;
        GILVIEWER_LOG_EXCEPTION(e.what())
        wxString message(e.what(), *wxConvCurrent);
        wxMessageBox( message );
    }

    return true;
}
