#pragma once
#include "atlcoll.h"
#include <iefix/iescanengexportdata.h>
#include <common/utility.h>

using namespace IEScanEng;


#define MACRO_FUNC_BEGIN
#define SAFE_CALL_FUN(x,y) if (NULL != (x)){(x)->y;}
#define MACRO_FUNC_ENG

#define STR_BEGIN
#define STR_INTERFACE_CREATE ("CreateObject")
#define STR_INTERFACE_RELEASE ("ReleaseObject")
#define STR_IEFIXENG_NAME TEXT("..\\kiefixeng.dll")
#define STR_END


typedef IIEScanEng* (*PFNCreateObject)(ULONG uVer);
typedef void (*PFNReleaseObject)(IIEScanEng** pObj);


class CIEFixEng
{
public:
	CIEFixEng(void)
	{
		m_pIEFixEng = NULL;
		m_pfnCreateEng = NULL;
		m_pfnReleaseEng = NULL;
		m_hModule = NULL;
	}
	~CIEFixEng(void)
	{
		if (NULL != m_pfnReleaseEng && NULL != m_pIEFixEng)
			m_pfnReleaseEng(&m_pIEFixEng);

		m_pIEFixEng = NULL;
		if (NULL == m_hModule)
			FreeLibrary(m_hModule);


	}
	int IEFix_CreateEng(DWORD dwEngVer)
	{
		int nRet = -1;
		CString str;
		TCHAR pszModuleFileName[FILEPATH_MAX_LEN] = {0};
		GetModuleFileName(NULL, pszModuleFileName, sizeof(pszModuleFileName));
		PathAppend(pszModuleFileName, STR_IEFIXENG_NAME);
		m_hModule = LoadLibrary(pszModuleFileName);

		_OutPutDebug(pszModuleFileName);

		str.Format(TEXT("mode = %x ErrorCode = %x"), m_hModule, GetLastError());
		_OutPutDebug(str);

		if (NULL == m_hModule)
		{
			_OutPutDebug(TEXT("_LoadMode Error...."));
			return nRet;
		}
		//如果之前已经存在一个实例的话，就先把以前的数据释放掉再重新加载一份
		if (NULL == m_pfnCreateEng)
		{
			m_pfnCreateEng = (PFNCreateObject)GetProcAddress(m_hModule, STR_INTERFACE_CREATE);
		}
		else
		{
			if (NULL != m_pIEFixEng && NULL != m_pfnReleaseEng)
				m_pfnReleaseEng(&m_pIEFixEng);
		}

		str.Format(TEXT("GetInterface = %x"), m_pfnCreateEng);
		_OutPutDebug(str);

		m_pIEFixEng = m_pfnCreateEng(dwEngVer);
		if (NULL == m_pIEFixEng)
			return nRet;

		m_pfnReleaseEng = (PFNReleaseObject)GetProcAddress(m_hModule, STR_INTERFACE_RELEASE);
		if (NULL == m_pfnReleaseEng)
			return nRet;

		nRet = 0;

		return nRet;
	}
	int IEFix_InitEng()
	{
		int nRet = -1;

		if(NULL == m_pIEFixEng)
			return -1;

		return m_pIEFixEng->InitIEScanEng();//初始化引擎数据

	}

