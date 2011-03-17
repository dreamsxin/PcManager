#pragma once

class CInt2Str
{
public:
	CInt2Str(int i)
	{
		m_strInt.Format(_T("%d"),i);
	}
	LPCTSTR GetStr()
	{
		return m_strInt;
	}
protected:
	CString m_strInt;
};

class CStr2Int
{
public:
	CStr2Int(LPCTSTR lpstr)
	{
		m_iInt = _ttoi(lpstr);
	}
	int GetInt()
	{
		return m_iInt;
	}

	DWORD GetDWORD()
	{
		return (DWORD)m_iInt;
	}

protected:
	int m_iInt;
};

class CFileSizeConvert
{
public:
	CFileSizeConvert(DWORD nSize)
	{
		if (nSize < 1024)
		{
			m_strConvert.Format(_T("%dB"), nSize);
		}
		else if (nSize < 1024*1024 )
		{
			float f= ((float)nSize)/1024;
			m_strConvert.Format(_T("%.2fKB"), f);
		}
		else if (nSize < 1024*1024*1024 )
		{
			float f= ((float)nSize)/(1024*1024);
			m_strConvert.Format(_T("%.2fMB"), f);
		}
		else
		{
			float f= ((float)nSize)/(1024*1024*1024);
			m_strConvert.Format(_T("%.2fGB"), f);
		}
	}
	~CFileSizeConvert(){}

	LPCTSTR GetStr()
	{
		return m_strConvert;
	}

protected:
	CString m_strConvert;
};
