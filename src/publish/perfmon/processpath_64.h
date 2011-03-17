
#pragma once

#include <comdef.h>
#include <Wbemidl.h>
#include "common/kaccesscontrol.h"
#pragma comment(lib, "wbemuuid.lib")
#include "Psapi.h"

#define SYSTEM_ROOT_PREFIX		L"\\SystemRoot\\"
#define WINDOWS_FOLDER_PREFIX	L"\\WINDOWS\\"
#define DEVICE_PREFIX			L"\\Device\\"

#define SYSTEM_FULL_PATH_PREFIX L"\\??\\"
#define SYSTEM_SMSS				L"smss"
#define SYSTEM_SMSS_EXE			L"\\smss.exe"

#define SYSTEM_AUDI				L"audiodg"
#define SYSTEM_AUDI_EXE				L"\\audiodg.exe"

inline INT FixDosPathToNormalPath(LPCWSTR strDevicePath, WCHAR path[MAX_PATH])
{
	if (_wcsnicmp(strDevicePath, SYSTEM_ROOT_PREFIX, wcslen(SYSTEM_ROOT_PREFIX)) == 0)
	{
		WCHAR pathSystem[MAX_PATH];
		::SHGetFolderPath(NULL, CSIDL_WINDOWS, NULL, SHGFP_TYPE_CURRENT, pathSystem);
		wcscpy_s(path, MAX_PATH, pathSystem);
		wcscat_s(path , MAX_PATH - 2, strDevicePath + wcslen(SYSTEM_ROOT_PREFIX)-1);
		if (::PathFileExists(path))
			return 2;
	}

	if (_wcsnicmp(strDevicePath, SYSTEM_FULL_PATH_PREFIX, wcslen(SYSTEM_FULL_PATH_PREFIX)) == 0)
	{
		wcscat_s(path , MAX_PATH, strDevicePath + wcslen(SYSTEM_FULL_PATH_PREFIX));
		if (::PathFileExists(path))
			return 2;
	}
	return 0;
}

class KX64ProcPathQuery
{
public:
	KX64ProcPathQuery():m_dwProcCount(0)
	{

	}
	~KX64ProcPathQuery(){}

	static KX64ProcPathQuery& Instance()
	{
		static KX64ProcPathQuery _ins;
		return _ins;
	}

