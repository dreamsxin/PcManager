#pragma once

#define KNESCAN_DLL_NAME                _T("knescan.dll")
#define KNESCAN_PFN_NewKnEScanObject    "NewKnEScanObject"
#define KNESCAN_PFN_FreeKnEScanObject   "FreeKnEScanObject"

enum RiskLevel
{
	eRiskLevelUnknown = 0,
	eRiskLevelLow = 1,
	eRiskLevelMid = 2,
	eRiskLevelHig = 3
};

enum RiskType
{
	eRiskTypeUnknown = 0,
	eRiskTypeTrojan = 1,
	eRiskTypeVirus = 2
};

enum {
	eCleanMethod_DisableStartup = 1,
	eCleanMethod_RemoveFile = 2
};

struct KnEScanResult
{
	KnEScanResult()
	{
		dwSize = sizeof (KnEScanResult);
		dwRiskLevel = 0;
		dwCleanMethod = 0;
		szVirusName[0] = 0;
	}

	DWORD	dwSize;
	DWORD	dwRiskLevel;
	DWORD	dwCleanMethod;
	TCHAR	szVirusName[MAX_PATH];

	BOOL	IsRisk()
	{
		return HIWORD(dwRiskLevel) != eRiskTypeUnknown;
	}
};

class IKnEScan
{
public:
	virtual HRESULT WINAPI Scan(DWORD dwSource, LPCTSTR lpFilePath, KnEScanResult& Result) = 0;
};

typedef IKnEScan* (WINAPI* PFN_NewKnEScan)();
typedef void (WINAPI* PFN_FreeKnEScan)(IKnEScan** lpObject);

class CKnEScanLoader
{
public:
	CKnEScanLoader()
	{
		m_hDll = NULL;
		m_pfnFreeKnEScan = NULL;
		m_pfnNewKnEScan = NULL;

		Init();
	}

	virtual ~CKnEScanLoader()
	{
		Uninit();
	}

	IKnEScan* NewKnEScan()
	{
		if ( m_pfnNewKnEScan != NULL )
			return m_pfnNewKnEScan();

		return NULL;
	}

	void FreeKnEScan(IKnEScan** lpObject)
	{
		if ( m_pfnFreeKnEScan != NULL )
			m_pfnFreeKnEScan(lpObject);
	}

protected:
	void	Init()
	{
		m_hDll = LoadLibrary(KNESCAN_DLL_NAME);
		if ( m_hDll != NULL )
		{
			m_pfnNewKnEScan = (PFN_NewKnEScan)GetProcAddress(m_hDll, KNESCAN_PFN_NewKnEScanObject);
			m_pfnFreeKnEScan = (PFN_FreeKnEScan)GetProcAddress(m_hDll, KNESCAN_PFN_FreeKnEScanObject);
		}
	}

	void	Uninit()
	{
		m_pfnFreeKnEScan = NULL;
		m_pfnNewKnEScan = NULL;

		FreeLibrary(m_hDll);
		m_hDll = NULL;
	}

protected:
	HMODULE m_hDll;
	PFN_NewKnEScan m_pfnNewKnEScan;
	PFN_FreeKnEScan m_pfnFreeKnEScan;
};