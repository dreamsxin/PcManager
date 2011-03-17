/**
* @file    winmodinetsession.h
* @brief   ...
* @author  bbcallen
* @date    2009-08-07 15:13
*/

#ifndef WINMODINETSESSION_H
#define WINMODINETSESSION_H

#include <assert.h>
#include <atlstr.h>
#include "winmodinethandle.h"
#include "winmodinethttpconnection.h"
#include "winmodinethttpdownloadfile.h"

NS_WINMOD_BEGIN

class CInetSession: public CInetHandle
{
public:
    CInetSession();
    virtual ~CInetSession();

    virtual void Close() {Interrupt(); CInetHandle::Close();}

    HRESULT Open(
        LPCTSTR lpszAgent       = NULL,
        DWORD   dwAccessType    = INTERNET_OPEN_TYPE_PRECONFIG, 
        LPCTSTR lpszProxy       = NULL, 
        LPCTSTR lpszProxyBypass = NULL, 
        DWORD   dwFlags         = 0);

    HINTERNET HttpConnect(
        LPCTSTR         lpszServerName,
        INTERNET_PORT   nServerPort     = INTERNET_DEFAULT_HTTP_PORT,
        LPCTSTR         lpszUsername    = NULL,
        LPCTSTR         lpszPassword    = NULL,
        DWORD           dwFlags         = 0);

    HRESULT HttpRequest(
        /* [in ] */ LPCTSTR         lpszServerName,
        /* [in ] */ INTERNET_PORT   nServerPort,
        /* [in ] */ LPCTSTR         lpObject,
        /* [in ] */ DWORD           dwTimeout,
        /* [in ] */ LPCTSTR         lpszContentType,
        /* [in ] */ const CStringA& strCommand,
        /* [out] */ CStringA*       pstrResponse        = NULL,
        /* [out] */ DWORD*          pdwStatusCode       = NULL,
        /* [in ] */ LPCTSTR         lpszSpecHostName    = NULL);

    HRESULT HttpDownload(
        /* [in ] */ IInetHttpDownloadFile*      piDownloadFile,
        /* [in ] */ IInetHttpDownloadProgress*  piCallback,
        /* [in ] */ LPCTSTR                     lpszServerName,
        /* [in ] */ INTERNET_PORT               nServerPort,
        /* [in ] */ LPCTSTR                     lpObject,
        /* [in ] */ DWORD                       dwTimeout,
        /* [out] */ DWORD*                      pdwStatusCode       = NULL,
        /* [in ] */ LPCTSTR                     lpszSpecHostName    = NULL, 
        /* [in ] */ BOOL                        bAcceptGZip         = FALSE);

    HRESULT HttpDownload(
        /* [in ] */ LPCWSTR                     lpszLocalFilePath,
        /* [in ] */ IInetHttpDownloadProgress*  piCallback,
        /* [in ] */ LPCTSTR                     lpszServerName,
        /* [in ] */ INTERNET_PORT               nServerPort,
        /* [in ] */ LPCTSTR                     lpObject,
        /* [in ] */ DWORD                       dwTimeout,
        /* [out] */ DWORD*                      pdwStatusCode       = NULL,
        /* [in ] */ LPCTSTR                     lpszSpecHostName    = NULL, 
        /* [in ] */ BOOL                        bAcceptGZip         = FALSE);



    HRESULT SetConnectTimeOut(DWORD  dwMilliSeconds);
    HRESULT GetConnectTimeOut(DWORD& dwMilliSeconds);

    void    Interrupt();

private:
    // denied
    CInetSession(CInetSession& h);
    explicit CInetSession(HANDLE h);
    CInetSession& operator=(CInetSession& h);


private:

    // used by HttpRequest and HttpDownload
    CInetHttpConnection m_hHttpConnection;
};




inline CInetSession::CInetSession()
{
}

inline CInetSession::~CInetSession()
{
}

inline HRESULT CInetSession::Open(
    LPCTSTR lpszAgent,
    DWORD   dwAccessType, 
    LPCTSTR lpszProxy, 
    LPCTSTR lpszProxyBypass, 
    DWORD   dwFlags)
{
    Close();
    HINTERNET hSession = ::InternetOpen(lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwFlags);
    if (!hSession)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    Attach(hSession);
    return S_OK;
}

