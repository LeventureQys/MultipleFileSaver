#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(TSG_FILESAVER_LIB)
#  define TSG_FILESAVER_EXPORT Q_DECL_EXPORT
# else
#  define TSG_FILESAVER_EXPORT Q_DECL_IMPORT
# endif
#else
# define TSG_FILESAVER_EXPORT
#endif
