/**
* @file    bootreplacetool.cpp
* @brief   ...
* @author  zhangjun
* @date    2009-10-16 14:00
*/

#include "stdafx.h"
#include "bootreplacetool.h"

#include <atlbase.h>
#include <atlpath.h>
#include <atlfile.h>

#include "encryptfile.h"

#pragma warning(disable: 4996)

using namespace SafeApi;
#pragma comment(lib, "Version.lib")

#define BOOT_CHECK_MAX_ENTRY_LIMIT  65535

//HRESULT CBootReplaceTool::InstallDriver(LPCWSTR lpszDriverPath)
//{
//	// 如果驱动较新,则复制之
//	HRESULT hr = CopyDriverIfNew(lpszDriverPath);
//	if (FAILED(hr))
//		return hr;
//
//
//	// 获取系统目录
//	WCHAR szSystemDriver[MAX_PATH + 1];
//	UINT  uRet = ::GetSystemDirectory(szSystemDriver, MAX_PATH);
//	if (!uRet)
//		return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
//
//	::PathAppend(szSystemDriver, L"Drivers");
//	::PathAppend(szSystemDriver, BC_DriverFileName);
//
//
//
//	// 对驱动路径进行加工,对包含空格的路径加引号(见MSDN)
//	CPath quotedPathDriver = szSystemDriver;
//	quotedPathDriver.QuoteSpaces();
//
//
//
//	// 安装驱动/修改驱动设置
//	SC_HANDLE hScm = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
//	if (NULL == hScm)
//		return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
//
//
//
//	SC_HANDLE hService = ::OpenService(hScm, BC_ServiceName, SC_MANAGER_ALL_ACCESS);
//	if (hService)
//	{
//		BOOL bRet = ::ChangeServiceConfig(
//			hService,
//			SERVICE_KERNEL_DRIVER, 
//			SERVICE_BOOT_START,
//			SERVICE_ERROR_NORMAL,
//			quotedPathDriver.m_strPath,
//			BC_ServiceGroup,
//			NULL,
//			NULL,
//			NULL,
//			NULL,
//			NULL);
//		if (!bRet)
//		{
//			::CloseServiceHandle(hService);
//			::CloseServiceHandle(hScm);
//			return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
//		}
//	}
//	else
//	{
//		hService = ::CreateService(
//			hScm,
//			BC_ServiceName,
//			BC_ServiceName,
//			SERVICE_ALL_ACCESS,
//			SERVICE_KERNEL_DRIVER,
//			SERVICE_BOOT_START,
//			SERVICE_ERROR_NORMAL,
//			quotedPathDriver.m_strPath,
//			BC_ServiceGroup,
//			NULL,
//			NULL,
//			NULL,
//			NULL);
//		if (NULL == hService)
//		{
//			::CloseServiceHandle(hScm);
//			return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
//		}
//	}
//
//
//
//	::CloseServiceHandle(hService);
//	::CloseServiceHandle(hScm);
//	return S_OK;
//}
//
//HRESULT CBootReplaceTool::UninstallDriver()
//{
//	WCHAR szConfigFile[MAX_PATH + 1];
//	::GetSystemDirectory(szConfigFile, MAX_PATH);
//	::PathAppend(szConfigFile, FileBootCleanConfigFileName);
//	::DeleteFile(szConfigFile);
//
//	::GetSystemDirectory(szConfigFile, MAX_PATH);
//	::PathAppend(szConfigFile, FileBootReplaceConfigFileName);
//	::DeleteFile(szConfigFile);
//
//	::GetSystemDirectory(szConfigFile, MAX_PATH);
//	::PathAppend(szConfigFile, RegBootCleanConfigFileName);
//	::DeleteFile(szConfigFile);
//
//	::GetSystemDirectory(szConfigFile, MAX_PATH);
//	::PathAppend(szConfigFile, DriverApprovedConfigFileName);
//	::DeleteFile(szConfigFile);
//
//	::GetSystemDirectory(szConfigFile, MAX_PATH);
//	::PathAppend(szConfigFile, DriverBlockedConfigFileName);
//	::DeleteFile(szConfigFile);
//
//
//	// 卸载驱动
//	SC_HANDLE hScm = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
//	if (NULL == hScm)
//		return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
//
//
//	SC_HANDLE hService = ::OpenService(hScm, BC_ServiceName, SC_MANAGER_ALL_ACCESS);
//	if (NULL == hScm)
//	{
//		::CloseServiceHandle(hScm);
//		return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
//	}
//
//
//	// 不支持停止
//	//SERVICE_STATUS status;
//	//BOOL bStopService = ::ControlService(hService, SERVICE_CONTROL_STOP, &status);
//	//if (!bStopService)
//	//{
//	//    ::CloseServiceHandle(hService);
//	//    ::CloseServiceHandle(hScm);
//	//    return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
//	//}
//
//
//	BOOL bDeleteService = ::DeleteService(hService);
//	if (!bDeleteService)
//	{
//		::CloseServiceHandle(hService);
//		::CloseServiceHandle(hScm);
//		return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
//	}
//
//
//	WCHAR szDriverFile[MAX_PATH + 1];
//	::GetSystemDirectory(szDriverFile, MAX_PATH);
//	::PathAppend(szDriverFile, L"Drivers");
//	::PathAppend(szDriverFile, BC_DriverFileName);
//	::DeleteFile(szDriverFile);
//
//
//	::CloseServiceHandle(hService);
//	::CloseServiceHandle(hScm);
//	return S_OK;
//}

