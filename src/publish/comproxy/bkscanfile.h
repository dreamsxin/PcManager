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
#include "bkscanfile.h"
************************************************************************/
#pragma once
#include "com_scan_interface.h"
#include "com_scan_xdx.h"

class CBkScan
    : public IBkScan
{

BKCOM_PROXY_DECLARE_MODULE_ID(CBkScan)

BKCOM_PROXY_DECLARE_INIT()

public:
    virtual HRESULT Scan( int nScanMode, BOOL bBackGround, CAtlArray<CString>& lpParam )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScan, Scan)
            BKCOM_PROXY_METHOD_XDX_STRUCT(Scan, CBkScan) param;
            param.nScanMode = nScanMode;
            param.bBackGround = bBackGround;
            param.lpParam.Copy( lpParam );
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

    virtual HRESULT Scan2( int nScanMode, BOOL bBackGround, CAtlArray<BK_SCAN_ADDITIONAL_PATH>& lpParam )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScan, Scan2)
            BKCOM_PROXY_METHOD_XDX_STRUCT(Scan2, CBkScan) param;
            param.nScanMode = nScanMode;
            param.bBackGround = bBackGround;
            param.lpParam.Copy( lpParam );
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

    virtual HRESULT QuerySteps( int nScanMode, CAtlArray<int>& steps )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScan, QuerySteps)
        BKCOM_PROXY_METHOD_XDX_STRUCT(QuerySteps, CBkScan) param;
        param.nScanMode = nScanMode;
        cursor.WriteData( param );
        try
        {
            bstrParams = cursor.m_doc.Dump().AllocSysString();
        }
        catch( ... )
        {
            bstrParams = NULL;
        }

        hr = m_pIbkcomm->Call(  
            nMethod,
            bstrParams, 
            &bstrResponse
            );
        if ( SUCCEEDED( hr ) )
        {
            BkXDX::CBKJsonCursor    curout;
            BKCOM_PROXY_METHOD_XDX_STRUCT(QuerySteps, CBkScanOut) out;
            curout.m_doc.LoadString( bstrResponse );
            curout.ReadData( out );
            steps.Copy( out.steps );
            ::SysFreeString( bstrResponse );
        }
        if ( bstrParams )
        {
            ::SysFreeString( bstrParams );
        }
        return hr;

    }

    virtual HRESULT Pause()
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScan, Pause)
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_OUT_END
    }

    virtual HRESULT Resume()
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScan, Resume)
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_OUT_END
    }

    virtual HRESULT Stop()
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScan, Stop)
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_OUT_END
    }

    virtual HRESULT QueryScanInfo( BK_SCAN_INFO& scanInfo )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScan, QueryScanInfo)
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_OUT_END(scanInfo)
    }

    virtual HRESULT QueryCleanInfo( BK_CLEAN_INFO& cleanInfo )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScan, QueryCleanInfo)
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_OUT_END(cleanInfo)
    }

    virtual HRESULT QueryFileInfo( int nType, DWORD dwIndex, BK_FILE_INFO& fileInfo )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScan, QueryFileInfo)
            BKCOM_PROXY_METHOD_XDX_STRUCT(QueryFileInfo, CBkScan) param;
            param.nType     = nType;
            param.dwIndex   = ( DWORD )dwIndex;
            cursor.WriteData( param );

            try
            {
                bstrParams = cursor.m_doc.Dump().AllocSysString();
            }
            catch( ... )
            {
                bstrParams = NULL;
            }
        BKCOM_PROXY_METHOD_IMPLEMENT_OUT_END(fileInfo)
    }

    virtual HRESULT Clean( CAtlArray<DWORD>& fileIndex )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScan, Clean)
            BKCOM_PROXY_METHOD_XDX_STRUCT(Clean, CBkScan) param;
            param.fileIndex.Copy(fileIndex);
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

    virtual HRESULT QueryLastScanInfo( int nScanMode, BK_SCAN_INFO& scanInfo )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScan, QueryLastScanInfo)
            BKCOM_PROXY_METHOD_XDX_STRUCT(QueryLastScanInfo, CBkScan) param;
            param.nScanMode = nScanMode;
            cursor.WriteData( param );
            try
            {
                bstrParams = cursor.m_doc.Dump().AllocSysString();
            }
            catch( ... )
            {
                bstrParams = NULL;
            }
        BKCOM_PROXY_METHOD_IMPLEMENT_OUT_END(scanInfo)
    }

    virtual HRESULT SetScanSetting( BK_SCAN_SETTING& setting )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScan, SetScanSetting)
            cursor.WriteData( setting );
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

    virtual HRESULT GetScanSetting( BK_SCAN_SETTING& setting )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScan, GetScanSetting)
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_OUT_END(setting)
    }

    virtual HRESULT ReportFile()
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScan, ReportFile)
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
        BKCOM_PROXY_METHOD_IMPLEMENT_NO_OUT_END
    }
