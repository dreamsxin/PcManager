// RegistryKey.cpp : implementation file
//

#include "stdafx.h"
#include "RegistryKey.h"

/////////////////////////////////////////////////////////////////////////////
// CRegistryKey

#define INVALID_KEY_RETURN	if(m_hKey==NULL)return FALSE;

CRegistryKey::CRegistryKey(HKEY hKey,LPCTSTR lpKey)
{
	m_hRootKey = hKey;
	m_hKey	   = NULL;
	
	LONG lReturn=RegOpenKeyEx(m_hRootKey,lpKey,0L,KEY_ALL_ACCESS,&m_hKey);

	if (lReturn != ERROR_SUCCESS)
		m_hKey = NULL;
}

CRegistryKey::CRegistryKey(HKEY hKey,LPCTSTR lpKey,BOOL bCreate)
{
	m_hRootKey = hKey;
	m_hKey	   = NULL;

	LONG lReturn=RegOpenKeyEx(m_hRootKey,lpKey,0L,KEY_ALL_ACCESS,&m_hKey);
	if(lReturn!=ERROR_SUCCESS)
	{
		//有时候并非是需要读写的。有时候，没有权限，但是是希望能读到的。
		lReturn = RegOpenKeyEx(m_hRootKey,lpKey,0L,KEY_QUERY_VALUE,&m_hKey);

		//如果不存在，则应该支持能够写一个键值。
		if (lReturn != ERROR_SUCCESS)
			lReturn = RegCreateKeyEx(m_hRootKey, lpKey, 0L, NULL, REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS, NULL, &m_hKey, NULL);

		if (lReturn != ERROR_SUCCESS)
			m_hKey = NULL;
	}
}


CRegistryKey::CRegistryKey()
{
	m_hRootKey  = NULL;
	m_hKey		= NULL;
}
CRegistryKey::~CRegistryKey()
{
	Close();
}

/////////////////////////////////////////////////////////////////////////////
// CRegistryKey Functions


BOOL CRegistryKey::AttachKey( HKEY hRoot, LPCTSTR lpKey )
{
	m_hRootKey = hRoot;
	m_hKey	   = NULL;

	LONG lReturn=RegOpenKeyEx(m_hRootKey,lpKey,0L,KEY_ALL_ACCESS,&m_hKey);
	if(lReturn!=ERROR_SUCCESS)
	{
		m_hKey=NULL;
		return FALSE;
	}
	return TRUE;
}

BOOL CRegistryKey::CreateKey(LPCTSTR lpSubKey)
{
	INVALID_KEY_RETURN;

	HKEY hKey;
	DWORD dw;
	long lReturn=RegCreateKeyEx(m_hKey,lpSubKey,0L,NULL,REG_OPTION_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,&dw);
    
	if(lReturn==ERROR_SUCCESS)
	{
		m_hKey=hKey;
		return TRUE;
	}
	
	return FALSE;
	
}

void CRegistryKey::Close()
{
	if(m_hKey)
	{
		RegCloseKey(m_hKey);
		m_hKey=NULL;
	}
	
}

BOOL CRegistryKey::DeleteValue(LPCTSTR lpValueName)
{
	INVALID_KEY_RETURN;

	long lReturn=RegDeleteValue(m_hKey,lpValueName);
	
	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	return FALSE;
	
}

BOOL CRegistryKey::DeleteKey(HKEY hKey, LPCTSTR lpSubKey)
{
	INVALID_KEY_RETURN;
	
	long lReturn=RegDeleteValue(hKey,lpSubKey);
	
	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	return FALSE;
	
}

BOOL CRegistryKey::Write(LPCTSTR lpSubKey, int nVal)
{
	INVALID_KEY_RETURN;
	
	DWORD dwValue;
	dwValue=(DWORD)nVal;
	
	long lReturn=RegSetValueEx(m_hKey,lpSubKey,0L,REG_DWORD,(const BYTE *) &dwValue,sizeof(DWORD));
	
   	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
	
}

BOOL CRegistryKey::Write(LPCTSTR lpSubKey, DWORD dwVal)
{
	INVALID_KEY_RETURN;
	
	long lReturn=RegSetValueEx(m_hKey,lpSubKey,0L,REG_DWORD,(const BYTE *) &dwVal,sizeof(DWORD));
	
   	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
	
}