	int GetProcessFullPath_x64()
	{
#define SQL_STRING	_T("Select * from Win32_Process")

		CString strProcPath = _T("");
		HRESULT hres;

		m_mapProcessPath.RemoveAll();
		// Step 1: Initialize COM--------------------------------------------------
		hres =  CoInitializeEx(0, COINIT_MULTITHREADED); 
		if (FAILED(hres))
		{
			//"Failed to initialize COM library. Error code = 0x" 
			goto Exit0;
		}

		// Step 3: ---------------------------------------------------
		// Obtain the initial locator to WMI -------------------------
		IWbemLocator *pLoc = NULL;
		hres = CoCreateInstance(
			CLSID_WbemLocator,             
			0, 
			CLSCTX_INPROC_SERVER, 
			IID_IWbemLocator, (LPVOID *) &pLoc);

		if (FAILED(hres))
		{
			//"Failed to create IWbemLocator object."
			CoUninitialize();
			goto Exit0;
		}

		// Step 4: -----------------------------------------------------
		// Connect to WMI through the IWbemLocator::ConnectServer method
		IWbemServices *pSvc = NULL;

		// Connect to the root\cimv2 namespace with
		// the current user and obtain pointer pSvc
		// to make IWbemServices calls.
		hres = pLoc->ConnectServer(
			_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
			NULL,                    // User name. NULL = current user
			NULL,                    // User password. NULL = current
			0,                       // Locale. NULL indicates current
			NULL,                    // Security flags.
			0,                       // Authority (e.g. Kerberos)
			0,                       // Context object 
			&pSvc                    // pointer to IWbemServices proxy
			);

		if (FAILED(hres))
		{
			//"Could not connect. Error code = 0x" 
			pLoc->Release();     
			CoUninitialize();
			goto Exit0;
		}

		// Step 5: --------------------------------------------------
		// Set security levels on the proxy -------------------------
		hres = CoSetProxyBlanket(
			pSvc,                        // Indicates the proxy to set
			RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
			RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
			NULL,                        // Server principal name 
			RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
			RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
			NULL,                        // client identity
			EOAC_NONE                    // proxy capabilities 
			);

		if (FAILED(hres))
		{
			//"Could not set proxy blanket. Error code = 0x" 
			pSvc->Release();
			pLoc->Release();     
			CoUninitialize();
			goto Exit0;               // Program has failed.
		}

		// Step 6: --------------------------------------------------
		// Use the IWbemServices pointer to make requests of WMI ----
		// For example, get the name of the operating system
		IEnumWbemClassObject* pEnumerator = NULL;
		hres = pSvc->ExecQuery(
			bstr_t("WQL"), 
			bstr_t(SQL_STRING),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
			NULL,
			&pEnumerator);

		if (FAILED(hres))
		{
			//"Query for operating system name failed."
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
			goto Exit0;               // Program has failed.
		}

		// Step 7: -------------------------------------------------
		// Get the data from the query in step 6 -------------------

		IWbemClassObject *pclsObj = NULL;
		ULONG uReturn = 0;
		while (pEnumerator)
		{
			HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
				&pclsObj, &uReturn);

			if(0 == uReturn) {
				break;
			}

			VARIANT vtProp;
			CString proc_info= _T("");

			DWORD nPID = -1;
			USES_CONVERSION;
			vtProp.ulVal = 0;
			hr = pclsObj->Get(L"ProcessId", 0, &vtProp, 0, 0);
			if (SUCCEEDED(hr))
			{
				nPID = vtProp.ulVal;
			}

			vtProp.bstrVal=NULL;
			HRESULT hr2 = pclsObj->Get(L"ExecutablePath", 0, &vtProp, 0, 0);
			if(SUCCEEDED(hr2) && vtProp.bstrVal != NULL)
				strProcPath = vtProp.bstrVal;

			if (SUCCEEDED(hr) && SUCCEEDED(hr2))
			{
				m_mapProcessPath[nPID] = strProcPath;
				m_dwProcCount++;
			}

			VariantClear(&vtProp);
			pclsObj->Release();
			pclsObj=NULL;
		}

		// Cleanup
		pSvc->Release();
		pLoc->Release();
		pEnumerator->Release();

		if(pclsObj != NULL)
			pclsObj->Release();

