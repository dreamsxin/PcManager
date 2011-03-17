//////////////////////////////////////////////////////////////////////
//  @ File		:	kxecommon.h
//  @ version	:	1.0
//  @ Author	:	梁光彩<liangguangcai@kingsoft.com>
//  @ Date		:	[2009-12-29, 14:47:47]
//  @ Brief		:	定义 SCOM 和 KXEBASE 的配置信息，提供相关功能调用
//					
//  @ Last changed
//    #Who		:
//    #When		:
//    #What		:
//////////////////////////////////////////////////////////////////////
#ifndef __KXECOMMON__H__5B70D9E3_BBC4_4a8f_AB1E_CF469D36B43E
#define __KXECOMMON__H__5B70D9E3_BBC4_4a8f_AB1E_CF469D36B43E
#if defined(_MSC_VER) && (_MSC_VER > 1020)
#pragma once
#endif	/*	_MSC_VER > 1020	*/

#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

//------------------------------------------------------------------------------


/*
* @brief	SP 注册信息在注册表中的路径
*/
//#define		KXE_SP_PIPE_INFO_PATH_W			L"SOFTWARE\\Kingsoft\\KISCommon\\SP_ROOT\\"
//#define		KXE_PRODUCT_DEFAULT_ROOT_PATH_W	L"SOFTWARE\\Kingsoft\\KISCommon"
#define		KXE_SP_PIPE_INFO_PATH_W			L"SOFTWARE\\Kingsoft\\KxECommon\\SP_ROOT\\"
#define		KXE_PRODUCT_DEFAULT_ROOT_PATH_W	L"SOFTWARE\\Kingsoft\\KxECommon"
/*
* @brief	SP 在注册表中的管道名键名
*/
#define		KXE_SP_PIPE_NAME_SUB_KEY_W		L"Name"


//------------------------------------------------------------------------------


/*
* @brief	配置文件名
* @remark	使用 scom.dll 或 kxebase.dll 的 EXE 必须保证拥有一个与之同目录
*			的配置文件，并且名字为 KXECOMMON_CONFIG_FILE_NAME 的定义
*/
#define		KXECOMMON_CONFIG_FILE_NAME					"kxecomm.dat"
#define		KXECOMMON_CONFIG_FILE_NAME_W				L"kxecomm.dat"

/*
* @brief	配置文件项名称
*/
#define		KXECOMMON_CONFIG_DLL_PATH_SECTION_NAME		"ProductInfo"
#define		KXECOMMON_CONFIG_DLL_PATH_SECTION_NAME_W	L"ProductInfo"

//////////////////////////////////////////////////////////////////////////
//sp名称配置项,例子
//[SPCfg]
//KBootCare=KBootCareKWS
//这个配置项可以让原来sp固定名称为KBootCare的sp注册时把名字注册为KBootCareKWS
#define		KXECOMMON_CONFIG_DLL_SP_CONFIG_SECTION_NAME		"SPCfg"
#define		KXECOMMON_CONFIG_DLL_SP_CONFIG_SECTION_NAME_W	L"SPCfg"

//////////////////////////////////////////////////////////////////////////
//sp根配置项,例子
//[SPRoot]
//SPRoot=SOFTWARE\Kingsoft\KWS\SP_ROOT\
//这个配置项可以修改注册表sp的根路径
#define		KXECOMMON_CONFIG_DLL_SP_ROOT_SECTION_NAME	"SPRoot"
#define		KXECOMMON_CONFIG_DLL_SP_ROOT_SECTION_NAME_W	L"SPRoot"

/*
* @brief	配置文件中键名称
* @remark	键名为 DLL 名称，忽略大小写
*/
#define		KXECOMMON_CONFIG_KXEBASE_DLL_NAME			"kxebase.dll"
#define		KXECOMMON_CONFIG_KXEBASE_DLL_NAME_W			L"kxebase.dll"
#define		KXECOMMON_CONFIG_SCOM_DLL_NAME				"scom.dll"
#define		KXECOMMON_CONFIG_SCOM_DLL_NAME_W			L"scom.dll"

// #define		KXECOMMON_CONFIG_DEFAULT_REG_ROOT_PATH		"SOFTWARE\\Kingsoft\\KXECommon"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
//------------------------------------------------------------------------------

static inline bool _kxe_check_is_file_exist(const wchar_t* pszFile)
{
	DWORD dwAttr = GetFileAttributesW(pszFile);
	return ((dwAttr != INVALID_FILE_ATTRIBUTES) &&  !(dwAttr & FILE_ATTRIBUTE_DIRECTORY));
}