BOOL CRegistryKey::Write( LPCTSTR lpSubKey, CStringW strVal )
{
	INVALID_KEY_RETURN;

	int nLen = strVal.GetLength() * 2;
	const BYTE * p = (const BYTE *)(LPCWSTR)strVal;

	long lReturn=RegSetValueEx(m_hKey,lpSubKey,0L,REG_SZ, p, nLen);

	if(lReturn==ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistryKey::Read(LPCTSTR lpValueName, int* pnVal)
{
	INVALID_KEY_RETURN;
	
	DWORD dwType;
	DWORD dwSize=sizeof(DWORD);
	DWORD dwDest;
	long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)&dwDest,&dwSize);
	
	if(lReturn==ERROR_SUCCESS)
	{
		*pnVal=(int)dwDest;
		return TRUE;
	}
	return FALSE;
	
}

BOOL CRegistryKey::Read(LPCTSTR lpValueName, DWORD* pdwVal)
{
	INVALID_KEY_RETURN;
	
	DWORD dwType;
	DWORD dwSize=sizeof(DWORD);
	DWORD dwDest;
	long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)&dwDest,&dwSize);
	
	if(lReturn==ERROR_SUCCESS)
	{
		*pdwVal=dwDest;
		return TRUE;
	}
	return FALSE;
	
}

BOOL CRegistryKey::Read(LPCTSTR lpValueName, CString* lpVal)
{
	INVALID_KEY_RETURN;
	
	DWORD dwType;
	TCHAR szString[MAX_PATH] = {0};
	DWORD dwSize = sizeof(szString);
	
	long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)szString,&dwSize);
	
	if(lReturn==ERROR_SUCCESS)
	{
		*lpVal=szString;
		return TRUE;
	}
	return FALSE;
	
}

BOOL CRegistryKey::IsKeyValid()
{
	return (m_hKey==NULL);
}

BOOL CRegistryKey::IsValueExist( LPCTSTR lpValue )
{
	DWORD dwType;
	DWORD dwSize=1;
	TCHAR szString[2];

	LONG lReturn=RegQueryValueEx(m_hKey,lpValue,NULL,&dwType,(BYTE *)szString,&dwSize);

	if(lReturn==ERROR_SUCCESS || lReturn==ERROR_MORE_DATA )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CRegistryKey::GetValueByData( LPCTSTR lpData, CString* lpVal )
{
	INVALID_KEY_RETURN;

	if ( lpData == NULL )
		return FALSE;

	LONG	nRet	= ERROR_SUCCESS;
	DWORD	nIndex	= 0;

	do 
	{
		BYTE*	pValueBuffer = NULL;
		BYTE*	pDataBuffer  = NULL; 
		DWORD	dwValueSize  = 0;
		DWORD   dwDataSize	 = 0;
		DWORD	dwType		 = 0;

		nRet = RegEnumValue( m_hKey, nIndex, NULL, &dwValueSize, NULL, &dwType, NULL, &dwDataSize);
		if ( nRet == ERROR_NO_MORE_ITEMS )
			break;

		if ( nRet == ERROR_MORE_DATA )
		{
			pValueBuffer = new BYTE[dwValueSize+1];
			memset(pValueBuffer, 0, dwValueSize+1);
			pDataBuffer  = new BYTE[dwDataSize+1];
			memset(pDataBuffer, 0, dwDataSize+1);

			nRet = RegEnumValue(m_hKey,nIndex, (LPTSTR)pValueBuffer, &dwValueSize, NULL, &dwType, pDataBuffer, &dwDataSize);

			if ( nRet == ERROR_SUCCESS )
			{
				if ( _tcsicmp( (LPCTSTR)pDataBuffer, lpData) == 0 )
				{
					*lpVal = pValueBuffer;
					delete pDataBuffer;
					delete pValueBuffer;
					return TRUE;
				}
			}
			delete pDataBuffer;
			delete pValueBuffer;
		}
		nIndex++;
	} while(TRUE);

	return FALSE;
}

BOOL CRegistryKey::IsRegExistKey( HKEY hRoot, LPCTSTR lpKey )
{
	if ( lpKey == NULL )
		return FALSE;

	if ( _tcslen(lpKey) == NULL )
		return FALSE;

	HKEY	hReg = NULL;
	if ( RegOpenKey(hRoot, lpKey, &hReg) == ERROR_SUCCESS )
	{
		RegCloseKey(hReg);
		return TRUE;
	}
	return FALSE;
}

BOOL CRegistryKey::IsKeyExistValue( HKEY hRoot, LPCTSTR lpKey, LPCTSTR lpValue )
{
	if ( lpKey == NULL || lpValue == NULL )
		return FALSE;

	if ( _tcslen(lpKey)==0 || _tcslen(lpValue) == 0 )
		return FALSE;

	DWORD	dwSize	= 0;
	DWORD	dwType	= 0;
	LONG	nRet	= SHGetValue(hRoot, lpKey, lpValue, &dwType, NULL, &dwSize);

	if ( nRet == ERROR_SUCCESS || nRet == ERROR_MORE_DATA )
		return TRUE;

	return FALSE;
}
