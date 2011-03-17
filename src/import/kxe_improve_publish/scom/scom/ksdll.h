/********************************************************************
* CreatedOn: 2005-8-18   17:06
* FileName: KSDll.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/
#ifndef KSDLL_H
#define KSDLL_H

#include <string>
#include "SCOMDef.h"
#include <vector>
#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#ifndef WIN32
inline HMODULE __stdcall LoadLibrary(LPCSTR pcszFileName)
{
    return  ::dlopen(pcszFileName, RTLD_NOW);
}
inline BOOL __stdcall FreeLibrary(HMODULE hLibModule)
{
    ::dlclose(hLibModule);
    return 1;
}
inline void* __stdcall GetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
    return ::dlsym(hModule, lpProcName);
}
#endif

class KSDll
{
public:
	KSDll():m_hModule(0),
		  m_pfnDllGetClassObject(0),
		  m_pfnDllGetClassCount(0),
		  m_pfnDllGetClassInfo(0),
		  m_pfnDllCanUnloadNow(0)
	  {
	  }

	  ~KSDll()
	  {
		  if (m_hModule)
			  FreeLibrary(m_hModule);
	  }

	KSRESULT Open(const char* pszModulePath);
	KSRESULT Open(const wchar_t* pszModulePath);
	KSRESULT Release();
	KSRESULT GetClassObject(const KSCLSID& clsid, const KSIID& riid, void** ppv) const;
	KSRESULT GetClassCount(int &nReturnSize) const;
	KSRESULT GetClassInfo(CLASSINFO *ClassInfo, int nInSize) const;
	KSRESULT CanUnloadNow(void) const;
	HSCOMMODULE GetModuleHandle(void);
	void Swap(KSDll& Other);

	void Attach(HMODULE hModule);
	HMODULE Detach(void);
private:
	KSRESULT GetFuncPtr(HMODULE hModule);
private:
	HMODULE m_hModule;
	_SCOM_KSDLLGETCLASSOBJECT*	m_pfnDllGetClassObject;
	_SCOM_KSDLLGETCLASSCOUNT*	m_pfnDllGetClassCount;
	_SCOM_KSDLLGETCLASSINFO*	m_pfnDllGetClassInfo;
	_SCOM_KSDLLCANUNLOADNOW*	m_pfnDllCanUnloadNow;
};



inline KSRESULT KSDll::Release()
{	
	if (m_hModule == NULL)
	{
		return S_OK;
	}
	FreeLibrary(m_hModule);
	m_hModule				= NULL;
	m_pfnDllGetClassObject	= NULL;
	m_pfnDllGetClassCount	= NULL;
	m_pfnDllGetClassInfo	= NULL;
	m_pfnDllCanUnloadNow	= NULL;
	return S_OK;	
}

inline KSRESULT KSDll::GetClassObject(const KSCLSID& clsid,const KSIID& riid,void** ppv) const
{
	return m_pfnDllGetClassObject(clsid, riid, ppv);
}

inline KSRESULT KSDll::GetClassCount(int &nReturnSize) const
{
	return m_pfnDllGetClassCount(nReturnSize);
}

inline KSRESULT KSDll::GetClassInfo(CLASSINFO *ClassInfo, int nInSize) const
{
	return m_pfnDllGetClassInfo(ClassInfo, nInSize);
}

inline KSRESULT KSDll::CanUnloadNow(void) const
{
	return m_pfnDllCanUnloadNow();
}

inline HSCOMMODULE KSDll::GetModuleHandle(void)
{
	return m_hModule;
}

inline void KSDll::Swap(KSDll& Other)
{
	HMODULE tmp_hModule = m_hModule;
	_SCOM_KSDLLGETCLASSOBJECT*	tmp_pfnDllGetClassObject= m_pfnDllGetClassObject;
	_SCOM_KSDLLGETCLASSCOUNT*	tmp_pfnDllGetClassCount	= m_pfnDllGetClassCount;
	_SCOM_KSDLLGETCLASSINFO*	tmp_pfnDllGetClassInfo	= m_pfnDllGetClassInfo;
	_SCOM_KSDLLCANUNLOADNOW*	tmp_pfnDllCanUnloadNow	= m_pfnDllCanUnloadNow;

	m_hModule				= Other.m_hModule;
	m_pfnDllGetClassObject	= Other.m_pfnDllGetClassObject;
	m_pfnDllGetClassCount	= Other.m_pfnDllGetClassCount;
	m_pfnDllGetClassInfo	= Other.m_pfnDllGetClassInfo;
	m_pfnDllCanUnloadNow	= Other.m_pfnDllCanUnloadNow;

	Other.m_hModule					= tmp_hModule;
	Other.m_pfnDllGetClassObject	= tmp_pfnDllGetClassObject;
	Other.m_pfnDllGetClassCount		= tmp_pfnDllGetClassCount;
	Other.m_pfnDllGetClassInfo		= tmp_pfnDllGetClassInfo;
	Other.m_pfnDllCanUnloadNow		= tmp_pfnDllCanUnloadNow;
}

inline void KSDll::Attach(HMODULE hModule)
{
	Release();
	m_hModule = hModule;
	GetFuncPtr(m_hModule);
}

inline HMODULE KSDll::Detach(void)
{
	HMODULE hModule			= m_hModule;
	m_hModule				= NULL;
	m_pfnDllGetClassObject	= NULL;
	m_pfnDllGetClassCount	= NULL;
	m_pfnDllGetClassInfo	= NULL;
	m_pfnDllCanUnloadNow	= NULL;

	return hModule;	
}

/*
inline const char* KSDll::GetModulePath(void)
{
return m_strModulePath.c_str();
}
*/
inline KSRESULT KSDll::Open(const char* pszModulePath)
{
	if (m_hModule != NULL)
	{
		return ERROR_ALREADY_EXISTS;
	}
	m_hModule = ::LoadLibraryA(pszModulePath);
	
	return GetFuncPtr(m_hModule);
}

