/**
* @file    bkcache_mod.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-21 22:00
*/

#ifndef BKCACHE_MOD_H
#define BKCACHE_MOD_H

#include "winmod\winmodule.h"
#include "skylarkbase.h"
#include "bkcache.h"

NS_SKYLARK_BEGIN

class CWinModule_bkcache: public WinMod::CWinModule
{
public:
    CWinModule_bkcache()
        : m_pfnBKCacheInitialize(NULL)
        , m_pfnBKCacheCreateObject(NULL)
    {     
    }


    virtual void FreeLib()
    {
        m_pfnBKCacheInitialize      = NULL;
        m_pfnBKCacheCreateObject    = NULL;
        return WinMod::CWinModule::FreeLib();
    }


    HRESULT WINAPI BKCacheInitialize()
    {
        if (NULL == m_pfnBKCacheInitialize)
        {
            m_pfnBKCacheInitialize = (PFN_BKCacheInitialize) GetProcAddr(FN_BKCacheInitialize);
            if (!m_pfnBKCacheInitialize)
                return E_POINTER;
        }

        return m_pfnBKCacheInitialize();
    }


    HRESULT WINAPI BKCacheCreateObject(
        REFIID  riid, 
        void**  ppvObj,
        DWORD   dwBKEngVer = BKENG_CURRENT_VERSION)
    {
        if (NULL == m_pfnBKCacheCreateObject)
        {
            m_pfnBKCacheCreateObject = (PFN_BKCacheCreateObject) GetProcAddr(FN_BKCacheCreateObject);
            if (!m_pfnBKCacheCreateObject)
                return E_POINTER;
        }

        return m_pfnBKCacheCreateObject(riid, ppvObj, dwBKEngVer);
    }


private:
    PFN_BKCacheInitialize   m_pfnBKCacheInitialize;
    PFN_BKCacheCreateObject m_pfnBKCacheCreateObject;
};

NS_SKYLARK_END

#endif//BKCACHE_MOD_H