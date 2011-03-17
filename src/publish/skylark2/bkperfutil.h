/**
* @file    bkperfutil.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-29 21:14
*/

#ifndef BKPERFUTIL_H
#define BKPERFUTIL_H

#include <atltime.h>
#include "com_s\com\scom.h"
#include "bkperf.h"

NS_SKYLARK_BEGIN




class CProtoPerfGuard
{
public:
    CProtoPerfGuard(DWORD dwPerfID, IBKEngPerformanceCounter* perfCounter)
        : m_uTickBegin(0)
        , m_dwPerfID(dwPerfID)
        , m_piPerfCounter(perfCounter)
    {
        if (m_piPerfCounter)
            m_uTickBegin = m_piPerfCounter->GetTick();
    }

    ~CProtoPerfGuard()
    {
        CountAndRelease();
    }

    ULONGLONG CountAndRelease()
    {
        if (m_piPerfCounter)
        {
            ULONGLONG uTickEnd  = m_piPerfCounter->GetTick();
            ULONGLONG uTickDiff = uTickEnd - m_uTickBegin;
            m_piPerfCounter->AddPerf(m_dwPerfID, uTickDiff);
            m_piPerfCounter = NULL;
            return uTickDiff;
        }

        return 0;
    }

    ULONGLONG                   m_uTickBegin;
    DWORD                       m_dwPerfID;
    IBKEngPerformanceCounter*   m_piPerfCounter;
};





/// 性能计数的简单实现
class ABKEngPerformanceCounter:
    public IBKEngPerformanceView,
    public IBKEngPerformanceCounter
{
public:

    typedef CAtlMap<DWORD, BKENG_PERF_DATA>  CPerfMap;



    virtual HRESULT STDMETHODCALLTYPE GetPerformanceData(
        DWORD               dwPerfID,
        BKENG_PERF_DATA*    pPerfData)
    {
        CObjGuard guard(m_objLock);

        if (!pPerfData)
            return E_POINTER;

        BKENG_INIT(pPerfData);
        m_perfMap.Lookup(dwPerfID, *pPerfData);

        return S_OK;
    }

    virtual ULONGLONG STDMETHODCALLTYPE TickToSystemTime(ULONGLONG uTick)
    {
#ifdef _DEBUG
        LARGE_INTEGER liFrequency;
        if (!QueryPerformanceFrequency(&liFrequency))
            return 0;
        
        if (!liFrequency.QuadPart)
            return 0;

        return uTick * CFileTime::Second / liFrequency.QuadPart;
#else
        return uTick;
#endif
    }





    virtual void STDMETHODCALLTYPE ResetPerf()
    {
        CObjGuard guard(m_objLock);

        m_perfMap.RemoveAll();
    }


    virtual ULONGLONG STDMETHODCALLTYPE GetTick()
    {
#ifdef _DEBUG
        LARGE_INTEGER liPerform;
        ::QueryPerformanceCounter(&liPerform);
        return liPerform.QuadPart;
#else
        CFileTime ftBegin;
        GetSystemTimeAsFileTime(&ftBegin);
        return ftBegin.GetTime();
#endif
    }

    virtual void STDMETHODCALLTYPE AddPerf(DWORD dwPerfID, ULONGLONG uTickDiff)
    {
        CObjGuard guard(m_objLock);

        BKENG_PERF_DATA perfData;
        BKENG_INIT(&perfData);
        if (m_perfMap.Lookup(dwPerfID, perfData))
        {
            perfData.m_uPerfCount += 1;
            perfData.m_uPerfTotal += uTickDiff;
            

            // 简单插入排序
            size_t i = 0;
            for (NULL; i < _countof(perfData.m_uTopPerf); ++i)
            {
                if (uTickDiff > perfData.m_uTopPerf[i])
                    break;
            }


            for (NULL; i < _countof(perfData.m_uTopPerf); ++i)
            {
                std::swap(uTickDiff, perfData.m_uTopPerf[i]);
            }
        }
        else
        {
            perfData.m_uPerfCount   = 1;
            perfData.m_uPerfTotal   = uTickDiff;

            perfData.m_uTopPerf[0]  = uTickDiff;
        }

        m_perfMap.SetAt(dwPerfID, perfData);
    }





private:
    typedef CComAutoCriticalSection                     CObjLock;
    typedef CComCritSecLock<CComAutoCriticalSection>    CObjGuard;

    CObjLock m_objLock;
    CPerfMap m_perfMap;
};



class CBKEngPerformanceCounter: public ABKEngPerformanceCounter
{
public:
    SCOM_BEGIN_COM_MAP(CBKEngPerformanceCounter)
        SCOM_INTERFACE_ENTRY(IBKEngPerformanceView)
        SCOM_INTERFACE_ENTRY(IBKEngPerformanceCounter)
    SCOM_END_COM_MAP()
};





NS_SKYLARK_END

#endif//BKPERFUTIL_H