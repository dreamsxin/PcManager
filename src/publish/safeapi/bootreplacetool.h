/**
* @file    bootreplacetool.h
* @brief   ...
* @author  zhangjun
* @date    2009-10-16 14:00
*/

#ifndef BOOTREPLACETOOL_H
#define BOOTREPLACETOOL_H

#include <windows.h>
#include <atlcoll.h>
#include <atlstr.h>
#include "safeapi\bootcheckstruct.h"
#include "safeapi\bcdef.h"

NS_SAFEAPI_BEGIN

class CBootReplaceTool
{
public:

	// 每次替换前,或批量替换前调用此函数确认驱动已安装
	// 调用会判断驱动是否已安装
	// 安装前会检查数字签名,并判断要安装驱动的版本是否比旧驱动新
	//HRESULT InstallDriver(LPCWSTR lpszDriverPath);

	//HRESULT UninstallDriver();

	HRESULT BootReplaceFile(LPCWSTR lpszDestFilePath, LPCWSTR lpszSrcFilePath);


	//HRESULT         CopyDriverIfNew(LPCWSTR lpszNewDriver);

	//ULARGE_INTEGER  GetDllVersion(LPCWSTR lpszFilePath);

	// 删除bootclean配置文件
	//HRESULT         RemoveConfigFile();


public:

	HRESULT GetDriverPath(WCHAR szSystemDriver[MAX_PATH + 1], LPCWSTR lpszConfigName);

	HRESULT BootFillEntry(FILE_REPLACE_ENTRY& entry, LPCWSTR lpszDestFilePath, LPCWSTR lpszSrcFilePath);

	HRESULT BootAddToConfigFile(LPCWSTR lpszConfigName, LPCWSTR lpszDestFilePath, LPCWSTR lpszSrcFilePath);

	// CAtlMap<CString, CString>对应关系<destFilePath, srcFilePath>,确定destFilePath为key
	HRESULT BootRemoveFromConfigFile(LPCWSTR lpszConfigName, CAtlMap<CString, CString>& removeList);

	HRESULT BootSaveConfigFile(LPCWSTR lpszConfigName, CAtlMap<CString, CString>& writeList);

	HRESULT BootLoadConfigFile(LPCWSTR lpszConfigName, CAtlMap<CString, CString>& fileList);
};

NS_SAFEAPI_END

#endif//BOOTREPLACETOOL_H