inline HINTERNET CInetSession::HttpConnect(
    LPCTSTR         lpszServerName,
    INTERNET_PORT   nServerPort,
    LPCTSTR         lpszUsername,
    LPCTSTR         lpszPassword,
    DWORD           dwFlags)
{
    assert(m_h);
    HINTERNET hConn = ::InternetConnect(
        m_h,
        lpszServerName,
        nServerPort,
        lpszUsername,
        lpszPassword,
        INTERNET_SERVICE_HTTP,
        dwFlags,
        0);
    if (!hConn)
        return NULL;

    return hConn;
}

inline HRESULT CInetSession::HttpRequest(
    /* [in ] */ LPCTSTR         lpszServerName,
    /* [in ] */ INTERNET_PORT   nServerPort,
    /* [in ] */ LPCTSTR         lpObject,
    /* [in ] */ DWORD           dwTimeout,
    /* [in ] */ LPCTSTR         lpszContentType,
    /* [in ] */ const CStringA& strCommand,
    /* [out] */ CStringA*       pstrResponse,
    /* [out] */ DWORD*          pdwStatusCode,
    /* [in ] */ LPCTSTR         lpszSpecHostName)
{
    if (!m_h)
        return E_HANDLE;

    if (!lpszServerName || !lpObject)
        return E_POINTER;


    m_hHttpConnection.Close();
    m_hHttpConnection.Attach(HttpConnect(lpszServerName, nServerPort));
    if (!m_hHttpConnection)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;


    return m_hHttpConnection.HttpRequest(
        lpObject,
        dwTimeout,
        lpszContentType,
        strCommand,
        pstrResponse,
        pdwStatusCode,
        lpszSpecHostName);
}

inline HRESULT CInetSession::HttpDownload(
    /* [in ] */ IInetHttpDownloadFile*      piDownloadFile,
    /* [in ] */ IInetHttpDownloadProgress*  piCallback,
    /* [in ] */ LPCTSTR                     lpszServerName,
    /* [in ] */ INTERNET_PORT               nServerPort,
    /* [in ] */ LPCTSTR                     lpObject,
    /* [in ] */ DWORD                       dwTimeout,
    /* [out] */ DWORD*                      pdwStatusCode,
    /* [in ] */ LPCTSTR                     lpszSpecHostName, 
    /* [in ] */ BOOL                        bAcceptGZip)
{
    if (!m_h)
        return E_HANDLE;

    if (!lpszServerName || !lpObject)
        return E_POINTER;


    m_hHttpConnection.Close();
    m_hHttpConnection.Attach(HttpConnect(lpszServerName, nServerPort));
    if (!m_hHttpConnection)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    m_hHttpConnection.SetUseGZip(bAcceptGZip);

    return m_hHttpConnection.HttpDownload(
        piDownloadFile,
        piCallback,
        lpObject,
        dwTimeout,
        pdwStatusCode,
        lpszSpecHostName);
}

inline HRESULT CInetSession::HttpDownload(
    /* [in ] */ LPCWSTR                     lpszLocalFilePath,
    /* [in ] */ IInetHttpDownloadProgress*  piCallback,
    /* [in ] */ LPCTSTR                     lpszServerName,
    /* [in ] */ INTERNET_PORT               nServerPort,
    /* [in ] */ LPCTSTR                     lpObject,
    /* [in ] */ DWORD                       dwTimeout,
    /* [out] */ DWORD*                      pdwStatusCode,
    /* [in ] */ LPCTSTR                     lpszSpecHostName, 
    /* [in ] */ BOOL                        bAcceptGZi)
{
    assert(lpszLocalFilePath);

    CInetHttpDownloadFile hDownloadFile;
    HRESULT hr = hDownloadFile.m_hFile.Create(
        lpszLocalFilePath,
        GENERIC_WRITE,
        FILE_SHARE_DELETE,
        CREATE_ALWAYS);
    if (FAILED(hr))
        return hr;

    return HttpDownload(
        &hDownloadFile,
        piCallback,
        lpszServerName,
        nServerPort,
        lpObject,
        dwTimeout,
        pdwStatusCode,
        lpszSpecHostName);
}

inline HRESULT CInetSession::SetConnectTimeOut(DWORD  dwMilliSeconds)
{
    return DoSetOptionDWORD(INTERNET_OPTION_CONNECT_TIMEOUT, dwMilliSeconds);
}

inline HRESULT CInetSession::GetConnectTimeOut(DWORD& dwMilliSeconds)
{
    return DoGetOptionDWORD(INTERNET_OPTION_CONNECT_TIMEOUT, dwMilliSeconds);
}


inline void CInetSession::Interrupt()
{
    m_hHttpConnection.Close();
}



NS_WINMOD_END

#endif//WINMODINETSESSION_H