		CoUninitialize();

Exit0:
		return m_dwProcCount;
	}

	BOOL GetProcPathByID(DWORD dwPID, LPCWSTR strName, CString& path)
	{
		TCHAR szPathName[MAX_PATH] = {0};
		BOOL bRet = KAccessControl::SetCurrentProcessPrivilege(SE_DEBUG_NAME, true);
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);
		if (!hProcess)
			hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, dwPID);
		bRet = FALSE;
		if (hProcess)
		{    
			TCHAR szBuf[MAX_PATH] = {0};
			if ( 0 != GetModuleFileNameEx(hProcess, NULL, szBuf, MAX_PATH) )
			{
				if (FixDosPathToNormalPath(szBuf, szPathName) == 2)
				{
					path = szPathName;
					bRet = TRUE;
				}
				else
				{
					path = szBuf;
					bRet = TRUE;
				}
			}
			CloseHandle(hProcess);
		}
		if (!bRet)
		{
			if (_wcsnicmp(strName, SYSTEM_SMSS, wcslen(SYSTEM_SMSS)) == 0)
			{
				WCHAR pathSystem[MAX_PATH];
				::SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, pathSystem);
				wcscpy_s(szPathName, MAX_PATH, pathSystem);
				wcscat_s(szPathName, MAX_PATH, SYSTEM_SMSS_EXE);
				path = szPathName;
				bRet = TRUE;
			}
			else if (_wcsnicmp(strName, SYSTEM_AUDI, wcslen(SYSTEM_AUDI)) == 0)
			{
				WCHAR pathSystem[MAX_PATH];
				::SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, pathSystem);
				wcscpy_s(szPathName, MAX_PATH, pathSystem);
				wcscat_s(szPathName, MAX_PATH, SYSTEM_AUDI_EXE);
				path = szPathName;
				bRet = TRUE;
			}
		}
		return bRet;
	}

	CString GetProcessFullPathByPid(DWORD dwPid)
	{
		CAtlMap<DWORD, CString>::CPair* pair = m_mapProcessPath.Lookup(dwPid);
		if (pair != NULL)
		{
			return m_mapProcessPath[dwPid];
		}
		else
			return GetProcessFullPathByPid_WMI(dwPid);
	}

	CString GetProcessFullPathByPid_WMI(DWORD dwPid)
	{
#define SQL_STRING	_T("Select * from Win32_Process Where ProcessId = %d")

		CString strProcPath = _T("");
		HRESULT hres;
		CString strSql;
		strSql.Format(SQL_STRING, dwPid);

		// Step 1: Initialize COM--------------------------------------------------
		hres =  CoInitializeEx(0, COINIT_MULTITHREADED); 
		if (FAILED(hres))
		{
			//"Failed to initialize COM library. Error code = 0x" 
			goto Exit0;
		}

		// Step 3: ---------------------------------------------------
		// Obtain the initial locator to WMI -------------------------
		IWbemLocator *pLoc = NULL;
		hres = CoCreateInstance(
			CLSID_WbemLocator,             
			0, 
			CLSCTX_INPROC_SERVER, 
			IID_IWbemLocator, (LPVOID *) &pLoc);

		if (FAILED(hres))
		{
			//"Failed to create IWbemLocator object."
			CoUninitialize();
			goto Exit0;
		}

		// Step 4: -----------------------------------------------------
		// Connect to WMI through the IWbemLocator::ConnectServer method
		IWbemServices *pSvc = NULL;

		// Connect to the root\cimv2 namespace with
		// the current user and obtain pointer pSvc
		// to make IWbemServices calls.
		hres = pLoc->ConnectServer(
			_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
			NULL,                    // User name. NULL = current user
			NULL,                    // User password. NULL = current
			0,                       // Locale. NULL indicates current
			NULL,                    // Security flags.
			0,                       // Authority (e.g. Kerberos)
			0,                       // Context object 
			&pSvc                    // pointer to IWbemServices proxy
			);

		if (FAILED(hres))
		{
			//"Could not connect. Error code = 0x" 
			pLoc->Release();     
			CoUninitialize();
			goto Exit0;
		}

		// Step 5: --------------------------------------------------
		// Set security levels on the proxy -------------------------
		hres = CoSetProxyBlanket(
			pSvc,                        // Indicates the proxy to set
			RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
			RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
			NULL,                        // Server principal name 
			RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
			RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
			NULL,                        // client identity
			EOAC_NONE                    // proxy capabilities 
			);

		if (FAILED(hres))
		{
			//"Could not set proxy blanket. Error code = 0x" 
			pSvc->Release();
			pLoc->Release();     
			CoUninitialize();
			goto Exit0;               // Program has failed.
		}

		// Step 6: --------------------------------------------------
		// Use the IWbemServices pointer to make requests of WMI ----
		// For example, get the name of the operating system
		IEnumWbemClassObject* pEnumerator = NULL;
		hres = pSvc->ExecQuery(
			bstr_t("WQL"), 
			bstr_t(strSql.GetString()),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
			NULL,
			&pEnumerator);

		if (FAILED(hres))
		{
			//"Query for operating system name failed."
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
			goto Exit0;               // Program has failed.
		}

		// Step 7: -------------------------------------------------
		// Get the data from the query in step 6 -------------------

		IWbemClassObject *pclsObj = NULL;
		ULONG uReturn = 0;
		while (pEnumerator)
		{
			HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
				&pclsObj, &uReturn);

			if(0 == uReturn) {
				break;
			}

			VARIANT vtProp;
			CString proc_info= _T("");

			DWORD nPID = -1;
			USES_CONVERSION;

			vtProp.bstrVal=NULL;
			HRESULT hr2 = pclsObj->Get(L"ExecutablePath", 0, &vtProp, 0, 0);
			if(SUCCEEDED(hr2) && vtProp.bstrVal != NULL)
			{
				strProcPath = vtProp.bstrVal;
				m_mapProcessPath[dwPid] = strProcPath;
			}

			VariantClear(&vtProp);
			pclsObj->Release();
			pclsObj=NULL;
		}

		// Cleanup
		pSvc->Release();
		pLoc->Release();
		pEnumerator->Release();

		if(pclsObj != NULL)
			pclsObj->Release();

		CoUninitialize();

Exit0:
		return strProcPath;
	}

private:
	CAtlMap<DWORD, CString> m_mapProcessPath;
	DWORD	m_dwProcCount;
};


