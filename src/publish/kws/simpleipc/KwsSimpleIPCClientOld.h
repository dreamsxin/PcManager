///////////////////////////////////////////////////////////////
//
//	Filename: 	KwsSimpleIPCClient.h
//	Creator:	lichenglin  <lichenglin@kingsoft.com>
//	Date:		2009-4-1  17:14
//	Comment:	
//
///////////////////////////////////////////////////////////////


#ifndef _KwsSimpleIPCClient_old_h_
#define _KwsSimpleIPCClient_old_h_
#include "KwsIPCDef.h"
#include <Windows.h>

class KwsProtocol;

namespace kws_old_ipc {

class KwsSimpleIPCClient
{
public:
	int SendCmd( KwsProtocol* pProtocol, DWORD dwWaitTimeout = 5000 );
};

}

#endif // _KwsSimpleIPCClient_h_
