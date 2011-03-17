/********************************************************************
* CreatedOn: 2005-8-16   15:41
* FileName: SCOMDef.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/

#ifndef SCOMDEF_H
#define SCOMDEF_H
#include <cassert>
#include "SCOMError.h"
#include "Threads.h"
#include "SCOMMsgAssert.h"
#include "../../kxecommon/kxecommon.h"

#ifdef WIN32
#pragma warning(disable: 4244)  //disable warning C4244: conversion from 'int' to
                                //'unsigned char', possible loss of data
#if MSCVER >= 1400

#include <GuidDef.h>

#endif

#endif

#ifndef WIN32

#define APIENTRY		__stdcall

typedef void*           HANDLE;
typedef void*           HMODULE;
typedef unsigned long   DWORD;
typedef unsigned long   ULONG;
typedef const char *    LPCSTR;

typedef long            BOOL;
const   BOOL    TRUE	= 1;
const   BOOL	FALSE	= 0;

typedef struct _GUID {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[ 8 ];
} GUID;
#endif


#define SCOMLIB_VERSION 1 

//配置文件格式错误
#define E_CONFIGFILEFORMAT_ERROR	(long)0x81000001L
//读出配置文件错误
#define E_CONFIGFILEREAD_ERROR		(long)0x81000002L
//写入配置文件错误
#define E_CONFIGFILEWRITE_ERROR		(long)0x81000003L
//找不到组件
#define E_SCOM_NOT_FOUND			(long)0x81000004L
//没有定位文件
#define E_SCOM_NO_LOCATE_FILE		(long)0x81000005L
//没有初始化
#define E_SCOM_NOTINIT				(long)0x81000006L


#define COMPONENT_PROP_STARTUP		1

typedef GUID _KSGUID;
typedef GUID KSGUID;
typedef long			KSRESULT;
typedef KSGUID			KSIID;
typedef KSGUID			KSCLSID;
typedef HMODULE			HSCOMMODULE;
#define KS_DEFINE_GUID_NULL   \
{ 0x00000000, 0x0000, 0x0000, \
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} }

#define KS_DEFINE_GUID_NULL_STR "00000000-0000-0000-0000-000000000000"

typedef struct _CLASSINFO
{
	KSCLSID m_CLSID;
	const char *m_pszProgID;
	DWORD m_dwProperty;
}CLASSINFO;

typedef KSRESULT (__stdcall _SCOM_CREATORFUNC)(const KSIID &iid, void **ppv);

typedef struct _KMODULEINFO
{
	KSCLSID m_CLSID;
	_SCOM_CREATORFUNC *m_pfnCreateInstance;
	const char *m_pszProgID;
	DWORD m_dwProperty;
}KMODULEINFO;

//////////////////////////////////////////////////////////////////////////

typedef KSRESULT (__stdcall _SCOM_KSDLLGETCLASSOBJECT)(const KSCLSID& clsid, const KSIID &riid , void **ppv);
typedef KSRESULT (__stdcall _SCOM_KSDLLGETCLASSCOUNT)(int &nReturnSize);
typedef KSRESULT (__stdcall _SCOM_KSDLLGETCLASSINFO)(CLASSINFO *ClassInfo, int nInSize);
typedef KSRESULT (__stdcall _SCOM_KSDLLCANUNLOADNOW)(void);

//////////////////////////////////////////////////////////////////////////
#define RGUID const KSGUID&

#define KS_ASSERT(exp) assert(exp)

#define KS_DEFINE_GETIID(_iid) public:\
	static const KSIID& GetIID() {static const KSIID iid = _iid; return iid;}

#define KS_DEFINE_GETCLSID(_clsid) public:\
	static const KSCLSID& GetCLSID() {static const KSCLSID clsid = _clsid; return clsid;}
//////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#define KS_IIDOF(_interface) __uuidof(_interface)
#else
#define KS_IIDOF(_interface) _interface::GetIID()
#endif

#define KS_CLSIDOF(_component) _component::GetCLSID()

//引用计数类
#ifdef WIN32
class KSRefCnt
{
public:
	KSRefCnt() : m_lValue(0) {}
	KSRefCnt(long lValue) : m_lValue(lValue) {}

	long operator++() 
	{
		return InterlockedIncrement(&m_lValue);
	}
	long operator--() 
	{
		return InterlockedDecrement(&m_lValue);
	}
	long operator=(long lValue) { return (m_lValue = lValue); }
	operator long() const { return m_lValue; }
private:
	long operator++(int);
	long operator--(int);
	long m_lValue;
};
#else
//可考虑使用SGI stl提供的原子操作
class KSRefCnt
{
public:
	KSRefCnt() : m_lValue(0) {}
	KSRefCnt(long lValue) : m_lValue(lValue) {}

	long operator++() 
	{
		KSAutoLockGuard guard(m_mutex);
		return ++m_lValue;
	}
	long operator--()
	{ 
		KSAutoLockGuard guard(m_mutex);
		return --m_lValue;
	}
	long operator=(long lValue) { return (m_lValue = lValue); }
	operator long() const { return m_lValue; }
private:
	long operator++(int);
	long operator--(int);
	long m_lValue;
	KSAutoLockGuard::ThreadMutex m_mutex;
};
#endif

template<long lRefInitCnt>
class KSRefCnt_WithArgument : public KSRefCnt
{
public:
	KSRefCnt_WithArgument():KSRefCnt(lRefInitCnt)
	{
	}
};

//////////////////////////////////////////////////////////////////////////
#ifndef WIN32

inline bool IsEqualGUID(KSGUID rguid1, KSGUID rguid2)
{
	return !memcmp(&rguid1, &rguid2, sizeof(KSGUID));
}

#define IsEqualIID(riid1, riid2) IsEqualGUID(riid1, riid2)
#define IsEqualCLSID(rclsid1, rclsid2) IsEqualGUID(rclsid1, rclsid2)

inline bool operator==(const KSGUID& guidOne, const KSGUID& guidOther)
{
	return !memcmp(&guidOne,&guidOther,sizeof(KSGUID));
}

inline bool operator!=(const KSGUID& guidOne, const KSGUID& guidOther)
{
	return !(guidOne == guidOther);
}
#endif

inline bool InlineIsLessGUID(RGUID rguid1, RGUID rguid2)
{	
	int nRet = memcmp(&rguid1, &rguid2, sizeof(KSGUID));
	return (nRet < 0) ? true : false;
}

inline bool operator<(RGUID rguid1, RGUID rguid2)
{
	return InlineIsLessGUID(rguid1, rguid2);
}

#endif //SCOMDEF_H
