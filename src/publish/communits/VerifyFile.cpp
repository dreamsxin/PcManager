
#include "stdafx.h"
#include "VerifyFile.h"
#include <SoftPub.h>

typedef HANDLE HCATADMIN;
typedef HANDLE HCATINFO;

typedef BOOL (WINAPI* PFN_CryptCATAdminAcquireContext)(HCATADMIN* phCatAdmin, const GUID* pgSubsystem, DWORD dwFlags);
typedef BOOL (WINAPI* PFN_CryptCATAdminReleaseContext)(HCATADMIN hCatAdmin, DWORD dwFlags);
typedef BOOL (WINAPI* PFN_CryptCATAdminCalcHashFromFileHandle)(HANDLE hFile, DWORD* pcbHash, BYTE* pbHash, DWORD dwFlags);
typedef HCATINFO (WINAPI* PFN_CryptCATAdminEnumCatalogFromHash)(HCATADMIN hCatAdmin, BYTE* pbHash, DWORD cbHash, DWORD dwFlags, HCATINFO* phPrevCatInfo );
typedef BOOL (WINAPI* PFN_CryptCATAdminReleaseCatalogContext)(HCATADMIN hCatAdmin, HCATINFO hCatInfo, DWORD dwFlags);
typedef LONG (WINAPI* PFN_WinVerifyTrust)(HWND hWnd, GUID* pgActionID, WINTRUST_DATA* pWinTrustData);

static PFN_CryptCATAdminAcquireContext __pfnCryptCATAdminAcquireContext = NULL;
static PFN_CryptCATAdminReleaseContext __pfnCryptCATAdminReleaseContext = NULL;
static PFN_CryptCATAdminCalcHashFromFileHandle __pfnCryptCATAdminCalcHashFromFileHandle = NULL;
static PFN_CryptCATAdminEnumCatalogFromHash __pfnCryptCATAdminEnumCatalogFromHash = NULL;
static PFN_CryptCATAdminReleaseCatalogContext __pfnCryptCATAdminReleaseCatalogContext = NULL;
static PFN_WinVerifyTrust __pfnWinVerifyTrust = NULL;


static int LoadWinTrustDll()
{
	static int nResult = 0;
	
	if ( nResult == 0 )
	{
		nResult = -1;

		HMODULE hModule = LoadLibraryW(SP_POLICY_PROVIDER_DLL_NAME);
		if ( hModule != NULL )
		{
			(FARPROC&)__pfnCryptCATAdminAcquireContext = GetProcAddress(hModule, "CryptCATAdminAcquireContext");
			if ( __pfnCryptCATAdminAcquireContext == NULL )
			{
				goto _Failed_Exit;
			}

			(FARPROC&)__pfnCryptCATAdminReleaseContext = GetProcAddress(hModule, "CryptCATAdminReleaseContext");
			if ( __pfnCryptCATAdminReleaseContext == NULL )
			{
				goto _Failed_Exit;
			}

			(FARPROC&)__pfnCryptCATAdminCalcHashFromFileHandle = GetProcAddress(hModule, "CryptCATAdminCalcHashFromFileHandle");
			if ( __pfnCryptCATAdminCalcHashFromFileHandle == NULL )
			{
				goto _Failed_Exit;
			}

			(FARPROC&)__pfnCryptCATAdminEnumCatalogFromHash = GetProcAddress(hModule, "CryptCATAdminEnumCatalogFromHash");
			if ( __pfnCryptCATAdminEnumCatalogFromHash == NULL )
			{
				goto _Failed_Exit;
			}

			(FARPROC&)__pfnCryptCATAdminReleaseCatalogContext = GetProcAddress(hModule, "CryptCATAdminReleaseCatalogContext");
			if ( __pfnCryptCATAdminReleaseCatalogContext == NULL )
			{
				goto _Failed_Exit;
			}

			(FARPROC&)__pfnWinVerifyTrust = GetProcAddress(hModule, "WinVerifyTrust");
			if ( __pfnWinVerifyTrust == NULL )
			{
				goto _Failed_Exit;
			}

			nResult = 1;
		}

_Failed_Exit:;
	}

	return nResult;
}

