// yykingking@126.com

#pragma once

#include <atlconv.h>

class CCmdLineParser2 
{
public:
	CCmdLineParser2(LPCTSTR lpCmdLine )
	{
		USES_CONVERSION;

		int		iCount = 0;

#ifdef _UNICODE
		LPWSTR* lpArray = CommandLineToArgvW(lpCmdLine,&iCount);
#else
		LPWSTR* lpArray = CommandLineToArgvW( A2W(lpCmdLine) ,&iCount);
#endif

		if (lpArray!=NULL&& iCount>0)
		{
#ifdef _UNICODE
			m_strAppPath = lpArray[0];
#else
			m_strAppPath = W2A(lpArray[0]);
#endif
			for ( int i=1; i < iCount; i++)
			{
#ifdef _UNICODE
				m_listArgv.Add(CString(lpArray[i]));
#else
				m_listArgv.Add(CString(W2A(lpArray[i])));
#endif
				
			}
			::GlobalFree(lpArray);
		}
	}

	virtual ~CCmdLineParser2()
	{

	}

	CString& GetAppPath()
	{
		return m_strAppPath;
	}

	LPCTSTR GetAppPathStr()
	{
		return m_strAppPath;
	}

	int GetArgvCount()
	{
		return m_listArgv.GetSize();
	}

	LPCTSTR GetArgv(int i)
	{
		return m_listArgv[i];
	}

protected:
	CSimpleArray<CString>	m_listArgv;
	CString					m_strAppPath;
};
