#pragma once

#ifndef NS_BEGIN
#define  NS_BEGIN(name) namespace name {
#endif

#ifndef NS_END
#define  NS_END(name)   };
#endif

#ifndef STDAPIN_
#define STDAPIN_(type)	extern "C" type __declspec(dllexport)
#endif

#include <string>
#include "kfile.h"
#include "kdirectory.h"
#include "kpath.h"
#include "KOemKsfe.h"

//判断指针是否有效。如果无效，则该表达式为TRUE；否则为FALSE
#ifndef ValidPtrRange
#define ValidPtrRange(p) 	((p == NULL) || !(((ULONG_PTR)p > (1024 * 1024)) && ((ULONG_PTR)p < 0x7FFF0000)))
#endif


//清除new函数分配的内存
#ifndef DeletePointer
#define DeletePointer(p)	\
	do						\
	{						\
		if (p)				\
		{					\
			delete[] p;		\
			p = NULL;		\
		}					\
	} while(0);

#endif

//使用malloc分配内存,可使用DeleteMalloc释放
#ifndef MallocWhar
#define MallocWhar(p, nSize)										\
	{																\
		nSize ++;													\
		p = (wchar_t *)malloc(nSize * sizeof(wchar_t)/sizeof(char));\
		if (p == NULL)	return NULL;								\
		::ZeroMemory(p, nSize * sizeof(wchar_t)/sizeof(char));		\
	}
#endif

//释放malloc分配内存
#ifndef DeleteMalloc
#define DeleteMalloc(p)		\
	do						\
	{						\
		if (p)				\
		{					\
			free(p);		\
			p = NULL;		\
		}					\
	} while(0);
#endif


///////////////////////////////////////////////////////////////
//调用有字符串返回的函数的方法

//调用的函数的类型为：int BeCall(LPSTR lpBuffer, size_t& nSize);
template<class TRet, class T1, class T2>
BOOL CallFunc(IN void* pFunc, OUT std::string& strValue)
{
	std::wstring strValueW;
	BOOL bRet = CallFunc(pFunc, strValueW);
	KAString strValueA = strValueW;
	strValue = strValueA.c_str();
	return bRet;
}

//调用的函数的类型为：int BeCall(LPWSTR lpBuffer, size_t& nSize);
template<class TRet, class T1, class T2>
BOOL CallFunc(IN void* pFunc, OUT std::wstring& strValue)
{
	typedef TRet (*TempLateFunc)(T1, T2);
	TempLateFunc pCallFunc = (TempLateFunc) pFunc;
	
	
	WCHAR szBuffer[MAX_PATH] = {0};
	size_t nSize = MAX_PATH;
	TRet nRet = pCallFunc(szBuffer, nSize);
	if (nRet)
	{
		strValue = szBuffer;
		return TRUE;
	}

	wchar_t* pT1 = NULL;
	MallocWhar(pT1, nSize);

	nRet = pCallFunc(pT1, nSize);
	if (!nRet)		return FALSE;

	strValue = pT1;

	DeletePointer(pT1);

	return TRUE;
}

//调用的函数的类型为：int BeCall(XXX p1, LPSTR lpBuffer, size_t& nSize);
//其中XXX为任意一种类型
template<class TRet, class T1, class T2, class T3>
BOOL CallFunc(IN void* pFunc, IN T1 t1, OUT std::string& strValue)
{
	std::wstring strValueW;
	BOOL bRet = CallFunc(pFunc, t1, strValueW);
	KAString strValueA = strValueW;
	strValue = strValueA.c_str();
	return bRet;
}

//调用的函数的类型为：int BeCall(XXX p1, LPWSTR lpBuffer, size_t& nSize);
//其中XXX为任意一种类型
template<class TRet, class T1, class T2, class T3>
BOOL CallFunc(IN void* pFunc, IN T1 t1, OUT std::wstring& strValue)
{
	typedef  TRet ( *TempLateFunc)(T1, T2, T3);
	TempLateFunc pCallFunc = (TempLateFunc) pFunc;

	WCHAR szBuffer[MAX_PATH] = {0};
	size_t nSize = MAX_PATH;
	TRet nRet = pCallFunc(t1, szBuffer, nSize);
	if (nRet)
	{
		strValue = szBuffer;
		return TRUE;
	}

	wchar_t* pT2 = NULL;
	MallocWhar(pT2, nSize);

	nRet = pCallFunc(t1, pT2, nSize);
	if (!nRet)		return FALSE;

	strValue = pT2;

	DeleteMalloc(pT2);
	return TRUE;
}
