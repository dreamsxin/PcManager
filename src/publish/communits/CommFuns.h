/*
dummyz@126.com
*/
#ifndef _COMMFUNS_INC_
#define _COMMFUNS_INC_

#include <tchar.h>
#include <atlstr.h>
#include "dbgprintf.h"
#include "communits/CommFuns.h"

#define ITEMS_COUNT(x) (sizeof (x) / sizeof (x[0]))
#define RETURN_ADDR(first_param) *((PVOID*)&first_param - 1)
#define ONE_MB_BYTES			(1024 * 1024)

inline BOOL IsValidPtrRange(LPCVOID p) // 只能判断指针指向区域是否有效
{
	return (p == NULL) || (((ULONG_PTR)p > ONE_MB_BYTES) && ((ULONG_PTR)p < 0x7FFF0000));
}

#define UNFLY_DEBUG_PRINT_LOG 0 // 0,关闭调试日志; 1,开启调试日志
#define UNFLY_LOG_TO_FILE     0 // 0,输出调试信息; 1,打印到文件

#if UNFLY_DEBUG_PRINT_LOG
void UnflyLogA( char *format, ... );
#else
#define UnflyLogA(...) 
#endif

inline BOOL MyIsEmptyString(LPCTSTR lpStr)
{
	return (lpStr == NULL) || (*lpStr == 0);
}

inline BOOL MyIsClsidString(LPCTSTR lpStr)
{
	// ex: {FB3DAD64-A832-46a8-BBD4-4C0C431E09C8}
	const size_t nLen = _tcslen(lpStr);
	return (nLen >= 38) && 
		(lpStr[0] == _T('{')) && (lpStr[37] == _T('}')) &&
		(lpStr[9] == _T('-')) && (lpStr[14] == _T('-')) && (lpStr[19] == _T('-')) &&
		(lpStr[24] == _T('-'));
}

inline LPCTSTR MySkipWhite(LPCTSTR lpStr1)
{
	TCHAR c;
	while ( (c = *lpStr1) != 0 )
	{
		if ( c != _T(' ') && c != _T('\t') )
		{
			break;
		}

		lpStr1++;
	}

	return lpStr1;
}

inline LPCTSTR	MySkipNoWhite(LPCTSTR lpStr1)
{
	TCHAR c;
	while ( (c = *lpStr1) != 0 )
	{
		if ( c == _T(' ') || c == _T('\t') )
		{
			break;
		}

		lpStr1++;
	}

	return lpStr1;
}

inline BOOL MyCheckStringRange(LPCTSTR lpStr, LPVOID lpDataBegin, LPVOID lpDataEnd)
{
	if ( (PVOID)lpStr < lpDataBegin )
	{
		return FALSE;
	}

	while ( *lpStr != 0 )
	{
		if ( (PVOID)lpStr == lpDataEnd )
		{
			return FALSE;
		}

		lpStr++;
	}

	return TRUE;
}

inline void MySafeCopyString(LPTSTR lpDst, LPCTSTR lpSrc, DWORD dwLen)
{
	if ( lpDst != NULL && lpSrc != NULL )
	{
		_tcsncpy(lpDst, lpSrc, dwLen);
		lpDst[dwLen - 1] = 0;
	}
}

LPCTSTR		MyStrStrI(LPCTSTR lpStr1, LPCTSTR lpStr2);

HKEY		MyOpenRegKey(HKEY hKey, LPCTSTR lpSubKey, BOOL bReadOnly);
void		MyCloseRegKey(HKEY hKey);
BOOL		MyReadRegValue(HKEY hKey, LPCTSTR lpValueName, LPTSTR lpValue, DWORD dwLength);
BOOL		MyReadRegValue(HKEY hKey, LPCTSTR lpValueName, LPDWORD lpdwValue);
BOOL		MyWriteRegValue(HKEY hKey, LPCTSTR lpValueName, LPCTSTR lpValue);
BOOL		MyWriteRegValue(HKEY hKey, LPCTSTR lpValueName, DWORD dwValue);


LPCTSTR		MyAppendFileName(LPTSTR lpPath, LPCTSTR lpFileName);

inline LPCTSTR MyGetFileName(LPCTSTR lpPath)
{
	TCHAR c;
	LPCTSTR lpFileName = lpPath;
	LPCTSTR lpStr = lpPath;

	while ( (c = *lpStr++) != 0 )
	{
		if ( c == _T('\\') || c == _T('/') )
		{
			lpFileName = lpStr;
		}
	}

	return lpFileName;
}

