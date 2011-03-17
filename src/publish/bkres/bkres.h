#pragma once

//////////////////////////////////////////////////////////////////////////

#ifdef __BKRES_RC

#   define DEFINE_BKRES(id, type, id_number, file_name)  \
    id_number type file_name

#else

#   define DEFINE_BKRES(id, type, id_number, file_name)  \
    const UINT id = id_number;

#endif

//////////////////////////////////////////////////////////////////////////

#   define DEFINE_XML(id, id_number, file_name)     \
    DEFINE_BKRES(id, XML, id_number, file_name)

#   define DEFINE_BMP(id, id_number, file_name)     \
    DEFINE_BKRES(id, BITMAP, id_number, file_name)

#   define DEFINE_ICO(id, id_number, file_name)     \
    DEFINE_BKRES(id, ICON, id_number, file_name)

#   define DEFINE_PNG(id, id_number, file_name)     \
    DEFINE_BKRES(id, PNG, id_number, file_name)

//////////////////////////////////////////////////////////////////////////

#ifndef __BKRES_RC

#   include "bkwinres.rc2"

#   include "bkwinres.h"

#endif
