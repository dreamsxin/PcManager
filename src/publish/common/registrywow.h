//Registry for wow compatible 
#pragma once
#include <shlobj.h>

#define REGISTRY_NORMAL		1
#define REGISTRY_WOW		2 

template<int T>
class  CRegistryWow_Open
{

};

template<>
class CRegistryWow_Open<REGISTRY_NORMAL>
{
public:
	LSTATUS OpenKeyEx( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult )
	{
		return RegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, phkResult);
	}
	
	BOOL GetSpecialFolderPath( HWND hwnd, LPTSTR pszPath, int csidl, BOOL fCreate )
	{
		return SHGetSpecialFolderPath( hwnd, pszPath, csidl, fCreate );
	}

	BOOL GetProgramfilePath(LPTSTR pszPath, DWORD nSize)
	{
		return ExpandEnvironmentStrings(_T("%ProgramFiles%"), pszPath, nSize)!=0;
	}
};

template<>
class CRegistryWow_Open<REGISTRY_WOW>
{
public:

	LSTATUS OpenKeyEx( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult )
	{
		static DWORD dwwow = 1;
		if(dwwow==1)
		{
			dwwow = Is64() ? KEY_WOW64_64KEY : 0;  
		}
		return RegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired|dwwow, phkResult);
	}
	
	BOOL GetSpecialFolderPath( HWND hwnd, LPTSTR pszPath, int csidl, BOOL fCreate )
	{
		static DWORD dwwow = 1;
		if(dwwow==1)
		{
			dwwow = Is64() ? KEY_WOW64_64KEY : 0;  
		}
		
		BOOL bRet = FALSE;
		if(dwwow==KEY_WOW64_64KEY)
		{
			PVOID OldValue = NULL;
			Wow64DisableWow64FsRedirection(&OldValue);

			bRet = SHGetSpecialFolderPath( hwnd, pszPath, csidl, fCreate );

			Wow64RevertWow64FsRedirection(OldValue);
		}
		else
			bRet = SHGetSpecialFolderPath( hwnd, pszPath, csidl, fCreate );
		return bRet;
	}

	BOOL GetProgramfilePath(LPTSTR pszPath, DWORD nSize)
	{
		static DWORD dwwow = 1;
		if(dwwow==1)
		{
			dwwow = Is64() ? KEY_WOW64_64KEY : 0;  
		}
		if(dwwow==KEY_WOW64_64KEY)
		{
			return ExpandEnvironmentStrings(_T("%ProgramW6432%"), pszPath, nSize)!=0;
		}
		else
			return ExpandEnvironmentStrings(_T("%ProgramFiles%"), pszPath, nSize)!=0;
	}

protected:
	BOOL Is64()
	{
		BOOL bRet = FALSE;
		static HMODULE hKernel32 = NULL;
		if( !hKernel32 )
			hKernel32 = ::LoadLibrary(TEXT("Kernel32.DLL"));
		if( !hKernel32 )
			return FALSE;

		typedef BOOL (WINAPI *FunctionIsWow64Process)(HANDLE hProcess, PBOOL Wow64Process);
		FunctionIsWow64Process pfnIsWow64Process = NULL;
		pfnIsWow64Process = (FunctionIsWow64Process)GetProcAddress(hKernel32, "IsWow64Process");

		if (NULL == pfnIsWow64Process)
			return FALSE;

		HANDLE hCurrentProcess = GetCurrentProcess();
		pfnIsWow64Process(hCurrentProcess, &bRet);
		return bRet;
	}
};

template<int T>
class CRegistryWow : public CRegistryWow_Open<T>
{
public:	
	LSTATUS GetValue( HKEY hkey, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD *pdwType, void *pvData, DWORD *pcbData)
	{
		HKEY hSubKey;
		LSTATUS nStatus = OpenKeyEx( hkey, pszSubKey, 0, KEY_READ, &hSubKey );
		if(nStatus==ERROR_SUCCESS)
		{
			nStatus = SHGetValue(hSubKey, _T(""), pszValue, pdwType, pvData, pcbData);
			RegCloseKey(hSubKey);
		}
		return nStatus;
	}

