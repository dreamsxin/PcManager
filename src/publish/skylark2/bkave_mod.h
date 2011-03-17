/**
* @file    bkave_mod.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-14 16:01
*/

#ifndef BKAVE_MOD_H
#define BKAVE_MOD_H

#include "winmod\winmodule.h"
#include "skylarkbase.h"
#include "bkave.h"

NS_SKYLARK_BEGIN

class CWinModule_bkave: public WinMod::CWinModule
{
public:
    CWinModule_bkave()
        : m_pfnBKAveInitialize(NULL)
        , m_pfnBKAveUninitialize(NULL)
        , m_pfnBKAveCreateObject(NULL)
    {     
    }

    virtual void FreeLib()
    {
        m_pfnBKAveInitialize    = NULL;
        m_pfnBKAveUninitialize  = NULL;
        m_pfnBKAveCreateObject  = NULL;
        return WinMod::CWinModule::FreeLib();
    }


    HRESULT WINAPI BKAveInitialize(BKAVE_PLATFORM_INIT* pInit)
    {
        if (NULL == m_pfnBKAveInitialize)
        {
            m_pfnBKAveInitialize = (PFN_BKAveInitialize) GetProcAddr(FN_BKAveInitialize);
            if (!m_pfnBKAveInitialize)
                return E_POINTER;
        }

        return m_pfnBKAveInitialize(pInit);
    }


    HRESULT WINAPI BKAveUninitialize()
    {
        if (NULL == m_pfnBKAveUninitialize)
        {
            m_pfnBKAveUninitialize = (PFN_BKAveUninitialize) GetProcAddr(FN_BKAveUninitialize);
            if (!m_pfnBKAveUninitialize)
                return E_POINTER;
        }

        return m_pfnBKAveUninitialize();
    }


    HRESULT WINAPI BKAveCreateObject(
        REFIID  riid, 
        void**  ppvObj,
        DWORD   dwBKEngVer = BKENG_CURRENT_VERSION)
    {
        if (NULL == m_pfnBKAveCreateObject)
        {
            m_pfnBKAveCreateObject = (PFN_BKAveCreateObject) GetProcAddr(FN_BKAveCreateObject);
            if (!m_pfnBKAveCreateObject)
                return E_POINTER;
        }

        return m_pfnBKAveCreateObject(riid, ppvObj, dwBKEngVer);
    }


private:
    PFN_BKAveInitialize     m_pfnBKAveInitialize;
    PFN_BKAveUninitialize   m_pfnBKAveUninitialize;
    PFN_BKAveCreateObject   m_pfnBKAveCreateObject;
};

NS_SKYLARK_END

#endif//BKAVE_MOD_H