	int IEFix_InitEng2(DWORD dwType)
	{
		int nRet = -1;
		if (NULL == m_pIEFixEng)
		{
			return nRet;
		}

		nRet = m_pIEFixEng->InitIEScanEng2(dwType);
		return nRet;
	}
	GROUP_STRUCT_INFO_EXPORT* IEFix_EnumGroup(ULONG uIndex)
	{
		if (NULL == m_pIEFixEng)
			return NULL;

		return m_pIEFixEng->EnumGroup(uIndex);

	}
	int IEFix_GetGroupCount()
	{
		int nCount = 0;
		if (NULL != m_pIEFixEng)
			nCount = m_pIEFixEng->GetGroupCount();

		return nCount;

	}
	int IEFix_Scan(int nGroup, int nID, PFNIEScanEngCallBack pfnCallBack, void* lpConext)
	{
		int nRet = -1;
		if (NULL == m_pIEFixEng)
			return nRet;

		nRet = m_pIEFixEng->Scan(nGroup, nID, pfnCallBack, lpConext);

		return nRet;

	}
	int IEFix_GetSafeLevel(int nLevel, LPWSTR pszSafeLevel, DWORD cbSize)
	{
		int nRet = -1;
		if (NULL == m_pIEFixEng)
			return nRet;

		pszSafeLevel = TEXT("");
		nRet = 0;
		//nRet = m_pIEFixEng->GetSafeLevel(nLevel, pszSafeLevel, cbSize);

		return nRet;

	}
	int IEFix_Fix(int nGroup, int nID)
	{
		int nRet = -1;
		if (nGroup < 0 || NULL == m_pIEFixEng)
			return nRet;

		nRet = m_pIEFixEng->Fix(nGroup, nID);

		return nRet;

	}
	int IEFix_Fix(SCAN_RESULT_EXPORT* pScanData)
	{
		int nRet = -1;
		if (NULL == pScanData || NULL == m_pIEFixEng)
			return nRet;

		nRet = m_pIEFixEng->Fix(pScanData);

		return nRet;
	}
	int IEFix_Reset()
	{
		int nRet = 0;
		if (NULL == m_pIEFixEng)
			return nRet;

		m_pIEFixEng->Reset();

		return nRet;
	}
	int IEFix_GetUrlSafeLevel(LPCTSTR pszUrl)
	{
		int nRet = -1;
		if (NULL == m_pIEFixEng)
			return nRet ;
		nRet = m_pIEFixEng->GetUrlSafeLevel(pszUrl);

		return nRet;

	}
	GROUP_STRUCT_INFO_EXPORT* IEFix_GetAnGroup(ULONG uGroupID)
	{
		if (NULL == m_pIEFixEng)
			return NULL;

		return m_pIEFixEng->GetAnGroup(uGroupID);
	}
	//plug
	int IEFix_ScanAllPlug(BOOL bSyn = FALSE)
	{
		if (NULL == m_pIEFixEng)
			return -1;

		return m_pIEFixEng->ScanAllPlug(bSyn);

	}
	CPlugNullCallback* IEFix_SetPlugScanCallBack(CPlugNullCallback* pICallBack)
	{
		if (NULL == m_pIEFixEng)
			return NULL;

		return m_pIEFixEng->SetPlugScanCallBack(pICallBack);

	}
	int IEFix_GetItemPlug(DWORD nItemid, PLUG_SCAN_RESULT** pItemInfo)
	{
		if (NULL == m_pIEFixEng)
			return -1;

		return m_pIEFixEng->GetItemPlug(nItemid, pItemInfo);

	}
	int IEFix_FreeItemPlug(PLUG_SCAN_RESULT* itemInfo)
	{
		if (NULL == m_pIEFixEng)
			return -1;

		return m_pIEFixEng->FreeItemPlug(itemInfo);
	}
	int IEFix_RemovePlug(DWORD nItemid)
	{
		if (NULL == m_pIEFixEng)
			return -1;

		return m_pIEFixEng->RemovePlug(nItemid);

	}
	int IEFix_ScanArrayPlug(BOOL bSyn = FALSE)
	{
		if (NULL == m_pIEFixEng)
			return -1;

		return m_pIEFixEng->ScanArryPlug(bSyn);
	}
	int IEFix_StopScanPlug()
	{
		if (NULL == m_pIEFixEng)
			return -1;

		return m_pIEFixEng->StopScanPlug();
	}
	int IEFix_Stop()
	{
		if (NULL == m_pIEFixEng)
		{
			return -1;
		}
		return m_pIEFixEng->Stop();
	}