static BOOL CalcCatHash(LPCWSTR lpFileName, DWORD* pcbHash, BYTE** ppbHash)
{
	HANDLE hFile = CreateFileW(lpFileName, 
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0, 
		NULL
		);
	if ( hFile != INVALID_HANDLE_VALUE )
	{
		DWORD Err;
		LPBYTE Hash = NULL;
		DWORD HashSize = 0;
		
		//
		// Start out with a hash buffer size that should be large enough for
		// most requests.
		//
		HashSize = 100;
		
		do
		{
			Hash = (LPBYTE)LocalAlloc(LPTR, HashSize);
			
			if(!Hash)
			{
				Err = ERROR_NOT_ENOUGH_MEMORY;
				break;
			}
			
			if( __pfnCryptCATAdminCalcHashFromFileHandle(hFile, &HashSize, Hash, 0))
			{
				Err = NO_ERROR;
			}
			else
			{
				Err = GetLastError();
				
				//
				// If this API did screw up and not set last error, go ahead
				// and set something.
				//
				if(Err == NO_ERROR)
				{
					Err = ERROR_INVALID_DATA;
				}
				
				LocalFree(Hash);
				
				if(Err != ERROR_INSUFFICIENT_BUFFER)
				{
					//
					// The API failed for some reason other than
					// buffer-too-small.  We gotta bail.
					//
					Hash = NULL;  // reset this so we won't try to free it later
					break;
				}
			}
		} while(Err != NO_ERROR);
		
		CloseHandle(hFile);

		if(Err == NO_ERROR)
		{
			*pcbHash = HashSize;
			*ppbHash = Hash;

			return TRUE;
		}
	}

	*pcbHash = 0;
	*ppbHash = NULL;
	return FALSE;
}

HANDLE  AcquireVerifyHandle()
{
	if ( LoadWinTrustDll() != 1 )
	{
		return NULL;
	}
	
	HANDLE hHandle = NULL;
	if ( __pfnCryptCATAdminAcquireContext(&hHandle, NULL, 0) )
	{
		return hHandle;
	}

	return NULL;
}

void ReleaseVerifyHandle(HANDLE hHandle)
{
	if ( LoadWinTrustDll() != 1 )
	{
		return ;
	}

	__pfnCryptCATAdminReleaseContext(hHandle, 0);
}

int VerifyFile(HANDLE hHandle, LPCTSTR lpPath)
{
	int nResult = 0;
	if ( LoadWinTrustDll() != 1 )
	{
		return 0;
	}

	WINTRUST_DATA wd = { 0 };
	WINTRUST_FILE_INFO wfi = { 0 };
	GUID guid = WINTRUST_ACTION_GENERIC_VERIFY_V2;
	
	wfi.cbStruct = sizeof(WINTRUST_FILE_INFO); 
	wfi.pcwszFilePath = lpPath; 
	
	wd.cbStruct = sizeof(WINTRUST_DATA);
	wd.dwUnionChoice = WTD_CHOICE_FILE;
	wd.pFile = &wfi;
	wd.dwUIChoice = WTD_UI_NONE;
	wd.fdwRevocationChecks = WTD_REVOKE_NONE;
	wd.dwStateAction = WTD_STATEACTION_AUTO_CACHE;
	wd.dwProvFlags = WTD_REVOCATION_CHECK_NONE;
	if ( __pfnWinVerifyTrust(NULL, &guid , &wd) == 0 ) // pe 签名
	{
		nResult = 1;
	}
	else
	{
		if ( hHandle != NULL )
		{
			// cat 签名
			LPBYTE lpHash;
			DWORD dwHashSize;
			
			if ( CalcCatHash(lpPath, &dwHashSize, &lpHash))
			{
				HANDLE hCatalogContext = __pfnCryptCATAdminEnumCatalogFromHash(hHandle, lpHash, dwHashSize, 0, NULL);
				if ( NULL != hCatalogContext)
				{
					__pfnCryptCATAdminReleaseCatalogContext(hHandle, hCatalogContext, 0);
					nResult = 2;
				}
				
				LocalFree(lpHash);
				lpHash = NULL;
			}
		}
	}

	return nResult;
}


BOOL IsVerifiedFile(LPCTSTR lpPath)
{
	BOOL bResult = FALSE;
	
	HANDLE hHandle = AcquireVerifyHandle();
	if ( hHandle != NULL )
	{
		__try
		{
			// 验证签名异常，崩溃在系统函数里面
			// 暂时无法确定原因
			if ( VerifyFile(hHandle, lpPath) != 0 )
			{
				bResult = TRUE;
			}
		}
		__except ( EXCEPTION_EXECUTE_HANDLER )
		{
		}
		
		ReleaseVerifyHandle(hHandle);
	}

	return bResult;
}