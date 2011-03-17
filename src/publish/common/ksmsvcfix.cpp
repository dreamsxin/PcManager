//=============================================================================
/**
* @file ksmsvcfix.cpp
* @brief 
* @author qiuruifeng <qiuruifeng@kingsoft.com>
* @date 2009-8-6   11:48
*/
//=============================================================================
#include "stdafx.h"
#include <Windows.h>
#include <string>
#include "ksmsvcfix.h"
#include "kserviceutil.h"
#include "kosversion.h"

#define	KSM_SERVICE_NAME_W			(L"KSafeSvc")
#define	KSM_SERVICE_DISPLAY_NAME_W	(L"KSafe service")
#define	KSM_SERVICE_DESCRIPTION_W	(L"KSafe service")
#define	KSM_SERVICE_FILE_NAME_W		(L"KSafeSvc.exe")


#define	KSM_SERVICE_ORDER_GROUP_1_W	(L"SchedulerGroup")
#define	KSM_SERVICE_ORDER_GROUP_2_W	(L"SpoolerGroup")

#define KXESERV_SERVICE_NAME_W			(L"KxEServBeta")
#define KXESERV_SERVICE_DISPLAY_NAME_W	(L"Kingsoft Antivirus XEngine Service(Beta)")
#define KXESERV_SERVICE_DESCRIPTION_W	(L"Kingsoft Antivirus XEngine Service(Beta)")
#define KXESERV_SERVICE_FILE_NAME_W		(L"kxeserv.exe")
#define KXESERV_SERVICE_PATH_NAME_W		(L"\\Program Files\\Common Files\\Kingsoft\\CommonService_Beta\\kxeserv.exe")

#define KXESERV_SERVICE_ORDERGROUP_1_W  (L"RemoteValidation")
#define KXESERV_SERVICE_ORDERGROUP_2_W  (L"RemoteValidation")

// BOOL CheckAndFixKXEBaseService(int* pnFixAction,
// 							   DWORD* pdwError)
// {
// 	return FALSE;
// }

BOOL GetModuleDirPathW(HMODULE hHandle, wchar_t* szDirPath, DWORD dwSize)
{
	if (NULL == szDirPath || 0 == dwSize)
		return FALSE;

	BOOL bRetCode = FALSE;
	szDirPath[0] = 0;
	szDirPath[dwSize - 1] = 0;
	if (::GetModuleFileNameW(hHandle, szDirPath, (dwSize - 1)) != 0)
	{
		wchar_t* pszPos = wcsrchr(szDirPath, L'\\');
		if (pszPos != NULL)
		{
			++pszPos;
			*pszPos = 0;
			bRetCode = TRUE;
		}		
	}
	return bRetCode;
}

BOOL CheckFileExistsExceptDir(const wchar_t* pszFile, BOOL& bExist)
{
	WIN32_FILE_ATTRIBUTE_DATA attr = { 0 };

	DWORD dwAttr = ::GetFileAttributesW(pszFile);

	if ( dwAttr != INVALID_FILE_ATTRIBUTES && !(dwAttr & FILE_ATTRIBUTE_DIRECTORY) )
		bExist = TRUE;
	else
		bExist = FALSE;

	return TRUE;
}

BOOL GetKXEBaseServicePath(std::wstring& strPath)
{
	wchar_t szWindowsDir[MAX_PATH] = { 0 };
	unsigned int uRetCode = 0;
	uRetCode = ::GetWindowsDirectory(szWindowsDir, MAX_PATH);
	if (0 == uRetCode)
		return FALSE;

	szWindowsDir[2] = 0;
	strPath += szWindowsDir;
	strPath += KXESERV_SERVICE_PATH_NAME_W;

	return TRUE;

}

