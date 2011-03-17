/**
* @file    bootchecktool.h
* @brief   ...
* @author  bbcallen
* @date    2009-07-31 11:46
*/

#ifndef BOOTCHECKTOOL_H
#define BOOTCHECKTOOL_H

#include <windows.h>
#include <atlcoll.h>
#include <atlstr.h>
#include "bootcheckstruct.h"
#include "bcdef.h"

NS_SAFEAPI_BEGIN

class CBootCheckTool
{
public:

    // 每次清除前,或批量清除前调用此函数确认驱动已安装
    // 调用会判断驱动是否已安装
    // 安装前会检查数字签名,并判断要安装驱动的版本是否比旧驱动新
    HRESULT InstallDriver(LPCWSTR lpszDriverPath);

    HRESULT UninstallDriver();

    HRESULT BootDeleteFile(LPCWSTR lpszFilePath);

    HRESULT BootOccupyFile(LPCWSTR lpszFilePath);


    HRESULT BootLoadOccupyFile(CAtlMap<CString, DWORD>& fileList);


    HRESULT         CopyDriverIfNew(LPCWSTR lpszNewDriver);

    ULARGE_INTEGER  GetDllVersion(LPCWSTR lpszFilePath);

    // 删除bootclean配置文件
    HRESULT         RemoveConfigFile();


public:

    HRESULT GetDriverPath(WCHAR szSystemDriver[MAX_PATH + 1], LPCWSTR lpszConfigName);

    HRESULT BootFillEntry(FILE_CLEAN_ENTRY& entry, LPCWSTR lpszFilePath);

    HRESULT BootAddToConfigFile(LPCWSTR lpszConfigName, LPCWSTR lpszFilePath);

    HRESULT BootRemoveFromConfigFile(LPCWSTR lpszConfigName, CAtlMap<CString, DWORD>& removeList);

    HRESULT BootSaveConfigFile(LPCWSTR lpszConfigName, CAtlMap<CString, DWORD>& writeList);

    HRESULT BootLoadConfigFile(LPCWSTR lpszConfigName, CAtlMap<CString, DWORD>& fileList);
};

NS_SAFEAPI_END

#endif//BOOTCHECKTOOL_H