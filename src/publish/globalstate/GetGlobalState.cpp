#include "StdAfx.h"
#include "GetGlobalState.h"
#include <ShlObj.h>

#pragma comment(lib,"ShFolder.Lib")

CGetGlobalState* GetGlobalState()
{
	static CGetGlobalState	state;
	return &state;
}

CGetGlobalState::CGetGlobalState(void)
{
	InitModule( (HMODULE)&__ImageBase );
}

CGetGlobalState::~CGetGlobalState(void)
{
}

VOID CGetGlobalState::InitModule( HMODULE hMod )
{
	m_hMod = hMod;

	OSVERSIONINFO	osver = {0};
	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osver);
	m_dwMajorVer = osver.dwMajorVersion;
	m_dwMinVer	 = osver.dwMinorVersion;

	m_bIsWOW64 = GetIsWOW64();

	GetModulePath();
	GetModuleDir();
	GetProgramDir();
	GetSystem32Dir();
	GetWindowsDir();
	GetProgramDir();
	GetCommonAppDataDir();
	GetAppDataDir();
}

LPCTSTR CGetGlobalState::GetModulePath()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		GetModuleFileName(m_hMod,szPath, MAX_PATH);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetModuleDir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		GetModuleFileName(m_hMod,szPath, MAX_PATH);
		PathRemoveFileSpec(szPath);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetWindowsDir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		GetSystemWindowsDirectory(szPath, MAX_PATH);
	//	GetWindowsDirectory(szPath, MAX_PATH);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetSystem32Dir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		GetSystemDirectory(szPath, MAX_PATH);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetProgramDir()
{
	static TCHAR szDir[MAX_PATH] = {0};

	if ( szDir[0] == 0 )
	{
		DWORD dwType = REG_SZ;
		DWORD dwSize = sizeof(szDir);
		if( ERROR_SUCCESS == SHGetValue( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion"), 
			_T("ProgramFilesDir"), &dwType, szDir, &dwSize ) && dwSize > 1 )
		{
			FixPathLastSpec(szDir);
			return szDir ;
		}

		if( 0 != GetEnvironmentVariable( _T("ProgramFiles"), szDir, dwSize ) )
		{
			FixPathLastSpec(szDir);
			return szDir ;
		}	
	}
	return szDir;
}

BOOL CGetGlobalState::FixPathLastSpec( LPTSTR lpPath )
{
	if ( lpPath == NULL )
		return FALSE;

	size_t	nLen = _tcslen(lpPath);
	if ( nLen > 1 )
	{
		if ( lpPath[nLen-1] == _T('\\') )
			lpPath[nLen-1] = 0;

		return TRUE;
	}
	return FALSE;
}

LPCTSTR CGetGlobalState::GetTempDir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		GetTempPath(MAX_PATH,szPath);
		FixPathLastSpec(szPath);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetCommonAppDataDir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		SHGetSpecialFolderPath( NULL, szPath, CSIDL_COMMON_APPDATA, FALSE );
		FixPathLastSpec(szPath);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetAppDataDir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		SHGetSpecialFolderPath( NULL, szPath, CSIDL_APPDATA, FALSE );
		FixPathLastSpec(szPath);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetRundll32Path()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		LPCTSTR	lpWinPath = GetSystem32Dir();
		_sntprintf_s( szPath, MAX_PATH, _T("%s\\Rundll32.exe"), lpWinPath);
	}
	return szPath;
}


LPCTSTR CGetGlobalState::GetRegSrvPath()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		LPCTSTR	lpWinPath = GetSystem32Dir();
		_sntprintf_s( szPath, MAX_PATH, _T("%s\\regsvr32.exe"), lpWinPath);
	}
	return szPath;
}

BOOL CGetGlobalState::GetIsWOW64()
{
	HMODULE hLib;
	BOOL bNeedFree = FALSE;
	BOOL bRet = FALSE;

	hLib = GetModuleHandle(_T("shlwapi.dll"));
	if(hLib == NULL)
	{
		hLib = LoadLibrary(_T("shlwapi.dll"));
		bNeedFree = TRUE;
	}

	if(hLib)
	{
		BOOL (__stdcall* pfnIsOS)(DWORD dwVer);

		(FARPROC&)pfnIsOS = GetProcAddress(hLib, MAKEINTRESOURCEA(437) );
		if(pfnIsOS)
		{
			bRet = pfnIsOS(30);
		}

		if(bNeedFree)
			FreeLibrary(hLib);

		SetLastError(ERROR_SUCCESS);
	}
	else
	{
		SetLastError(ERROR_OLD_WIN_VERSION);
	}

	return bRet;
}

