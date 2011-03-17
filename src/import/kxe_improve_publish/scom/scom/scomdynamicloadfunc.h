/********************************************************************
* CreatedOn: 2006-9-5   16:49
* FileName: SCOMDynamicLoadFunc.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/
#ifndef SCOMDYNAMICLOADFUNC_H
#define SCOMDYNAMICLOADFUNC_H

#ifndef SCOM_USE_STATIC_LOADING
#include "SCOMAPI.h"
#include "SCOMDynamicLoader.h"
//获取SCOM库版本
inline unsigned long  __stdcall KSCoBuildVersion(void)
{
	return GlobalSCOMLoader::Instance().KSCoBuildVersion();
}

//创建组件
inline KSRESULT __stdcall KSCoCreateInstance(
	const KSCLSID& clsid,
	const KSIID &riid , 
	void **ppv)
{
	return GlobalSCOMLoader::Instance().KSCoCreateInstance(clsid, riid, ppv);
}
//SCOM库初始化
inline KSRESULT __stdcall KSCoInitialize(void *pvReserved)
{
	return GlobalSCOMLoader::Instance().KSCoInitialize(pvReserved);
}

//SCOM库反初始化,强行卸载所有的组件
inline KSRESULT __stdcall KSCoUninitialize(void)
{
	return GlobalSCOMLoader::Instance().KSCoUninitialize();
}

//返回是否可以卸载所有Dll,当没有任何组件对象被使用,则返回成功
inline KSRESULT __stdcall KSCanUnLoadAllLibNow(void)
{
	return GlobalSCOMLoader::Instance().KSCanUnLoadAllLibNow();
}

//卸载指定组件所依附的Dll
inline KSRESULT __stdcall KSFreeLib(const KSCLSID& clsid)
{
	return GlobalSCOMLoader::Instance().KSFreeLib(clsid);
}

//卸载指定路径的Dll
inline KSRESULT __stdcall KSFreeLibEx(const char *pszModulePath)
{
	return GlobalSCOMLoader::Instance().KSFreeLibEx(pszModulePath);
}

//卸载没有在使用的DLL(当前使用的组件数为0的Dll),并返回还没有被卸载的DLL数目
inline KSRESULT __stdcall KSFreeUnusedLib(void)
{
	return GlobalSCOMLoader::Instance().KSFreeUnusedLib();
}

//注册组件,可以一次注册DLL里的一个或几个组件
inline KSRESULT __stdcall KSRegisterServer(
	const CLASSINFO *ClassInfo, 
	int nSize, 
	const char *pszModulePath)
{
	return GlobalSCOMLoader::Instance().KSRegisterServer(
		ClassInfo, 
		nSize, 
		pszModulePath);
}

//反注册组件,可以对一个或多个组件进行反注册
inline KSRESULT __stdcall KSUnRegisterServer(const KSCLSID *clsids, int nSize)
{
	return GlobalSCOMLoader::Instance().KSUnRegisterServer(clsids, nSize);
}

//注册组件,可以一次注册一个Dll里面的所有组件
inline KSRESULT __stdcall KSRegisterServerEx(const char *pszModulePath)
{
	return GlobalSCOMLoader::Instance().KSRegisterServerEx(pszModulePath);
}

//反注册组件,可以一次反注册一个Dll里面的所有组件
inline KSRESULT __stdcall KSUnRegisterServerEx(const char *pszModulePath)
{
	return GlobalSCOMLoader::Instance().KSUnRegisterServerEx(pszModulePath);
}

//提供统一的内存分配
inline void* __stdcall KSCoTaskMemAlloc(size_t uSize)
{
	return GlobalSCOMLoader::Instance().KSCoTaskMemAlloc(uSize);
}

//提供统一的内存回收 
inline void __stdcall KSCoTaskMemFree(void *pv, size_t uSize)
{
	GlobalSCOMLoader::Instance().KSCoTaskMemFree(pv, uSize);
}

//创建空的配置文件
inline KSRESULT __stdcall KSInitConfigData(void)
{
	return GlobalSCOMLoader::Instance().KSInitConfigData();
}
//获取内存管理器接口
inline KSRESULT __stdcall KSCoGetMalloc(ISCOMMalloc **ppSimpleMalloc)
{
	return GlobalSCOMLoader::Instance().KSCoGetMalloc(ppSimpleMalloc);
}

//获取组件所在的dll路径
inline KSRESULT __stdcall KSCoGetLibraryPath(const KSCLSID& clsid, char* szModulePath, size_t& uSize)
{
	return GlobalSCOMLoader::Instance().KSCoGetLibraryPath(clsid, szModulePath, uSize);
}

//加载SCOM组件dll,只能加载配置文件中没有出现的文件
inline KSRESULT __stdcall KSCoLoadLibrary(const char *pszModulePath)
{
	return GlobalSCOMLoader::Instance().KSCoLoadLibrary(pszModulePath);
}

//重新加载配置文件
inline KSRESULT __stdcall KSReLoadConfigure(void)
{
	return GlobalSCOMLoader::Instance().KSReLoadConfigure();
}

//从组件ID获取组件的友好名称,uSize将会输出需要的buffer的大小
inline KSRESULT __stdcall KSCoProgIDFromCLSID(const KSCLSID& clsid, 
											  char* pszProgID,
											  size_t& uSize)
{
	return GlobalSCOMLoader::Instance().KSCoProgIDFromCLSID(clsid, pszProgID, uSize);
}

inline KSRESULT __stdcall KSGetPropertyCLSID(DWORD dwProperty, KSCLSID* pCLSIDs, size_t* psizeCLSID)
{
	return GlobalSCOMLoader::Instance().KSGetPropertyCLSID(dwProperty, pCLSIDs, psizeCLSID);
}

inline KSRESULT __stdcall KSSetCLSIDProperty(const KSCLSID* pCLSID, DWORD dwProperty)
{
	return GlobalSCOMLoader::Instance().KSSetCLSIDProperty(pCLSID, dwProperty);
}

inline KSRESULT __stdcall KSCoCLSIDFromProgID(const char* pszProgID, KSCLSID* pclsid)
{
	return GlobalSCOMLoader::Instance().KSCoCLSIDFromProgID(pszProgID, pclsid);
}


inline KSRESULT __stdcall KSFreeLibExW(const wchar_t* pszModulePath)
{
	return GlobalSCOMLoader::Instance().KSFreeLibExW(pszModulePath);
}


inline KSRESULT __stdcall KSRegisterServerW(const CLASSINFO *pClassInfo,
									 int nSize, 
									 const wchar_t *pszModulePath)
{
	return GlobalSCOMLoader::Instance().KSRegisterServerW(pClassInfo, nSize, pszModulePath);
}

inline KSRESULT __stdcall KSRegisterServerExW(const wchar_t *pszModulePath)
{
	return GlobalSCOMLoader::Instance().KSRegisterServerExW(pszModulePath);
}

inline KSRESULT __stdcall KSUnRegisterServerExW(const wchar_t *pszModulePath)
{
	return GlobalSCOMLoader::Instance().KSUnRegisterServerExW(pszModulePath);
}

inline KSRESULT __stdcall KSCoGetLibraryPathW(const KSCLSID& clsid, 
									   wchar_t* szModulePath, 
									   size_t& uSize)
{
	return GlobalSCOMLoader::Instance().KSCoGetLibraryPathW(clsid, szModulePath, uSize);
}

inline KSRESULT __stdcall KSCoProgIDFromCLSIDW(const KSCLSID& clsid, 
										wchar_t* pszProgID, 
										size_t& uSize)
{
	return GlobalSCOMLoader::Instance().KSCoProgIDFromCLSIDW(clsid, pszProgID, uSize);
}

inline KSRESULT __stdcall KSCoCLSIDFromProgIDW(const wchar_t* pszProgID, KSCLSID* pclsid)
{
	return GlobalSCOMLoader::Instance().KSCoCLSIDFromProgIDW(pszProgID, pclsid);
}

inline KSRESULT __stdcall KSCoLoadLibraryW(const wchar_t* pszModulePath)
{
	return GlobalSCOMLoader::Instance().KSCoLoadLibraryW(pszModulePath);
}
#endif

#endif //SCOMDYNAMICLOADFUNC_H