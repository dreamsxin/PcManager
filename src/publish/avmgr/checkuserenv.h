#pragma once

#include <bksafe/bksafedefines.h>
#include "avmgr/avmgrinterface.h"
#include "communits/Locker.h"

class CCheckUserEnv
{
public:
	CCheckUserEnv()
	{
		m_hAVMgrModule = NULL;
		m_pfnCreateAVProductMgr = NULL;
		m_pfnFreeAVProductMrg = NULL;
		m_lpAVMgr = NULL;
	}

	virtual ~CCheckUserEnv()
	{
		FreeAVMgrModule();
	}

	BOOL IsMachineRubbish()
	{
		static BOOL bCheck	= FALSE;
		static BOOL bRes	= FALSE;

		if ( bCheck )
			return bRes;

		// 如果用户电脑配置为：单核，<=512MB内存，建议用户设置为轻巧模式
		SYSTEM_INFO si = { 0 };
		MEMORYSTATUSEX ms = { 0 };

		ms.dwLength = sizeof (ms);

		GetSystemInfo(&si);
		bRes = (si.dwNumberOfProcessors == 1 &&
			GlobalMemoryStatusEx(&ms) &&
			ms.ullTotalPhys <= 512 * 1024 * 1024
			);

		bCheck = TRUE;
		return bRes;
	}


	BOOL Check3rdAntiVirus(CString* pstrAvNames = NULL)
	{
		BOOL bResult = FALSE;
		CString strAvNames;
		
		m_lkAVMgr.Lock();
		if ( LoadAVMgrModule() )
		{
			if ( m_lpAVMgr != NULL )
			{
				LPCTSTR lpAvNames = m_lpAVMgr->GetInstallAVs();
				if ( lpAvNames != NULL )
				{
					strAvNames	= lpAvNames;
					bResult	= TRUE;
				}
			}
		}
		m_lkAVMgr.Unlock();

		if (pstrAvNames && !strAvNames.IsEmpty() )
		{
			*pstrAvNames = strAvNames;
		}

		return bResult;
	}

protected:
	void FreeAVMgrModule()
	{
		m_lkAVMgr.Lock();

		if ( m_hAVMgrModule == NULL )
		{
			if ( m_pfnFreeAVProductMrg != NULL && m_lpAVMgr != NULL )
			{
				m_pfnFreeAVProductMrg(m_lpAVMgr);
				m_lpAVMgr = NULL;
			}

			FreeLibrary(m_hAVMgrModule);
			m_hAVMgrModule = NULL;

			m_pfnCreateAVProductMgr = NULL;
			m_pfnFreeAVProductMrg = NULL;
		}

		m_lkAVMgr.Unlock();
	}

	BOOL LoadAVMgrModule()
	{
		m_lkAVMgr.Lock();

		if ( m_hAVMgrModule == NULL )
		{
			m_lpAVMgr = NULL;

			m_hAVMgrModule = LoadLibrary(BKPLAT__DLL_AVMGR);
			if ( m_hAVMgrModule != NULL )
			{
				m_pfnCreateAVProductMgr = (PFN_CreateAVProductMgr)GetProcAddress(m_hAVMgrModule, FN_CreateAVProductMgr);
				m_pfnFreeAVProductMrg = (PFN_FreeAVProductMgr)GetProcAddress(m_hAVMgrModule, FN_FreeAVProductMgr);

				if ( m_pfnCreateAVProductMgr != NULL && m_pfnFreeAVProductMrg != NULL )
				{
					CString strLibPath;

					CShandianPath::Instance().GetProductDataPath(strLibPath);
					strLibPath += _T("\\knavlib.dat");

					HRESULT hr = m_pfnCreateAVProductMgr(__uuidof(IAVProductMgr), (PVOID*)&m_lpAVMgr);
					if ( hr == S_OK && m_lpAVMgr != NULL )
					{
						m_lpAVMgr->LoadAVLib(strLibPath);
					}
				}
			}
		}

		m_lkAVMgr.Unlock();

		return (m_lpAVMgr != NULL);
	}

protected:
	HMODULE					m_hAVMgrModule;
	IAVProductMgr*			m_lpAVMgr;
	PFN_CreateAVProductMgr	m_pfnCreateAVProductMgr;
	PFN_FreeAVProductMgr	m_pfnFreeAVProductMrg;
	CLocker					m_lkAVMgr;
};