protected:
};

class CBkScanSingleFile
    : public IBkScanSingleFile
{
BKCOM_PROXY_DECLARE_MODULE_ID(CBkScanSingleFile)

BKCOM_PROXY_DECLARE_INIT()

public:
    virtual HRESULT ScanFile( CString& strFile, DWORD dwScanMask, DWORD dwScanPolicy, BK_FILE_RESULT& fr )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScanSingleFile, ScanFile)
            BKCOM_PROXY_METHOD_XDX_STRUCT(ScanFile, CBkScanSingleFile) param;
            param.strFile = strFile;
            param.dwScanMask = dwScanMask;
            param.dwScanPolicy = dwScanPolicy;
            cursor.WriteData( param );
            try
            {
                bstrParams = cursor.m_doc.Dump().AllocSysString();
            }
            catch( ... )
            {
                bstrParams = NULL;
            }
        BKCOM_PROXY_METHOD_IMPLEMENT_OUT_END( fr );
    }

    virtual HRESULT CleanFile( CString& strFile, DWORD dwScanMask, DWORD dwScanPolicy, BK_FILE_RESULT& fr )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScanSingleFile, CleanFile)
            BKCOM_PROXY_METHOD_XDX_STRUCT(CleanFile, CBkScanSingleFile) param;
            param.strFile = strFile;
            param.dwScanMask = dwScanMask;
            param.dwScanPolicy = dwScanPolicy;
            cursor.WriteData( param );
            try
            {
                bstrParams = cursor.m_doc.Dump().AllocSysString();
            }
            catch( ... )
            {
                bstrParams = NULL;
            }
        BKCOM_PROXY_METHOD_IMPLEMENT_OUT_END( fr )
    }

    virtual HRESULT ForceClean( CString& strFile, BK_FORCE_CLEAN_DETAIL& detail, BK_FILE_RESULT& fr )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScanSingleFile, ForceClean)
            BKCOM_PROXY_METHOD_XDX_STRUCT(ForceClean, CBkScanSingleFile) param;
        param.strFile = strFile;
        param.detail  = detail;
        cursor.WriteData( param );
        try
        {
            bstrParams = cursor.m_doc.Dump().AllocSysString();
        }
        catch( ... )
        {
            bstrParams = NULL;
        }
        BKCOM_PROXY_METHOD_IMPLEMENT_OUT_END( fr )
    }
    //virtual HRESULT SetScanSetting( BK_SCAN_SETTING& setting )
    //{
    //    BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScanSingleFile, SetScanSetting)
    //        cursor.WriteData( setting );
    //        try
    //        {
    //            bstrParams = cursor.m_doc.Dump().AllocSysString();
    //        }
    //        catch( ... )
    //        {
    //            bstrParams = NULL;
    //        }
    //    BKCOM_PROXY_METHOD_IMPLEMENT_NO_OUT_END
    //}

    //virtual HRESULT GetScanSetting( BK_SCAN_SETTING& setting )
    //{
    //    BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScanSingleFile, GetScanSetting)
    //    BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()
    //    BKCOM_PROXY_METHOD_IMPLEMENT_OUT_END(setting)
    //}
};

