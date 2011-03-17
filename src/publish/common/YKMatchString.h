

#pragma once

class CYKMatchString
{
public:
	CYKMatchString(){}
	~CYKMatchString(){}


	BOOL MatchLikeUrl(LPCTSTR lpLikeUrl)
	{
		static LPCTSTR lpUrlMark[] = {
			_T("*http"),
			_T("*%h%t"),
			_T("*.c"),
			_T("*%2E"),
			_T("*%2F"),
			_T("*/?"),
			_T("*.net"),
			_T("*.me"),
			_T("*.org"),
			_T("*.ru"),
		};

		return MatchString(lpUrlMark, sizeof(lpUrlMark)/sizeof(lpUrlMark[1]), lpLikeUrl);
	}

	BOOL MatchString(LPCTSTR* lpArray, int iSize, LPCTSTR lpCmp)
	{
		if ( _tcslen(lpCmp) == 0 )
			return FALSE;

		for (int i=0; i<iSize; i++)
		{
			if ( IsRegularString(lpCmp,lpArray[i]) )
				return TRUE;
		}
		return FALSE;
	}

	BOOL MatchString(CSimpleArray<CString>& strArray, LPCTSTR lpCMP)
	{
		if ( _tcslen(lpCMP) == 0 )
			return FALSE;

		for (int i=0; i<strArray.GetSize(); i++)
		{
			if ( IsRegularString(lpCMP,strArray[i]) )
				return TRUE;
		}
		return FALSE;
	}

protected:
	BOOL IsRegularString( LPCTSTR lpstr, LPCTSTR lpRegular )
	{
		BOOL	bMatch = FALSE;
		LPCTSTR	lpCmp  = lpRegular;

		if (_tcslen(lpCmp) > 1 && lpCmp[0] == _T('*') )
		{
			lpCmp++;
			CString	strLowWhite = lpCmp;
			CString	strLowDll	= lpstr;
			strLowDll.MakeLower();
			strLowWhite.MakeLower();
			if ( _tcsstr(strLowDll, strLowWhite) != NULL )
			{
				bMatch = TRUE;
			}
		}
		else
		{
			if(_tcsicmp(lpstr,lpRegular)==0)
			{
				bMatch = TRUE;
			}
		}
		return bMatch;
	}
};