//////////////////////////////////////////////////////////////////////////
//获取模块所在的路径
inline HRESULT kxe_get_load_module_path(HMODULE hModule, std::wstring& strPath)
{
	HRESULT hr		= E_FAIL;
	wchar_t* pBuf	= NULL;
	DWORD nAllocSize= 256;//2的8次方
	DWORD nBufferSize = 0;
	DWORD dwLen		= 0;
	//2的15次方是32768,路径最长不会超过这个数
	for (;nAllocSize <= 32768; nAllocSize *= 2)
	{
		if (pBuf != NULL)
		{
			delete[] pBuf;
			pBuf = NULL;
		}
		pBuf = new wchar_t[nAllocSize];
		nBufferSize = nAllocSize - 1;
		dwLen = GetModuleFileNameW(hModule, pBuf, nBufferSize);
		if (0 == dwLen)
		{
			break;//执行失败
		}
		//dwLen 至少应该比nBufferSize小1
		if (dwLen < nBufferSize)
		{
			hr = S_OK;
			strPath = pBuf;
			break;
		}
	}
	if (pBuf)
	{
		delete[] pBuf;
		pBuf = NULL;
	}
	return hr;
}

//////////////////////////////////////////////////////////////////////////
//同过配置文件获取sp名字配置项
inline HRESULT kxe_get_config_service_name_form_cfgfile(const wchar_t* pszFile, const wchar_t* pszBaseServiceName, std::wstring& strConfigServiceName)
{
	if (NULL == pszFile || NULL == pszBaseServiceName)
		return E_FAIL;

	HRESULT hr = E_FAIL;
	wchar_t szSpName[256] = { 0 };
	DWORD dwSpNameBufSize = 256;

	DWORD dwRead = ::GetPrivateProfileStringW(
		KXECOMMON_CONFIG_DLL_SP_CONFIG_SECTION_NAME_W,
		pszBaseServiceName,
		NULL,
		szSpName,
		dwSpNameBufSize,
		pszFile
		);

	if (dwRead)
	{
		strConfigServiceName = szSpName;
		hr = S_OK;
	}
	return hr;
}

//////////////////////////////////////////////////////////////////////////
//获取配置文件的路径
inline void kxe_get_cfgfile_path(HMODULE hModule, std::wstring& strConfigFilePath)
{
	std::wstring strPath;
	kxe_get_load_module_path(hModule, strPath);
	::PathRemoveFileSpecW(&(strPath[0]));
	strConfigFilePath = &(strPath[0]);
	strConfigFilePath += L"\\";
	strConfigFilePath += KXECOMMON_CONFIG_FILE_NAME_W;
}

/**
* @brief		从配置文件 kxecommon.dat 中读取指定 DLL 路径所保存到的注册表路径
* @remark		配置文件 kxecommon.dat 须与当前 EXE 文件或当前dll位于同一目录下
* @param[in]	: lpszDllName 为指定 DLL 的名字，作为配置文件中的一个键名
* @param[out]	: lpszDllRegPath 接收查找的注册表路径的缓冲区
* @param[in]	: dwChSize 为缓冲区长度（字符数）
* @param[inout]	: pdwChRead 为实际接收的字符数（可以为 NULL）
* @return		0 成功，其他为错误码
*/
inline int kxecommon_get_dll_reg_path_from_config_file(
	LPCWSTR	lpszDllName,
	LPWSTR	lpszDllRegPath,
	DWORD	dwChSize,
	PDWORD	pdwChRead
	)
{
	if (NULL == lpszDllName || NULL == lpszDllRegPath)
	{
		return E_INVALIDARG;
	}

	std::wstring strConfigFilePath;
	kxe_get_cfgfile_path(NULL, strConfigFilePath);
	if (!_kxe_check_is_file_exist(strConfigFilePath.c_str()))
	{
		kxe_get_cfgfile_path((HINSTANCE)&__ImageBase, strConfigFilePath);
	}

	DWORD dwRead = ::GetPrivateProfileStringW(
		KXECOMMON_CONFIG_DLL_SP_ROOT_SECTION_NAME_W,
		lpszDllName,
		NULL,
		lpszDllRegPath,
		dwChSize,
		strConfigFilePath.c_str()
		);
	if (NULL != pdwChRead)
	{
		*pdwChRead = dwRead;
	}

	return (0 == dwRead) ? ::GetLastError() : 0;
}
//////////////////////////////////////////////////////////////////////////
//自动获取sp名字的配置
inline HRESULT kxe_get_config_service_name(const wchar_t* pszBaseServiceName, std::wstring& strConfigServiceName)
{
	if (NULL == pszBaseServiceName)
		return E_FAIL;

	HRESULT hr = E_FAIL;
	std::wstring strConfigFilePath;
	kxe_get_cfgfile_path(NULL, strConfigFilePath);
	hr = kxe_get_config_service_name_form_cfgfile(strConfigFilePath.c_str(), pszBaseServiceName, strConfigServiceName);
	if (FAILED(hr))
	{		
		kxe_get_cfgfile_path((HINSTANCE)&__ImageBase, strConfigFilePath);
		hr = kxe_get_config_service_name_form_cfgfile(strConfigFilePath.c_str(), pszBaseServiceName, strConfigServiceName);
	}	
	return hr;
}