	LSTATUS SetValue(HKEY hkey, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD dwType, LPCVOID pvData, DWORD cbData)
	{
		HKEY hSubKey;
		LSTATUS nStatus = OpenKeyEx( hkey, pszSubKey, 0, KEY_WRITE, &hSubKey );
		if(nStatus==ERROR_SUCCESS)
		{
			nStatus = SHSetValue(hSubKey, _T(""), pszValue, dwType, pvData, cbData);
			RegCloseKey(hSubKey);
		}
		return nStatus;
	}

	LSTATUS DeleteValue(HKEY hkey, LPCTSTR pszSubKey, LPCTSTR pszValue)
	{
		HKEY hSubKey;
		LSTATUS nStatus = OpenKeyEx( hkey, pszSubKey, 0, KEY_WRITE, &hSubKey );
		if(nStatus==ERROR_SUCCESS)
		{
			nStatus = SHDeleteValue(hSubKey, _T(""), pszValue);
			RegCloseKey(hSubKey);
		}
		return nStatus;
	}

	BOOL ExistsKey( HKEY hRoot, LPCTSTR lpSubKey )
	{
		BOOL bRet = FALSE;
		HKEY hReg;
		LONG nRet;

		nRet = OpenKeyEx( hRoot, lpSubKey, 0, KEY_READ, &hReg );
		if( nRet == ERROR_SUCCESS )
		{
			RegCloseKey( hReg );
			bRet = TRUE;
		}
		return bRet;
	}

	BOOL ExistsValue( HKEY hRoot, LPCTSTR lpSubKey, LPCTSTR lpValueName )
	{
		DWORD dwType;
		DWORD dwSize;
		return GetValue(hRoot, lpSubKey, lpValueName, &dwType, NULL, &dwSize)==ERROR_SUCCESS;
	}

	BOOL ExistsSubKey(HKEY hKey, LPCTSTR lpszSubKey)
	{
		BOOL hasKey = FALSE;

		HKEY hSubKey;
		OpenKeyEx(hKey, lpszSubKey, 0, KEY_READ, &hSubKey);

		CString str;
		DWORD dwLen = MAX_PATH;
		if( ERROR_SUCCESS==SHEnumKeyEx(hSubKey, 0, str.GetBuffer(MAX_PATH), &dwLen) )
		{
			hasKey = TRUE;
		}
		str.ReleaseBuffer();
		RegCloseKey(hSubKey);
		return hasKey;
	}

	BOOL ExistsSubValue(HKEY hKey, LPCTSTR lpszSubKey)
	{
		BOOL hasKey = FALSE;

		HKEY hSubKey;
		OpenKeyEx(hKey, lpszSubKey, 0, KEY_READ, &hSubKey);

		CString str;
		DWORD dwLen = MAX_PATH, dwLenValue = 0;
		DWORD dwType = 0;

		if( ERROR_SUCCESS==SHEnumValue(hSubKey, 0, str.GetBuffer(MAX_PATH), &dwLen, &dwType, NULL, &dwLenValue) )
		{
			hasKey = TRUE;
		}
		str.ReleaseBuffer();
		RegCloseKey(hSubKey);
		return hasKey;
	}
	
	BOOL ReadString(HKEY hkey, LPCTSTR pszSubKey, LPCTSTR pszValue, CString &str)
	{
		DWORD dwBuffer = MAX_PATH;
		DWORD dwType = REG_SZ;
		BOOL bRet = ERROR_SUCCESS==GetValue( hkey, pszSubKey, pszValue, &dwType, str.GetBuffer(MAX_PATH), &dwBuffer );
		str.ReleaseBuffer();
		return bRet;
	}

