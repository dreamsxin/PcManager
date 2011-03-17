//=============================================================================
/**
* @file kserviceutil.cpp
* @brief 
* @author qiuruifeng <qiuruifeng@kingsoft.com>
* @date 2009-8-6   21:31
*/
//=============================================================================

#include "stdafx.h"
#include <Windows.h>
#include "kserviceutil.h"
#include <ShellAPI.h>
#include <tchar.h>
#define _SERVICE_QUERY_SLEEP_TIME	(500)

#define _MAX_SERVICE_QUERY_TIMES	(10)

#define _SERVICE_GET_LOCK_SLEEP_TIME (500)

#define _MAX_SERVICE_TRY_GET_LOCK_TIMES (10)

static BOOL s_bIsReboot = FALSE;

BOOL KillProcess(LPCTSTR lpClassName, LPCTSTR lpWindowName)
{
	HWND	hProcWnd;
	HANDLE	hProcHandle;
	DWORD	dwProcId;
	BOOL	bRet;

	hProcWnd = ::FindWindow(lpClassName, lpWindowName);
	if (hProcWnd == NULL)
	{
		return FALSE;
	}
	::GetWindowThreadProcessId(hProcWnd, &dwProcId);
	hProcHandle = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcId);
	if (hProcHandle == NULL)
	{
		return FALSE;
	}
	bRet = ::TerminateProcess(hProcHandle, 0);
	CloseHandle(hProcHandle);
	return bRet;
}

BOOL IsNeededReboot(void)
{
	return s_bIsReboot;
}

void RunasAdmin(void)
{
	TCHAR szPath[MAX_PATH] = {0};   
	::GetModuleFileName(NULL, szPath, MAX_PATH);   
	SHELLEXECUTEINFO sei = {0};   
	sei.cbSize = sizeof(SHELLEXECUTEINFOW);   
	sei.fMask = 0;
	sei.nShow = SW_SHOWNORMAL;

	sei.lpFile = szPath;   
	sei.lpVerb = TEXT("runas");   
	sei.lpDirectory = NULL;   

	KillProcess(NULL, TEXT("kwstray"));
	ShellExecuteEx(&sei);   
	s_bIsReboot = TRUE;
}

BOOL KCheckServiceStatus(const wchar_t* pszServiceName, 
						 BOOL* pbServiceExist, 
						 DWORD* pdwStatus, 
						 DWORD* pdwError)
{
	if (NULL == pszServiceName || NULL == pbServiceExist || NULL == pdwStatus)
		return FALSE;

	BOOL		bResult		= FALSE;
	BOOL		bRetCode	= FALSE;
	DWORD		dwErr		= 0;
	SC_HANDLE	hScm		= NULL;
	SC_HANDLE	hService	= NULL;

	SERVICE_STATUS ssStatus = { 0 }; 

	hScm = ::OpenSCManagerW(NULL, NULL, SERVICE_QUERY_STATUS);

	if (NULL == hScm)
	{
		dwErr = ::GetLastError();
		goto Exit0;
	}

	hService = ::OpenServiceW(hScm, pszServiceName, SERVICE_QUERY_STATUS);

	if (NULL == hService)
	{
		dwErr = ::GetLastError();

		if (ERROR_SERVICE_DOES_NOT_EXIST == dwErr)
		{
			bRetCode = TRUE;
		}
		goto Exit0;
	}
	else
	{
		bRetCode = TRUE;
		*pbServiceExist = TRUE;
	}

	bResult = ::QueryServiceStatus(hService, &ssStatus);

	if (bResult)
	{
		*pdwStatus = ssStatus.dwCurrentState;
	}
	else
	{
		dwErr = ::GetLastError();
	}

Exit0:
	if (pdwError)
		*pdwError = dwErr;

	if (hService)
		::CloseServiceHandle(hService);

	if (hScm)
		::CloseServiceHandle(hScm);

	return bRetCode;
}

