/**
* @file    bkatrun_mod.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-28 17:17
*/

#ifndef BKATRUN_MOD_H
#define BKATRUN_MOD_H

#include "winmod\winmodule.h"
#include "skylarkbase.h"
#include "bkatrun.h"

NS_SKYLARK_BEGIN

class CWinModule_bkatrun: public WinMod::CWinModule
{
public:
    CWinModule_bkatrun()
        : m_pfnBKAtrunCreateObject(NULL)
    {     
    }


    virtual void FreeLib()
    {
        m_pfnBKAtrunCreateObject = NULL;
        return WinMod::CWinModule::FreeLib();
    }


    HRESULT WINAPI BKAtrunCreateObject(
        REFIID  riid, 
        void**  ppvObj,
        DWORD   dwBKEngVer = BKENG_CURRENT_VERSION)
    {
        if (NULL == m_pfnBKAtrunCreateObject)
        {
            m_pfnBKAtrunCreateObject = (PFN_BKAtrunCreateObject) GetProcAddr(FN_BKAtrunCreateObject);
            if (!m_pfnBKAtrunCreateObject)
                return E_POINTER;
        }

        return m_pfnBKAtrunCreateObject(riid, ppvObj, dwBKEngVer);
    }


private:
    PFN_BKAtrunCreateObject   m_pfnBKAtrunCreateObject;
};

NS_SKYLARK_END

#endif//BKATRUN_MOD_H