	BOOL HostsFileModify()
	{
		BOOL bRet = FALSE;
		CString strHostFile, strLogFile;
		CAppPath::Instance().GetLeidianLogPath(strLogFile, TRUE);
		strLogFile += TEXT("\\exam.log");
		TCHAR pszHostFilePath[FILEPATH_MAX_LEN] = {0};
		if (FALSE == SHGetSpecialFolderPath(NULL, pszHostFilePath, CSIDL_SYSTEM, FALSE) || 
			0 == _tcslen(pszHostFilePath) ||
			FALSE == PathAppend(pszHostFilePath, TEXT("drivers\\etc\\hosts")))
			return bRet;

		do 
		{
			if (FALSE == PathFileExists(pszHostFilePath))
				break;

			if (FALSE == PathFileExists(strLogFile))
			{
				bRet = TRUE;
				break;
			}

			int nHightValue, nLowValue, nFileSize;
			nHightValue = GetPrivateProfileInt(TEXT("time"), TEXT("hostlastwriteH"), 0, strLogFile);
			nLowValue= GetPrivateProfileInt(TEXT("time"), TEXT("hostlastwriteL"), 0, strLogFile);
			nFileSize= GetPrivateProfileInt(TEXT("value"), TEXT("hostsize"), 0, strLogFile);
			if (0 == nFileSize || 0 == nLowValue)
		 {
			 bRet = TRUE;
			 break; 
		 }

			FILETIME saveFileTime;
			saveFileTime.dwHighDateTime = reinterpret_cast<DWORD&>(nHightValue);
			saveFileTime.dwLowDateTime = reinterpret_cast<DWORD&>(nLowValue);
			HANDLE hFile = INVALID_HANDLE_VALUE;
			hFile = CreateFile(pszHostFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE == hFile)
				break;
			else
				CloseHandle(hFile);

			WIN32_FILE_ATTRIBUTE_DATA win32File;
			if (FALSE == ::GetFileAttributesEx(pszHostFilePath, GetFileExInfoStandard, &win32File))
				break;


			if (0 != CompareFileTime(&saveFileTime, &win32File.ftLastWriteTime) && nFileSize != win32File.nFileSizeLow)
				bRet = TRUE;

		} while (FALSE);

		return bRet;
	}
	void SaveHostFileInfo()
	{
		do 
		{
			TCHAR pszHostFilePath[1024] = {0};
			if (FALSE == SHGetSpecialFolderPath(NULL, pszHostFilePath, CSIDL_SYSTEM, FALSE) || 
				0 == _tcslen(pszHostFilePath) ||
				FALSE == PathAppend(pszHostFilePath, TEXT("drivers\\etc\\hosts")))
				break;

			HANDLE hFile = INVALID_HANDLE_VALUE;
			hFile = CreateFile(pszHostFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE == hFile)
				break;
			else
				CloseHandle(hFile);

			WIN32_FILE_ATTRIBUTE_DATA win32File;
			if (FALSE == ::GetFileAttributesEx(pszHostFilePath, GetFileExInfoStandard, &win32File))
				break;
			CString strLogFile;
			CAppPath::Instance().GetLeidianLogPath(strLogFile, TRUE);
			strLogFile += TEXT("\\exam.log");
			CString strValue;
			strValue.Format(TEXT("%d"), win32File.ftLastWriteTime.dwHighDateTime);
			WritePrivateProfileString(TEXT("time"), TEXT("hostlastwriteH"), strValue, strLogFile);
			strValue.Format(TEXT("%d"), win32File.ftLastWriteTime.dwLowDateTime);
			WritePrivateProfileString(TEXT("time"), TEXT("hostlastwriteL"), strValue, strLogFile);
			strValue.Format(TEXT("%d"), win32File.nFileSizeLow);
			WritePrivateProfileString(TEXT("value"), TEXT("hostsize"), strValue, strLogFile);

		} while (FALSE);
	}

private:
	IIEScanEng* m_pIEFixEng;
	PFNCreateObject m_pfnCreateEng;
	PFNReleaseObject m_pfnReleaseEng;
	void _OutPutDebug(CString str)
	{
		OutputDebugString(str);
	}
	HMODULE m_hModule;
};


