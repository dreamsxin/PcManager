/**
* @file    bkdb_mod.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-21 21:39
*/

#ifndef BKDB_MOD_H
#define BKDB_MOD_H

#include "winmod\winmodule.h"
#include "skylarkbase.h"
#include "bkdb.h"

NS_SKYLARK_BEGIN

class CWinModule_bkdb: public WinMod::CWinModule
{
public:
    CWinModule_bkdb()
        : m_pfnBKDbInitialize(NULL)
        , m_pfnBKDbUninitialize(NULL)
        , m_pfnBKDbCreateObject(NULL)
    {     
    }

    virtual void FreeLib()
    {
        m_pfnBKDbInitialize     = NULL;
        m_pfnBKDbUninitialize   = NULL;
        m_pfnBKDbCreateObject   = NULL;
        return WinMod::CWinModule::FreeLib();
    }

    HRESULT WINAPI BKDbInitialize(BKDB_PLATFORM_INIT* pInit)
    {
        if (NULL == m_pfnBKDbInitialize)
        {
            m_pfnBKDbInitialize = (PFN_BKDbInitialize) GetProcAddr(FN_BKDbInitialize);
            if (!m_pfnBKDbInitialize)
                return E_POINTER;
        }

        return m_pfnBKDbInitialize(pInit);
    }


    HRESULT WINAPI BKDbUninitialize()
    {
        if (NULL == m_pfnBKDbUninitialize)
        {
            m_pfnBKDbUninitialize = (PFN_BKDbUninitialize) GetProcAddr(FN_BKDbUninitialize);
            if (!m_pfnBKDbUninitialize)
                return E_POINTER;
        }

        return m_pfnBKDbUninitialize();
    }


    HRESULT WINAPI BKDbCreateObject(
        REFIID  riid, 
        void**  ppvObj,
        DWORD   dwBKEngVer = BKENG_CURRENT_VERSION)
    {
        if (NULL == m_pfnBKDbCreateObject)
        {
            m_pfnBKDbCreateObject = (PFN_BKDbCreateObject) GetProcAddr(FN_BKDbCreateObject);
            if (!m_pfnBKDbCreateObject)
                return E_POINTER;
        }

        return m_pfnBKDbCreateObject(riid, ppvObj, dwBKEngVer);
    }


private:
    PFN_BKDbInitialize      m_pfnBKDbInitialize;
    PFN_BKDbUninitialize    m_pfnBKDbUninitialize;
    PFN_BKDbCreateObject    m_pfnBKDbCreateObject;
};

NS_SKYLARK_END

#endif//BKDB_MOD_H