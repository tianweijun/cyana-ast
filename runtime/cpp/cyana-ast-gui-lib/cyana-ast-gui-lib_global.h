#ifndef CYANAASTGUILIB_GLOBAL_H
#define CYANAASTGUILIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CYANAASTGUILIB_LIBRARY)
#  define CYANAASTGUILIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CYANAASTGUILIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CYANAASTGUILIB_GLOBAL_H