BOOL CheckAndFixKXEBaseService(int* pnFixAction, DWORD* pdwError)
{
	BOOL bResult	= FALSE;
	//BOOL bRetCode	= FALSE;
	BOOL bFileExist = FALSE;
	DWORD dwErr		= 0;
	int	nFixAction  = 0;
	std::wstring strSvcPath;

	const wchar_t* pszServiceName	= KXESERV_SERVICE_NAME_W;
	const wchar_t* pszImagePath		= NULL;
	const wchar_t* pszSvcOrderGroup = NULL;
	const wchar_t* pszDisplayName	= KXESERV_SERVICE_DISPLAY_NAME_W;
	const wchar_t* pszDescription	= KXESERV_SERVICE_DESCRIPTION_W;

	bResult = GetKXEBaseServicePath(strSvcPath);
	CheckFileExistsExceptDir(strSvcPath.c_str(), bFileExist);

	if (bFileExist)
		pszImagePath = strSvcPath.c_str();

	pszSvcOrderGroup = 
		OS_VER::IsWinXPOrLater() ? KXESERV_SERVICE_ORDERGROUP_1_W : KXESERV_SERVICE_ORDERGROUP_2_W;

	bResult = CheckAndFixService(
		pszServiceName,
		pszImagePath,  
		pszSvcOrderGroup, 
		pszDisplayName,
		pszDescription,
		&nFixAction,
		&dwErr);

	if (pnFixAction)
		*pnFixAction = nFixAction;

	if (pdwError)
		*pdwError = dwErr;

	return bResult;
}

BOOL GetKSMServicePath(std::wstring& strPath)
{
	BOOL bRetCode = FALSE;
	wchar_t szDirPath[MAX_PATH] = { 0 };
	GetModuleDirPathW(NULL, szDirPath, MAX_PATH);
	std::wstring strKSMSvcPath = szDirPath;
	strKSMSvcPath += KSM_SERVICE_FILE_NAME_W;
	
	BOOL bFileExist = FALSE;

	CheckFileExistsExceptDir(strKSMSvcPath.c_str(), bFileExist);

	if (!bFileExist)
	{
		strKSMSvcPath = szDirPath;
		strKSMSvcPath += L"ksm\\";
		strKSMSvcPath += KSM_SERVICE_FILE_NAME_W;
		CheckFileExistsExceptDir(strKSMSvcPath.c_str(), bFileExist);
	}

	if (bFileExist)
	{
		strPath = strKSMSvcPath;
		bRetCode = TRUE;
	}
	return bRetCode;
}

BOOL CheckAndFixKSMService(int* pnFixAction, DWORD* pdwError)
{
	BOOL bResult	= FALSE;
	//BOOL bRetCode	= FALSE;
	BOOL bFileExist = FALSE;
	DWORD dwErr		= 0;
	int	nFixAction  = 0;
	std::wstring strKSMSvcPath;

	const wchar_t* pszServiceName	= KSM_SERVICE_NAME_W;
	const wchar_t* pszImagePath		= NULL;
	const wchar_t* pszSvcOrderGroup = NULL;
	const wchar_t* pszDisplayName	= KSM_SERVICE_DISPLAY_NAME_W;
	const wchar_t* pszDescription	= KSM_SERVICE_DESCRIPTION_W;

	bResult = GetKSMServicePath(strKSMSvcPath);
	//CheckFileExistsExceptDir(strKSMSvcPath.c_str(), bFileExist);

	if (bResult)
		pszImagePath = strKSMSvcPath.c_str();

	pszSvcOrderGroup = 
		OS_VER::IsWinXPOrLater() ? KSM_SERVICE_ORDER_GROUP_1_W : KSM_SERVICE_ORDER_GROUP_2_W;

	bResult = CheckAndFixService(
		pszServiceName,
		pszImagePath,  
		pszSvcOrderGroup, 
		pszDisplayName,
		pszDescription,
		&nFixAction,
		&dwErr);

	if (pnFixAction)
		*pnFixAction = nFixAction;

	if (pdwError)
		*pdwError = dwErr;

	return bResult;
}