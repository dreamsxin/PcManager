/**
* @file    bkbak_mod.h
* @brief   ...
* @author  bbcallen
* @date    2010-01-05 21:47
*/

#ifndef BKBAK_MOD_H
#define BKBAK_MOD_H

#include "winmod\winmodule.h"
#include "skylarkbase.h"
#include "bkbak.h"

NS_SKYLARK_BEGIN

class CWinModule_bkbak: public WinMod::CWinModule
{
public:
    CWinModule_bkbak()
        : m_pfnBKBakCreateObject(NULL)
    {     
    }

    virtual void FreeLib()
    {
        m_pfnBKBakCreateObject  = NULL;
        return WinMod::CWinModule::FreeLib();
    }

    HRESULT WINAPI BKBakCreateObject(
        REFIID  riid, 
        void**  ppvObj,
        DWORD   dwBKEngVer = BKENG_CURRENT_VERSION)
    {
        if (NULL == m_pfnBKBakCreateObject)
        {
            m_pfnBKBakCreateObject = (PFN_BKBakCreateObject) GetProcAddr(FN_BKBakCreateObject);
            if (!m_pfnBKBakCreateObject)
                return E_POINTER;
        }

        return m_pfnBKBakCreateObject(riid, ppvObj, dwBKEngVer);
    }


private:
    PFN_BKBakCreateObject   m_pfnBKBakCreateObject;
};

NS_SKYLARK_END

#endif//BKBAK_MOD_H