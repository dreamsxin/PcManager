
#pragma once

#include <winreg.h>
#include <atlbase.h>

class CRegKeyHepler : public CRegKey
{
// Construction
public:
	CRegKeyHepler(){};
	virtual ~CRegKeyHepler(){};

public:
	LONG EnumKey( DWORD iIndex, CString & strSubKey )
	{
		TCHAR	szBuffer[MAX_PATH] = {0};
		DWORD	nLength = MAX_PATH;
		LONG nRet = CRegKey::EnumKey(iIndex,szBuffer,&nLength,NULL);

		if ( nRet == ERROR_SUCCESS )
		{
			strSubKey = szBuffer;
		}
		else if ( nRet == ERROR_MORE_DATA && nLength > 0 )
		{
			nRet = CRegKey::EnumKey(iIndex, strSubKey.GetBuffer(nLength),&nLength,NULL);
			strSubKey.ReleaseBuffer();
		}
		return nRet;
	}

	LONG QueryDefaultStringVal(CString & strData)
	{
		return QueryStringValue(_T(""),strData);
	}

	LONG QueryStringValue(LPCTSTR pszValueName, CString & strData)
	{
		TCHAR	szBuffer[MAX_PATH] = {0};
		DWORD	nLength = MAX_PATH;
		LONG	nRet = CRegKey::QueryStringValue( pszValueName, szBuffer, &nLength);

		if ( nRet == ERROR_SUCCESS )
		{
			strData = szBuffer;
		}
		else if ( nRet == ERROR_MORE_DATA )
		{
			int i = 1;
			while (nRet==ERROR_MORE_DATA)
			{
				nLength = MAX_PATH*i++;
				nRet = CRegKey::QueryStringValue( pszValueName, strData.GetBuffer((1+nLength)*sizeof(TCHAR)), &nLength);
				strData.ReleaseBuffer();
			}
		}
		return nRet;
	}
};

