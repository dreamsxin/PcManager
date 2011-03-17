///////////////////////////////////////////////////////////////
//
//	Filename: 	KwsSimpleIPCClient.h
//	Creator:	lichenglin  <lichenglin@kingsoft.com>
//	Date:		2009-4-1  17:14
//	Comment:	
//
///////////////////////////////////////////////////////////////


#ifndef _KwsSimpleIPCClient_h_
#define _KwsSimpleIPCClient_h_
#include "KwsIPCDef.h"

class KwsProtocol;

class KwsSimpleIPCClient
{
public:
	int SendCmd( KwsProtocol* pProtocol );

private:
};

#endif // _KwsSimpleIPCClient_h_