HRESULT CBootReplaceTool::BootReplaceFile(LPCWSTR lpszDestFilePath, LPCWSTR lpszSrcFilePath)
{
	return BootAddToConfigFile(FileBootReplaceConfigFileName, lpszDestFilePath, lpszSrcFilePath);
}

//HRESULT CBootReplaceTool::CopyDriverIfNew(LPCWSTR lpszNewDriver)
//{
//	WCHAR szOldDriver[MAX_PATH + 1];
//	UINT  uRet = ::GetSystemDirectory(szOldDriver, MAX_PATH);
//	if (!uRet)
//		return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
//
//
//	::PathAppend(szOldDriver, L"Drivers");
//	::PathAppend(szOldDriver, BC_DriverFileName);
//
//
//	ULARGE_INTEGER uiOldVersion = GetDllVersion(szOldDriver);
//	ULARGE_INTEGER uiNewVersion = GetDllVersion(lpszNewDriver);
//
//
//	if (uiNewVersion.QuadPart <= uiOldVersion.QuadPart)
//		return S_FALSE; // 不需要拷贝
//
//
//	// 拷贝较新的版本
//	BOOL br = ::CopyFile(lpszNewDriver, szOldDriver, FALSE);
//	if (!br)
//		return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
//
//
//	return S_OK;
//}
//
//ULARGE_INTEGER CBootReplaceTool::GetDllVersion(LPCWSTR lpszFilePath)
//{
//	BOOL  bRet              = FALSE;
//	DWORD dwHandle          = 0;
//	DWORD dwFileInfoSize    = 0;
//	VS_FIXEDFILEINFO *pFixFileInfo = NULL;
//	TCHAR *pszFileInfo      = NULL;
//	UINT  uLen              = 0;
//	ULARGE_INTEGER uiVersion;
//	uiVersion.HighPart = 0;
//	uiVersion.LowPart  = 0;
//
//	dwFileInfoSize = ::GetFileVersionInfoSize(lpszFilePath, &dwHandle);
//	if (dwFileInfoSize == 0)
//		goto Exit0;      
//
//	pszFileInfo = new TCHAR[dwFileInfoSize];
//	if (pszFileInfo == NULL)
//		goto Exit0;      
//
//	bRet = ::GetFileVersionInfo(lpszFilePath, dwHandle, dwFileInfoSize, pszFileInfo);
//	if (!bRet)
//		goto Exit0;
//
//	bRet = ::VerQueryValue(pszFileInfo, _T("\\"), (LPVOID *)&pFixFileInfo, &uLen);
//	if (!bRet)
//		goto Exit0;
//
//	if (uLen > 0)
//	{
//		uiVersion.HighPart = pFixFileInfo->dwFileVersionMS;
//		uiVersion.LowPart  = pFixFileInfo->dwFileVersionLS;
//	}
//
//
//Exit0:
//	if (pszFileInfo)
//	{
//		delete pszFileInfo;
//		pszFileInfo = NULL;
//	}
//
//	return uiVersion;
//}
//
//HRESULT CBootReplaceTool::RemoveConfigFile()
//{
//	WCHAR szConfigPath[MAX_PATH + 1];
//	UINT  uRet = ::GetSystemDirectory(szConfigPath, MAX_PATH);
//	if (!uRet)
//		return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
//
//
//	::PathAppend(szConfigPath, FileBootReplaceConfigFileName);
//
//
//	BOOL br = ::DeleteFile(szConfigPath);
//	if (!br)
//		return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
//
//
//	return S_OK;
//}