inline LPCTSTR MyGetFileDir(LPCTSTR lpPath, LPTSTR lpDir)
{
	LPCTSTR lpFileName = MyGetFileName(lpPath);
	int nFileNameOffset = (int)(lpFileName - lpPath);

	_tcsncpy(lpDir, lpPath, nFileNameOffset);
	lpDir[nFileNameOffset] = 0;

	return lpDir;
}

inline LPCTSTR MyGetExtName(LPCTSTR lpPath) 
{
	LPCTSTR lpFileName = MyGetFileName(lpPath);
	return _tcsrchr(lpFileName, _T('.'));
}

inline BOOL MyIsValidFileName(LPCTSTR lpFileName)
{
	TCHAR c;
	while ( (c = *lpFileName) != 0 )
	{
		if ( _tcschr(_T("\\/:*?\"<>|"), c) != NULL )
		{
			return FALSE;
		}

		lpFileName++;
	}

	return TRUE;
}


DWORD		MyGetFileSize(LPCTSTR lpPath);
DWORD		MyGetOsVersion();
HICON		MyGetFileIconHandle(LPCTSTR lpFilePath, BOOL bSmall = FALSE);
DWORD		MyFindProcess(LPCTSTR lpName);
DWORD		MyGetModulePath(HMODULE hModule, LPTSTR lpFilePath);
BOOL		MyGetProcessPath(DWORD dwProcessId, LPTSTR lpPath);
ULARGE_INTEGER		MyGetProcessUniqID(HANDLE hProcess = NULL, DWORD dwProcessID = 0); // low=pid, hig=creattime


HRESULT		MyCoCreateInstance(LPCTSTR lpDll, const CLSID& rclsid, const IID& riid, void** ppv);

// 0 不存在的路径
// 1 文件路径
// 2 目录路径
int			MyGetPathType(LPCTSTR lpPath);


BOOL		HaveInternetConnection();
BOOL		IsXPLast(); // 判断操作系统是否是 xp 以后的版本（包括 xp）
BOOL		IsWin64();
BOOL		IsDownloadFile(LPCTSTR lpPath); // 通过文件安全属性判断是否是下载的文件

// 返回结果调用 delete 删除
LPWSTR		Ansi2Unicode(LPCSTR lpStr, DWORD nMaxLen = 0);
LPSTR		Unicode2Ansi(LPCWSTR lpStr, DWORD nMaxLen = 0);
LPSTR		ToUTF8Str(LPCSTR lpStr);
LPSTR		ToUTF8Str(LPCWSTR lpStr, DWORD nMaxLen = 0);
LPSTR		UTF8ToAnsi(LPCSTR lpStr);
LPSTR		ToBase64Str(LPCWSTR lpStr);
void		ToBase64Str(LPCWSTR lpStr, CString& strBase64);
BOOL		IsEquHostName(LPCTSTR lpUrl1, LPCTSTR lpUrl2);
CString		MyGetHostName(LPCTSTR lpUrl);


BOOL		IsFullScreen();

template <class T>T MD5ToStr(PBYTE lpMD5, T lpStr)
{
	T lp = lpStr;
	for ( int i = 0; i < 16; i++ )
	{
		BYTE t = lpMD5[i];
		*lp++ = "0123456789abcdef"[t >> 4];
		*lp++ = "0123456789abcdef"[t & 0xf];
	}
	*lp = 0;
	
	return lpStr;
}

template <class T>
BOOL ToHex(T a, int& r)
{
	r = *a;
	if ( r >= 'a' && r <= 'f' ) 
	{
		r = r - 'a' + 10;
	}
	else if ( r >= 'A' && r <= 'F' )
	{
		r = r - 'A' + 10;
	}
	else if ( r >= '0' && r <= '9' )
	{
		r -= '0';
	}
	else
	{
		return FALSE;
	}
	
	return TRUE;
}

template <class T>BOOL StrToMD5(T lpStr, PBYTE lpMD5)
{
	for ( int i = 0; i < 16; i++ )
	{
		int t1, t2;
		
		if ( ToHex(lpStr, t1) && ToHex(lpStr + 1, t2) )
		{
			lpMD5[i] = (t1 << 4) | t2;
			lpStr += 2;
		}
		else
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

// 计算 md5
void CalcMD5(PVOID lpBuff, DWORD dwSize, LPBYTE lpMd5);
BOOL CalcFileMD5(LPCTSTR lpFilePath, LPBYTE lpMD5);


#endif /* _COMMFUNS_INC_ */