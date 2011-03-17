/********************************************************************
created:	2011/02/17
filename: 	ikwebshieldui.h
author:		huangchaoling@kingsoft.com

purpose:	网盾模块对外接口
*********************************************************************/

#pragma once

#include <unknwn.h>


[
	uuid(F4E20C78-FA7C-4e7f-813A-AFDED258F040)
]
interface IKWebShieldUI
{
	
	virtual HWND __cdecl CreateWebShieldUI(HWND hParent) = 0;

	virtual BOOL __cdecl Navigate(const wchar_t* szNavigate) = 0;

	virtual int __cdecl  InitCtrl(void) = 0;
	virtual void __cdecl InitCtrlDelay(void) = 0;

	virtual void __cdecl ReloadEngData(void) = 0;
	virtual void __cdecl ModifyMonitorState(int nType) = 0;

	virtual LRESULT __cdecl OnKwsShowSetting(void) = 0;
	virtual LRESULT __cdecl OnKwsOpenDLSPage(void) = 0;

	virtual void __cdecl Release(void) = 0;
};


typedef HRESULT (__cdecl *CreateWebShieldUIObject_t)(REFIID riid, void** ppObject);
HRESULT __cdecl CreateWebShieldUIObject(REFIID riid, void** ppObject);