HRESULT CBootReplaceTool::GetDriverPath(WCHAR szSystemDriver[MAX_PATH + 1], LPCWSTR lpszConfigName)
{
	// 获取系统目录
	UINT  uRet = ::GetSystemDirectory(szSystemDriver, MAX_PATH);
	if (!uRet)
		return GetLastError() ? AtlHresultFromLastError() : E_FAIL;


	::PathAppend(szSystemDriver, lpszConfigName);
	return S_OK;
}

HRESULT CBootReplaceTool::BootFillEntry(FILE_REPLACE_ENTRY& entry, LPCWSTR lpszDestFilePath, LPCWSTR lpszSrcFilePath)
{
	// 生成删除项
	memset(&entry, 0, sizeof(FILE_REPLACE_ENTRY));
	// 如果有,则去掉unicode前缀
	if (0 == StrCmpNW(lpszSrcFilePath, L"\\\\?\\", 4))
	{
		lpszSrcFilePath += 4;
	}

	if (0 == StrCmpNW(lpszDestFilePath, L"\\\\?\\", 4))
	{
		lpszDestFilePath += 4;
	}

	// 如果没有,则添加内核路径前缀
	if (0 != StrCmpNW(lpszSrcFilePath, L"\\??\\", 4))
	{
		wcsncpy(entry.wstrFileFrom,     L"\\??\\",    4);
		wcsncpy(entry.wstrFileFrom + 4, lpszSrcFilePath,  _countof(entry.wstrFileFrom) - 4);
	}
	else
	{
		wcsncpy(entry.wstrFileFrom, lpszSrcFilePath, _countof(entry.wstrFileFrom));
	}

	if (0 != StrCmpNW(lpszDestFilePath, L"\\??\\", 4))
	{
		wcsncpy(entry.wstrFileTo,     L"\\??\\",    4);
		wcsncpy(entry.wstrFileTo + 4, lpszDestFilePath,  _countof(entry.wstrFileTo) - 4);
	}
	else
	{
		wcsncpy(entry.wstrFileTo, lpszDestFilePath, _countof(entry.wstrFileTo));
	}

	// 确保结束符
	entry.wstrFileFrom[_countof(entry.wstrFileFrom) - 1] = L'\0';
	CharLower(entry.wstrFileFrom);

	entry.wstrFileTo[_countof(entry.wstrFileTo) - 1] = L'\0';
	CharLower(entry.wstrFileTo);

	// md5没有使用,这里不填写md5
	return S_OK;
}

