//////////////////////////////////////////////////////////////////////////
//
//	检测是否安装毒霸,调用毒霸安装器
//		
#pragma once

#include "../kws/simpleipc/KwsIPCWarpperOld.h"

#define REG_DUBA				TEXT("SOFTWARE\\Kingsoft\\AntiVirus")
#define KEY_DUBA09				TEXT("kis2009")
#define KEY_DUBA10				TEXT("kis2010")
#define KEY_DUBA11				TEXT("kis2011")
#define NAME_DUBA09				TEXT("kav32.exe")
#define NAME_DUBA10				TEXT("kismain.exe")
#define NAME_DUBA11				TEXT("kismain.exe")
#define	DOWN_LOAD_FILENAME		TEXT("KAV_ONLINE_95_11.exe")
#define	DOWN_LOAD_FILEPARAM		TEXT("kav_online_10_144.exe -_iid 186345782")



class KCallDuba
{
public:

	BOOL IsDubaProcExist()
	{
		BOOL bResult = FALSE;
		HANDLE hMutex = OpenMutex(GENERIC_READ, FALSE, _T("{6E8BF4D4-75FE-4e6e-8F22-E2AC2E900DF3}")); // 判断毒霸是否在运行
		if ( hMutex != NULL )
		{
			CloseHandle(hMutex);
			hMutex = NULL;

			bResult = TRUE;
		}

		return bResult;
	}

	BOOL IsDubaExist()
	{
		TCHAR szDubaPath[MAX_PATH] = {0};
		ULONG uLen = MAX_PATH;			
		return _GetDubaPath(szDubaPath, uLen);
	}

	BOOL CallDuba()
	{
		TCHAR szDubaPath[MAX_PATH] = {0};
		ULONG uLen = MAX_PATH;	
		BOOL bRet = FALSE;
		if (!_GetDubaPath(szDubaPath, uLen))
		{
			return FALSE;
		}

		STARTUPINFO si          = {0};
		PROCESS_INFORMATION pi  = {0};
		bRet = ::CreateProcess(szDubaPath, NULL,
			NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);
		if (bRet)
		{	
			if (pi.hThread)
			{
				CloseHandle(pi.hThread);
				pi.hThread = NULL;
			}
			if (pi.hProcess)
			{
				CloseHandle(pi.hProcess);
				pi.hProcess = NULL;
			}
		}

		return bRet;
	}

	BOOL GetInstallerPath( ATL::CString& strPath )
	{
		TCHAR tszPath[MAX_PATH*2] = {0};
		GetModuleFileName( NULL, tszPath, MAX_PATH * 2 );
		CPath path( tszPath );
		path.RemoveFileSpec();
		path.Append( DOWN_LOAD_FILENAME );
		strPath = path.m_strPath;

		return TRUE;
	}

	BOOL CallDubaInstall()
	{
		CString strPath;
		GetInstallerPath( strPath );
		ATL::CString strComLine;
		strComLine.Format( TEXT("%s"), strPath );
		kws_old_ipc::KwsCallAsUserOrSystem( strComLine );
			
		return TRUE;
	}



private:
	BOOL _GetDubaPath(LPTSTR pszPath, ULONG uLen)
	{	
		LONG nRet = -1;		
		if (pszPath == NULL)
		{
			return FALSE;
		}

		nRet = _GetRegStringValue(HKEY_LOCAL_MACHINE, REG_DUBA, KEY_DUBA09, pszPath, uLen);
		if (nRet == 0)
		{
			::PathAppend(pszPath, NAME_DUBA09);
			if (::PathFileExists(pszPath))
			{
				return TRUE;
			}			
		}

		nRet = _GetRegStringValue(HKEY_LOCAL_MACHINE, REG_DUBA, KEY_DUBA10, pszPath, uLen);
		if (nRet == 0)
		{
			::PathAppend(pszPath, NAME_DUBA10);
			if (::PathFileExists(pszPath))
			{
				return TRUE;
			}
		}

		nRet = _GetRegStringValue(HKEY_LOCAL_MACHINE, REG_DUBA, KEY_DUBA11, pszPath, uLen);
		if (nRet == 0)
		{
			::PathAppend(pszPath, NAME_DUBA11);
			if (::PathFileExists(pszPath))
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	LONG _GetRegStringValue( HKEY hKey, LPCTSTR pszKey, LPCTSTR pszSubKey, LPTSTR pszBuffer, ULONG uLen )
	{
		if( pszKey == NULL || pszSubKey == NULL || pszBuffer == NULL)
			return -1;	
		CRegKey regk;
		LONG lRet = regk.Open( hKey, pszKey, KEY_READ );
		if( lRet != 0 )
			return lRet;
		lRet = regk.QueryStringValue( pszSubKey, pszBuffer, &uLen );
		return lRet;
	}
};
