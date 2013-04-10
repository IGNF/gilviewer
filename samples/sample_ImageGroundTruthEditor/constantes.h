#define FILE_PATH_SEPARATOR "/"
#define SIGNIFICANT_MASK_PIXEL_VALUE 1
#define DEFAULT_MASK_PIXEL_VALUE 0
#define EPS .00000001
#define IMAGE_DIMENSION 2
#define BEGIN_COORDINATE_INDEX 0
#define X_COORDINATE_INDEX     0
#define Y_COORDINATE_INDEX     1

#ifndef PI
#define PI 3.141592654
#endif

#include <stdexcept>
#define JPB_THROW(E)\
    std :: ostringstream erreur ; \
    erreur << "Exception" << std :: endl ; \
    erreur << "File : " << __FILE__ << std :: endl ; \
    erreur << "Function : " << __FUNCTION__ << std :: endl ; \
    erreur << "Line : " << __LINE__ << std :: endl ; \
    erreur << "Object : " << E << std :: endl ; \
    throw std :: runtime_error ( erreur . str ( ) . c_str ( ) ) ;

#define JPB_CATCH(E) \
    catch ( const std :: runtime_error & e ) \
    { \
        throw e ; \
    } \
    catch ( const std :: exception & e ) \
    { \
        throw e ; \
    } \
    catch( ... ) \
    { \
        JPB_THROW(E) \
    }

#define JPB_ITK_CATCH(E) \
    catch ( const itk :: ExceptionObject & e ) \
    { \
        throw e ; \
    } \
    JPB_CATCH ( E )
