
#pragma once

#include <PshPack4.h>

typedef struct SM_CTRL_ITEM
{
	IN	DWORD	dwSize;
	OUT DWORD	dwCtrl;
	// 返回的字符串，不需要外边来释放!
	OUT	LPCTSTR	lpVirusName; // 扫描出的名称
	OUT	DWORD	dwFileEngine; // SM_FILE_ENGINE_XX
	OUT	DWORD	dwFileType; // SM_FILE_TYPE_XX
	OUT	LPCTSTR	lpFilePath; // 如果不为null, 一定是文件路径
	OUT LPCTSTR	lpPath; // lpPath ~= lpFilePath, 也可能是文件路径，也可能是注册表路径, 都是全路径
} SM_CTRL_ITEM;

#include <PopPack.h>

class ISafeMonitorCtrlQuerier
{
public:
	virtual BOOL	Init() = 0;
	virtual void	Uninit() = 0;

	virtual BOOL	Enum(DWORD dwIndex, SM_CTRL_ITEM* lpItem) = 0;
	virtual	BOOL	Delete(DWORD dwIndex) = 0;
	virtual	BOOL	Save() = 0;
};


typedef ISafeMonitorCtrlQuerier* (WINAPI* PFN_NewSafeMonitorCtrlQuerier)();
typedef void (WINAPI* PFN_FreeSafeMonitorCtrlQuerier)(ISafeMonitorCtrlQuerier** lpObject);

class CSafeMonitorCtrlQuerierLoader
{
public:
	CSafeMonitorCtrlQuerierLoader()
	{
		m_pfnNew = NULL;
		m_pfnFree = NULL;

		m_hModule = LoadLibrary(_T("ksfmon.dll"));
		if ( m_hModule != NULL )
		{
			m_pfnNew = (PFN_NewSafeMonitorCtrlQuerier)GetProcAddress(m_hModule, "C1");
			m_pfnFree = (PFN_FreeSafeMonitorCtrlQuerier)GetProcAddress(m_hModule, "C2");
		}
	}

	~CSafeMonitorCtrlQuerierLoader()
	{
		FreeLibrary(m_hModule);
	}

	operator bool ()
	{
		return (m_hModule != NULL) && (m_pfnFree != NULL) && (m_pfnNew != NULL);
	}

	ISafeMonitorCtrlQuerier* NewSafeMonitorCtrlQuerier()
	{
		assert(m_pfnNew != NULL);

		return (m_pfnNew != NULL) ? m_pfnNew() : NULL;
	}

	void FreeSafeMonitorCtrlQuerier(ISafeMonitorCtrlQuerier** lppObj)
	{
		assert(m_pfnFree != NULL);

		if ( m_pfnFree != NULL )
		{
			m_pfnFree(lppObj);
		}
	}

protected:
	PFN_NewSafeMonitorCtrlQuerier m_pfnNew;
	PFN_FreeSafeMonitorCtrlQuerier m_pfnFree;
	HMODULE		m_hModule;
};

/*
inline void exmp_ISafeMonitorCtrlQuerier()
{
	CSafeMonitorCtrlQuerierLoader loader;

	ISafeMonitorCtrlQuerier* a = loader.NewSafeMonitorCtrlQuerier();
	if ( a != NULL )
	{
		SM_CTRL_ITEM item = { 0 };

		a->Init();
	
		DWORD dwIndex = 0;
		item.dwSize = sizeof (item);

		while ( a->Enum(dwIndex, &item) )
		{
			item.lpPath = item.lpPath;
			dwIndex++;
		}

		a->Delete(1);
		a->Save();
		a->Uninit();

		CSafeMonitorTrayShell::NotifySafeMonitorCtrlUpdated();

		loader.FreeSafeMonitorCtrlQuerier(&a);
	}
}
*/