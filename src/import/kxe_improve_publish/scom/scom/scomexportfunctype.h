/********************************************************************
* CreatedOn: 2006-9-1   10:20
* FileName: SCOMExportFuncType.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/
#ifndef SCOMEXPORTFUNCTYPE_H
#define SCOMEXPORTFUNCTYPE_H
#include "SCOMDef.h"
#include "ISCOMMalloc.h"
#include "../../kxecommon/kxecommon.h"

typedef unsigned long  (__stdcall *pfnKSCoBuildVersionType)(void);

//创建组件

typedef KSRESULT (__stdcall *pfnKSCoCreateInstanceType)(
									  const KSCLSID& clsid,
									  const KSIID &riid ,
									  void **ppv);
//SCOM库初始化

typedef KSRESULT (__stdcall *pfnKSCoInitializeType)(void *pvReserved);

//SCOM库反初始化,强行卸载所有的组件

typedef KSRESULT (__stdcall *pfnKSCoUninitializeType)(void);

//卸载指定组件所依附的Dll

typedef KSRESULT (__stdcall *pfnKSFreeLibType)(const KSCLSID& clsid);

//卸载指定路径的Dll

typedef KSRESULT (__stdcall *pfnKSFreeLibExType)(const char *pszModulePath);

//卸载没有在使用的DLL(当前使用的组件数为0的Dll)
 
typedef KSRESULT (__stdcall *pfnKSFreeUnusedLibType)(void);

//返回是否可以卸载所有Dll,当没有任何组件对象被使用,则返回成功

typedef KSRESULT (__stdcall *pfnKSCanUnLoadAllLibNowType)(void);

//注册组件,可以一次注册DLL里的一个或几个组件
 
typedef KSRESULT (__stdcall *pfnKSRegisterServerType)(const CLASSINFO *ClassInfo,
									int nSize, 
									const char *pszModulePath);

//反注册组件,可以对一个或多个组件进行反注册
 
typedef KSRESULT (__stdcall *pfnKSUnRegisterServerType)(const KSCLSID *clsids, int nSize);

//注册组件,可以一次注册一个Dll里面的所有组件

typedef KSRESULT (__stdcall *pfnKSRegisterServerExType)(const char *pszModulePath);

//反注册组件,可以一次反注册一个Dll里面的所有组件

typedef KSRESULT (__stdcall *pfnKSUnRegisterServerExType)(const char *pszModulePath);

//提供统一的内存分配

typedef void* (__stdcall *pfnKSCoTaskMemAllocType)(size_t uSize);

//提供统一的内存回收 

typedef void (__stdcall *pfnKSCoTaskMemFreeType)(void *pv,  size_t uSize);

//获取内存管理器接口

typedef KSRESULT (__stdcall *pfnKSCoGetMallocType)(ISCOMMalloc **ppISCOMMalloc);

//获取组件所在的dll路径
 
typedef KSRESULT (__stdcall *pfnKSCoGetLibraryPathType)(const KSCLSID& clsid, 
									  char* szModulePath, 
									  size_t& uSize
									  );

//创建空的配置文件

typedef KSRESULT (__stdcall *pfnKSInitConfigDataType)(void);

//加载SCOM组件dll,只能加载配置文件中没有出现的文件

typedef KSRESULT (__stdcall *pfnKSCoLoadLibraryType)(const char *pszModulePath);

//重新装载配置文件

typedef KSRESULT (__stdcall *pfnKSReLoadConfigureType)(void);


//从组件ID获取组件的友好名称,uSize将会输出需要的buffer的大小
typedef KSRESULT (__stdcall *pfnKSCoProgIDFromCLSIDType)(const KSCLSID& clsid, 
									   char* pszProgID, 
									   size_t& uSize
									   );

typedef KSRESULT (__stdcall *pfnKSGetPropertyCLSIDType)(DWORD dwProperty, 
														KSCLSID* pCLSIDs, 
														size_t* psizeCLSID
														);
	
typedef KSRESULT (__stdcall *pfnKSSetCLSIDProperty)(const KSCLSID* pCLSID, DWORD dwProperty);

typedef KSRESULT (__stdcall *pfnKSCoCLSIDFromProgIDType)(const char* pszProgID, KSCLSID* pclsid);

typedef KSRESULT (__stdcall *pfnKSFreeLibExWType)(const wchar_t* pszModulePath);

typedef KSRESULT (__stdcall *pfnKSRegisterServerWType)(const CLASSINFO *pClassInfo,
									 int nSize, 
									 const wchar_t *pszModulePath);

typedef KSRESULT (__stdcall *pfnKSRegisterServerExWType)(const wchar_t *pszModulePath);

typedef KSRESULT (__stdcall *pfnKSUnRegisterServerExWType)(const wchar_t *pszModulePath);

typedef KSRESULT (__stdcall *pfnKSCoGetLibraryPathWType)(const KSCLSID& clsid, 
									   wchar_t* szModulePath, 
									   size_t& uSize
									   );

typedef KSRESULT (__stdcall *pfnKSCoProgIDFromCLSIDWType)(const KSCLSID& clsid, 
										wchar_t* pszProgID, 
										size_t& uSize
										);

typedef KSRESULT (__stdcall *pfnKSCoCLSIDFromProgIDWType)(const wchar_t* pszProgID, KSCLSID* pclsid);

typedef KSRESULT (__stdcall *pfnKSCoLoadLibraryWType)(const wchar_t* pszModulePath);

#endif //SCOMEXPORTFUNCTYPE_H