HRESULT CBootReplaceTool::BootAddToConfigFile(LPCWSTR lpszConfigName, LPCWSTR lpszDestFilePath, LPCWSTR lpszSrcFilePath)
{
	if (!lpszConfigName || !*lpszConfigName)
		return E_INVALIDARG;

	if (!lpszSrcFilePath || !*lpszSrcFilePath || !lpszDestFilePath || !*lpszDestFilePath)
		return S_FALSE;


	// 获取系统目录
	WCHAR szSystemDriver[MAX_PATH + 1];
	HRESULT hr = GetDriverPath(szSystemDriver, lpszConfigName);
	if (FAILED(hr))
		return hr;



	// 读取配置文件
	CAtlMap<CString, CString> fileList;
	BootLoadConfigFile(lpszConfigName, fileList);


	// 如果有,则去掉unicode前缀
	FILE_REPLACE_ENTRY replaceEntry;
	BootFillEntry(replaceEntry, lpszDestFilePath, lpszSrcFilePath);
	CString t_csDestFile = replaceEntry.wstrFileTo;
	CString t_csSrcFile  = replaceEntry.wstrFileFrom;

	// 检查是否有指定的条目
	if (fileList.Lookup(t_csDestFile))
		return S_FALSE;


	// 添加条目
	fileList.SetAt(t_csDestFile, t_csSrcFile);


	// 更新配置文件
	return BootSaveConfigFile(lpszConfigName, fileList);
}

HRESULT CBootReplaceTool::BootRemoveFromConfigFile(LPCWSTR lpszConfigName, CAtlMap<CString, CString>& removeList)
{
	if (!lpszConfigName || !*lpszConfigName)
		return E_INVALIDARG;

	if (removeList.IsEmpty())
		return S_FALSE;

	// 获取系统目录
	WCHAR szSystemDriver[MAX_PATH + 1];
	HRESULT hr = GetDriverPath(szSystemDriver, lpszConfigName);
	if (FAILED(hr))
		return hr;



	// 读取配置文件
	CAtlMap<CString, CString> fileList;
	BootLoadConfigFile(lpszConfigName, fileList);


	// 删除指定的条目
	for (POSITION pos = removeList.GetStartPosition(); pos; removeList.GetNext(pos))
	{
		CString t_csDestFile = removeList.GetKeyAt(pos);
		// 如果有,则去掉unicode前缀
		FILE_REPLACE_ENTRY replaceEntry;
		BootFillEntry(replaceEntry, t_csDestFile, t_csDestFile); // note:这里为方便srcFile部分也填destFile

		// 删除指定项目
		fileList.RemoveKey(replaceEntry.wstrFileTo);
	}


	// 更新配置文件
	return BootSaveConfigFile(lpszConfigName, fileList);
}

HRESULT CBootReplaceTool::BootSaveConfigFile(LPCWSTR lpszConfigName, CAtlMap<CString, CString>& writeList)
{
	if (!lpszConfigName || !*lpszConfigName)
		return E_INVALIDARG;


	// 获取系统目录
	WCHAR szSystemDriver[MAX_PATH + 1];
	HRESULT hr = GetDriverPath(szSystemDriver, lpszConfigName);
	if (FAILED(hr))
		return hr;


	CString strBackup = szSystemDriver;
	strBackup.Append(L".write");


	// 打开配置文件
	CEncryptFile hCfgFile;
	hr = hCfgFile.Create(
		strBackup,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_DELETE,      // do not share file
		OPEN_ALWAYS);
	if (FAILED(hr))
		return hr;


	hr = hCfgFile.SetSize(0);
	if (FAILED(hr))
	{
		::DeleteFile(strBackup);
		return hr;
	}


	// 更新文件头
	FILE_REPLACE_HEADER fileReplaceHeader;
	memset(&fileReplaceHeader, 0, sizeof(fileReplaceHeader));
	fileReplaceHeader.ulWindowsVersion = ::GetVersion();
	fileReplaceHeader.ulNumber         = (DWORD)writeList.GetCount();

	hr = hCfgFile.Write(&fileReplaceHeader, sizeof(fileReplaceHeader));
	if (FAILED(hr))
	{
		::DeleteFile(strBackup);
		return hr;
	}



	// 更新配置项目
	POSITION pos = writeList.GetStartPosition();
	for (NULL; pos; writeList.GetNext(pos))
	{
		FILE_REPLACE_ENTRY entry;
		CString t_csDestFile = writeList.GetKeyAt(pos);
		CString t_csSrcFile  = writeList.GetValueAt(pos);
		BootFillEntry(entry, t_csDestFile, t_csSrcFile);

		hr = hCfgFile.Write(&entry, sizeof(entry));
		if (FAILED(hr))
		{
			::DeleteFile(strBackup);
			return hr;
		}
	}



	hCfgFile.Close();
	::DeleteFile(szSystemDriver);
	BOOL bRet = ::MoveFile(strBackup, szSystemDriver);
	if (!bRet)
	{
		::DeleteFile(strBackup);
		return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
	}


	return S_OK;
}

