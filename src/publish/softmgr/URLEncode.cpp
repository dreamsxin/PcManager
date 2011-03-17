#define _CRTDBG_MAP_ALLOC

#include "stdafx.h"

#include "URLEncode.h"
#include <atlutil.h>

static DWORD guess_url_encode_length(const wchar_t* lpURL)
{
	DWORD dwCount =0 ;
	WORD c1;
	const wchar_t* lpFileOffset = wcsrchr(lpURL, _T('/'));
	const wchar_t* lpSrc = lpURL;

	while ( (c1 = *lpSrc++) != _T('\0') )
	{
		if ( HIBYTE(c1) == 0 )
		{
			if ( (c1 <= 0x20) || (c1 == 0x7F) )
			{
				// hex encode
				dwCount += 3;
			}
			else
			{
				dwCount++;
			}
		}
		else
		{
			const wchar_t* lpNextSrc = lpSrc;
			while ( (c1 = *lpNextSrc), HIBYTE(c1) != 0 )
			{
				lpNextSrc++;
			}

			--lpSrc; // 回溯一个字符

			const int nInputLen = (lpNextSrc - lpSrc);
			int nOutputLen = 0;
			char* lpOutputBuff = NULL;

			if ( (lpFileOffset != NULL) && (lpSrc > lpFileOffset) )
			{
				// hex encode
				nOutputLen = WideCharToMultiByte(CP_ACP, 0, lpSrc, nInputLen, NULL, 0, 0, NULL);
			}
			else
			{
				nOutputLen = WideCharToMultiByte(CP_UTF8, 0, lpSrc, nInputLen, NULL, 0, 0, NULL);
			}

			dwCount += nOutputLen * 3;
			lpSrc = lpNextSrc;
		}
	}

	return dwCount;
}

CString CURLEncode::URLEncode(const wchar_t* lpURL)
{
	CString strTemp;

	const int nBuffLen = guess_url_encode_length(lpURL) + 1;

	WORD c1;
	const wchar_t* lpFileOffset = wcsrchr(lpURL, _T('/'));
	const wchar_t* lpSrc = lpURL;
	TCHAR* lpDst = strTemp.GetBuffer(nBuffLen);

	while ( (c1 = *lpSrc++) != _T('\0') )
	{
		if ( HIBYTE(c1) == 0 )
		{
			if ( (c1 <= 0x20) || (c1 == 0x7F) )
			{
				// hex encode
				*lpDst++ = _T('%');
				*lpDst++ = _T("0123456789ABCDEF")[c1 >> 4];
				*lpDst++ = _T("0123456789ABCDEF")[c1 & 0x0F];
			}
			else
			{
				*lpDst++ = (TCHAR)c1;
			}
		}
		else
		{
			const wchar_t* lpNextSrc = lpSrc;
			while ( (c1 = *lpNextSrc), HIBYTE(c1) != 0 )
			{
				lpNextSrc++;
			}

			--lpSrc; // 回溯一个字符

			const int nInputLen = (lpNextSrc - lpSrc);
			int nOutputLen = 0;
			char* lpOutputBuff = NULL;

			if ( (lpFileOffset != NULL) && (lpSrc > lpFileOffset) )
			{
				// hex encode
				nOutputLen = WideCharToMultiByte(CP_ACP, 0, lpSrc, nInputLen, NULL, 0, 0, NULL);

				lpOutputBuff = new char[nOutputLen + 1];
				WideCharToMultiByte(CP_ACP, 0, lpSrc, nInputLen, lpOutputBuff, nOutputLen, NULL, NULL);
			}
			else
			{
				nOutputLen = WideCharToMultiByte(CP_UTF8, 0, lpSrc, nInputLen, NULL, 0, 0, NULL);

				lpOutputBuff = new char[nOutputLen + 1];
				WideCharToMultiByte(CP_UTF8, 0, lpSrc, nInputLen, lpOutputBuff, nOutputLen, NULL, NULL);
			}

			for ( int i = 0; i < nOutputLen; i++ )
			{
				BYTE c1 = lpOutputBuff[i];

				*lpDst++ = _T('%');
				*lpDst++ = _T("0123456789ABCDEF")[c1 >> 4];
				*lpDst++ = _T("0123456789ABCDEF")[c1 & 0x0F];
			}

			delete[] lpOutputBuff;
			lpOutputBuff = NULL;

			lpSrc = lpNextSrc;
		}
	}

	if ( lpDst != NULL )
	{
		*lpDst = 0;
		strTemp.ReleaseBuffer();
	}
	else
	{
		strTemp.Empty();
	}

	return strTemp;
}

CString CURLEncode::URLEncode2(const wchar_t* lpURL)
{
	CString strTemp;
	TCHAR* pszURLEncode = strTemp.GetBuffer(1024);
	DWORD dwLength = 0;
	AtlEscapeUrl(lpURL, pszURLEncode, &dwLength, 1024);
	if (pszURLEncode != NULL)
	{
		strTemp.ReleaseBuffer(1024);
	}
	else
		strTemp.Empty();

	return strTemp;
}