inline void kxe_append_slash_to_path(std::wstring& strPath)
{
	if (strPath[strPath.length() - 1] != L'\\')
	{
		strPath.append(L"\\");
	}
}

//////////////////////////////////////////////////////////////////////////
//从配置文件获取sp的注册表根路径
inline HRESULT kxe_direct_get_config_sp_root_path_form_cfgfile(const wchar_t* pszFile, std::wstring& strSPRootPath)
{
	if (NULL == pszFile)
		return E_FAIL;

	HRESULT hr = E_FAIL;
	wchar_t szSpRoot[1024] = { 0 };
	DWORD dwSpRootBufSize = 1024;

	DWORD dwRead = ::GetPrivateProfileStringW(
		KXECOMMON_CONFIG_DLL_SP_ROOT_SECTION_NAME_W,
		KXECOMMON_CONFIG_DLL_SP_ROOT_SECTION_NAME_W,
		NULL,
		szSpRoot,
		dwSpRootBufSize,
		pszFile
		);

	if (dwRead)
	{
		strSPRootPath = szSpRoot;
		kxe_append_slash_to_path(strSPRootPath);
		hr = S_OK;
	}
	return hr;
}

//////////////////////////////////////////////////////////////////////////
//自动从配置文件获取sp注册表根路径
inline HRESULT kxe_direct_get_config_sp_root_path(std::wstring& strSPRootPath)
{
	HRESULT hr = E_FAIL;
	std::wstring strConfigFilePath;
	kxe_get_cfgfile_path(NULL, strConfigFilePath);
	hr = kxe_direct_get_config_sp_root_path_form_cfgfile(strConfigFilePath.c_str(), strSPRootPath);
	if (FAILED(hr))
	{
		kxe_get_cfgfile_path((HINSTANCE)&__ImageBase, strConfigFilePath);
		hr = kxe_direct_get_config_sp_root_path_form_cfgfile(strConfigFilePath.c_str(), strSPRootPath);
	}
	return hr;
}

//////////////////////////////////////////////////////////////////////////
//从配置文件获取模块注册表配置路径
inline HRESULT kxe_direct_get_config_dll_reg_path_form_cfgfile(const wchar_t* pszFile, const wchar_t* pszDllName, std::wstring& strRootPath)
{
	if (NULL == pszFile || NULL == pszDllName)
		return E_FAIL;

	HRESULT hr = E_FAIL;
	wchar_t szSpRoot[1024] = { 0 };
	DWORD dwSpRootBufSize = 1024;

	DWORD dwRead = ::GetPrivateProfileStringW(
		KXECOMMON_CONFIG_DLL_PATH_SECTION_NAME_W,
		pszDllName,
		NULL,
		szSpRoot,
		dwSpRootBufSize,
		pszFile
		);

	if (dwRead)
	{
		strRootPath = szSpRoot;
		hr = S_OK;
	}
	return hr;
}

inline HRESULT kxe_get_dll_reg_path_from_config_file(const wchar_t* pszDllName, std::wstring& strRootPath)
{
	HRESULT hr = E_FAIL;
	std::wstring strConfigFilePath;
	kxe_get_cfgfile_path(NULL, strConfigFilePath);
	hr = kxe_direct_get_config_dll_reg_path_form_cfgfile(strConfigFilePath.c_str(), pszDllName, strRootPath);
	if (FAILED(hr))
	{
		kxe_get_cfgfile_path((HINSTANCE)&__ImageBase, strConfigFilePath);
		hr = kxe_direct_get_config_dll_reg_path_form_cfgfile(strConfigFilePath.c_str(), pszDllName, strRootPath);
	}
	return hr;
}

//从配置文件获取大引擎注册表根路径
inline HRESULT kxe_get_kxebase_reg_path_from_config_file(std::wstring& strRootPath)
{
	return kxe_get_dll_reg_path_from_config_file(KXECOMMON_CONFIG_KXEBASE_DLL_NAME_W, strRootPath);
}

