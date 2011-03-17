//////////////////////////////////////////////////////////////////////////
//   File Name: bkerror.h
// Description: Beike Error Definition
//     Creator: Zhang Xiaoxuan
//     Version: 2009.6.20 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#define BKERR_MAKE_HRESULT(sev, fac, code) \
    ((HRESULT) (((unsigned long)(sev)<<31) | 1<<29 | ((unsigned long)(fac)<<16) | ((unsigned long)(code))) )

#define BKERR_MAKE_ERROR(fac, code)     BKERR_MAKE_HRESULT(SEVERITY_ERROR, fac, code)

#define BK_FACILITY_WIN32               5
#define BK_FACILITY_HTTP                6

#define BKERR_MAKE_WIN32ERROR(code)     BKERR_MAKE_ERROR(BK_FACILITY_WIN32, code)
#define BKERR_MAKE_HTTPERROR(code)      BKERR_MAKE_ERROR(BK_FACILITY_HTTP, code)

#define BKERR_FROM_LASTERROR()          BKERR_MAKE_ERROR(BK_FACILITY_WIN32, ::GetLastError())