	BOOL ReadDWord(HKEY hkey, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD &dw)
	{
		DWORD dwSize = sizeof(DWORD);
		DWORD dwType = REG_DWORD;
		return ERROR_SUCCESS == GetValue( hkey, pszSubKey, pszValue, &dwType, &dw, &dwSize );
	}

	/* ===================================================
	*  CRegEntry::SetMulti(LPCTSTR lpszValue, size_t nLen, bool bInternal)
	*
	*	Stores an array of null-terminated string, terminated by two null characters.
	*	For Example: First String\0Second\Third\0\0
	*
	*  Important Params:
	*
	*		LPCTSTR lpszValue:	The string consisting of the null-terminated string array
	*		size_t  nLen:		The number of characters in the string, including null characters
	*
	*	Note: For inserting individual null-terminated strings into the array, 
	*	use MultiAdd or MultiSetAt.
	*/
	BOOL ReadMString( HKEY hkey, LPCTSTR pszSubKey, LPCTSTR pszValue, CSimpleArray<CString> &ms )
	{
		DWORD dwBuffer = _MAX_REG_VALUE;
		DWORD dwType = REG_MULTI_SZ;

		TCHAR *pbuf = new TCHAR[_MAX_REG_VALUE];

		if( pbuf && ERROR_SUCCESS==GetValue(hkey, pszSubKey, pszValue, &dwType, pbuf, &dwBuffer) )
		{
			size_t nCur = 0, nPrev = 0, nShortLen = dwBuffer/sizeof(TCHAR);

			if( nShortLen>2 )
			{
				if (*(pbuf + nShortLen-1) == '\0')
					nShortLen--;	

				while( (nCur = (int)(_tcschr(pbuf+nPrev, '\0')-pbuf)) < nShortLen ) 
				{
					ms.Add( pbuf+nPrev );
					nPrev = nCur+1;
				}
			}
			return TRUE;
		}
		return FALSE;	
	}


	BOOL WriteString( HKEY hkey, LPCTSTR pszSubKey, LPCTSTR pszValue, LPCTSTR strValue )
	{
		size_t	nValueLen = (_tcslen(strValue) + 1) * sizeof(TCHAR);
		return ERROR_SUCCESS==SetValue(hkey, pszSubKey, pszValue, REG_SZ, (LPBYTE)strValue, nValueLen);
	}

	BOOL WriteExString( HKEY hkey, LPCTSTR pszSubKey, LPCTSTR pszValue, LPCTSTR strValue )
	{
		size_t	nValueLen = (_tcslen(strValue) + 1) * sizeof(TCHAR);
		return ERROR_SUCCESS==SetValue(hkey, pszSubKey, pszValue, REG_EXPAND_SZ, (LPBYTE)strValue, nValueLen);
	}

	BOOL WriteDWord( HKEY hkey, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD dw )
	{
		return ERROR_SUCCESS==SetValue(hkey, pszSubKey, pszValue, REG_DWORD, (LPBYTE)&dw, sizeof(dw));
	}

	BOOL WriteMString( HKEY hkey, LPCTSTR pszSubKey, LPCTSTR pszValue, const CSimpleArray<CString> &ms )
	{
		INT nLen = 0;
		for(int i=0; i<ms.GetSize(); ++i)
		{
			nLen += ms[i].GetLength() + 1;
		}
		nLen += (nLen==0 ? 2 : 1); 

		TCHAR *pbuf = new TCHAR[nLen];
		if( !pbuf )
			return FALSE;

		ZeroMemory(pbuf, sizeof(TCHAR)*nLen );

		TCHAR *p = pbuf;
		for(int i=0; i<ms.GetSize(); ++i)
		{
			const CString &str = ms[i];
			_tcscpy(p, str);

			p += str.GetLength() + 1;
		}

		BOOL bRet = ERROR_SUCCESS==SetValue(hkey, pszSubKey, pszValue, REG_MULTI_SZ, pbuf, nLen * sizeof(TCHAR));
		delete pbuf;

		return bRet;
	}
};
