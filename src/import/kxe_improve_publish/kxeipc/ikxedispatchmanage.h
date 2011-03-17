//////////////////////////////////////////////////////////////////////
///		@file		ikxedispatchmanage.h
///		@author		luopeng
///		@date		2009-4-18 09:07:58
///
///		@brief		提供给IPC-RPC需要Dispatch时的查询接口
//////////////////////////////////////////////////////////////////////

#pragma once

class IKxERequestDispatch;

class IKxEDispatchManage
{
public:
	virtual int __stdcall RegisterRequestDispatch(
		const wchar_t* pwszServiceProviderName,
		IKxERequestDispatch* pDispatch
		) = 0;

	virtual int __stdcall UnregisterRequestDispatch(
		const wchar_t* pwszServiceProviderName		
		) = 0;

	virtual int __stdcall GetRequestDispatch(
		const wchar_t* pwszServiceProviderName,
		IKxERequestDispatch** ppDispatch
		) = 0;

	virtual int __stdcall ReleaseRequestDispatch(
		IKxERequestDispatch* pDispatch
		) = 0;
};
