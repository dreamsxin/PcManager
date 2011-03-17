
#pragma once


inline BOOL IsGUID(LPCTSTR lpStr)
{
	const size_t nLen = _tcslen(lpStr);
	return (nLen == 38) && (lpStr[0] == _T('{')) && (lpStr[37] == _T('}'));
}

inline BOOL IsBeginGUID(LPCTSTR lpStr)
{
	const size_t nLen = _tcslen(lpStr);
	return (nLen >= 38) && (lpStr[0] == _T('{')) && (lpStr[37] == _T('}'));
}

inline CString GetGUID(CString & strGUID)
{
	return strGUID.Left(38);
}