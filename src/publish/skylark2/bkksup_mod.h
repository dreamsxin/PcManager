/**
* @file    bkksup_mod.h
* @brief   ...
* @author  bbcallen
* @date    2010-01-26 11:05
*/

#ifndef BKKSUP_MOD_H
#define BKKSUP_MOD_H

#include "winmod\winmodule.h"
#include "skylarkbase.h"
#include "bkksup.h"

NS_SKYLARK_BEGIN

class CWinModule_bkksup: public WinMod::CWinModule
{
public:
    CWinModule_bkksup()
        : m_pfnBKKsupCreateObject(NULL)
    {     
    }

    virtual void FreeLib()
    {
        m_pfnBKKsupCreateObject  = NULL;
        return WinMod::CWinModule::FreeLib();
    }

    HRESULT WINAPI BKKsupCreateObject(
        REFIID  riid, 
        void**  ppvObj,
        DWORD   dwBKEngVer = BKENG_CURRENT_VERSION)
    {
        if (NULL == m_pfnBKKsupCreateObject)
        {
            m_pfnBKKsupCreateObject = (PFN_BKKsupCreateObject) GetProcAddr(FN_BKKsupCreateObject);
            if (!m_pfnBKKsupCreateObject)
                return E_POINTER;
        }

        return m_pfnBKKsupCreateObject(riid, ppvObj, dwBKEngVer);
    }


private:
    PFN_BKKsupCreateObject m_pfnBKKsupCreateObject;
};

NS_SKYLARK_END

#endif//BKKSUP_MOD_H