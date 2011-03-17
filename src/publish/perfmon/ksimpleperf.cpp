////////////////////////////////////////////////////////////////////////////////
//      
//      File for perf mon
//      
//      File      : ksimpleperf.cpp
//      Version   : 1.0
//      Comment   : 简单系统性能监视，获取cpu和内存使用
//      
//      Create at : 2011-2-14
//      Create by : liupeng
//		Motify by : 
//      
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ksimpleperf.h"

//////////////////////////////////////////////////////////////////////////
KSimplePerfMgr::KSimplePerfMgr(): m_dwMemUsage(0),
m_dwCpuUsage(0),
m_pfnNtQuerySystemInformation(NULL)
{
	memset(&m_LastInfo, 0, sizeof(m_LastInfo));
	memset(&m_LastTime, 0, sizeof(m_LastTime));
	m_dwLastTickCount = 0;
}

KSimplePerfMgr::~KSimplePerfMgr()
{

}

HRESULT	KSimplePerfMgr::Init()
{
	HMODULE hDll = GetModuleHandle(_T("ntdll.dll"));

	if (m_pfnNtQuerySystemInformation = (NtQuerySystemInformationFun*)GetProcAddress(hDll, "NtQuerySystemInformation"))
	{
		SYSTEM_PERFORMANCE_INFORMATION systemPerformance = {0};
		SYSTEM_TIME_INFORMATION newSystemTimeInformation = {0};
		char szBuffer[1024] = {0};

		ULONG ulRetrunLength = 0;
		NTSTATUS status = m_pfnNtQuerySystemInformation(
			SystemTimeOfDayInformation,
			&newSystemTimeInformation,
			sizeof(SYSTEM_TIME_INFORMATION),
			&ulRetrunLength
			);
		if (status == NO_ERROR && newSystemTimeInformation.CurrentTime.QuadPart != 0)
		{
			m_LastTime = newSystemTimeInformation;
		}
		else
		{
			m_dwLastTickCount = GetTickCount();
		}

		m_pfnNtQuerySystemInformation(
			SystemPerformanceInformation,
			&szBuffer,
			sizeof(szBuffer),
			&ulRetrunLength
			);
		m_LastInfo = *((SYSTEM_PERFORMANCE_INFORMATION_DETAIL*)szBuffer);

		SYSTEM_INFO sysInfo = {0};
		GetSystemInfo(&sysInfo);
		m_dwNumberProcessors = sysInfo.dwNumberOfProcessors;
		if (m_dwNumberProcessors == 0)
		{
			m_dwNumberProcessors = 1;
		}
		return S_OK;
	}
	return E_FAIL;
}

HRESULT	KSimplePerfMgr::UnInit()
{
	return S_OK;
}

DWORD	KSimplePerfMgr::GetSysMemUsage()
{
	return m_dwMemUsage;
}

DWORD  KSimplePerfMgr::GetSysCpuUsage()
{
	return m_dwCpuUsage;
}

VOID KSimplePerfMgr::Update()
{
	UpdateSystemCpuAndMem();
	if (m_pfnNtQuerySystemInformation)
	{
		SYSTEM_TIME_INFORMATION newSystemTimeInformation = {0};
		char szBuffer[1024] = {0};

		ULONG ulRetrunLength = 0;
		m_pfnNtQuerySystemInformation(
			SystemPerformanceInformation,
			szBuffer,
			sizeof(szBuffer),
			&ulRetrunLength
			);

		LONGLONG elapseTime = 0;
		if (m_dwLastTickCount != 0)
		{
			DWORD dwNewTickCount = GetTickCount();
			elapseTime = LONGLONG(dwNewTickCount - m_dwLastTickCount) * 10000;
			if (elapseTime >= 1000000)
			{
				m_dwLastTickCount = dwNewTickCount;
			}
		}
		else
		{
			m_pfnNtQuerySystemInformation(
				SystemTimeOfDayInformation,
				&newSystemTimeInformation,
				sizeof(SYSTEM_TIME_INFORMATION),
				&ulRetrunLength
				);
			elapseTime = newSystemTimeInformation.CurrentTime.QuadPart - m_LastTime.CurrentTime.QuadPart;
			if (elapseTime >= 1000000)
			{
				m_LastTime = newSystemTimeInformation;
			}
		}

		if (elapseTime >= 1000000)  // 如果经历的时间小于100ms，则跳过重新计算
		{
			// 64位系统下时间数据时没有问题的，其他字节的数据需要处理
			SYSTEM_PERFORMANCE_INFORMATION_DETAIL* pDetail = (SYSTEM_PERFORMANCE_INFORMATION_DETAIL*)szBuffer;

			LONGLONG idleTime = pDetail->liIdleTime.QuadPart - m_LastInfo.liIdleTime.QuadPart;
			idleTime *= 100;		
			m_dwCpuUsage = 100 - idleTime / (elapseTime * m_dwNumberProcessors);

			m_LastInfo = *pDetail;
		}
	}
}

void KSimplePerfMgr::UpdateSystemCpuAndMem()
{
	MEMORYSTATUSEX statex = {0};
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);
	m_dwMemUsage = 100 - (double)100 * statex.ullAvailPhys / statex.ullTotalPhys;
}