BOOL KStartService(const wchar_t* pszServiceName, DWORD* pdwError)
{
	if (NULL == pszServiceName)
		return FALSE;

	BOOL		bResult		= FALSE;
	BOOL		bRetCode	= FALSE;
	DWORD		dwErr		= 0;
	SC_HANDLE	hScm		= NULL;
	SC_HANDLE	hService	= NULL;
	int			nTimes		= 0;

	SC_HANDLE   schSCManager	= NULL;
	SC_HANDLE   schService		= NULL;

	SERVICE_STATUS ssStatus		= {0};

	hScm = ::OpenSCManager(NULL, NULL, SERVICE_START | SERVICE_QUERY_STATUS);
	if (NULL == hScm)
	{
		dwErr = ::GetLastError();
		goto Exit0;
	}

	hService = ::OpenServiceW(hScm, pszServiceName, SERVICE_START | SERVICE_QUERY_STATUS);
	if (NULL == hService)
	{
		dwErr = ::GetLastError();
		if (dwErr == ERROR_ACCESS_DENIED)
		{
			RunasAdmin();
		}
		goto Exit0;
	}

	// 查询服务当前的状态
	bResult = ::QueryServiceStatus(hService, &ssStatus);
	if (!bResult)
	{
		dwErr = ::GetLastError();
		goto Exit0;
	}

	// 如果正在运行就退出，不做什么
	if (ssStatus.dwCurrentState == SERVICE_RUNNING)
	{
		bRetCode = TRUE;
		goto Exit0;
	}

	// 开始服务
	bResult = ::StartService(hService, 0L, NULL);
	if (!bResult)
	{
		dwErr = ::GetLastError();

		if (ERROR_SERVICE_ALREADY_RUNNING == dwErr)
		{
			bRetCode = TRUE;
		}
		goto Exit0;
	}

	// 不停的查询服务状态,直到超过规定的次数
	do
	{
		bResult = ::QueryServiceStatus(hService, &ssStatus);

		if (ssStatus.dwCurrentState != SERVICE_START_PENDING)
			break;

		nTimes++;
		Sleep(_SERVICE_QUERY_SLEEP_TIME);

	} while(nTimes < _MAX_SERVICE_QUERY_TIMES);


	if (ssStatus.dwCurrentState == SERVICE_RUNNING)
		bRetCode = TRUE;

Exit0:

	if (pdwError)
		*pdwError = dwErr;

	if (hService)
		::CloseServiceHandle(hService);

	if (hScm)
		::CloseServiceHandle(hScm);

	return bRetCode;
}

BOOL KChangeServiceConfig(const wchar_t* pszServiceName, 
						  const wchar_t* pszImagePath,  
						  const wchar_t* pszSvcOrderGroup, 
						  const wchar_t* pszDisplayName,
						  const wchar_t* pszDescription,
						  DWORD dwStartType,
						  DWORD* pdwError)
{
	if (NULL == pszServiceName)
		return FALSE;

	BOOL		bResult		= FALSE;
	BOOL		bRetCode	= FALSE;
	DWORD		dwErr		= 0;
	SC_HANDLE	hScm		= NULL;
	SC_HANDLE	hService	= NULL;
	SC_LOCK		sclLock		= NULL; 
	int			nTimes		= 0;

	SERVICE_STATUS ssStatus = { 0 };
	SERVICE_DESCRIPTION sd = {0};

	hScm = ::OpenSCManagerW(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_LOCK | SC_MANAGER_CREATE_SERVICE);

	if (NULL == hScm)
	{
		dwErr = ::GetLastError();
		goto Exit0;
	}

	do
	{
		sclLock = LockServiceDatabase(hScm);
		if (sclLock)
			break;

		++nTimes;
		Sleep(_SERVICE_GET_LOCK_SLEEP_TIME);

	}while(nTimes < _MAX_SERVICE_TRY_GET_LOCK_TIMES);

	hService = ::OpenServiceW(hScm, pszServiceName, SERVICE_CHANGE_CONFIG | SERVICE_STOP | SERVICE_USER_DEFINED_CONTROL);

	if (NULL == hService)
	{
		dwErr = ::GetLastError();
		goto Exit0;
	}

	// 如果打开了, 则修改为我们想要的服务
	bResult = ChangeServiceConfig(
		hService,                 // handle of service 
		SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, // change service type
		dwStartType,         // change service start type 
		SERVICE_ERROR_NORMAL,//SERVICE_NO_CHANGE       // change error control
		pszImagePath,				// change binary path 
		pszSvcOrderGroup,			// change load order group
		NULL,                       // tag ID: no change 
		NULL,                       // dependencies: no change 
		NULL,                       // service start account: no change 
		NULL,                       // password: no change 
		pszDisplayName);			// change display name

	if (pszDescription)
	{
		SERVICE_DESCRIPTION sd = {0};
		sd.lpDescription = (LPWSTR)pszDescription;
		ChangeServiceConfig2(
			hService,
			SERVICE_CONFIG_DESCRIPTION,
			&sd
			);
	}

	if (bResult)
		bRetCode = TRUE;
	else
		dwErr = ::GetLastError();

Exit0:
	if (pdwError)
		*pdwError = dwErr;

	if(sclLock)
		::UnlockServiceDatabase(sclLock);


	if (hService)
		::CloseServiceHandle(hService);

	if (hScm)
		::CloseServiceHandle(hScm);

	return bRetCode;
}

