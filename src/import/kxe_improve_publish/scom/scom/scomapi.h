/********************************************************************
* CreatedOn: 2005-10-24   11:58
* FileName: SCOMAPI.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/
#ifndef SCOMAPI_H
#define SCOMAPI_H

#include "SCOMDef.h"
#include "ISCOMMalloc.h"

//获取SCOM库版本
extern "C"
{
	unsigned long  __stdcall KSCoBuildVersion(void);

	//创建组件
	KSRESULT __stdcall KSCoCreateInstance(const KSCLSID& clsid,
		const KSIID &riid ,
		void **ppv);

	//SCOM库初始化
	KSRESULT __stdcall KSCoInitialize(void *pvReserved = NULL);

	//SCOM库反初始化,强行卸载所有的组件
	KSRESULT __stdcall KSCoUninitialize(void);

	//卸载指定组件所依附的Dll
	KSRESULT __stdcall KSFreeLib(const KSCLSID& clsid);

	//卸载指定路径的Dll
	KSRESULT __stdcall KSFreeLibEx(const char *pszModulePath);

	//卸载没有在使用的DLL(当前使用的组件数为0的Dll)
	KSRESULT __stdcall KSFreeUnusedLib(void);

	//返回是否可以卸载所有Dll,当没有任何组件对象被使用,则返回成功
	KSRESULT __stdcall KSCanUnLoadAllLibNow(void);

	//注册组件,可以一次注册DLL里的一个或几个组件
	KSRESULT __stdcall KSRegisterServer(const CLASSINFO *ClassInfo,
		int nSize, 
		const char *pszModulePath);

	//反注册组件,可以对一个或多个组件进行反注册
	KSRESULT __stdcall KSUnRegisterServer(const KSCLSID *clsids, int nSize);

	//注册组件,可以一次注册一个Dll里面的所有组件
	KSRESULT __stdcall KSRegisterServerEx(const char *pszModulePath);

	//反注册组件,可以一次反注册一个Dll里面的所有组件
	KSRESULT __stdcall KSUnRegisterServerEx(const char *pszModulePath);

	//提供统一的内存分配
	void* __stdcall KSCoTaskMemAlloc(size_t uSize);

	//提供统一的内存回收 
	void __stdcall KSCoTaskMemFree(void *pv,  size_t uSize);

	//获取内存管理器接口
	KSRESULT __stdcall KSCoGetMalloc(ISCOMMalloc **ppISCOMMalloc);

	//获取组件所在的dll路径
	KSRESULT __stdcall KSCoGetLibraryPath(const KSCLSID& clsid, 
		char* szModulePath, 
		size_t& uSize
		);
	//将当前目录下的SCOM.dll（如果存在）的路径写入注册表中
	KSRESULT __stdcall KSInitConfigData(void);

	//加载SCOM组件dll,只能加载配置文件中没有出现的文件
	KSRESULT __stdcall KSCoLoadLibrary(const char *pszModulePath);

	//重新装载配置文件
	KSRESULT __stdcall KSReLoadConfigure(void);

	//从组件ID获取组件的友好名称,uSize将会输出需要的buffer的大小
	KSRESULT __stdcall KSCoProgIDFromCLSID(const KSCLSID& clsid, 
		char* pszProgID, 
		size_t& uSize
		);

	KSRESULT __stdcall KSGetPropertyCLSID(DWORD dwProperty, KSCLSID* pCLSIDs, size_t* psizeCLSID);
	KSRESULT __stdcall KSSetCLSIDProperty(const KSCLSID* pCLSID, DWORD dwProperty);



	//从组件友好名称获取组件id
	KSRESULT __stdcall KSCoCLSIDFromProgID(const char* pszProgID, KSCLSID* pclsid);
	
	//卸载指定路径的Dll
	KSRESULT __stdcall KSFreeLibExW(const wchar_t* pszModulePath);

	KSRESULT __stdcall KSRegisterServerW(const CLASSINFO *pClassInfo,
		int nSize, 
		const wchar_t *pszModulePath);

	KSRESULT __stdcall KSRegisterServerExW(const wchar_t *pszModulePath);

	//反注册组件,可以一次反注册一个Dll里面的所有组件
	KSRESULT __stdcall KSUnRegisterServerExW(const wchar_t *pszModulePath);

	KSRESULT __stdcall KSCoGetLibraryPathW(const KSCLSID& clsid, 
		wchar_t* szModulePath, 
		size_t& uSize
		);

	//从组件ID获取组件的友好名称,uSize将会输出需要的buffer的大小
	KSRESULT __stdcall KSCoProgIDFromCLSIDW(const KSCLSID& clsid, 
		wchar_t* pszProgID, 
		size_t& uSize
		);

	KSRESULT __stdcall KSCoCLSIDFromProgIDW(const wchar_t* pszProgID, KSCLSID* pclsid);

	KSRESULT __stdcall KSCoLoadLibraryW(const wchar_t* pszModulePath);


};

#endif //SCOMAPI_H