class CBkScanMultiFile
    : public IBkScanMultiFile
{
BKCOM_PROXY_DECLARE_MODULE_ID(CBkScanMultiFile)

BKCOM_PROXY_DECLARE_INIT()
public:
    virtual HRESULT ScanHash( CAtlArray<CString>& hashArray, DWORD dwScanPolicy, CAtlArray<BK_FILE_RESULT>& frArray )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScanMultiFile, ScanHash)
            BKCOM_PROXY_METHOD_XDX_STRUCT(ScanHash, CBkScanMultiFile) param;
            param.hashArray.Copy( hashArray );
            param.dwScanPolicy = dwScanPolicy;
            cursor.WriteData( param );
            try
            {
                bstrParams = cursor.m_doc.Dump().AllocSysString();
            }
            catch( ... )
            {
                bstrParams = NULL;
            }

            hr = m_pIbkcomm->Call(  
                nMethod,
                bstrParams, 
                &bstrResponse
                );
            if ( SUCCEEDED( hr ) )
            {
                BkXDX::CBKJsonCursor    curout;
                BKCOM_PROXY_METHOD_XDX_STRUCT(ScanHash, CBkScanMultiFileOut)    out;
                curout.m_doc.LoadString( bstrResponse );
                curout.ReadData( out );
                frArray.Copy( out.frArray );
                ::SysFreeString( bstrResponse );
            }
            if ( bstrParams )
            {
                ::SysFreeString( bstrParams );
            }
            return hr;
    }

    virtual HRESULT ScanFile( CAtlArray<CString>& fileArray, DWORD dwScanPolicy, CAtlArray<BK_FILE_RESULT>& frArray )
    {
        BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(CBkScanMultiFile, ScanFile)
            BKCOM_PROXY_METHOD_XDX_STRUCT(ScanFile, CBkScanMultiFile) param;
        param.fileArray.Copy( fileArray );
        param.dwScanPolicy = dwScanPolicy;
        cursor.WriteData( param );
        try
        {
            bstrParams = cursor.m_doc.Dump().AllocSysString();
        }
        catch( ... )
        {
            bstrParams = NULL;
        }

        hr = m_pIbkcomm->Call(  
            nMethod,
            bstrParams, 
            &bstrResponse
            );
        if ( SUCCEEDED( hr ) )
        {
            BkXDX::CBKJsonCursor    curout;
            BKCOM_PROXY_METHOD_XDX_STRUCT(ScanFile, CBkScanMultiFileOut)    out;
            curout.m_doc.LoadString( bstrResponse );
            curout.ReadData( out );
            frArray.Copy( out.frArray );
            ::SysFreeString( bstrResponse );
        }
        if ( bstrParams )
        {
            ::SysFreeString( bstrParams );
        }
        return hr;
    }
};
//////////////////////////////////////////////////////////////////////////
//class CBkScan
//    : public IBkScan
//{
//protected:
//    int         m_nModuleId;
//    Ibkcomm*    m_pIbkcomm;
//public:
//    CBkScan() :
//        m_nModuleId( BKCOM_EXPORT_MODULE_BkScan ),
//        m_pIbkcomm( NULL )
//    {}
//    ~CBkScan(){}
//public:
//    HRESULT Initialize()
//    {
//        HRESULT hr = S_OK;
//
//        if ( NULL == m_pIbkcomm )
//        {
//            hr = ::CoCreateInstance( 
//                CLSID_bkcomm, 
//                NULL, 
//                CLSCTX_LOCAL_SERVER, 
//                IID_Ibkcomm, 
//                ( void** )&m_pIbkcomm 
//                );
//            if ( SUCCEEDED( hr ) )
//            {
//                hr = m_pIbkcomm->Initialize( m_nModuleId, GetCurrentProcessId() );
//                if ( FAILED( hr ) )
//                {
//                    m_pIbkcomm->Release();
//                    m_pIbkcomm = NULL;
//                }
//            }
//        }
//
//        return hr;
//    }
//
//    HRESULT Uninitialize()
//    {
//        if ( m_pIbkcomm )
//        {
//            m_pIbkcomm->Uninitialize();
//            m_pIbkcomm->Release();
//            m_pIbkcomm = NULL;
//        }
//        return S_OK;
//    }
//public:
//    virtual HRESULT Scan( int nScanMode, LPVOID lpParam )
//    {
//         HRESULT hr = E_NOTIMPL;
//         BkXDX::CBKJsonCursor    cursor;
//         int nMethod = 0;
//         CString strTmp;
//         BSTR  bstrParams, bstrResponse;
// 
//         if ( NULL == m_pIbkcomm )
//         {
//             return E_NOINTERFACE;
//         }
// 
//         struct  TMP_Scan_PARAM_0   param;
// 
//         param.nScanMode = nScanMode;
//         param.CustomFile.Copy( lpParam );
//         cursor.WriteData( param );
//         bstrParams = cursor.m_doc.Dump().AllocSysString();
// 
//         hr = m_pIbkcomm->Call( 
//             nMethod, 
//             bstrParams, 
//             &bstrResponse
//             );
// 
//         ::SysFreeString( bstrParams );
// 
//         return hr;
//    }
//    virtual HRESULT Pause()
//    {
//        HRESULT hr = E_NOTIMPL;
//        BkXDX::CBKJsonCursor    cursor;
//        int     nMethod = 1;
//        CString strTmp;
//        BSTR    bstrParams, bstrResponse;
//
//        if ( NULL == m_pIbkcomm )
//        {
//            return E_NOINTERFACE;
//        }
//
//        bstrParams = cursor.m_doc.Dump().AllocSysString();
//
//        hr = m_pIbkcomm->Call( 
//            nMethod, 
//            bstrParams, 
//            &bstrResponse
//            );
//
//        ::SysFreeString( bstrParams );
//        return hr;
//    }
//
//    virtual HRESULT Resume()
//    {
//        HRESULT hr = E_NOTIMPL;
//        BkXDX::CBKJsonCursor    cursor;
//        int     nMethod = 2;
//        CString strTmp;
//        BSTR    bstrParams, bstrResponse;
//
//        if ( NULL == m_pIbkcomm )
//        {
//            return E_NOINTERFACE;
//        }
//
//        bstrParams = cursor.m_doc.Dump().AllocSysString();
//
//        hr = m_pIbkcomm->Call(  
//            nMethod,
//            bstrParams, 
//            &bstrResponse
//            );
//
//        ::SysFreeString( bstrParams );
//        return hr;
//    }
//
//    virtual HRESULT Stop()
//    {
//         HRESULT hr = E_NOTIMPL;
//         BkXDX::CBKJsonCursor    cursor;
//         int     nMethod = 3;
//         CString strTmp;
//         BSTR    bstrParams, bstrResponse;
// 
//         if ( NULL == m_pIbkcomm )
//         {
//             return E_NOINTERFACE;
//         }
// 
//         bstrParams = cursor.m_doc.Dump().AllocSysString();
// 
//         hr = m_pIbkcomm->Call(  
//             nMethod,
//             bstrParams, 
//             &bstrResponse
//             );
// 
//         ::SysFreeString( bstrParams );
//         return hr;
//    }
//
//    virtual HRESULT QueryScanInfo( BK_SCAN_INFO& scanInfo )
//    {
//         HRESULT hr = E_NOTIMPL;
//         BkXDX::CBKJsonCursor    cursor;
//         int     nMethod = 4;
//         CString strTmp;
//         BSTR    bstrParams, bstrResponse;
// 
//         if ( NULL == m_pIbkcomm )
//         {
//             return E_NOINTERFACE;
//         }
// 
//         bstrParams = cursor.m_doc.Dump().AllocSysString();
// 
//         hr = m_pIbkcomm->Call(  
//             nMethod,
//             bstrParams, 
//             &bstrResponse
//             );
//         if ( SUCCEEDED( hr ) )
//         {
//             cursor.m_doc.LoadString( bstrResponse );
//             cursor.ReadData( scanInfo );
//             ::SysFreeString( bstrResponse );
//         }
// 
//         ::SysFreeString( bstrParams );
//         return hr;
//    }
//
//    virtual HRESULT QueryCleanInfo( BK_CLEAN_INFO& cleanInfo )
//    {
//        HRESULT hr = E_NOTIMPL;
//        BkXDX::CBKJsonCursor    cursor;
//        int     nMethod = 5;
//        CString strTmp;
//        BSTR    bstrParams, bstrResponse;
//
//        if ( NULL == m_pIbkcomm )
//        {
//            return E_NOINTERFACE;
//        }
//
//        bstrParams = cursor.m_doc.Dump().AllocSysString();
//
//        hr = m_pIbkcomm->Call(  
//            nMethod,
//            bstrParams, 
//            &bstrResponse
//            );
//        if ( SUCCEEDED( hr ) )
//        {
//            cursor.m_doc.LoadString( bstrResponse );
//            cursor.ReadData( cleanInfo );
//            ::SysFreeString( bstrResponse );
//        }
//
//        ::SysFreeString( bstrParams );
//        return hr;
//    }
//
//    virtual HRESULT QueryFileInfo( int nType, DWORD dwIndex, BK_FILE_INFO& fileInfo )
//    {
//        HRESULT hr = E_NOTIMPL;
//        BkXDX::CBKJsonCursor    cursor;
//        int     nMethod = 6;
//        CString strTmp;
//        BSTR    bstrParams, bstrResponse;
//
//        if ( NULL == m_pIbkcomm )
//        {
//            return E_NOINTERFACE;
//        }
//
//        struct TMP_QueryFileInfo_PARAM_0 param;
//
//        param.nType     = nType;
//        param.dwIndex   = ( DWORD )dwIndex;
//        cursor.WriteData( param );
//
//        bstrParams = cursor.m_doc.Dump().AllocSysString();
//
//        hr = m_pIbkcomm->Call(  
//            nMethod,
//            bstrParams, 
//            &bstrResponse
//            );
//        if ( SUCCEEDED( hr ) )
//        {
//            cursor.m_doc.LoadString( bstrResponse );
//            cursor.ReadData( fileInfo );
//            ::SysFreeString( bstrResponse );
//        }
//
//        ::SysFreeString( bstrParams );
//        return hr;
//    }
//
//    virtual HRESULT Clean( CAtlArray<DWORD>& fileIndex )
//    {
//        HRESULT hr = E_NOTIMPL;
//        BkXDX::CBKJsonCursor    cursor;
//        int     nMethod = 7;
//        CString strTmp;
//        BSTR    bstrParams, bstrResponse;
//
//        if ( NULL == m_pIbkcomm )
//        {
//            return E_NOINTERFACE;
//        }
//
//        cursor.WriteData( fileIndex );
//        bstrParams = cursor.m_doc.Dump().AllocSysString();
//
//        hr = m_pIbkcomm->Call(  
//            nMethod,
//            bstrParams, 
//            &bstrResponse
//            );
//
//        ::SysFreeString( bstrParams );
//        return hr;
//    }
//
//    virtual HRESULT QueryLastScanInfo( BK_SCAN_INFO& scanInfo )
//    {
//        HRESULT hr = E_NOTIMPL;
//        BkXDX::CBKJsonCursor    cursor;
//        int     nMethod = 8;
//        CString strTmp;
//        BSTR    bstrParams, bstrResponse;
//
//        if ( NULL == m_pIbkcomm )
//        {
//            return E_NOINTERFACE;
//        }
//
//        bstrParams = cursor.m_doc.Dump().AllocSysString();
//
//        hr = m_pIbkcomm->Call(  
//            nMethod,
//            bstrParams, 
//            &bstrResponse
//            );
//        if ( SUCCEEDED( hr ) )
//        {
//            cursor.m_doc.LoadString( bstrResponse );
//            cursor.ReadData( scanInfo );
//            ::SysFreeString( bstrResponse );
//        }
//
//        ::SysFreeString( bstrParams );
//        return hr;
//    }
//
//    virtual HRESULT SetScanSetting( BK_SCAN_SETTING& setting )
//    {
//        HRESULT hr = E_NOTIMPL;
//        BkXDX::CBKJsonCursor    cursor;
//        int     nMethod = 9;
//        CString strTmp;
//        BSTR    bstrParams, bstrResponse;
//
//        if ( NULL == m_pIbkcomm )
//        {
//            return E_NOINTERFACE;
//        }
//
//        cursor.WriteData( setting );
//        bstrParams = cursor.m_doc.Dump().AllocSysString();
//
//        hr = m_pIbkcomm->Call(  
//            nMethod,
//            bstrParams, 
//            &bstrResponse
//            );
//
//        ::SysFreeString( bstrParams );
//        return hr;
//    }
//
//    virtual HRESULT GetScanSetting( BK_SCAN_SETTING& setting )
//    {
//        HRESULT hr = E_NOTIMPL;
//        BkXDX::CBKJsonCursor    cursor;
//        int     nMethod = 10;
//        CString strTmp;
//        BSTR    bstrParams, bstrResponse;
//
//        if ( NULL == m_pIbkcomm )
//        {
//            return E_NOINTERFACE;
//        }
//
//        bstrParams = cursor.m_doc.Dump().AllocSysString();
//
//        hr = m_pIbkcomm->Call(  
//            nMethod,
//            bstrParams, 
//            &bstrResponse
//            );
//        if ( SUCCEEDED( hr ) )
//        {
//            cursor.m_doc.LoadString( bstrResponse );
//            cursor.ReadData( setting );
//            ::SysFreeString( bstrResponse );
//        }
//
//        ::SysFreeString( bstrParams );
//        return hr;
//    }
//protected:
//};