BOOL KCreateService(const wchar_t* pszServiceName, 
					const wchar_t* pszImagePath,  
					const wchar_t* pszSvcOrderGroup, 
					const wchar_t* pszDisplayName,
					const wchar_t* pszDescription,
					DWORD dwStartType,
					DWORD* pdwError,
					const wchar_t* pDepend,
					DWORD nServicesType)
{
	if (NULL == pszServiceName)
		return FALSE;

	BOOL		bResult		= FALSE;
	BOOL		bRetCode	= FALSE;
	DWORD		dwErr		= 0;
	SC_HANDLE	hScm		= NULL;
	SC_HANDLE	hService	= NULL;
	SC_LOCK		sclLock		= NULL; 
	int			nTimes		= 0;

	SERVICE_STATUS ssStatus = { 0 };

	hScm = ::OpenSCManagerW(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_LOCK | SC_MANAGER_CREATE_SERVICE);

	if (NULL == hScm)
	{
		dwErr = ::GetLastError();
		goto Exit0;
	}

	do
	{
		sclLock = LockServiceDatabase(hScm);
		if (sclLock)
			break;

		++nTimes;
		Sleep(_SERVICE_GET_LOCK_SLEEP_TIME);

	}while(nTimes < _MAX_SERVICE_TRY_GET_LOCK_TIMES);

	hService = ::OpenServiceW(hScm, pszServiceName, SERVICE_CHANGE_CONFIG | SERVICE_STOP | SERVICE_USER_DEFINED_CONTROL);

	if (NULL == hService)
	{
		hService = ::CreateService(
			hScm,					// SCManager database
			pszServiceName,			// name of service
			pszDisplayName,			// name to display
			SERVICE_QUERY_STATUS | SERVICE_CHANGE_CONFIG,// desired access
			nServicesType,// service type
			SERVICE_AUTO_START,         // start type
			SERVICE_ERROR_NORMAL,       // error control type
			pszImagePath,				// service's binary
			pszSvcOrderGroup,			// load ordering group
			NULL,						// no tag identifier
			pDepend,						// dependencies
			NULL,                       // LocalSystem account
			NULL);                      // no password

		if (hService)
			bResult = TRUE;
	}
	else
	{
		// 如果打开了, 则修改为我们想要的服务
		bResult = ChangeServiceConfig(
			hService,                 // handle of service 
			SERVICE_WIN32_OWN_PROCESS, // change service type
			dwStartType,         // change service start type 
			SERVICE_ERROR_NORMAL,//SERVICE_NO_CHANGE       // change error control
			pszImagePath,				// change binary path 
			pszSvcOrderGroup,			// change load order group
			NULL,                       // tag ID: no change 
			NULL,                       // dependencies: no change 
			NULL,                       // service start account: no change 
			NULL,                       // password: no change 
			pszDisplayName);			// change display name
	}

	if (pszDescription)
	{
		SERVICE_DESCRIPTION sd = {0};
		sd.lpDescription = (LPWSTR)pszDescription;
		ChangeServiceConfig2(
			hService,
			SERVICE_CONFIG_DESCRIPTION,
			&sd
			);
	}

	if (bResult)
		bRetCode = TRUE;
	else
		dwErr = ::GetLastError();

Exit0:
	if (pdwError)
		*pdwError = dwErr;

	if(sclLock)
		::UnlockServiceDatabase(sclLock);


	if (hService)
		::CloseServiceHandle(hService);

	if (hScm)
		::CloseServiceHandle(hScm);

	return bRetCode;
}

BOOL KStopService(const wchar_t* pszServiceName, DWORD* pdwError)
{
	if (NULL == pszServiceName)
		return FALSE;

	BOOL		bResult		= FALSE;
	BOOL		bRetCode	= FALSE;
	DWORD		dwErr		= 0;
	SC_HANDLE	hScm		= NULL;
	SC_HANDLE	hService	= NULL;

	SERVICE_STATUS ssStatus = { 0 };

	hScm = ::OpenSCManagerW(NULL, NULL, GENERIC_WRITE);

	if (NULL == hScm)
	{
		dwErr = ::GetLastError();
		goto Exit0;
	}

	hService = ::OpenServiceW(hScm, pszServiceName, SERVICE_STOP | SERVICE_QUERY_STATUS);

	if (NULL == hService)
	{
		dwErr = ::GetLastError();

		if (ERROR_SERVICE_DOES_NOT_EXIST == dwErr)
			bRetCode = TRUE;

		goto Exit0;
	}

	if (hService)
	{
		bRetCode = ::ControlService(hService, SERVICE_CONTROL_STOP, &ssStatus);

		if (bRetCode || ERROR_SERVICE_NOT_ACTIVE == ::GetLastError())
			bRetCode = TRUE;
		else
			dwErr = ::GetLastError();
	}

Exit0:
	if (pdwError)
		*pdwError = dwErr;

	if (hService)
		::CloseServiceHandle(hService);

	if (hScm)
		::CloseServiceHandle(hScm);

	return bRetCode;
}