//从配置文件获取scom注册表根路径
inline HRESULT kxe_get_scom_reg_path_from_config_file(std::wstring& strRootPath)
{	
	return kxe_get_dll_reg_path_from_config_file(KXECOMMON_CONFIG_SCOM_DLL_NAME_W, strRootPath);
}

//获取大引擎注册表根路径
inline HRESULT kxe_get_kxebase_reg_path(std::wstring& strRootPath)
{
	HRESULT hr = E_FAIL;
	hr = kxe_get_dll_reg_path_from_config_file(KXECOMMON_CONFIG_KXEBASE_DLL_NAME_W, strRootPath);
	if (FAILED(hr))
	{
		strRootPath = KXE_PRODUCT_DEFAULT_ROOT_PATH_W;
		strRootPath += L"\\KXEngine";
	}
	return S_OK;
}

//获取scom注册表根路径
inline HRESULT kxe_get_scom_reg_path(std::wstring& strRootPath)
{	
	HRESULT hr = E_FAIL;
	hr = kxe_get_dll_reg_path_from_config_file(KXECOMMON_CONFIG_SCOM_DLL_NAME_W, strRootPath);
	if (FAILED(hr))
	{
		strRootPath = KXE_PRODUCT_DEFAULT_ROOT_PATH_W;
		strRootPath += L"\\KXEngine\\CLASSES_ROOT";
	}
	return S_OK;
}


inline HRESULT kxe_direct_get_config_product_root_path(std::wstring& strRootPath)
{
	std::wstring strKxebasePath;//现在用kxebase的路径
	HRESULT hr = kxe_get_kxebase_reg_path_from_config_file(strKxebasePath);
	if (SUCCEEDED(hr))
	{
		if (strKxebasePath[strKxebasePath.length() - 1] == L'\\')
			strKxebasePath[strKxebasePath.length() - 1] = 0;

		const wchar_t* pszShortFileName = NULL;
		pszShortFileName = wcsrchr(strKxebasePath.c_str(), L'\\');
		if (pszShortFileName)
		{
			*((wchar_t*)pszShortFileName) = 0;
		}

		strRootPath = strKxebasePath.c_str();//注意不能直接用strRootPath = strKxebasePath;因为strKxebasePath中间已经用填充一个0的方式把字符串截断了
	}
	return hr;
}

class KxeAutoGetSpRoot
{
public:
	KxeAutoGetSpRoot()
	{
		if (FAILED(kxe_direct_get_config_sp_root_path(m_strSpRoot)))
			m_strSpRoot = KXE_SP_PIPE_INFO_PATH_W;	
	}
	const wchar_t* GetSpRootString(void)
	{
		return m_strSpRoot.c_str();
	}
private:
	std::wstring m_strSpRoot;
};

template<int instid>
class KxeGetSpRootUtil
{
public:
	static const wchar_t* GetSpRootString(void)
	{
		return ms_SpRootGettor.GetSpRootString();
	}
private:
	static KxeAutoGetSpRoot ms_SpRootGettor;
};

template<int instid>
KxeAutoGetSpRoot KxeGetSpRootUtil<instid>::ms_SpRootGettor;

inline const wchar_t* kxe_get_config_sp_root_path(void)
{
	return KxeGetSpRootUtil<0>::GetSpRootString();
}

//////////////////////////////////////////////////////////////////////////
class KxeAutoGetProductRoot
{
public:
	KxeAutoGetProductRoot()
	{
		if (FAILED(kxe_direct_get_config_product_root_path(m_strRoot)))
			m_strRoot = KXE_PRODUCT_DEFAULT_ROOT_PATH_W;	
	}
	const wchar_t* GetProductRootString(void)
	{
		return m_strRoot.c_str();
	}
private:
	std::wstring m_strRoot;
};

template<int instid>
class KxeGetProductRootUtil
{
public:
	static const wchar_t* GetProductRootString(void)
	{
		return ms_ProductRootGettor.GetProductRootString();
	}
private:
	static KxeAutoGetProductRoot ms_ProductRootGettor;
};

template<int instid>
KxeAutoGetProductRoot KxeGetProductRootUtil<instid>::ms_ProductRootGettor;

inline const wchar_t* kxe_get_config_product_root_path(void)
{
	return KxeGetProductRootUtil<0>::GetProductRootString();
}

//------------------------------------------------------------------------------


#endif	/*	not __KXECOMMON__H__5B70D9E3_BBC4_4a8f_AB1E_CF469D36B43E	*/
