#pragma once

#include <unknwn.h>
#include "../../src_bksafe/beikesafe/bkmsgdefine.h"
//////////////////////////////////////////////////////////////////////////


interface IScanToMain
{
	virtual void	TrojanToMainChange(LPCWSTR strCommand) = 0;
	virtual void	VirusInstallFinish() = 0;
	virtual int		SysCnvertExamind( int nId ) = 0;
	virtual void    ExamRemove1( WPARAM wParam ) = 0;
	virtual void	ExamRemove2( WPARAM wParam ) = 0;
	virtual void	OpenDuba( WPARAM wParam ) = 0;
	virtual void	TryOpenShell( WPARAM wParam ) = 0;
	virtual void	ShowLocalTrojanVer( WPARAM wParam, LPARAM lParam ) = 0;//显示本地木马库版本号	
};

[
	uuid(40478EBA-78B7-4a2e-ADB1-92BF3CEE5EA9)
]
interface IKTrojan
{
	virtual STATE_CLOSE_CHECK __cdecl CloseCheck() = 0;
	virtual void __cdecl Release() = 0;
	virtual HWND __cdecl CreateKTrojan(HWND hParent) = 0;
 	virtual void __cdecl Navigate(const wchar_t* szNavigate) = 0;
	virtual void __cdecl SetNotify( IScanToMain* pNotify ) = 0;
	virtual void __cdecl Init() = 0;
	virtual void __cdecl CloseSuccess(BOOL bSucClose) = 0;
};

//////////////////////////////////////////////////////////////////////////

typedef HRESULT (__cdecl *CreateKTrojanObject_t)(REFIID riid, void** ppObject);
HRESULT __cdecl CreateKTrojanObject(REFIID riid, void** ppObject);