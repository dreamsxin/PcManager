#pragma once
#include "bkxdx/bkxdx.h"
#include "bkxdx/bkxdx_cursor_bkjson.h"    // use bkjson implements
#include "bkxdx/bkxdx_exchange_c.h"       // support c type exchange
#include "bkxdx/bkxdx_exchange_atl.h"     // support ATL class exchange
#include "common_def.h"
#include "comdata_scan_def.h"

typedef struct  _BK_ECHO_RESULT
{
    DWORD   dwEchoStatus;                 //等于0才为握手成功，其它定义在bkengdef.h中，请包含此头文件
    HRESULT hEchoCode;                    //握手操作的错误码,遵循HRESULT规范    
    BOOL    bTrustProgram;                //握手验证的文件是否可信任
}BK_ECHO_RESULT, *PBK_ECHO_RESULT;

XDX_CLASS_BEGIN(BK_ECHO_RESULT)
    XDX_MEMBER(dwEchoStatus)
    XDX_MEMBER(hEchoCode)
    XDX_MEMBER(bTrustProgram)
XDX_CLASS_END

typedef struct  _BK_PLUGIN_INFO
{
    BOOL    bPlugineLog;
    CString strPath;
    CString strMD5;
}BK_PLUGIN_INFO, *PBK_BLUGIN_INFO;

XDX_CLASS_BEGIN(BK_PLUGIN_INFO)
    XDX_MEMBER(bPlugineLog)
    XDX_MEMBER(strPath)
    XDX_MEMBER(strMD5)
XDX_CLASS_END