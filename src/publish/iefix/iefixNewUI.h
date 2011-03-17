#pragma once


//////////////////////////////////////////////////////////////////////////
// 浏览器修复跟系统修复调用同样的接口
//////////////////////////////////////////////////////////////////////////

#define DLL_NAME_NEW_IEFIX		_T("kiefixeng.dll")

#define NUM_MAX_IEFIX_SCAN_FAST		10
#define NUM_MAX_IEFIX_SCAN_SLOW		11

#define VER_KSWEB_IEFIX			1
#define STR_EXPORT_INTERFACE			"CreateNewIeFixEng"
#define STR_CREATE_SYS_FIX_INTERFACE	"CreateNewSysFixEng"

#define NEW_IE_FIX_SCAN_TYPE_FAST	1
#define NEW_IE_FIX_SCAN_TYPE_SLOW	2

struct KsWebIeInitData
{
	KsWebIeInitData()
	{
		lpstrName	= NULL;
		dwId		= -1;
		lpstrDesc	= NULL;
	}

	DWORD		dwId;
	LPCTSTR		lpstrName;
	LPCTSTR		lpstrDesc;
};

struct KSWebIeScanData
{
	KSWebIeScanData()
	{
		bScanning	= FALSE;
		bScanned	= FALSE;
		nDanger		= 0;
		bVerifying	= FALSE;
	}

	BOOL	bScanning;
	BOOL	bScanned;
	DWORD	nDanger;
	BOOL	bVerifying;
};

struct KSWebIEFixProgressData
{
	KSWebIeScanData*	arrayx;
	DWORD				nsize;
	DWORD				nRate;
	LPCWSTR				lpFile;
};

class IKSWebIEFixEngCallback
{
public:
	virtual VOID	KsIeFix_ScanStart()					= 0;
	virtual	VOID	KsIeFix_ScanEnd(BOOL bStopped)		= 0;
	virtual	VOID	KsIeFix_FindItem()					= 0;
	virtual VOID	KsIeFix_ScanProgress(KSWebIEFixProgressData* pData)	= 0;
	virtual	VOID	KsIeFix_RemoveStart()				= 0;
	virtual	VOID	KsIeFix_RemoveProgress()			= 0;
	virtual	VOID	KsIeFix_RemoveEnd(BOOL bNeedReboot, DWORD nCount )	= 0;

	// 初始化成功后抛出一些数据回来进行要扫描数据的初始化
	virtual VOID	KsIeFix_InitData(KsWebIeInitData* pArray, DWORD nSize)	= 0;
};

class IKSWebIEFixEngCallbackNull :public IKSWebIEFixEngCallback
{
public:
	virtual VOID	KsIeFix_ScanStart(){return;}
	virtual	VOID	KsIeFix_ScanEnd(BOOL bStopped){return;}
	virtual	VOID	KsIeFix_FindItem(){return;}
	virtual VOID	KsIeFix_ScanProgress(KSWebIEFixProgressData* pData){return;}
	virtual	VOID	KsIeFix_RemoveStart(){return;}
	virtual	VOID	KsIeFix_RemoveProgress(){return;}
	virtual	VOID	KsIeFix_RemoveEnd(BOOL bNeedReboot, DWORD nCount ){return;}
	virtual VOID	KsIeFix_InitData(KsWebIeInitData* pArray, DWORD nSize){return;}
};

class IKSWebIEFixEng
{
public:
	virtual	VOID		Delete() = 0;
	virtual HRESULT		Init(IKSWebIEFixEngCallback* pCallback, int nBcType) = 0;
	virtual HRESULT		BeginScan(DWORD nType,BOOL bSyn = FALSE) = 0;
	virtual	HRESULT		BeginRemove(PVOID fixparam) = 0;
	virtual	HRESULT		StopScan() = 0;
	virtual	HRESULT		StopRemove() = 0;
	virtual HRESULT		PauseScan() = 0;
	virtual HRESULT		ResumeScan() = 0;

	virtual HRESULT		QueryResult(PVOID* lpResult) = 0;
	virtual DWORD		ConvertKsTypeToId(DWORD nType) = 0;

	// 通知所有的实例退出
	virtual DWORD		NotifyAllCpExit() = 0;
};


class CNewIEFixLoader
{
public:

	typedef IKSWebIEFixEng* ( *PFNCreateNewIeEng)(
		DWORD	nVer
		);

// 	CNewIEFixLoader(void)
// 	{
// 		m_hModDLL				= NULL;
// 		fnpGetNewIeEng	= NULL;
// 
// 		LoadEng(DLL_NAME_NEW_IEFIX);
// 		m_hModDLL	= ::LoadLibrary(DLL_NAME_NEW_IEFIX);
// 		if ( m_hModDLL != NULL )
// 		{
// 			fnpGetNewIeEng = (PFNCreateNewIeEng)GetProcAddress(m_hModDLL, STR_EXPORT_INTERFACE);
// 		}
//	}

	CNewIEFixLoader(CString strAppPath = L"")
	{
		m_hModDLL				= NULL;
		fnpGetNewIeEng	= NULL;

		CString strDllPath = strAppPath;
		if (TRUE == strAppPath.IsEmpty())
		{
			strDllPath = DLL_NAME_NEW_IEFIX;
		}
		else
		{
			strDllPath.Append(L"\\");
			strDllPath.Append(DLL_NAME_NEW_IEFIX);
		}

		LoadEng(strDllPath);
	}

	void LoadEng(CString strDllPath)
	{
		m_hModDLL	= ::LoadLibrary(strDllPath);
		if ( m_hModDLL != NULL )
		{
			fnpGetNewIeEng		= (PFNCreateNewIeEng)GetProcAddress(m_hModDLL, STR_EXPORT_INTERFACE);
			fnpGetNewSysFixEng	= (PFNCreateNewIeEng)GetProcAddress(m_hModDLL, STR_CREATE_SYS_FIX_INTERFACE);
		}
	}

	static CNewIEFixLoader* GetPtr(CString strAppPath = L"")
	{
		static CNewIEFixLoader x(strAppPath);
		return &x;
	}

	~CNewIEFixLoader(void)
	{
		if ( m_hModDLL != NULL )
		{
			::FreeLibrary(m_hModDLL);
			m_hModDLL = NULL;
		}
	}

	IKSWebIEFixEng*	GetKswebIEFixEng()
	{
		IKSWebIEFixEng*	ppv = NULL;
		if ( fnpGetNewIeEng != NULL )
		{
			return fnpGetNewIeEng(IESCANENG_VER);
		}
		return NULL;
	}

	IKSWebIEFixEng*	GetKsSysFixEng()
	{
		IKSWebIEFixEng*	ppv = NULL;
		if ( fnpGetNewSysFixEng != NULL )
		{
			return fnpGetNewSysFixEng(IESCANENG_VER);
		}
		return NULL;
	}

protected:
	HMODULE					m_hModDLL;
	PFNCreateNewIeEng		fnpGetNewIeEng;
	PFNCreateNewIeEng		fnpGetNewSysFixEng;
};
