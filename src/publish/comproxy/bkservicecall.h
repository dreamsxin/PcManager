/************************************************************************
配置要求
1. 工程引入bksafesvc.idl
2. 在工程中增加新的filter，引入bksafesvc.idl生成的.c文件
3. 去除.c文件的预编译选项
4. filter的属性设置: parse file : TRUE | SCC file : FASLE
5. 在包含该头文件之前，首先要包含bksafesvc.idl生成的.h文件
6. 同时import 文件，并去除.cpp文件的预编译选项
    http://192.168.1.234/svn/3rdlib/trunk/publish/miniutil/bkjson.cpp
    http://192.168.1.234/svn/3rdlib/trunk/publish/miniutil/bkjson.h
推荐idl配置如下：
output
   output directory : ./_idl_gen
   header file      : bksafesvc.h
   IID file         : bksafesvc_i.c

Example:
#include "stdafx.h"
#include "_idl_gen/bksafesvc.h"
#include "bkservicecall.h"
************************************************************************/
#include "com_sc_interface.h"
#include "com_sc_xdx.h"

class CBkServiceCall
    : public IBkServiceCall
{
BKCOM_PROXY_DECLARE_MODULE_ID(CBkServiceCall)

BKCOM_PROXY_DECLARE_INIT()

public:
    virtual HRESULT Execute( 
        const CString& strExePath, 
        const CString& strCmdline, 
        BOOL bShow 
        )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkServiceCall, Execute)
            BKCOM_PROXY_METHOD_XDX_STRUCT(Execute, CBkServiceCall) param;
            param.strExePath = strExePath;
            param.strCmdline = strCmdline;
            param.bShow = bShow;
            cursor.WriteData( param );
            try
            {
                bstrParams = cursor.m_doc.Dump().AllocSysString();
            }
            catch( ... )
            {
                bstrParams = NULL;
            }
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_OUT_END
    }
};