inline KSRESULT KSDll::Open(const wchar_t* pszModulePath)
{
	if (m_hModule != NULL)
	{
		return ERROR_ALREADY_EXISTS;
	}
	m_hModule = ::LoadLibraryW(pszModulePath);

	return GetFuncPtr(m_hModule);
}

inline KSRESULT KSDll::GetFuncPtr(HMODULE hModule)
{
	KSRESULT ret = S_OK;

	if (hModule == NULL)
	{

		//#ifdef WIN32
		//		return GetLastError();
		//#else
		//		return ERROR_MOD_NOT_FOUND;
		//#endif
		ret = E_SCOM_MOD_NOT_FOUND;
		//return CO_E_DLLNOTFOUND;
	}
	else if (NULL == (m_pfnDllGetClassObject = (_SCOM_KSDLLGETCLASSOBJECT*)GetProcAddress(
		hModule,
		"KSDllGetClassObject")))
	{
		ret = E_SCOM_PROC_NOT_FOUND;
	}
	else if (NULL == (m_pfnDllGetClassCount = (_SCOM_KSDLLGETCLASSCOUNT*)GetProcAddress(
		hModule, 
		"KSDllGetClassCount")))
	{
		ret = E_SCOM_PROC_NOT_FOUND;
	}
	else if (NULL == (m_pfnDllGetClassInfo = (_SCOM_KSDLLGETCLASSINFO*)GetProcAddress(
		hModule, 
		"KSDllGetClassInfo")))
	{
		ret = E_SCOM_PROC_NOT_FOUND;
	}
	else if (NULL == (m_pfnDllCanUnloadNow = (_SCOM_KSDLLCANUNLOADNOW*)GetProcAddress(
		hModule, 
		"KSDllCanUnloadNow")))
	{
		ret = E_SCOM_PROC_NOT_FOUND;
	}

	return ret;

}
#endif //KSDLL_H