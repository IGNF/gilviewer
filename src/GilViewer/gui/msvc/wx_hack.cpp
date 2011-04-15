// We need to include this file with some compilers ...
// See CMakeLists.txt

#ifdef _WINDOWS
#	include <wx/msw/winundef.h>

/* seen on http://onegazhang.spaces.live.com/Blog/cns!D5E642BC862BA286!480.entry */
#	include <wx/apptrait.h>
#	if wxUSE_STACKWALKER && defined( __WXDEBUG__ )
	// silly workaround for the link error with debug configuration:
	// \src\common\appbase.cpp
	wxString wxAppTraitsBase::GetAssertStackTrace()
	{
	return wxT("");
	}
#	endif // wxUSE_STACKWALKER && defined( __WXDEBUG__ )

#endif // _WINDOWS