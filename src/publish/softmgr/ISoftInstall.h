#pragma once

#include "iksautoinstcallback.h"

class IKSAutoInstCallBack2 : public IKSAutoInstCallBack
{
public:
	virtual HRESULT	OnInstSoftComplete(HRESULT hr) = 0;
};

namespace ksm
{
	
class __declspec(uuid("0B55608E-4690-4b37-86AB-06C2DF2A9A88")) ISoftInstall
{
public:
	virtual void __stdcall SetNotify(IKSAutoInstCallBack2 *pNotify) = 0;
	virtual BOOL __stdcall Initialize(LPCWSTR pKSafePath, LPCWSTR pInstLibPath) = 0;
	virtual void __stdcall Uninitialize() = 0;

	virtual BOOL __stdcall StartInstSoft(LPCSTR pXml, DWORD cch) = 0;
	virtual void __stdcall StopInstSoft() = 0;
};

}