BOOL KDeleteService(const wchar_t* pszServiceName, DWORD* pdwError)
{
	if (NULL == pszServiceName)
		return FALSE;

	BOOL		bResult		= FALSE;
	BOOL		bRetCode	= FALSE;
	DWORD		dwErr		= 0;
	SC_HANDLE	hScm		= NULL;
	SC_HANDLE	hService	= NULL;

	SERVICE_STATUS ssStatus = { 0 };

	hScm = ::OpenSCManagerW(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

	if (NULL == hScm)
	{
		dwErr = ::GetLastError();
		goto Exit0;
	}

	hService = ::OpenServiceW(hScm, pszServiceName, DELETE);

	if (NULL == hService)
	{
		dwErr = ::GetLastError();

		if (ERROR_SERVICE_DOES_NOT_EXIST == dwErr)
			bRetCode = TRUE;

		goto Exit0;
	}

	if (hService)
	{
		bRetCode = ::DeleteService(hService);

		if (bRetCode)
			bRetCode = TRUE;
		else
			dwErr = ::GetLastError();
	}

Exit0:
	if (pdwError)
		*pdwError = dwErr;

	if (hService)
		::CloseServiceHandle(hService);

	if (hScm)
		::CloseServiceHandle(hScm);

	return bRetCode;
}

BOOL CheckAndFixService(const wchar_t* pszServiceName, 
						const wchar_t* pszImagePath,  
						const wchar_t* pszSvcOrderGroup, 
						const wchar_t* pszDisplayName,
						const wchar_t* pszDescription,
						int* pnFixAction,
						DWORD* pdwError)
{
	if (pnFixAction)
		*pnFixAction = enumSERVICE_FIX_NONE;

	if (pdwError)
		*pdwError = 0;

	if (NULL == pszServiceName)
		return FALSE;

	DWORD dwStatus	= 0;
	DWORD dwErr		= 0;
	BOOL bSvcExist	= FALSE;
	BOOL bResult	= FALSE;

	//查询服务状态
	s_bIsReboot = FALSE;
	bResult = KCheckServiceStatus(pszServiceName, &bSvcExist, &dwStatus, &dwErr);
	if (bSvcExist && dwStatus == SERVICE_RUNNING)
		return TRUE;


	if ( bSvcExist && dwStatus == SERVICE_PAUSED )
	{
		SC_HANDLE			hScm		= NULL;
		SC_HANDLE			hService	= NULL;
		SERVICE_STATUS		status		= {0};


		do
		{
			hScm = ::OpenSCManager(NULL, NULL, SERVICE_START | SERVICE_QUERY_STATUS);
			if (NULL == hScm)
			{
				dwErr = ::GetLastError();
				if (dwErr == ERROR_ACCESS_DENIED)
				{
					RunasAdmin();
				}
				break;
			}

			hService = ::OpenServiceW(hScm, pszServiceName, SERVICE_START | SERVICE_PAUSE_CONTINUE);
			if (NULL == hService)
			{
				dwErr = ::GetLastError();
				if (dwErr == ERROR_ACCESS_DENIED)
				{
					RunasAdmin();
				}
				break;
			}

			ControlService(hService, SERVICE_CONTROL_CONTINUE, &status);
		} while (0);

		if ( NULL != hService )
		{
			::CloseServiceHandle(hService);
			hService = NULL;
		}

		if ( NULL != hScm )
		{
			::CloseServiceHandle(hScm);
			hScm = NULL;
		}

		if ( status.dwCurrentState == SERVICE_RUNNING )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	if (bSvcExist)
	{
		//启动服务
		bResult = KStartService(pszServiceName, &dwErr);
		if (bResult)
		{
			if (pnFixAction)
				*pnFixAction = enumSERVICE_FIX_START;

			return TRUE;
		}

		if (IsNeededReboot())
		{
			return FALSE;
		}

		if (!bResult && ERROR_SERVICE_DISABLED == dwErr)
		{
			//如果是由于服务被禁用而无法启动,那么把服务设置为手工启动(因为可能是用户禁用,所有不设置为自动启动)
			bResult = KChangeServiceConfig(pszServiceName, NULL, NULL, NULL, NULL, SERVICE_DEMAND_START, &dwErr);

			//修改成功后再次尝试启动
			if (bResult && KStartService(pszServiceName, &dwErr))
			{
				if (pnFixAction)
					*pnFixAction = enumSERVICE_FIX_START;

				return TRUE;
			}
		}

		//可能是注册表损坏导致无法启动或修改,那么先删除服务然后再安装
		if (!bResult && (ERROR_PATH_NOT_FOUND == dwErr || ERROR_FILE_NOT_FOUND == dwErr))
		{
			bResult =  KDeleteService(pszServiceName, &dwErr);
			bSvcExist = FALSE;
		}
	}

	if (!bSvcExist && pszImagePath)//安装服务必须要有服务程序路径
	{
		bResult = KCreateService(
			pszServiceName, 
			pszImagePath,
			pszSvcOrderGroup,
			pszDisplayName,
			pszDescription,
			SERVICE_AUTO_START,
			&dwErr);

		if (bResult && KStartService(pszServiceName, &dwErr))
		{	
			if (pnFixAction)
				*pnFixAction = enumSERVICE_FIX_INSTALL_AND_START;

			return TRUE;
		}
	}//!bSvcExist

	if (pdwError)
		*pdwError = dwErr;

	return FALSE;
}

int FixCryptService()
{
	// 修复CryptSvc的临时解决方案
	TCHAR const*	pSvrName = _T("CryptSvc");
	BOOL			nExist = FALSE;
	DWORD			nStatus = 0;
	DWORD			nError = 0;
	TCHAR			szSystemPath[MAX_PATH] = _T("");
	int				nResult = -1;
	DWORD			OSVersion = 5;


	OSVersion = (DWORD)(LOBYTE(LOWORD(::GetVersion())));
	if ( OSVersion != 5 )
	{
		nResult = -2;
		goto Exit0;
	}

	::GetSystemDirectory(szSystemPath, MAX_PATH);
	if ( szSystemPath[_tcslen(szSystemPath) - 1] != _T('\\') )
	{
		_tcscat(szSystemPath, _T("\\"));
	}
	_tcscat(szSystemPath, _T("svchost.exe -k netsvcs"));


	KCheckServiceStatus(pSvrName, &nExist, &nStatus, &nError);
	if ( SERVICE_RUNNING != nStatus || !nExist || nError != ERROR_SUCCESS )
	{
		if ( KStartService(pSvrName, &nError) )
		{
			nResult = nError;
			goto Exit0;
		}

		KCreateService(
			pSvrName, 
			szSystemPath, 
			NULL, 
			_T("Cryptographic Services"), 
			_T("提供三种管理服务: 编录数据库服务，它确定 Windows 文件的签字; 受保护的根服务，它从此计算机添加和删除受信根证书机构的证书;和密钥(Key)服务，它帮助注册此计算机获取证书。如果此服务被终止，这些管理服务将无法正常运行。如果此服务被禁用，任何依赖它的服务将无法启动。"),
			SERVICE_AUTO_START,
			&nError,
			_T("RpcSs\0"),
			SERVICE_WIN32_SHARE_PROCESS);
		if ( ERROR_SUCCESS != nError )
		{
			nResult = nError;
			goto Exit0;
		}
		else
		{
			::SHSetValue(
				HKEY_LOCAL_MACHINE, 
				_T("SYSTEM\\CurrentControlSet\\services\\CryptSvc\\Parameters"), 
				_T("ServiceDll"),
				REG_EXPAND_SZ,
				(LPVOID)_T("%SystemRoot%\\system32\\cryptsvc.dll"),
				sizeof(_T("%SystemRoot%\\system32\\cryptsvc.dll")));
			::SHSetValue(
				HKEY_LOCAL_MACHINE, 
				_T("SYSTEM\\CurrentControlSet\\services\\CryptSvc\\Parameters"), 
				_T("ServiceMain"),
				REG_SZ,
				(LPVOID)_T("CryptServiceMain"),
				sizeof(_T("CryptServiceMain")));

			KStartService(pSvrName, &nError);
			if ( ERROR_SUCCESS != nError )
			{
				nResult = nError;
				goto Exit0;
			}
		}
	}

	nResult = 0;

Exit0:

	return nResult;
}