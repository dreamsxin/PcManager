#pragma once
#include "export_module_def.h"

/************************************************************************/
/* Tmp Struct XDX                                                       */
/************************************************************************/
//struct  TMP_SCAN_PARAM_0
//{
//    int         nScanMode;
//    ULONG_PTR  lpParam;
//};
//XDX_CLASS_BEGIN(struct  TMP_SCAN_PARAM_0)
//    XDX_MEMBER(nScanMode)
//    XDX_MEMBER(lpParam)
//XDX_CLASS_END
#define BKCOM_PROXY_METHOD_PARAM_BEGIN(method, condition)   \
struct TMP_##method##_PARAM_##condition  {

#define BKCOM_PROXY_METHOD_PARAM(type, param)   type    param;
#define BKCOM_PROXY_METHOD_PARAM_END    };

#define BKCOM_PROXY_METHOD_PARAM_XDX_BEGIN(method, condition)    XDX_CLASS_BEGIN(struct TMP_##method##_PARAM_##condition)
#define BKCOM_PROXY_METHOD_PARAM_XDX(param)  XDX_MEMBER(param)
#define BKCOM_PROXY_METHOD_PARAM_XDX_END XDX_CLASS_END
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* proxy class                                                          */
/************************************************************************/
#define BKCOM_PROXY_KEYWORD_PUBLIC  public:
#define BKCOM_PROXY_KEYWORD_DEFAULT default:
//////////////////////////////////////////////////////////////////////////
// protected:
//     int         m_nModuleId;
//     Ibkcomm*    m_pIbkcomm;
// public:
//     CBkScan() :
//         m_nModuleId( BKCOM_EXPORT_MODULE_BkScan ),
//         m_pIbkcomm( NULL )
//     {}
//     ~CBkScan(){}
#define BKCOM_PROXY_CONSTRUCT(module)   module() :   \
    m_nModuleId( BKCOM_EXPORT_MODULE_##module ),        \
    m_nModuleVersion( BKCOM_EXPORT_MODULE_##module##_VER ),\
    m_pIbkcomm( NULL )                                  \
    {}                                                  \
    ~module(){}

#define BKCOM_PROXY_DECLARE_MODULE_ID(module)       \
    protected:                                      \
    int         m_nModuleId;                    \
    int         m_nModuleVersion;               \
    Ibkcomm*    m_pIbkcomm;                     \
    BKCOM_PROXY_KEYWORD_PUBLIC                             \
    BKCOM_PROXY_CONSTRUCT(module)
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
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
//                hr = m_pIbkcomm->Initialize( m_nModuleId, GetCurrentProcessId(), m_nModuleVersion );
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
#define BKCOM_PROXY_DECLARE_INIT()                  \
    BKCOM_PROXY_KEYWORD_PUBLIC\
    HRESULT Initialize()                            \
    {                                               \
        HRESULT hr = S_OK;\
        int     nCount = 0;\
        while( nCount < 3 )\
        {\
            AtlTrace( L"[com interface]initialize retry count %d\n", nCount );\
            AtlTrace( L"[com interface]initialize m_pIbkcomm  %08x\n", m_pIbkcomm );\
            if ( NULL == m_pIbkcomm )\
            {\
                hr = ::CoCreateInstance( \
                    CLSID_bkcomm, \
                    NULL, \
                    CLSCTX_LOCAL_SERVER, \
                    IID_Ibkcomm, \
                    ( void** )&m_pIbkcomm \
                    );\
                    AtlTrace( L"[com interface]CoCreateInstance result %08x m_pIbkcomm  %08x\n", hr, m_pIbkcomm );\
                if ( SUCCEEDED( hr ) )\
                {\
                    hr = m_pIbkcomm->Initialize( m_nModuleId, GetCurrentProcessId(), m_nModuleVersion );\
                    if ( FAILED( hr ) )\
                    {\
                        m_pIbkcomm->Release();\
                        m_pIbkcomm = NULL;\
                    }\
                    break;\
                }\
                else\
                {\
                    nCount++;\
                    Sleep( 500 );\
                }\
            }\
            else\
            {\
                if( FAILED( hr ) )\
                    m_pIbkcomm = NULL;\
                break;\
            }\
        }\
        return hr;\
    }\
\
    HRESULT Uninitialize()\
    {\
        if ( m_pIbkcomm )\
        {\
            m_pIbkcomm->Uninitialize();\
            m_pIbkcomm->Release();\
            m_pIbkcomm = NULL;\
        }\
        return S_OK;\
    }
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
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
//         switch ( nScanMode )
//         {
//         case BkScanModeFastScan:
//             {
//                 TMP_Scan_PARAM_BkScanModeFastScan   param;
//                 param.nScanMode = nScanMode;
//                 param.lpParam   = ( ULONG )( ULONG_PTR )lpParam;
//                 cursor.WriteData( param );
// 
//                 bstrParams = cursor.m_doc.Dump().AllocSysString();
// 
//                 break;
//             }
//         case BkScanModeCustomScan:
//             {
//                 struct  TMP_Scan_PARAM_BkScanModeCustomScan   param;
// 
//                 param.nScanMode = nScanMode;
//                 if ( lpParam )
//                 {
//                     param.CustomFile.Copy( *( CAtlArray<CString>* )lpParam );
//                     cursor.WriteData( param );
// 
//                     bstrParams = cursor.m_doc.Dump().AllocSysString();
//                 }
//                 else
//                 {
//                     hr = E_INVALIDARG;
//                 }
// 
//                 break;
//             }
//         case BkScanModeFullScan:
//             {
//                 struct TMP_Scan_PARAM_BkScanModeFullScan param;
// 
//                 param.nScanMode = nScanMode;
//                 param.lpParam   = ( ULONG )( ULONG_PTR )lpParam;
//                 cursor.WriteData( param );
// 
//                 bstrParams = cursor.m_doc.Dump().AllocSysString();
// 
//                 break;
//             }
//         default:
//             {
//                 return hr;
//             }
//         }
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
#define BKCOM_PROXY_METHOD_XDX_STRUCT(method, condition)    struct TMP_##method##_PARAM_##condition
#define BKCOM_PROXY_METHOD_IMPLEMENT_BEGIN(module, method)  \
        HRESULT hr = E_NOTIMPL;\
        BkXDX::CBKJsonCursor    cursor;\
        int nMethod = BKCOM_EXPORT_##module##_METHOD_##method;\
        CString strTmp;\
        BSTR  bstrParams, bstrResponse;\
        if ( NULL == m_pIbkcomm )\
        {\
            return E_NOINTERFACE;\
        }

#define BKCOM_PROXY_METHOD_IMPLEMENT_NO_IN_PARAMS()\
        try\
        {\
            bstrParams = cursor.m_doc.Dump().AllocSysString();\
        }\
        catch ( ... )\
        {\
            bstrParams = NULL;\
        }

#define BKCOM_PROXY_METHOD_IMPLEMENT_NO_OUT_END\
        hr = m_pIbkcomm->Call( \
            nMethod, \
            bstrParams, \
            &bstrResponse\
            );\
        if ( bstrParams )\
        {\
            ::SysFreeString( bstrParams );\
        }\
        return hr;

#define BKCOM_PROXY_METHOD_IMPLEMENT_OUT_END(arg)\
        hr = m_pIbkcomm->Call(  \
            nMethod,\
            bstrParams, \
            &bstrResponse\
            );\
        if ( SUCCEEDED( hr ) )\
        {\
            BkXDX::CBKJsonCursor    curout;\
            curout.m_doc.LoadString( bstrResponse );\
            curout.ReadData( arg );\
            ::SysFreeString( bstrResponse );\
        }\
        if ( bstrParams )\
        {\
            ::SysFreeString( bstrParams );\
        }\
        return hr;


//////////////////////////////////////////////////////////////////////////