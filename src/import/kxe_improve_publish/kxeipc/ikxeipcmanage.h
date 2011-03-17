//////////////////////////////////////////////////////////////////////
///		@file		ikxeipcmanage.h
///		@author		luopeng
///		@date		2009-4-18 09:07:58
///
///		@brief		统一的IPC管理
//////////////////////////////////////////////////////////////////////

#pragma once
#include <Unknwn.h>

class IKxERequestDispatch;

MIDL_INTERFACE("C63842D5-86BC-4097-AE18-ED6AF35A5775")
IKxEIPCManage : public IUnknown
{
public:
	virtual int __stdcall Initialize() = 0;

	virtual int __stdcall Uninitialize() = 0;

	virtual int __stdcall Start() = 0;

	virtual int __stdcall Stop() = 0;

	virtual int __stdcall RegisterRequestDispatch(
		const wchar_t* pwszServiceProviderName,
		IKxERequestDispatch* pDispatch
		) = 0;

	virtual int __stdcall UnregisterRequestDispatch(
		const wchar_t* pwszServiceProviderName		
		) = 0;

	// 以下三个函数仅为兼容HTTP
	virtual int __stdcall RegisterWebPath(
		const char* pszName,
		const char* pszDirectory
		) = 0;

	virtual int __stdcall  UnregisterWebPath(
		const char* pszName
		) = 0;

	virtual int __stdcall GetServicePort(
		unsigned short* pnPort
		) = 0;
};
