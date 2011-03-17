///////////////////////////////////////////////////////////////
//
//	Filename: 	kwssp.h
//	Creator:	lichenglin  <lichenglin@kingsoft.com>
//	Date:		2009-4-7  17:13
//	Comment:	
//
///////////////////////////////////////////////////////////////


#ifndef _kwssp_h_
#define _kwssp_h_
#include <Unknwn.h>




//////////////////////////////////////////////////////////////////////////
//
// 基础服务
//
//////////////////////////////////////////////////////////////////////////

typedef int BASICINTERFACE;
#define BASICINTERFACE_LOG    1


struct IKwsServiceBase
{
	virtual HRESULT __stdcall QueryBasicInterface( BASICINTERFACE niid, void** p ) = 0;
};




struct IBasicLog
{
	virtual int __stdcall Log( const wchar_t* pwszLine ) = 0;
};



//////////////////////////////////////////////////////////////////////////
//
// 服务调用模块
//
//////////////////////////////////////////////////////////////////////////

#define UIDDEF_IKWSSERVICEPROVIDER __declspec( uuid( "5B1035EE-05A9-4f53-B54E-186C469F24F5" ) )
struct UIDDEF_IKWSSERVICEPROVIDER IKwsServiceProvider : public IUnknown
{
	virtual int __stdcall InitializeService( IKwsServiceBase* pIKwsServiceBase ) = 0;

	virtual int __stdcall UninitializeService() = 0;

	virtual int __stdcall StartService() = 0;

	virtual int __stdcall StopService() = 0;	
};

#endif // _kwssp_h_