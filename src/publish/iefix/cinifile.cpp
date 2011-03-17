#include "StdAfx.h"
#include "cinifile.h"
#include "strsafe.h"

using namespace IniFileOperate;

CIniFile::CIniFile(LPCTSTR pszFileFullPath)
{
	m_strIniFile.Empty();
	m_strIniFile = pszFileFullPath;
	m_bFileExist = FALSE;
	m_bFileExist = PathFileExists(pszFileFullPath);
}

CIniFile::~CIniFile(void)
{
}

int CIniFile::GetIntValue(LPCTSTR pszSec, LPCTSTR pszItem, int& nValue)
{
// 	nValue = -1;
// 	if (FALSE == m_bFileExist)
// 		nValue = 1;
// 	else
	nValue = -1;
	nValue = GetPrivateProfileInt(pszSec, pszItem, nValue, m_strIniFile);

	if (nValue < 0)
		return nValue;

	return 0;
}
int CIniFile::GetStrValue(LPCTSTR pszSec, LPCTSTR pszItem, LPTSTR pszValue, DWORD cbSize)
{
	if (FALSE == m_bFileExist)
		return -1;

	if (FALSE == GetPrivateProfileString(pszSec, pszItem, NULL, pszValue, cbSize, m_strIniFile))
		return -1;

	return 0;
}
int CIniFile::SetIntValue(LPCTSTR pszSec, LPCTSTR pszItem, int nValue)
{
	TCHAR pszValue[20] = {0};
	StringCbPrintf(pszValue, sizeof(pszValue), TEXT("%d"), nValue);
	return SetStrValue(pszSec, pszItem, pszValue);
	return 0;
}
int CIniFile::SetStrValue(LPCTSTR pszSec, LPCTSTR pszItem, LPCTSTR pszValue)
{
	if (FALSE == WritePrivateProfileString(pszSec, pszItem, pszValue, m_strIniFile))
		return -1;

	return 0;
}

int CIniFile::SetSection(LPCTSTR pszSec, LPCTSTR pszItem)
{
	if (FALSE == WritePrivateProfileSection(pszSec, pszItem, m_strIniFile))
		return -1;

	return 0;
}