HRESULT CBootReplaceTool::BootLoadConfigFile(LPCWSTR lpszConfigName, CAtlMap<CString, CString>& fileList)
{
	if (!lpszConfigName || !*lpszConfigName)
		return E_INVALIDARG;

	fileList.RemoveAll();


	// 获取系统目录
	WCHAR szSystemDriver[MAX_PATH + 1];
	HRESULT hr = GetDriverPath(szSystemDriver, lpszConfigName);
	if (FAILED(hr))
		return hr;


	if (INVALID_FILE_ATTRIBUTES == ::GetFileAttributes(szSystemDriver))
		return S_FALSE;



	CString strBackup = szSystemDriver;
	strBackup.Append(L".read");


	// 复制一个副本用于读取
	::CopyFile(szSystemDriver, strBackup, FALSE);




	// 读取配置文件,退出时自动删除
	CEncryptFile hCfgFile;
	hr = hCfgFile.Create(
		strBackup,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_DELETE,      // do not share file
		OPEN_EXISTING,
		FILE_FLAG_DELETE_ON_CLOSE);
	if (FAILED(hr))
		return S_FALSE;




	FILE_REPLACE_HEADER fileReplaceHeader;
	memset(&fileReplaceHeader, 0, sizeof(fileReplaceHeader));



	ULONGLONG uFileSize = 0;
	hr = hCfgFile.GetSize(uFileSize);
	if (FAILED(hr))
		return S_FALSE;


	hr = hCfgFile.Read(&fileReplaceHeader, sizeof(fileReplaceHeader));
	if (FAILED(hr))
		return S_FALSE;


	if (uFileSize < sizeof(FILE_REPLACE_HEADER))
		return S_FALSE;



	// 检查文件大小和信息头中的个数是否吻合
	// 同时限制entry数目
	ULONGLONG uRealEntryCount = (uFileSize - sizeof(FILE_REPLACE_HEADER)) / sizeof(FILE_REPLACE_HEADER);
	fileReplaceHeader.ulNumber = min(fileReplaceHeader.ulNumber, (DWORD)uRealEntryCount);
	fileReplaceHeader.ulNumber = min(fileReplaceHeader.ulNumber, BOOT_CHECK_MAX_ENTRY_LIMIT);



	for (DWORD i = 0; i < fileReplaceHeader.ulNumber; ++i)
	{
		FILE_REPLACE_ENTRY readEntry;
		CString t_csDestFile, t_csSrcFile;
		hr = hCfgFile.Read(&readEntry, sizeof(readEntry));
		if (FAILED(hr))
			return S_FALSE;

		// 强制0结束符
		readEntry.wstrFileFrom[_countof(readEntry.wstrFileFrom) - 1] = L'\0';
		readEntry.wstrFileTo[_countof(readEntry.wstrFileTo) - 1] = L'\0';

		CharLower(readEntry.wstrFileFrom);
		CharLower(readEntry.wstrFileTo);

		t_csDestFile = readEntry.wstrFileTo;
		t_csSrcFile  = readEntry.wstrFileFrom;
		fileList.SetAt(t_csDestFile, t_csSrcFile);
	}



	return S_OK;
}