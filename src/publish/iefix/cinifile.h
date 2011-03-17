#pragma once


namespace IniFileOperate
{
	class CIniFile
	{
	public:
		CIniFile(LPCTSTR pszFileFullPath);
		~CIniFile(void);
		int GetIntValue(LPCTSTR pszSec, LPCTSTR pszItem, int& nValue);
		int GetStrValue(LPCTSTR pszSec, LPCTSTR pszItem, LPTSTR pszValue, DWORD cbSize);
		int SetIntValue(LPCTSTR pszSec, LPCTSTR pszItem, int nValue);
		int SetStrValue(LPCTSTR pszSec, LPCTSTR pszItem, LPCTSTR pszValue);
		int SetSection(LPCTSTR pszSec, LPCTSTR pszItem);
	private:
		CString m_strIniFile;
		BOOL m_bFileExist;
	};
}


