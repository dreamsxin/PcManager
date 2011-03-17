
#pragma once


class CHKeyRootConvert
{
public:

	CHKeyRootConvert(LPCTSTR lpValue)
	{
		if ( _tcscmp(lpValue, _T("HKLM")) == 0 )
			m_hRoot = HKEY_LOCAL_MACHINE;
		else if ( _tcscmp(lpValue, _T("HKCU")) == 0 )
			m_hRoot = HKEY_CURRENT_USER;
		else if ( _tcscmp(lpValue, _T("HKCR")) == 0 )
			m_hRoot = HKEY_CLASSES_ROOT;
		else if ( _tcscmp(lpValue, _T("HKUS")) == 0 )
			m_hRoot = HKEY_USERS;
		else if ( _tcscmp(lpValue, _T("HKCG")) == 0 )
			m_hRoot = HKEY_CURRENT_CONFIG;
		else 
			m_hRoot = NULL;
	}

	CHKeyRootConvert(HKEY hRoot)
	{
		if ( hRoot == HKEY_LOCAL_MACHINE )
			m_lpValue = _T("HKLM");
		else if ( hRoot == HKEY_CURRENT_USER )
			m_lpValue = _T("HKCU");
		else if ( hRoot == HKEY_CLASSES_ROOT )
			m_lpValue = _T("HKCR");		
		else if ( hRoot == HKEY_USERS )
			m_lpValue = _T("HKUS");		
		else if ( hRoot == HKEY_CURRENT_CONFIG )
			m_lpValue = _T("HKCG");	
		else
			m_lpValue = _T("NULL");
	}

public:
	operator LPCTSTR()
	{
		return m_lpValue;
	}

	LPCTSTR GetStr()
	{
		return m_lpValue;
	}

	HKEY GetHKey()
	{
		return m_hRoot;
	}	

protected:
	LPCTSTR	m_lpValue;
	HKEY	m_hRoot;
};


class CRegStringConvert
{
public:
	CRegStringConvert(HKEY hRoot, LPCTSTR lpKey)
	{
		m_strRes.Format( _T("%s\\%s"), CHKeyRootConvert(hRoot).GetStr(), lpKey);
	}

	CRegStringConvert(HKEY hRoot, LPCTSTR lpKey, LPCTSTR lpValue)
	{
		m_strRes.Format( _T("%s\\%s - [%s]"), CHKeyRootConvert(hRoot).GetStr(), lpKey, lpValue);
	}

	CRegStringConvert(HKEY hRoot, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData)
	{
		m_strRes.Format( _T("%s\\%s - [%s] : (%s)"), CHKeyRootConvert(hRoot).GetStr(), lpKey, lpValue,lpData);
	}

	CString& GetStr()
	{
		return m_strRes;
	}
protected:
	CString	m_strRes;
};