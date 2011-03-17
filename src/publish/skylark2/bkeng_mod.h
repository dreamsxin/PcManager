/**
* @file    bkeng_mod.h
* @brief   ...
* @author  bbcallen
* @date    2010-04-09 14:21
*/

#ifndef BKENG_MOD_H
#define BKENG_MOD_H

#include "winmod\winmodule.h"
#include "skylarkbase.h"
#include "bkeng.h"

NS_SKYLARK_BEGIN

class CWinModule_bkeng: public WinMod::CWinModule
{
public:
    CWinModule_bkeng()
        : m_pfnBKEngInitialize(NULL)
        , m_pfnBKEngUninitialize(NULL)
        , m_pfnBKEngCreateObject(NULL)
    {     
    }

    virtual void FreeLib()
    {
        m_pfnBKEngInitialize    = NULL;
        m_pfnBKEngUninitialize  = NULL;
        m_pfnBKEngCreateObject  = NULL;
        return WinMod::CWinModule::FreeLib();
    }


    HRESULT WINAPI BKEngInitialize(BKENG_PLATFORM_INIT* pInit)
    {
        if (NULL == m_pfnBKEngInitialize)
        {
            m_pfnBKEngInitialize = (PFN_BKEngInitialize) GetProcAddr(FN_BKEngInitialize);
            if (!m_pfnBKEngInitialize)
                return E_POINTER;
        }

        return m_pfnBKEngInitialize(pInit);
    }


    HRESULT WINAPI BKEngUninitialize()
    {
        if (NULL == m_pfnBKEngUninitialize)
        {
            m_pfnBKEngUninitialize = (PFN_BKEngUninitialize) GetProcAddr(FN_BKEngUninitialize);
            if (!m_pfnBKEngUninitialize)
                return E_POINTER;
        }

        return m_pfnBKEngUninitialize();
    }


    HRESULT WINAPI BKEngCreateObject(
        REFIID  riid, 
        void**  ppvObj,
        DWORD   dwBKEngVer = BKENG_CURRENT_VERSION)
    {
        if (NULL == m_pfnBKEngCreateObject)
        {
            m_pfnBKEngCreateObject = (PFN_BKEngCreateObject) GetProcAddr(FN_BKEngCreateObject);
            if (!m_pfnBKEngCreateObject)
                return E_POINTER;
        }

        return m_pfnBKEngCreateObject(riid, ppvObj, dwBKEngVer);
    }


private:
    PFN_BKEngInitialize     m_pfnBKEngInitialize;
    PFN_BKEngUninitialize   m_pfnBKEngUninitialize;
    PFN_BKEngCreateObject   m_pfnBKEngCreateObject;
};

NS_SKYLARK_END

#endif//BKENG_MOD_H