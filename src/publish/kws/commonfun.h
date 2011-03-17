/* -------------------------------------------------------------------------
// 文件名		:	common/commonfun.h
// 创建人		:	chenghui
// 创建时间		:	2009-4-1 13:47:35
// 功能描述     :	
//
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __COMMON_COMMONFUN_H__
#define __COMMON_COMMONFUN_H__

#include <direct.h>
#include <Shlobj.h>
#include <string>
using std::wstring;

// -------------------------------------------------------------------------
void MakeDirEx(LPCSTR szPathDir);
void MakeDirEx(LPCWSTR szPathDir);

// 获取AllUser用户目录相关
BOOL GetAllUserPath(WCHAR* strPath, size_t nSize);
BOOL GetAllUserPathA(CHAR* strPath, size_t nSize);

// 获取AllUser下KWS的目录
BOOL GetAllUserKWSPath(WCHAR* strPath, size_t nSize);
BOOL GetAllUserKWSPathA(CHAR* strPath, size_t nSize);

// 获取ALLUser下KCLT的目录
BOOL GetAllUserKISKCLTPathA(CHAR* strPath, size_t nSize);
BOOL GetAllUserKISKCLTPathW(WCHAR* strPath, size_t nSize);

// 获取AllUser下KWS的Log目录
BOOL GetAllUserKWSLogPath(WCHAR* strPath, size_t nSize);
namespace KWS
{
BOOL IsFileExist(LPCWSTR strFile);
}
BOOL IsFileExistA(LPCSTR strFile);

BOOL IsDirExist(LPCWSTR strDir);
BOOL IsDirExistA(LPCSTR strDir);

// 是否安装了急救箱
namespace KWS
{
	BOOL IsKsmExist();
}

// 获取当前程序所在目录
wstring GetModuleDirW();

// 获取快速启动目录
wstring GetQuickLaunchDir();

/// 重启后移动标识是否在注册表内
bool IsMoveFileExInfoInReg();

//获取用户UUID,标准的{00000000-0000-0000-0000-000000000000}格式,该函数会缓存以前的值
BOOL    GetKwsUserUUID(wstring& wstrUserUUID);

//在HKEY_CLASSES_ROOT的APPID及CLSID键下新建数据,默认浏览器锁定相关
BOOL    RegWriteKwsDefaultBrowserPath(const wstring& wstrKwsDefaultBrowserPath);

// 取得当前系统时间
std::wstring GetSysDateString();

BOOL	GetProIDByName(LPCTSTR szProName, DWORD & dwPID);

BOOL    RegSetStartPage(PCWCH pcwszUrl, HKEY hKey);

BOOL    CheckLockFlag();

BOOL    SetLockFlag(PCWCH pcwszUrl, BOOL bLocked);
// -------------------------------------------------------------------------
// $Log: $

#endif /* __COMMON_COMMONFUN_H__ */
