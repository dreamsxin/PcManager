/**
* @file    sfc_osmod.h
* @brief   ...
* @author  bbcallen
* @date    2009-08-10 10:57
*/

#ifndef SFC_OSMOD_H
#define SFC_OSMOD_H


#include <psapi.h>
#include "winmodule.h"

NS_WINMOD_BEGIN

typedef DWORD(__stdcall *PFN_SetSfcFileException) (DWORD param1, LPCWSTR param2, DWORD param3);

class CWinModule_sfc_os: public CWinModule
{
public:
    CWinModule_sfc_os():
        m_pfnSetSfcFileException(NULL)
    {

    }

    virtual void FreeLib()
    {
        m_pfnSetSfcFileException = NULL;
        return CWinModule::FreeLib();
    }

    DWORD WINAPI SetSfcFileException(
        DWORD   param1,  /* = 0 */
        LPCWSTR param2,
        DWORD   param3   /* = -1 */)
    {
        if (NULL == m_pfnSetSfcFileException)
        {
            m_pfnSetSfcFileException =
                (PFN_SetSfcFileException) GetProcAddr((LPCSTR)5);

            if (!m_pfnSetSfcFileException)
                return 0;
        }

        return m_pfnSetSfcFileException(param1, param2, param3);
    }

private:
    PFN_SetSfcFileException m_pfnSetSfcFileException;
};

NS_WINMOD_END

#endif//SFC_OSMOD_H