/********************************************************************
* CreatedOn: 2006-9-1   10:33
* FileName: SCOMDynamicLoader.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/
#ifndef SCOMDYNAMICLOADER_H
#define SCOMDYNAMICLOADER_H

#include "Singleton.h"
#include "SCOMExportFuncType.h"
#include "KLoadDllUtility.h"
#define KAN_SETUP_COMMON_IN_PROGRAM_FILES_DIR TEXT("Kingsoft\\Common\\KSCE")

class SCOMDynamicLoader;

typedef SingletonHolder<SCOMDynamicLoader> GlobalSCOMLoader;

class SCOMDynamicLoader
{
public:
	SCOMDynamicLoader()
	{
		m_bSCOMSelfLoaded	= false;
		m_handle			= NULL;
		ClearFunction();
		
		HMODULE	hModule = NULL;
		hModule = ::GetModuleHandle(_TEXT(KXECOMMON_CONFIG_SCOM_DLL_NAME));
		if (hModule)
		{
			if (0 == GetFunction(hModule))
				m_handle = hModule;
			else
				ClearFunction();
		}
	}

	~SCOMDynamicLoader()
	{
		if (m_handle)
			UnLoad();
	}
	
	unsigned long  __stdcall KSCoBuildVersion(void)//这个函数可以在KSCoInitialize运行前执行
	{
		long lversion = -1;
		if (m_handle)
		{
			assert(m_pfnKSCoBuildVersion);
			lversion = m_pfnKSCoBuildVersion ? m_pfnKSCoBuildVersion() : -1;
		}
		else
		{
			KSRESULT ksResult = Load();//函数执行后还原原来的状态
			if (m_handle)
			{
				assert(m_pfnKSCoBuildVersion);
				lversion = m_pfnKSCoBuildVersion ? m_pfnKSCoBuildVersion() : -1;
			}

			if (S_OK == ksResult && m_bSCOMSelfLoaded)
				UnLoad();
		}
		return lversion;
	}

	//创建组件

	KSRESULT __stdcall KSCoCreateInstance(
		const KSCLSID& clsid,
		const KSIID &riid , 
		void **ppv)
	{
		assert(m_pfnKSCoCreateInstance);
		return (m_handle && m_pfnKSCoCreateInstance) ? m_pfnKSCoCreateInstance(clsid, riid, ppv) : -1;
	}
	//SCOM库初始化

	KSRESULT __stdcall KSCoInitialize(void* pvReserved = NULL)
	{
		if (NULL == m_handle)
			Load();

		assert(m_pfnKSCoInitialize);
		return (m_handle && m_pfnKSCoInitialize) ? m_pfnKSCoInitialize(pvReserved) : -1;
	}

	//SCOM库反初始化,强行卸载所有的组件
	KSRESULT __stdcall KSCoUninitialize(void)
	{
		assert(m_pfnKSCoUninitialize);
		KSRESULT ks = -1;
		if (m_handle && m_pfnKSCoUninitialize)
		{
			ks = m_pfnKSCoUninitialize();
			if (ks == S_OK && m_bSCOMSelfLoaded)
				UnLoad();
		}
		return ks;
	}

	//返回是否可以卸载所有Dll,当没有任何组件对象被使用,则返回成功

	KSRESULT __stdcall KSCanUnLoadAllLibNow(void)
	{
		assert(m_pfnKSCanUnLoadAllLibNow);
		return (m_handle && m_pfnKSCanUnLoadAllLibNow) ? m_pfnKSCanUnLoadAllLibNow() : -1;
	}

	//卸载指定组件所依附的Dll

	KSRESULT __stdcall KSFreeLib(const KSCLSID& clsid)
	{
		assert(m_pfnKSFreeLib);
		return (m_handle && m_pfnKSFreeLib) ? m_pfnKSFreeLib(clsid) : -1;
	}

	//卸载指定路径的Dll

	KSRESULT __stdcall KSFreeLibEx(const char *pszModulePath)
	{
		assert(m_pfnKSFreeLibEx);
		return (m_handle && m_pfnKSFreeLibEx) ? m_pfnKSFreeLibEx(pszModulePath) : -1;
	}

	//卸载没有在使用的DLL(当前使用的组件数为0的Dll),并返回还没有被卸载的DLL数目

	KSRESULT __stdcall KSFreeUnusedLib(void)
	{
		assert(m_pfnKSFreeUnusedLib);
		return (m_handle && m_pfnKSFreeUnusedLib) ? m_pfnKSFreeUnusedLib() : -1;
	}

	//注册组件,可以一次注册DLL里的一个或几个组件

	KSRESULT __stdcall KSRegisterServer(
		const CLASSINFO *ClassInfo, 
		int nSize, 
		const char *pszModulePath)
	{
		assert(m_pfnKSRegisterServer);
		return (m_handle && m_pfnKSRegisterServer) ? m_pfnKSRegisterServer(
			ClassInfo, 
			nSize, 
			pszModulePath) : -1;
	}

	//反注册组件,可以对一个或多个组件进行反注册

	KSRESULT __stdcall KSUnRegisterServer(const KSCLSID *clsids, int nSize)
	{
		assert(m_pfnKSUnRegisterServer);
		return (m_handle && m_pfnKSUnRegisterServer) ? m_pfnKSUnRegisterServer(clsids, nSize) : -1;
	}

	//注册组件,可以一次注册一个Dll里面的所有组件

	KSRESULT __stdcall KSRegisterServerEx(const char *pszModulePath)
	{
		assert(m_pfnKSRegisterServerEx);
		return (m_handle && m_pfnKSRegisterServerEx) ? m_pfnKSRegisterServerEx(pszModulePath) : -1;
	}

	//反注册组件,可以一次反注册一个Dll里面的所有组件

	KSRESULT __stdcall KSUnRegisterServerEx(const char *pszModulePath)
	{
		assert(m_pfnKSUnRegisterServerEx);
		return (m_handle && m_pfnKSUnRegisterServerEx) ? m_pfnKSUnRegisterServerEx(pszModulePath) : -1;
	}

	//提供统一的内存分配

	void* __stdcall KSCoTaskMemAlloc(size_t uSize)
	{
		assert(m_pfnKSCoTaskMemAlloc);
		return (m_handle && m_pfnKSCoTaskMemAlloc) ? m_pfnKSCoTaskMemAlloc(uSize) : NULL;
	}

	//提供统一的内存回收 

	void __stdcall KSCoTaskMemFree(void *pv, size_t uSize)
	{
		assert(m_pfnKSCoTaskMemFree);
		if (m_handle && m_pfnKSCoTaskMemFree)
			 m_pfnKSCoTaskMemFree(pv, uSize);
	}

	//初始化SCOM的配置信息
	//这个函数可以在KSCoInitialize运行前执行
	KSRESULT __stdcall KSInitConfigData()
	{
		KSRESULT ks = E_FAIL;
	
		if (m_handle)
		{
			assert(m_pfnKSInitConfigData);
			if (m_pfnKSInitConfigData)
				ks = m_pfnKSInitConfigData();
		}
		else
		{
			KSRESULT ksResult = Load();

			if (m_handle && m_pfnKSInitConfigData)
				ks = m_pfnKSInitConfigData();

			if (S_OK == ksResult && m_bSCOMSelfLoaded)
				UnLoad();
		}
		return ks;
	}

	//获取内存管理器接口
	KSRESULT __stdcall KSCoGetMalloc(ISCOMMalloc **ppSimpleMalloc)
	{
		assert(m_pfnKSCoGetMalloc);
		return (m_handle && m_pfnKSCoGetMalloc) ? m_pfnKSCoGetMalloc(ppSimpleMalloc) : -1;
	}

	//获取组件所在的dll路径

	KSRESULT __stdcall KSCoGetLibraryPath(const KSCLSID& clsid, char* szModulePath, size_t& uSize)
	{
		assert(m_pfnKSCoGetLibraryPath);
		return (m_handle && m_pfnKSCoGetLibraryPath) ? m_pfnKSCoGetLibraryPath(clsid, szModulePath, uSize) : -1;
	}

	//加载SCOM组件dll,只能加载配置文件中没有出现的文件

	KSRESULT __stdcall KSCoLoadLibrary(const char *pszModulePath)
	{
		assert(m_pfnKSCoLoadLibrary);
		return (m_handle && m_pfnKSCoLoadLibrary) ? m_pfnKSCoLoadLibrary(pszModulePath) : -1;
	}

	//重新加载配置文件

	KSRESULT __stdcall KSReLoadConfigure(void)
	{
		assert(m_pfnKSReLoadConfigure);
		return (m_handle && m_pfnKSReLoadConfigure) ? m_pfnKSReLoadConfigure() : -1;
	}

	//从组件ID获取组件的友好名称,uSize将会输出需要的buffer的大小
	KSRESULT __stdcall KSCoProgIDFromCLSID(const KSCLSID& clsid, 
		char* pszProgID,
		size_t& uSize)
	{
		assert(m_pfnKSCoProgIDFromCLSID);
		return (m_handle && m_pfnKSCoProgIDFromCLSID) ? m_pfnKSCoProgIDFromCLSID(clsid, pszProgID, uSize) : -1;
	}


	KSRESULT __stdcall KSGetPropertyCLSID(DWORD dwProperty, KSCLSID* pCLSIDs, size_t* psizeCLSID)
	{
		assert(m_pfnKSGetPropertyCLSID);
		return (m_handle && m_pfnKSGetPropertyCLSID) ? m_pfnKSGetPropertyCLSID(dwProperty, pCLSIDs, psizeCLSID) : -1;
	}

	KSRESULT __stdcall KSSetCLSIDProperty(const KSCLSID* pCLSID, DWORD dwProperty)
	{
		assert(m_pfnKSSetCLSIDProperty);
		return (m_handle && m_pfnKSSetCLSIDProperty) ? m_pfnKSSetCLSIDProperty(pCLSID, dwProperty) : -1;
	}



	KSRESULT __stdcall KSCoCLSIDFromProgID(const char* pszProgID, KSCLSID* pclsid)
	{
		assert(m_pfnKSCoCLSIDFromProgID);
		return (m_handle && m_pfnKSCoCLSIDFromProgID) ? m_pfnKSCoCLSIDFromProgID(pszProgID, pclsid) : -1;
	}

	
	KSRESULT __stdcall KSFreeLibExW(const wchar_t* pszModulePath)
	{
		assert(m_pfnKSFreeLibExW);
		return (m_handle && m_pfnKSFreeLibExW) ? m_pfnKSFreeLibExW(pszModulePath) : -1;
	}

	KSRESULT __stdcall KSRegisterServerW(const CLASSINFO *pClassInfo,
		int nSize, 
		const wchar_t *pszModulePath)
	{
		assert(m_pfnKSRegisterServerW);
		return (m_handle && m_pfnKSRegisterServerW) ? m_pfnKSRegisterServerW(pClassInfo, nSize, pszModulePath) : -1;
	}

	KSRESULT __stdcall KSRegisterServerExW(const wchar_t *pszModulePath)
	{
		assert(m_pfnKSRegisterServerExW);
		return (m_handle && m_pfnKSRegisterServerExW) ? m_pfnKSRegisterServerExW(pszModulePath) : -1;
	}


	KSRESULT __stdcall KSUnRegisterServerExW(const wchar_t *pszModulePath)
	{
		assert(m_pfnKSUnRegisterServerExW);
		return (m_handle && m_pfnKSUnRegisterServerExW) ? m_pfnKSUnRegisterServerExW(pszModulePath) : -1;
	}

	KSRESULT __stdcall KSCoGetLibraryPathW(const KSCLSID& clsid, 
		wchar_t* szModulePath, 
		size_t& uSize)
	{
		assert(m_pfnKSCoGetLibraryPathW);
		return (m_handle && m_pfnKSCoGetLibraryPathW) ? m_pfnKSCoGetLibraryPathW(clsid, szModulePath, uSize) : -1;
	}

	KSRESULT __stdcall KSCoProgIDFromCLSIDW(const KSCLSID& clsid, 
		wchar_t* pszProgID, 
		size_t& uSize)
	{
		assert(m_pfnKSCoProgIDFromCLSIDW);
		return (m_handle && m_pfnKSCoProgIDFromCLSIDW) ? m_pfnKSCoProgIDFromCLSIDW(clsid, pszProgID, uSize) : -1;
	}

	KSRESULT __stdcall KSCoCLSIDFromProgIDW(const wchar_t* pszProgID, KSCLSID* pclsid)
	{
		assert(m_pfnKSCoCLSIDFromProgIDW);
		return (m_handle && m_pfnKSCoCLSIDFromProgIDW) ? m_pfnKSCoCLSIDFromProgIDW(pszProgID, pclsid) : -1;
	}

	KSRESULT __stdcall KSCoLoadLibraryW(const wchar_t* pszModulePath)
	{
		return (m_handle && m_pfnKSCoLoadLibraryW) ? m_pfnKSCoLoadLibraryW(pszModulePath) : -1;
	}

private:
	KSRESULT Load(BOOL bRegFirst = TRUE)
	{
		KSAutoLockGuard guard(m_Mutex);

		if (m_handle != NULL)
			return S_FALSE;

		int nRetCode = 0;

		HMODULE	hModule = NULL;
		hModule = ::GetModuleHandle(_TEXT(KXECOMMON_CONFIG_SCOM_DLL_NAME));
		if (NULL == hModule)
		{
			hModule = KLoadDllUtility::AutoSearchLoadLibrary(
				_TEXT(KXECOMMON_CONFIG_SCOM_DLL_NAME),
				bRegFirst
				);
			if (hModule != NULL)
				m_bSCOMSelfLoaded = true;
		}
		
		if (hModule)
		{
			if (0 != GetFunction(hModule))
			{
				ClearFunction();
				if (m_bSCOMSelfLoaded)
				{
					::FreeLibrary(hModule);
					hModule = NULL;
					m_bSCOMSelfLoaded = false;
				}
			}
		}
		else
		{
			nRetCode = -1;
		}

		if (hModule)
			m_handle = hModule;

		return S_OK;		
	}

	void UnLoad()
	{
		KSAutoLockGuard guard(m_Mutex);

		ClearFunction();

		if (m_bSCOMSelfLoaded && m_handle)	
			FreeLibrary(m_handle);

		m_handle			= NULL;
		m_bSCOMSelfLoaded	= false;
	}

	int GetFunction(HMODULE hModule)
	{
		if (!hModule)
			return -1;

		m_pfnKSCoCreateInstance		= (pfnKSCoCreateInstanceType)	::GetProcAddress(hModule, "KSCoCreateInstance");
		assert(m_pfnKSCoCreateInstance);
		if (NULL == m_pfnKSCoCreateInstance)
			return -1;

		m_pfnKSCoInitialize			= (pfnKSCoInitializeType)		::GetProcAddress(hModule, "KSCoInitialize");
		assert(m_pfnKSCoInitialize);
		if (NULL == m_pfnKSCoInitialize)
			return -1;

		m_pfnKSCoUninitialize		= (pfnKSCoUninitializeType)		::GetProcAddress(hModule, "KSCoUninitialize");
		assert(m_pfnKSCoUninitialize);
		if (NULL == m_pfnKSCoInitialize)
			return -1;

		m_pfnKSFreeLib				= (pfnKSFreeLibType)			::GetProcAddress(hModule, "KSFreeLib");
		assert(m_pfnKSFreeLib);
		m_pfnKSFreeLibEx			= (pfnKSFreeLibExType)			::GetProcAddress(hModule, "KSFreeLibEx");
		assert(m_pfnKSFreeLibEx);
		m_pfnKSFreeUnusedLib		= (pfnKSFreeUnusedLibType)		::GetProcAddress(hModule, "KSFreeUnusedLib");
		assert(m_pfnKSFreeUnusedLib);
		m_pfnKSCanUnLoadAllLibNow	= (pfnKSCanUnLoadAllLibNowType)	::GetProcAddress(hModule, "KSCanUnLoadAllLibNow");
		assert(m_pfnKSCanUnLoadAllLibNow);
		m_pfnKSRegisterServer		= (pfnKSRegisterServerType)		::GetProcAddress(hModule, "KSRegisterServer");
		assert(m_pfnKSRegisterServer);
		m_pfnKSUnRegisterServer		= (pfnKSUnRegisterServerType)	::GetProcAddress(hModule, "KSUnRegisterServer");
		assert(m_pfnKSUnRegisterServer);
		m_pfnKSRegisterServerEx		= (pfnKSRegisterServerExType)	::GetProcAddress(hModule, "KSRegisterServerEx");
		assert(m_pfnKSRegisterServerEx);
		m_pfnKSUnRegisterServerEx	= (pfnKSUnRegisterServerExType)	::GetProcAddress(hModule, "KSUnRegisterServerEx");
		assert(m_pfnKSUnRegisterServerEx);
		m_pfnKSCoTaskMemAlloc		= (pfnKSCoTaskMemAllocType)		::GetProcAddress(hModule, "KSCoTaskMemAlloc");
		assert(m_pfnKSCoTaskMemAlloc);
		m_pfnKSCoTaskMemFree		= (pfnKSCoTaskMemFreeType)		::GetProcAddress(hModule, "KSCoTaskMemFree");
		assert(m_pfnKSCoTaskMemFree);
		m_pfnKSCoGetMalloc			= (pfnKSCoGetMallocType)		::GetProcAddress(hModule, "KSCoGetMalloc");
		assert(m_pfnKSCoGetMalloc);
		m_pfnKSCoGetLibraryPath		= (pfnKSCoGetLibraryPathType)	::GetProcAddress(hModule, "KSCoGetLibraryPath");
		assert(m_pfnKSCoGetLibraryPath);
		m_pfnKSCoLoadLibrary		= (pfnKSCoLoadLibraryType)		::GetProcAddress(hModule, "KSCoLoadLibrary");
		assert(m_pfnKSCoLoadLibrary);
		//m_pfnKSReLoadConfigure		= (pfnKSReLoadConfigureType)	::GetProcAddress(m_handle, "KSReLoadConfigure");
		//assert(m_pfnKSReLoadConfigure);
		m_pfnKSCoProgIDFromCLSID	= (pfnKSCoProgIDFromCLSIDType)	::GetProcAddress(hModule, "KSCoProgIDFromCLSID");
		assert(m_pfnKSCoProgIDFromCLSID);
		m_pfnKSCoBuildVersion		= (pfnKSCoBuildVersionType)		::GetProcAddress(hModule, "KSCoBuildVersion");
		assert(m_pfnKSCoBuildVersion);
		m_pfnKSGetPropertyCLSID		= (pfnKSGetPropertyCLSIDType)	::GetProcAddress(hModule, "KSGetPropertyCLSID");
		assert(m_pfnKSGetPropertyCLSID);
		m_pfnKSSetCLSIDProperty		= (pfnKSSetCLSIDProperty)		::GetProcAddress(hModule, "KSSetCLSIDProperty");
		//assert(m_pfnKSSetCLSIDProperty);
		m_pfnKSCoCLSIDFromProgID	= (pfnKSCoCLSIDFromProgIDType)	::GetProcAddress(hModule, "KSCoCLSIDFromProgID");
		assert(m_pfnKSCoCLSIDFromProgID);
		m_pfnKSFreeLibExW			= (pfnKSFreeLibExWType)			::GetProcAddress(hModule, "KSFreeLibExW");
		assert(m_pfnKSFreeLibExW);
		m_pfnKSRegisterServerW		= (pfnKSRegisterServerWType)	::GetProcAddress(hModule, "KSRegisterServerW");
		assert(m_pfnKSRegisterServerW);
		m_pfnKSRegisterServerExW	= (pfnKSRegisterServerExWType)	::GetProcAddress(hModule, "KSRegisterServerExW");
		assert(m_pfnKSRegisterServerExW);
		m_pfnKSUnRegisterServerExW	= (pfnKSUnRegisterServerExWType)::GetProcAddress(hModule, "KSUnRegisterServerExW");
		assert(m_pfnKSUnRegisterServerExW);
		m_pfnKSCoGetLibraryPathW	= (pfnKSCoGetLibraryPathWType)	::GetProcAddress(hModule, "KSCoGetLibraryPathW");
		assert(m_pfnKSCoGetLibraryPathW);
		m_pfnKSCoProgIDFromCLSIDW	= (pfnKSCoProgIDFromCLSIDWType)	::GetProcAddress(hModule, "KSCoProgIDFromCLSIDW");
		assert(m_pfnKSCoProgIDFromCLSIDW);
		m_pfnKSCoCLSIDFromProgIDW	= (pfnKSCoCLSIDFromProgIDWType)	::GetProcAddress(hModule, "KSCoCLSIDFromProgIDW");
		assert(m_pfnKSCoCLSIDFromProgIDW);
		m_pfnKSInitConfigData		= (pfnKSInitConfigDataType)		::GetProcAddress(hModule, "KSInitConfigData");
		assert(m_pfnKSInitConfigData);
		m_pfnKSCoLoadLibraryW		= (pfnKSCoLoadLibraryWType)		::GetProcAddress(hModule, "KSCoLoadLibraryW");
		return 0;
	}

	void ClearFunction(void)
	{
		m_pfnKSCoCreateInstance		= NULL;
		m_pfnKSCoInitialize			= NULL;
		m_pfnKSCoUninitialize		= NULL;
		m_pfnKSFreeLib				= NULL;
		m_pfnKSFreeLibEx			= NULL;
		m_pfnKSFreeUnusedLib		= NULL;
		m_pfnKSCanUnLoadAllLibNow	= NULL;
		m_pfnKSRegisterServer		= NULL;
		m_pfnKSUnRegisterServer		= NULL;
		m_pfnKSRegisterServerEx		= NULL;
		m_pfnKSUnRegisterServerEx	= NULL;
		m_pfnKSCoTaskMemAlloc		= NULL;
		m_pfnKSCoTaskMemFree		= NULL;
		m_pfnKSCoGetMalloc			= NULL;
		m_pfnKSCoGetLibraryPath		= NULL;
		m_pfnKSInitConfigData		= NULL;
		m_pfnKSCoLoadLibrary		= NULL;
		m_pfnKSReLoadConfigure		= NULL;
		m_pfnKSCoProgIDFromCLSID	= NULL;
		m_pfnKSCoBuildVersion		= NULL;
		m_pfnKSGetPropertyCLSID		= NULL;
		m_pfnKSSetCLSIDProperty		= NULL;
		m_pfnKSCoCLSIDFromProgID	= NULL;
		m_pfnKSFreeLibExW			= NULL;
		m_pfnKSRegisterServerW		= NULL;
		m_pfnKSRegisterServerExW	= NULL;
		m_pfnKSUnRegisterServerExW	= NULL;
		m_pfnKSCoGetLibraryPathW	= NULL;
		m_pfnKSCoProgIDFromCLSIDW	= NULL;
		m_pfnKSCoCLSIDFromProgIDW	= NULL;
		m_pfnKSCoLoadLibraryW		= NULL;

	}

private:
	bool						m_bSCOMSelfLoaded;
	HMODULE						m_handle;
	KSComAutoThreadMutex		m_Mutex;
	pfnKSCoCreateInstanceType	m_pfnKSCoCreateInstance;
	pfnKSCoInitializeType		m_pfnKSCoInitialize;
	pfnKSCoUninitializeType		m_pfnKSCoUninitialize;
	pfnKSFreeLibType			m_pfnKSFreeLib;
	pfnKSFreeLibExType			m_pfnKSFreeLibEx;
	pfnKSFreeUnusedLibType		m_pfnKSFreeUnusedLib;
	pfnKSCanUnLoadAllLibNowType m_pfnKSCanUnLoadAllLibNow;
	pfnKSRegisterServerType		m_pfnKSRegisterServer;
	pfnKSUnRegisterServerType	m_pfnKSUnRegisterServer;
	pfnKSRegisterServerExType	m_pfnKSRegisterServerEx;
	pfnKSUnRegisterServerExType m_pfnKSUnRegisterServerEx;
	pfnKSCoTaskMemAllocType		m_pfnKSCoTaskMemAlloc;
	pfnKSCoTaskMemFreeType		m_pfnKSCoTaskMemFree;
	pfnKSCoGetMallocType		m_pfnKSCoGetMalloc;
	pfnKSCoGetLibraryPathType	m_pfnKSCoGetLibraryPath;
	pfnKSInitConfigDataType		m_pfnKSInitConfigData;
	pfnKSCoLoadLibraryType		m_pfnKSCoLoadLibrary;
	pfnKSReLoadConfigureType	m_pfnKSReLoadConfigure;
	pfnKSCoProgIDFromCLSIDType	m_pfnKSCoProgIDFromCLSID;
	pfnKSCoBuildVersionType		m_pfnKSCoBuildVersion;
	pfnKSGetPropertyCLSIDType	m_pfnKSGetPropertyCLSID;
	pfnKSSetCLSIDProperty		m_pfnKSSetCLSIDProperty;
	pfnKSCoCLSIDFromProgIDType	m_pfnKSCoCLSIDFromProgID;
	pfnKSFreeLibExWType			m_pfnKSFreeLibExW;
	pfnKSRegisterServerWType	m_pfnKSRegisterServerW;
	pfnKSRegisterServerExWType	m_pfnKSRegisterServerExW;
	pfnKSUnRegisterServerExWType m_pfnKSUnRegisterServerExW;
	pfnKSCoGetLibraryPathWType	m_pfnKSCoGetLibraryPathW;
	pfnKSCoProgIDFromCLSIDWType	m_pfnKSCoProgIDFromCLSIDW;
	pfnKSCoCLSIDFromProgIDWType	m_pfnKSCoCLSIDFromProgIDW;
	pfnKSCoLoadLibraryWType		m_pfnKSCoLoadLibraryW;

};
#endif //SCOMDYNAMICLOADER_H