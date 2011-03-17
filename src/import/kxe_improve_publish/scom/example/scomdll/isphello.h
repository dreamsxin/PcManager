#pragma once

#include "../../../KisRpc/rfcdef.h"
#include <Unknwn.h>
#include <string>

MIDL_INTERFACE("174322A5-CFDD-4b71-8B11-DDA0589C3354")
ISPHello:public IUnknown
{
	virtual bool __stdcall Hello() = 0;
	virtual bool __stdcall MakeEvent(DWORD dwType, std::wstring sEventInfo) = 0;
};

RCF_CLASS_BEGIN_WITH_COM(ISPHello)
RCF_METHOD_R_0(bool, Hello)
RCF_METHOD_R_2(bool, MakeEvent, DWORD, std::wstring)
RCF_CLASS_END
