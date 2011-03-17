#pragma once
#ifndef NETFLOW_FORMAT_H
#define NETFLOW_FORMAT_H


#include<atlstr.h>
#include <Windows.h>

void NetFlowToString( ULONGLONG netFlow, ATL::CString& str );
void NetFlowToString2( ULONGLONG netFlow, ATL::CString& str );

#endif //NETFLOW_FORMAT_H