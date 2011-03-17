#pragma once
#include "com_utility_interface.h"
#include "com_utility_xdx.h"
#include "common_def.h"

class   CBkRescan
    : public IBkReScan
{

BKCOM_PROXY_DECLARE_MODULE_ID(CBkRescan)

BKCOM_PROXY_DECLARE_INIT()
public:
    virtual HRESULT GetRescanResult( CAtlArray<BK_FILE_RESULT>& result )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkRescan, GetRescanResult)
            BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        hr = m_pIbkcomm->Call(  
            nMethod,
            bstrParams, 
            &bstrResponse
            );
        if ( SUCCEEDED( hr ) )
        {
            BkXDX::CBKJsonCursor    curout;
            BKCOM_PROXY_METHOD_XDX_STRUCT(GetRescanResult, CBkRescanOut)    out;
            curout.m_doc.LoadString( bstrResponse );
            curout.ReadData( out );
            result.Copy( out.result );
            ::SysFreeString( bstrResponse );
        }
        if ( bstrParams )
        {
            ::SysFreeString( bstrParams );
        }
        return hr;
    }
};

class   CBkUtility
    : public IBkUtility
{

BKCOM_PROXY_DECLARE_MODULE_ID(CBkUtility)

BKCOM_PROXY_DECLARE_INIT()
public:
    virtual HRESULT ReportFile()
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN( CBkUtility, ReportFile )
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_OUT_END
    }

    virtual HRESULT SettingChange()
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN( CBkUtility, SettingChange )
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_OUT_END
    }

    virtual HRESULT QueryLastScanInfo( DWORD dwScanMode, BK_SCAN_INFO& ScanInfo )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN( CBkUtility, QueryLastScanInfo )
        BKCOM_PROXY_METHOD_XDX_STRUCT( QueryLastScanInfo, CBkUtility ) param;
        param.dwScanMode = dwScanMode;
        cursor.WriteData( param );
        try
        {
            bstrParams = cursor.m_doc.Dump().AllocSysString();
        }
        catch ( ... )
        {
            bstrParams = NULL;
        }
        BKCOM_PROXY_METHOD_IMPLEMENT_OUT_END( ScanInfo )
    }

    virtual HRESULT EnableAvEngine()
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN( CBkUtility, EnableAvEngine )
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_OUT_END
    }

    virtual HRESULT DisableAvEngine()
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN( CBkUtility, DisableAvEngine )
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_OUT_END
    }

    virtual HRESULT LoadDaemonAvSign()
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN( CBkUtility, LoadDaemonAvSign )
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_OUT_END
    }

    virtual HRESULT UnloadDaemonAvSign()
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN( CBkUtility, UnloadDaemonAvSign )
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_OUT_END
    }

    virtual HRESULT ReloadAvSign()
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN( CBkUtility, ReloadAvSign )
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_OUT_END
    }

    virtual HRESULT WhiteListUpdate()
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN( CBkUtility, WhiteListUpdate )
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_OUT_END
    }
};

class CBkEcho
    : public IBkEcho
{
BKCOM_PROXY_DECLARE_MODULE_ID(CBkEcho)

BKCOM_PROXY_DECLARE_INIT()
public:
    virtual HRESULT Echo( CAtlArray<CString>& FileList )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkEcho, Echo)
        BKCOM_PROXY_METHOD_XDX_STRUCT(Echo, CBkEcho) param;
        param.FileList.Copy( FileList );
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

    virtual HRESULT GetEchoResult( BK_ECHO_RESULT& result )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkEcho, GetEchoResult)
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_OUT_END(result)
    }

    virtual HRESULT Stop()
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkEcho, Stop)
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_OUT_END
    }
};

class CBkPluginReport
    : public IBkPluginReport
{
BKCOM_PROXY_DECLARE_MODULE_ID(CBkPluginReport)

BKCOM_PROXY_DECLARE_INIT()
public:
    virtual HRESULT PluginReport( CAtlArray<BK_PLUGIN_INFO>& PluginList )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkPluginReport, PluginReport)
        BKCOM_PROXY_METHOD_XDX_STRUCT(PluginReport, CBkPluginReport) param;
        param.PluginList.Copy( PluginList );
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