LPCTSTR CGetGlobalState::GetExplorerPath()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		LPCTSTR	lpWinPath = GetWindowsDir();
		_sntprintf_s( szPath, MAX_PATH, _T("%s\\explorer.exe"), lpWinPath);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetSysExplorerPath()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		LPCTSTR	lpWinPath = GetSystem32Dir();
		_sntprintf_s( szPath, MAX_PATH, _T("%s\\explorer.exe"), lpWinPath);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetUserStartMenuProgDir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		SHGetSpecialFolderPath( NULL, szPath, CSIDL_PROGRAMS, FALSE );
		FixPathLastSpec(szPath);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetCommonStartMenuProgDir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		SHGetSpecialFolderPath( NULL, szPath, CSIDL_COMMON_PROGRAMS, FALSE );
		FixPathLastSpec(szPath);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetUserDesktopDir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		SHGetSpecialFolderPath( NULL, szPath, CSIDL_DESKTOPDIRECTORY, FALSE );
		FixPathLastSpec(szPath);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetCommonDesktopDir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		SHGetSpecialFolderPath( NULL, szPath, CSIDL_COMMON_DESKTOPDIRECTORY, FALSE );
		FixPathLastSpec(szPath);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetQuickLanchDir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		LPCTSTR	lpAppData = GetAppDataDir();
		if (lpAppData[0]!=0)
		{
			_sntprintf_s( szPath, MAX_PATH, _T("%s\\Microsoft\\Internet Explorer\\Quick Launch"), lpAppData);
		}
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetUserStartMenuRootDir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		SHGetSpecialFolderPath( NULL, szPath, CSIDL_STARTMENU, FALSE );
		FixPathLastSpec(szPath);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetCommonStartMenuRootDir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		SHGetSpecialFolderPath( NULL, szPath, CSIDL_COMMON_STARTMENU, FALSE );
		FixPathLastSpec(szPath);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetUserStartRunDir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		SHGetSpecialFolderPath( NULL, szPath, CSIDL_STARTUP, FALSE );
		FixPathLastSpec(szPath);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetCommonStartRunDir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		SHGetSpecialFolderPath( NULL, szPath, CSIDL_COMMON_STARTUP, FALSE );
		FixPathLastSpec(szPath);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetUserFavoriteDir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		SHGetSpecialFolderPath( NULL, szPath, CSIDL_FAVORITES, FALSE );
		FixPathLastSpec(szPath);
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetCommonFavoriteDir()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		SHGetSpecialFolderPath( NULL, szPath, CSIDL_COMMON_FAVORITES, FALSE );
		FixPathLastSpec(szPath);
	}
	return szPath;
}

VOID CGetGlobalState::GetLongPath( LPCTSTR lpFile, CString& strLongFile )
{
	DWORD	nSize = ::GetLongPathName(lpFile, NULL, 0);

	if ( nSize > 0 )
	{
		TCHAR*	pBuffer = new TCHAR[nSize+1];
		::GetLongPathName(lpFile, pBuffer, nSize+1);
		strLongFile = pBuffer;
		delete pBuffer;
	}
	else
		strLongFile = lpFile;
}

LPCTSTR CGetGlobalState::GetIExplorerPath()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		LPCTSTR	lpProgDir = GetProgramDir();
		if (lpProgDir[0]!=0)
		{
			_sntprintf_s( szPath, MAX_PATH, _T("%s\\Internet Explorer\\iexplore.exe"), lpProgDir);
		}
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetUserinitPath()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		LPCTSTR	lpSysDir = GetSystem32Dir();
		if (lpSysDir[0]!=0)
		{
			_sntprintf_s( szPath, MAX_PATH, _T("%s\\Userinit.exe"), lpSysDir);
		}
	}
	return szPath;
}

LPCTSTR CGetGlobalState::GetWinlogonPath()
{
	static	TCHAR	szPath[MAX_PATH] = {0};
	if ( szPath[0] == 0 )
	{
		LPCTSTR	lpSysDir = GetSystem32Dir();
		if (lpSysDir[0]!=0)
		{
			_sntprintf_s( szPath, MAX_PATH, _T("%s\\winlogon.exe"), lpSysDir);
		}
	}
	return szPath;
}